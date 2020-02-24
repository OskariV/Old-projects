/*
 * MotorController.cpp
 *
 *  Created on: 25 Sep 2019
 *      Author: omvir
 */

#include "MotorController.h"


/*
 * function for starting the interrupt timer for motor movement. Takes in the direction of the motors,
 * and the amount of steps they need to move and the speed of the motors
 */
void MotorController::RIT_start(int countx,int county, bool xd, bool xy,int us) {
	uint64_t cmp_value;

// Determine approximate compare value based on clock rate and passed interval
	cmp_value = (uint64_t) Chip_Clock_GetSystemClockRate() * (uint64_t) (1000000/us)
			/ 1000000;

// disable timer during configuration
	Chip_RIT_Disable(LPC_RITIMER);

	ritON = true;
	dirX = xd;
	dirY = xy;


	RIT_deltay = -county;
	RIT_deltax = countx;


	RIT_zerox = 0;
	RIT_zeroy = 0;
	RIT_movex = 0;
	RIT_movey = 0;
	RIT_p = countx + county;


// enable automatic clear on when compare value==timer value
// this makes interrupts trigger periodically
	Chip_RIT_EnableCompClear(LPC_RITIMER);
// reset the counter
	Chip_RIT_SetCounter(LPC_RITIMER, 0);
	Chip_RIT_SetCompareValue(LPC_RITIMER, cmp_value);
// start counting
	Chip_RIT_Enable(LPC_RITIMER);
// Enable the interrupt signal in NVIC (the interrupt controller)
	NVIC_EnableIRQ(RITIMER_IRQn);

// wait for ISR to tell that we're done
	if (xSemaphoreTake(sbRIT, portMAX_DELAY) == pdTRUE) {
		// Disable the interrupt signal in NVIC (the interrupt controller)
		NVIC_DisableIRQ(RITIMER_IRQn);
	} else {
		// unexpected error
	}
	kirjoittaja.write("motor x "+std::to_string(mx.getLocation())+"motor y "+std::to_string(my.getLocation())+ "delta x "+std::to_string(RIT_deltax)+  "delta y "+std::to_string(RIT_deltay)+ "\n\r");
}

/*
 * Function that calculates the amount of steps, direction and speed of the motors and position of the pen.
 * It sets the speed for the motors from a given value and calculates the distance and direction by comparing
 * new values to the location of the motors. It also passes the position value to the pen.
 */
void MotorController::move(int xNewPos,int yNewPos, int speed, int penPos) {

	bool xdire;
	bool ydire;

	//gets the current location from zero
	int x = mx.getLocation();
	int y = my.getLocation();

	//scales the value from mm to steps
	xNewPos = xNewPos * (xLenght)/(xPhysLenght);
	yNewPos = yNewPos * (yLenght)/(yPhysLenght);

	//determines the direction of the motors
	if (xNewPos < x) {
		xdire = true;
	} else {
		xdire = false;
	}

	if (yNewPos < y) {
		ydire = true;
	} else {
		ydire = false;
	}

	//calculates the steps needed to reach new position
	xNewPos = abs(xNewPos-x);
	yNewPos = abs(yNewPos-y);

	//sets pen and starts the interrupt driven motor movement
	pen.Penmove(penPos);
	RIT_start(xNewPos,yNewPos, xdire, ydire,speed);
}


//Function that is called in the interrupt to move the motors
//Uses Bresenham's algorithm to calculate movement to draw lines at a angle
void MotorController::RIT_move() {
	portBASE_TYPE xHigherPriorityWoken = pdFALSE;
	// Tell timer that we have processed the interrupt.
	// Timer then removes the IRQ until next match occurs
	Chip_RIT_ClearIntStatus(LPC_RITIMER);
	// clear IRQ flag

	//moves motor until both x and y have moved the set amount
	if (ritON) {
		//calculates a movement value for the motors, moves the motors
		//and calculates again until the total movement calculated in the move() function is reached
		if (RIT_movex == RIT_zerox && RIT_movey == RIT_zeroy) {
			if (RIT_zerox >= RIT_deltax && RIT_zeroy <= RIT_deltay) {
				ritON = false;
			}

			int e2 = 2 * RIT_p;

			if (e2 <= RIT_deltax) {
				RIT_p += RIT_deltax; /* e_xy+e_x > 0 */

				RIT_zeroy--;

			}
			if (e2 >= RIT_deltay) {/* e_xy+e_y < 0 */
				RIT_p += RIT_deltay;

				RIT_zerox++;

			}
		}else{
			//moves motors until a calculated value is reached
			//x is limited also by the the total movement of the motor to minimise drift on the x axis
			if(RIT_movex != RIT_zerox){

				RIT_movex++;
				if (RIT_deltax != 0 && RIT_zerox <= RIT_deltax) {
					mx.move(dirX);
				}
			}
			if(RIT_movey != RIT_zeroy){

				RIT_movey--;
				my.move(dirY);
			}
		}
	}

	else {
		Chip_RIT_Disable(LPC_RITIMER);
		// disable timer
		// Give semaphore and set context switch flag if a higher priority task was woken up
		xSemaphoreGiveFromISR(sbRIT, &xHigherPriorityWoken);
	}
	// End the ISR and (possibly) do a context switch
	portEND_SWITCHING_ISR(xHigherPriorityWoken);

}

