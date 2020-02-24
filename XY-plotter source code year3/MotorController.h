/*
 * MotorController.h
 *
 *  Created on: 25 Sep 2019
 *      Author: omvir
 */

#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_

#include "FreeRTOS.h"
#include "task.h"
#include "DigitalIoPins2.h"
#include "Motor.h"
#include "semphr.h"
#include "ITMwriter2.h"
#include "Pen.h"

#include <mutex>


class MotorController {
public:
	MotorController(DigitalIoPins limit1, DigitalIoPins limit2,
			DigitalIoPins limit3, DigitalIoPins limit4,
			DigitalIoPins directionX, DigitalIoPins stepX,
			DigitalIoPins directionY, DigitalIoPins stepY);
	virtual ~MotorController();
	Motor setLimit(DigitalIoPins limit1, DigitalIoPins limit2,
			DigitalIoPins limit3, DigitalIoPins limit4, DigitalIoPins directionX,
			DigitalIoPins stepX);
	void calibrateDistance(int &lenY, int &lenX);
	std::string getTen();


	void getDistance(int &lenY, int &lenX, int &plenY, int &plenX);
	void getPosition(int &lenY, int &lenX);


	void move(int xNewPos,int yNewPos, int speed, int penPos);
	void RIT_start(int countx, int county, bool xd, bool xy,int us);
	void RIT_move();

private:
	Motor mx;//moves in x axis
	Motor my;//moves in y axis
	Pen pen;//Handles pen movement
	SemaphoreHandle_t sbRIT;//semaphorehandler for interrupts
	ITMwriter kirjoittaja;

	volatile int RIT_xd;
	volatile int RIT_yd;
	volatile int RIT_p;//used in Bresenham's line drawing algorithm
	volatile int RIT_zerox;//used in Bresenham's line drawing algorithm,intermediate value for Bresenham's
	volatile int RIT_zeroy;//used in Bresenham's line drawing algorithm,intermediate value for Bresenham's
	volatile int RIT_deltax;//used in Bresenham's line drawing algorithm, the total movement to new position
	volatile int RIT_deltay;//used in Bresenham's line drawing algorithm, the total movement to new position
	volatile int RIT_movex;//used in Bresenham's line drawing algorithm,intermediate value for Bresenham's
	volatile int RIT_movey;//used in Bresenham's line drawing algorithm,intermediate value for Bresenham's

	int xLenght;//size of x axis in steps
	int yLenght;//size of y axis in steps

	int xPhysLenght;//size of x axis in mm
	int yPhysLenght;//size of y axis in mm

	bool dirX;//direction for x axis
	bool dirY;//direction for y axis
	bool ritON;//value that handles the interrupt for movement

};

#endif /* MOTORCONTROLLER_H_ */
