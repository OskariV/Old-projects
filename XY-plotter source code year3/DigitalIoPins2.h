/*
 * DigitalIoPins.h
 *
 *  Created on: 22 Jan 2019
 *      Author: omvir
 */

#ifndef DIGITALIOPINS2_H_
#define DIGITALIOPINS2_H_

class DigitalIoPins {
public:
	DigitalIoPins(int port, int pin, bool input, bool pullup, bool invert);
	DigitalIoPins();
	virtual ~DigitalIoPins();
	bool read();
	void write(bool value);
private:
	int ownPin;
	int ownPort;
	bool isInput;
};

#endif /* DIGITALIOPINS2_H_ */
