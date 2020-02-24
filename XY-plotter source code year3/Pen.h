/*
 * Pen.h
 *
 *  Created on: 30 Sep 2019
 *      Author: Jhenrik
 */

#include "DigitalIoPins2.h"
#include "MotorController.h"
#include "ITMwriter2.h"
#include "gParser.h"

#ifndef PEN_H_
#define PEN_H_

class Pen {
public:
	Pen();
	virtual ~Pen();
	void Peninit();
	int Penmove(int pen);

private:
	ITMwriter kirjoittaja;
	int penval;

};

#endif /* PEN_H_ */
