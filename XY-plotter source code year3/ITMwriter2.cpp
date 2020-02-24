/*
 * ITMwriter.cpp
 *
 *  Created on: 25 Jan 2019
 *      Author: omvir
 */

#include "ITMwriter2.h"

ITMwriter::ITMwriter() {
	// TODO Auto-generated constructor stub
	ITM_init();
}

void ITMwriter::write(std::string input)
{
	ITM_write(input.c_str());
}
void ITMwriter::write(const char* in)
{
	ITM_write(in);
}

void ITMwriter::write(int input)
{
	std::string temp = printing + std::to_string(input) + "\n";
	ITM_write(temp.c_str());
}

ITMwriter::~ITMwriter() {
	// TODO Auto-generated destructor stub
}

