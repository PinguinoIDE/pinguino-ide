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
    - use of Core Timer (GetCP0Count() function)
    Modified RB 29/11/2014
    - fixed unexpected stop when CP0 overroll
    --------------------------------------------------------------------------*/

#ifndef __DELAY_C
#define __DELAY_C

#include <typedef.h>
#include <system.c>

/*	--------------------------------------------------------------------
    Wait us microseconds
    Uses CP0 Count register which counts at half the CPU rate
    ------------------------------------------------------------------*/

void Delayus(u32 us)
{
    // CP0Count counts at half the CPU rate
    u32 Fcp0 = GetSystemClock() / 1000000 / 2;		// max = 40 for 80MHz

    // calculate last tick number for the given number of microseconds
    u32 stop = GetCP0Count() + us * Fcp0;

    // wait until count reaches the stop value
    while ((int) (GetCP0Count() - stop) < 0);
} 

/*	--------------------------------------------------------------------
    Wait ms milliseconds
    ------------------------------------------------------------------*/

void Delayms(u32 ms)
{
    do
    {
        Delayus(996); // 1 ms
    }
    while(--ms);
}

#endif	/* __DELAY_C */
