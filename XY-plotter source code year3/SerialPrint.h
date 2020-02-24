/*
 * SerialPrint.h
 *
 *  Created on: 27 Aug 2019
 *      Author: omvir
 */

#ifndef SERIALPRINT_H_
#define SERIALPRINT_H_

#include <mutex>
#include "Fmutex.h"
#include "board.h"

class SerialPrint {
public:
	SerialPrint();
	virtual ~SerialPrint();
	void printToSerial(std::string input);
private:
	Fmutex guard;
};

#endif /* SERIALPRINT_H_ */
