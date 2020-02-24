/*
 * Fmutex.cpp
 *
 *  Created on: 27 Aug 2019
 *      Author: omvir
 */

#include "Fmutex.h"

Fmutex::Fmutex()
{
	mutex = xSemaphoreCreateMutex();
}
Fmutex::~Fmutex()
{
	/* delete semaphore */ /* (not needed if object lifetime is known *  to be infinite) */
}
void Fmutex::lock()
{
	xSemaphoreTake(mutex, portMAX_DELAY);
}
void Fmutex::unlock()
{
	xSemaphoreGive(mutex);
}

