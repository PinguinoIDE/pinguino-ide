/*
 * pulseIn() function, for Pinguino 8bits
 * Port by Marcus Fazzi, anunakin@ieee.org 
 * 
 * 17/02/2011, first try
 */
 
#ifndef __PULSE__
#define __PULSE__

#include <typedef.h>
#include <digitalr.c>       // digitalread
 
/**
* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
* or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
* to 3 minutes in length, but must be called at least a few dozen microseconds
* before the start of the pulse. 
*/
u16 pulseIn(u8 pin, u8 state, u16 timeout){
	
	u16 width = 0; // keep initialization out of time critical area
	
	// convert the timeout from microseconds to a number of times through
	u16 numloops = 0;
	u16 maxloops = timeout / 10; //We have a microsecond by 10 loops (mean).
	
	// wait for any previous pulse to end
	while (digitalread(pin) == state)
		if (numloops++ == maxloops)
			return 0;
	
	// wait for the pulse to start
	while (digitalread(pin) != state)
		if (numloops++ == maxloops)
			return 0;
	
	// wait for the pulse to stop
	while (digitalread(pin) == state)
		width++;

	//There will be some error introduced by the interrupt handlers.
	//At last loop, each interaction have 12us + 60us from digitalRead() instructions
	return width * 12 + 60; 
}

#endif
