#include "MarketActive.h"


//Changes the state of the objects bool member
void MarketActive::changeState()
{
	if (isActive == true)
	{
		isActive = false;
	}
	else {
		isActive = true;
	}
}

//Returns the state of the objects bool member
bool MarketActive::getState()
{
	return isActive;
}

//Creates the object with it's bool member as true so that the thread can function
MarketActive::MarketActive()
{
	isActive = true;
}


MarketActive::~MarketActive()
{
}