//Finds the correct switches for a motor
Motor MotorController::setLimit(DigitalIoPins limit1, DigitalIoPins limit2,
		DigitalIoPins limit3, DigitalIoPins limit4, DigitalIoPins directionX,
		DigitalIoPins stepX) {
	bool limitTfound = true;
	bool limitFfound = true;
	//bool step = true;

	DigitalIoPins limitT;
	DigitalIoPins limitF;
	directionX.write(true);

	//Finds the correct limit for when the motor is driven to true direction
	while (limitTfound) {
		if (limit1.read()) {
			limitT = limit1;
			limitTfound = false;
		} else if (limit2.read()) {
			limitT = limit2;
			limitTfound = false;
		} else if (limit3.read()) {
			limitT = limit3;
			limitTfound = false;
		} else if (limit4.read()) {
			limitT = limit4;
			limitTfound = false;
		} else {

			stepX.write(true);
			stepX.write(true);
			stepX.write(true);
			stepX.write(true);
			stepX.write(false);

			vTaskDelay(1);
		}
	}


	directionX.write(false);
	//Drives the motor so that the limit is not pressed
	//step = true;
	for (int var = 0; var < 400; ++var) {
		stepX.write(true);
		stepX.write(true);
		stepX.write(true);
		stepX.write(true);
		stepX.write(false);

		vTaskDelay(1);
	}
	//Finds the correct limit for when the motor is driven to false direction
	while (limitFfound) {
		if (limit1.read()) {
			limitF = limit1;
			limitFfound = false;
		} else if (limit2.read()) {
			limitF = limit2;
			limitFfound = false;
		} else if (limit3.read()) {
			limitF = limit3;
			limitFfound = false;
		} else if (limit4.read()) {
			limitF = limit4;
			limitFfound = false;
		} else {
			stepX.write(true);
			stepX.write(true);
			stepX.write(true);
			stepX.write(true);
			stepX.write(false);

			vTaskDelay(1);
		}

	}
	//Creates the motor object
	Motor motor(directionX,stepX,limitT,limitF);
	//Drives the motor so that the limit is not pressed
	for (int var = 0; var < 300; ++var) {
		motor.move(true);
		vTaskDelay(1);
	}
	return motor;
}
/*
 * Finds the amount of steps each motor can travel
 * Also moves the motor to X0 Y0 and sets the motors location to zero
 */
void MotorController::calibrateDistance(int &lenY, int &lenX) {
	bool foundY = true;
	bool foundX = true;

	lenY = 0;
	lenX = 0;

	//moves the motors to the max they can move
	while (my.move(false) || mx.move(false)) {
		vTaskDelay(1);
	}

	//moves the motors to zero and calculates the steps between max and zero
	while (foundY || foundX) {
		if (my.move(true)) {
			++lenY;
		} else {
			foundY = false;
		}
		if (mx.move(true)) {
			++lenX;
		} else {
			foundX = false;
		}
		vTaskDelay(1);
	}
	//sets motors locations
	mx.setLocation(0);
	my.setLocation(0);
	return;
}

//returns the dimensions of the drawing area in steps and mm
void MotorController::getDistance(int &lenY, int &lenX, int &plenY, int &plenX) {
	lenY = yLenght;
	lenX = xLenght;
	plenY = yPhysLenght;
	plenX = xPhysLenght;
}

//returns the position of the motors from zero
void MotorController::getPosition(int &pY, int &pX) {
	pY = my.getLocation();
	pX = mx.getLocation();
}

//returns the response to M10
std::string MotorController::getTen() {

	std::string ten = "M10 XY " + std::to_string(xPhysLenght / 100) + " "
			+ std::to_string(yPhysLenght / 100)
			+ " 0.00 0.00 A0 B0 H0 S80 U10 D100\r\n";

	return ten;
}

//Creation of the motor which uses the calibrate and setlimit functions and creates motors and pen
MotorController::MotorController(DigitalIoPins limit1, DigitalIoPins limit2,
		DigitalIoPins limit3, DigitalIoPins limit4, DigitalIoPins directionX,
		DigitalIoPins stepX, DigitalIoPins directionY, DigitalIoPins stepY) {

	kirjoittaja = ITMwriter();
	pen = Pen();
	mx = setLimit(limit1,limit2,limit3,limit4,directionX,stepX);
	my = setLimit(limit1,limit2,limit3,limit4,directionY,stepY);
	calibrateDistance(yLenght, xLenght);
	sbRIT = xSemaphoreCreateBinary();
	xPhysLenght = 30000;
	yPhysLenght = 35000;
	ritON = false;

}

MotorController::~MotorController() {
	// TODO Auto-generated destructor stub
}

