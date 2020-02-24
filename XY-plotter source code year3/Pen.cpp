/*
 * Pen.cpp
 *
 *  Created on: 30 Sep 2019
 *      Author: Jhenrik
 */

#include "Pen.h"
#include "sct_15xx.h"
#include "chip.h"

//Initialises the pen object and its SCT timer and sets the prescaler
Pen::Pen() {
	// TODO Auto-generated constructor stub
			Chip_SCT_Init(LPC_SCT0);

			LPC_SCT0->CONFIG |= (1 << 17); // two 16-bit timers, auto limit
			LPC_SCT0->CTRL_L |= (72-1) << 5; // set prescaler, SCTimer/PWM clock = 1 MHz

			LPC_SCT0->MATCHREL[0].L = 20000-1; // match 0 @ 10/1MHz = 10 usec (100 kHz PWM freq)
			LPC_SCT0->MATCHREL[1].L = 1500; // match 1 used for duty cycle (in 10 steps)
			LPC_SCT0->EVENT[0].STATE = 0xFFFFFFFF; // event 0 happens in all states
			LPC_SCT0->EVENT[0].CTRL = (1 << 12); // match 0 condition only
			LPC_SCT0->EVENT[1].STATE = 0xFFFFFFFF; // event 1 happens in all states
			LPC_SCT0->EVENT[1].CTRL = (1 << 0) | (1 << 12); // match 1 condition only
			LPC_SCT0->OUT[0].SET = (1 << 0); // event 0 will set SCTx_OUT0
			LPC_SCT0->OUT[0].CLR = (1 << 1); // event 1 will clear SCTx_OUT0
			LPC_SCT0->CTRL_L &= ~(1 << 2); // unhalt it by clearing bit 2 of CTRL reg
			Chip_SWM_MovablePortPinAssign(SWM_SCT0_OUT0_O, 0, 10);
			kirjoittaja.write("Pen initialized");
			penval = 180;
			Penmove(10);

}

//void Pen::Peninit(){
//		}

//scales the given value for pen position and writes it to the timer register
int Pen::Penmove(int pen) {
	uint16_t PenU;
	PenU = abs(pen);

	PenU += (pen * ((2000-1000)/255))+1000;
	LPC_SCT0->MATCHREL[1].L = PenU;
	if (penval != pen){
		vTaskDelay(500);
	}
	penval = pen;
	return pen;
}



Pen::~Pen() {
	// TODO Auto-generated destructor stub
}

