//------------------------------------------------------------------------------
// TIMER: Contains all the functions and variables useful to manage a timer
//------------------------------------------------------------------------------

#include <Arduino.h>
#include "timer.h"

//------------------------------------------------------------------------------
// TIMER: Constructors of the class
//------------------------------------------------------------------------------

Timer::Timer(){ }

//------------------------------------------------------------------------------
// START: Saves the current timestamp and the timeout
//------------------------------------------------------------------------------

void Timer::start(unsigned long t){
	startTime = millis();
	timeout = t;
}

//------------------------------------------------------------------------------
// IS TIMEOUT REACHED: Returns true if the the timeout was reached
//------------------------------------------------------------------------------

boolean Timer::isTimeoutReached(){
	if ((millis() - startTime) > timeout)
		return true;
	return false;
}
