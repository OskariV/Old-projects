/*
 * gParser.cpp
 *
 *  Created on: 4 Sep 2019
 *      Author: omvir
 */

#include "gParser.h"

//creates the parser with a default location, max speed and pen position of UP
gParser::gParser() {
	// TODO Auto-generated constructor stub
	x = 0;
	y = 0;
	speed = 4000;
	pen = 180;
}
//deletes parser
gParser::~gParser() {
	// TODO Auto-generated destructor stub
}
/*parses commands received as a string
 * reads the string for a command and updates values for coordinates, speed and pen position
 * in the case of command M10 responds with sending required information.
 */
bool gParser::giveGcode(std::string input) {

	bool success = false;
	int whichCommand = 0;

	//All the commands parser recognises
	std::size_t foundG1 = input.find("G1");
	std::size_t foundG28 = input.find("G28");
	std::size_t foundM1 = input.find("M1");
	std::size_t foundM2 = input.find("M2");
	std::size_t foundM4 = input.find("M4");
	std::size_t foundM5 = input.find("M5");
	std::size_t foundM10 = input.find("M10");
	std::size_t foundM11 = input.find("M11");

	//used to isolate values from command strings
	std::size_t foundA = input.find("A0");
	std::size_t foundX = input.find("X");
	std::size_t foundY = input.find("Y");
	std::size_t foundend = input.find("\n");

	if (foundG1 != std::string::npos) {
		whichCommand = 1;
	} else if (foundG28 != std::string::npos) {
		whichCommand = 2;
	} else if (foundM10 != std::string::npos) {
		whichCommand = 7;
	} else if (foundM2 != std::string::npos) {
		whichCommand = 4;
	} else if (foundM4 != std::string::npos) {
		whichCommand = 5;
	} else if (foundM5 != std::string::npos) {
		whichCommand = 6;
	} else if (foundM11 != std::string::npos) {
		whichCommand = 8;
	} else if (foundM1 != std::string::npos) {
		whichCommand = 3;
	}

	switch (whichCommand) {
	case 1: //sends new coordinates for the motors
		if (foundG1 != std::string::npos && foundX != std::string::npos && foundY != std::string::npos) {
			std::size_t XtoY = foundY - foundX;
			std::size_t YtoA = foundA - foundY;

			std::string Xstring = input.substr(foundX + 1, XtoY - 2);
			std::string Ystring = input.substr(foundY + 1, YtoA - 2);

			float xRaw = std::stof(Xstring);
			float yRaw = std::stof(Ystring);

			xRaw = xRaw * 100;
			yRaw = yRaw * 100;

			x = xRaw;
			y = yRaw;

			success = true;
		} else {
			success = false;
		}
		break;
	case 2: // moves motors to x 0 y 0 and sets the pen up
		x = 0;
		y = 0;
		pen = 10;
		success = true;
		break;
	case 3: // sends new position for pen
		if (foundM1 != std::string::npos && foundend != std::string::npos) {
			std::string penstring = input.substr(foundM1 + 2, foundend - 1);
			pen = std::stoi(penstring);
			success = true;
		}else{
			success = false;
		}
		break;
	case 4:
		success = true;
		break;
	case 5:
		success = true;
		break;
	case 6: // sends a new speed value for motors
		if (foundM5 != std::string::npos && foundend != std::string::npos) {
			std::size_t foundS = input.find("S");
			std::string speedPercent = input.substr(foundS + 1, foundend - 1);
			speed = 40 * (std::stoi(speedPercent));
			success = true;
		} else {
			success = false;
		}
		break;
	case 7: // responds to the M10 commands
		success = false;
		break;
	case 8:
		success = true;
		break;
	default:
		success = false;
		break;
	}

	return success;

}

//send a command struct for the motors
command gParser::getCommand() {
	command nu;
	nu.movetoX = x;
	nu.movetoY = y;
	nu.speed = speed;
	nu.pen = pen;
	return nu;
}

//command used for testing functionality
void gParser::testCommand(int &xout, int &yout) {
	xout = x;
	yout = y;
}
