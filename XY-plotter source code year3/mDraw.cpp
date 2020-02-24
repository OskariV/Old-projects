/*
 ===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
#include <atomic>
static volatile std::atomic_int boost;
// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#include "FreeRTOS.h"
#include "task.h"
#include "DigitalIoPins2.h"
#include "Fmutex.h"
#include "ITMwriter2.h"
#include "gParser.h"
#include "MotorController.h"
#include <string>
#include <mutex>
#include "user_vcom.h"
#include <cstring>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
Fmutex guard;
DigitalIoPins *directionx;
DigitalIoPins *stepx;
DigitalIoPins *directiony;
DigitalIoPins *stepy;
DigitalIoPins *limit1;
DigitalIoPins *limit2;
DigitalIoPins *limit3;
DigitalIoPins *limit4;
DigitalIoPins *laser;
MotorController *controller;
QueueHandle_t commandQueue;
QueueHandle_t mTen;
/*****************************************************************************
 * Private functions
 ****************************************************************************/
void printUA(std::string input) {
	std::lock_guard<Fmutex> lock(guard);
	Board_UARTPutSTR(input.c_str());
}
/* Sets up system hardware */
static void prvSetupHardware(void) {
	SystemCoreClockUpdate();
	Board_Init();

	/* Initial LED0 state is off */
	Board_LED_Set(0, false);
}
/*
 * This task creates, calibrates and controls the motorcontroller object which controls a motor for each
 * axis and a pen. The task first calibrates then sends the response to M10 to a queue and then gets ready to recieve commands.
 */
static void movePen(void *pvParameters) {
	//delay to let the simulator handle pins
	vTaskDelay(2000);

	//creation and calibration of the controller
	controller = new MotorController(*limit1,*limit2,*limit3,*limit4,*directionx,*stepx,*directiony,*stepy);
	ITMwriter kirjoittaja;
	command nu;

	int xlen;
	int ylen;
	int xphys;
	int yphys;

	//prints steps and length to itm
	controller->getDistance(ylen, xlen,xphys,yphys);
	kirjoittaja.write("x: " + std::to_string(xlen) + " y: " + std::to_string(ylen)+  " xphys: " + std::to_string(yphys)+  " yphys: " + std::to_string(xphys)+ "\n\r");

	//gets the response to M10
	std::string ten = controller->getTen();
	std::string* pTen = new std::string (ten);

	if (xQueueSendToBack(mTen, &pTen, portMAX_DELAY)) {

							}
	//Receives commands and send them to motorcontroller
	while(1){
		if (xQueueReceive(commandQueue, &nu, portMAX_DELAY)) {
			controller->move(nu.movetoX, nu.movetoY, nu.speed, nu.pen);
		}
	}
}

/*
 * This task reads messages from mDraw trough the USB_receive function and returns messages trough USB_send.
 * It uses the gParser object to parse mDraws g-codes and responds ok when mDraw can send a new message.
 * Before the task can start parsing messages it needs to get a message from motorcontroller which contains
 * the response to M10 command. M10 message contains the length, width, pen UP and DOWN positions and the speed setting.
 *
 */
static void readmDraw(void *pvParameters) {
	ITMwriter kirjoittaja;
	gParser p;
	command nu;

	std::string print = "";
	std::string order = "";
	std::string ten ="";

	std::string* pTen ;

	//waits on motorcontroller to send the response to M10
	if (xQueueReceive(mTen, &pTen, portMAX_DELAY)) {
	}

	char ch;
	while (1) {

		char str[80];
		int len = USB_receive((uint8_t *) str, 79);
		str[len] = '\0';

		USB_send((uint8_t *) str, len);

		for (int i = 0; i < len; ++i) {
			ch = str[i];
			print += ch;
			order += ch;
			kirjoittaja.write(print);
			if (str[i] == '\n') {
				//parser either send new values to motorcontroller trough queue or responds to M10
				if (p.giveGcode(order)) {
					nu = p.getCommand();
					if (xQueueSendToBack(commandQueue, &nu, portMAX_DELAY)) {

					}
				} else {
					char * M10 = new char [pTen->length()+1];
					std::strcpy (M10, pTen->c_str());
					USB_send((uint8_t * )M10, strlen(M10));
					kirjoittaja.write(M10);
					delete[] M10;
				}
				//task sends the ok for a new message from mDraw
				//vTaskDelay(50);
				char k[] = ("OK\r\n");
				USB_send((uint8_t * )k, strlen(k));
				order = "";

			}
			print = "";

		}


	}
}

/* LED1 toggle thread */

/*****************************************************************************
 * Public functions
 ****************************************************************************/
extern "C" {
void RIT_IRQHandler(void) { // This used to check if a context switch is required
	controller->RIT_move();
}
}

/* the following is required if runtime statistics are to be collected */
extern "C" {

void vConfigureTimerForRunTimeStats(void) {
	Chip_SCT_Init(LPC_SCTSMALL1);
	LPC_SCTSMALL1->CONFIG = SCT_CONFIG_32BIT_COUNTER;
	LPC_SCTSMALL1->CTRL_U = SCT_CTRL_PRE_L(255) | SCT_CTRL_CLRCTR_L; // set prescaler to 256 (255 + 1), and start timer
}

}
/* end runtime statictics collection */

/**
 * @brief	main routine for FreeRTOS blinky example
 * @return	Nothing, function should not exit
 */
int main(void) {
	prvSetupHardware();

	directionx = new DigitalIoPins(1, 0, false, false, false);
	stepx = new DigitalIoPins(0, 24, false, false, false);
	limit1 = new DigitalIoPins(1, 3, true, true, true);
	limit2 = new DigitalIoPins(0, 0, true, true, true);

	directiony = new DigitalIoPins(0, 28, false, false, false);
	stepy = new DigitalIoPins(0, 27, false, false, false);
	limit3 = new DigitalIoPins(0, 9, true, true, true);
	limit4 = new DigitalIoPins(0, 29, true, true, true);

	laser = new DigitalIoPins(0, 12, false, false, false);
	laser->write(false);

	// Queues for sending commands to motorcontroller and responding to M10
	commandQueue = xQueueCreate( 5, sizeof( command ) );
	mTen = xQueueCreate( 5, sizeof( std::string *) );
	//controller = new MotorController(*limit1,*limit2,*limit3,*limit4,*directionx,*stepx,*directiony,*stepy);
	// initialize RIT (= enable clocking etc.)
	Chip_RIT_Init(LPC_RITIMER);

	// set the priority level of the interrupt
	// The level must be equal or lower than the maximum priority specified in FreeRTOS config
	// Note that in a Cortex-M3 a higher number indicates lower interrupt priority
	NVIC_SetPriority(RITIMER_IRQn,
			configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);

	xTaskCreate(readmDraw, "readmDraw",
			configMINIMAL_STACK_SIZE + 512, NULL, (tskIDLE_PRIORITY + 1UL),
			(TaskHandle_t *) NULL);

	xTaskCreate(movePen, "movePen",
			configMINIMAL_STACK_SIZE + 512, NULL, (tskIDLE_PRIORITY + 1UL),
			(TaskHandle_t *) NULL);

	xTaskCreate(cdc_task, "CDC",
					configMINIMAL_STACK_SIZE * 3, NULL, (tskIDLE_PRIORITY + 1UL),
					(TaskHandle_t *) NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

