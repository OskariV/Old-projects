/*
 * SerialPrint.cpp
 *
 *  Created on: 27 Aug 2019
 *      Author: omvir
 */

#include "SerialPrint.h"

SerialPrint::SerialPrint() {
	// TODO Auto-generated constructor stub

}

SerialPrint::~SerialPrint() {
	// TODO Auto-generated destructor stub
}

void SerialPrint::printToSerial(std::string input){
	std::lock_guard<Fmutex> lock(guard);
	Board_UARTPutSTR(input.c_str());
}
