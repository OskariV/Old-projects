/*
 * gParser.h
 *
 *  Created on: 4 Sep 2019
 *      Author: omvir
 */

#ifndef GPARSER_H_
#define GPARSER_H_

#include "ITMwriter2.h"
#include <string>
#include <cmath>

/*Struct which contains the location, pen position and speed of the plotter.
This is sent to the command que everytime one of these values changes.
*/
struct command{
	int movetoX;
	int movetoY;
	int pen;
	int speed;
};

//gParser parses the commands recieved from mDraw and saves the latest changed values
//gParser also constructs commands and sends them to the command que
class gParser {
public:
	gParser();
	virtual ~gParser();
	bool giveGcode(std::string input);
	command getCommand();
	void testCommand(int &xout, int &yout);
private:
	int x;	//coordinate of x as a integer
	int y;	//coordinate of y as a integer
	int speed; //speed of the pen as a integer
	int pen; //integer value for pens position
};

#endif /* GPARSER_H_ */
