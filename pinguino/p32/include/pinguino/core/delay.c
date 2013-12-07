/*	----------------------------------------------------------------------------
	delay.c
	Régis Blanchot
	----------------------------------------------------------------------------
	Modified JPM 21/12/2010
	- delay_us replaced with Delayus
	- delay_ms replaced with Delayms
	Modified RB 10/01/2011
	- use of GetSystemClock() function to fit to every CPU configuration
	Modified RB 12/02/2011
	- bug fosc/4
	- use of readCP0Count() function
	--------------------------------------------------------------------------*/

#ifndef __DELAY_C
	#define __DELAY_C

#include <typedef.h>
#include <system.c>

/*	----------------------------------------------------------------------------
	Wait us microseconds
	Use CP0 Count register which counts at half the CPU rate
	--------------------------------------------------------------------------*/

void Delayus(u16 us)
{
	// CP0Count counts at half the CPU rate
	u8 Fcp0 = GetSystemClock() / 1000000 / 2;		// max = 40 for 80MHz

	// get start ticks
	u32 start = GetCP0Count();

	// calculate last tick number for the given number of microseconds
	u32 stop = start + us * Fcp0;

	// wait till count reaches the stop value
	if (stop > start)
		while (GetCP0Count() < stop);
	else
		while (GetCP0Count() > start || GetCP0Count() < stop); 
} 

/*	----------------------------------------------------------------------------
	wait ms milliseconds
	--------------------------------------------------------------------------*/

void Delayms(u16 ms)
{
	do
	{
		Delayus(1000); // 1 ms
	}
	while(--ms);
}

#endif	/* __DELAY_C */

