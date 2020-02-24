/*
 * Motor.cpp
 *
 *  Created on: 18 Sep 2019
 *      Author: omvir
 */

#include "Motor.h"
Motor::Motor(){

}

//creates the motor and links the correct pins to motors steppers, direction and limit switches
Motor::Motor(DigitalIoPins dir, DigitalIoPins stepper, DigitalIoPins limit1, DigitalIoPins limit0) {
	// TODO Auto-generated constructor stub
	direction = dir;
	step = stepper;
	limitTrue = limit1;
	limitFalse = limit0;
	location = 0;

}

//moves the motor one step to the direction of bool dir
//also keeps track of steps moved, increasing count when moving from zero and decreasing when reaching zero
//writes true multiple times to lengthen the pulse so the motor can read it.
//cannot move if limit of the motors current direction is pressed
bool Motor::move(bool dir) {
	direction.write(dir);
	if (dir) {
		if (!limitTrue.read()) {
			step.write(true);
			step.write(true);
			step.write(true);
			step.write(true);
			step.write(true);
			step.write(true);
			step.write(false);
			location--;
			return true;
		}
	} else {
		if (!limitFalse.read()) {
			step.write(true);
			step.write(true);
			step.write(true);
			step.write(true);
			step.write(true);
			step.write(true);
			step.write(false);
			location++;
			return true;
		}
	}
	return false;
}

//sets the location when calibrating motors and motorcontroller
void Motor::setLocation(int newLocation) {
	location = newLocation;
}

//gets the current distance of the motor from zero
int Motor::getLocation() {
	return location;
}

Motor::~Motor() {
	// TODO Auto-generated destructor stub
}

