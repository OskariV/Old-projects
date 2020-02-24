/*
 * DigitalIoPins.cpp
 *
 *  Created on: 22 Jan 2019
 *      Author: omvir
 */

#include "DigitalIoPins2.h"

#include "chip.h"

DigitalIoPins::DigitalIoPins(){

}

DigitalIoPins::DigitalIoPins(int port, int pin, bool input, bool pullup, bool invert)
{
	// TODO Auto-generated constructor stub
	if(input)
	{
		if(pullup && invert)
		{
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
		}else if (pullup && !invert)
		{
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
		}else if (!pullup && invert)
		{
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_DIGMODE_EN | IOCON_INV_EN));
		} else
		{
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_DIGMODE_EN));
		}
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	}else
	{
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, IOCON_MODE_INACT | IOCON_DIGMODE_EN);
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
		Chip_GPIO_SetPinState(LPC_GPIO, port, pin, true);
	}
	ownPin = pin;
	ownPort = port;
	isInput = input;
}

bool DigitalIoPins::read()
{
	return Chip_GPIO_GetPinState(LPC_GPIO, ownPort, ownPin);
}

void DigitalIoPins::write(bool value)
{
	/*
	if (value)
	{
		value = false;
	}else if (!value)
	{
		value = true;
	}
	*/
	if(!isInput)
	{
		Chip_GPIO_SetPinState(LPC_GPIO, ownPort, ownPin, value);
	}
}

DigitalIoPins::~DigitalIoPins() {
	// TODO Auto-generated destructor stub
}

