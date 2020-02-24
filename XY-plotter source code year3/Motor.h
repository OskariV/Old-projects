/*
 * Motor.h
 *
 *  Created on: 18 Sep 2019
 *      Author: omvir
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "DigitalIoPins2.h"

class Motor {
public:
	Motor();
	Motor(DigitalIoPins dir, DigitalIoPins stepper, DigitalIoPins limit1, DigitalIoPins limit0);
	bool move(bool dir);
	virtual ~Motor();
	void setLocation(int newLocation);
	int getLocation();
private:
	DigitalIoPins direction; //value determines direction of motor
	DigitalIoPins step; //used to move motor by pulsing true the false
	DigitalIoPins limitTrue; //pin for the limit switch reached by moving the motor when direction is true
	DigitalIoPins limitFalse;//pin for the limit switch reached by moving the motor when direction is false
	int location;//holds the current steps from zero
};

#endif /* MOTOR_H_ */
