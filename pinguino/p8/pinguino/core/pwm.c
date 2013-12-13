/*	----------------------------------------------------------------------------
	FILE:			pwm.c
	PROJECT:		pinguino
	PURPOSE:		new hardware PWM control functions
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	10 oct. 2010
	LAST RELEASE:	27 apr. 2013
	----------------------------------------------------------------------------
	freely adapted from JAL PWM Control Library.
	----------------------------------------------------------------------------
    Changelog :
    * 12 Feb. 2013  regis blanchot - replaced duty cycle calculation
    * 27 Apr. 2013  regis blanchot - moved pin definition to pin.h
                                     renamed function (also in pwm.pdl)
                                     CCPR1L = duty; -> CCPR1L = duty & 0xFF;
    * 26 Jun. 2013  regis blanchot - fixed PWM_setDutyCycle()
	----------------------------------------------------------------------------
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	--------------------------------------------------------------------------*/

#ifndef __PWM__
#define __PWM__

#include <pic18fregs.h>
#include <typedef.h>
#include <pin.h>
#include <digitalw.c>
#include <oscillator.c>
//#include <interrupt.c>    // to save memory space

/*  --------------------------------------------------------------------
    GLOBAL VARIABLES
    ------------------------------------------------------------------*/

u16 _pr2_plus1 = 256;				// shadow value of PR2 set to max. + 1 
u8  _t2con;							// shadow value of T2CON

/*  --------------------------------------------------------------------
    PWM_setFrequency
    --------------------------------------------------------------------
    @param:	frequency in hertz (range 3kHz .. 12MHz)
    --------------------------------------------------------------------
    let's say p = TMR2 Prescale Value
    PWM Period 	= [(PR2) + 1] * 4 * TOSC * p
    so [(PR2) + 1] = PWM Period / (4 * TOSC * p)
    but PWM Period 	= 1 / PWM Frequency
    so [(PR2) + 1] = (1/PWM Frequency) / (4 * 1/FOSC * p)
    and [(PR2) + 1] = FOSC / (4 * PWM Frequency * p)
    then [(PR2) + 1] = FOSC / PWM Frequency / 4 / p
    --------------------------------------------------------------------
    When TMR2 (TMR4) is equal to PR2 (PR4) the CCPx pin is set
    ------------------------------------------------------------------*/

void PWM_setFrequency(u32 freq)
{
    // PR2+1 calculation
    _pr2_plus1 = System_getPeripheralFrequency() / freq;	// FOSC / (4 * PWM Frequency)

    // Timer2 prescaler calculation
    // PR2 max value is 255, so PR2+1 max value is 256
    // highest prescaler value is 16
    // 16 * 256 = 4096 so :
    if (_pr2_plus1 <= 4096)					// check if it's not too high
    {
        if (_pr2_plus1 <= 256)				// no needs of any prescaler
        {
            _t2con = 0b00000100;			// prescaler is 1, Timer2 On
        }
        else if (_pr2_plus1 <= 1024)		// needs prescaler 1:4
        {
            _pr2_plus1 = _pr2_plus1 >> 2;	// divided by 4
            _t2con = 0b00000101;			// prescaler is 4, Timer2 On
        }
        else								// needs prescaler 1:6
        {
            _pr2_plus1 = _pr2_plus1 >> 4;	// divided by 16
            _t2con = 0b00000110;			// prescaler is 16, Timer2 On
        }
    }
}

/*  --------------------------------------------------------------------
    PWM_setDutyCycle
    --------------------------------------------------------------------
    Set dutycycle with 10-bits resolution, allowing 1024 PWM steps.
    The 'duty' argument is a (max) 10-bits absolute value for the duty cycle:
    * duty<1:0> are the 2 LSbits
    * duty<9:2> are the 8 MSbits
    Allowed range: 0..1023
    @param pin:		CCPx pin where buzzer is connected (5 or 6)
    @param duty:	10-bit duty cycle
    --------------------------------------------------------------------
    PWM Duty Cycle = (CCPRxL:CCPxCON<5:4>) * Tosc * TMR2 prescaler
    (CCPRxL:CCPxCON<5:4>) = PWM Duty Cycle / (Tosc * TMR2 prescaler)

    CCPxCON = 0b00001100 => PWM mode
                            PxA and PxC are active-high;
                            PxB and PxD are active-high
    ------------------------------------------------------------------*/

void PWM_setDutyCycle(u8 pin, u16 duty)
{
    u8 temp;

    if (duty > 1023) duty = 1023;		// upper limit (10-bit)

    // 1- Set the PWM period by writing to the PR2 (PR4) register.

    PR2 = _pr2_plus1 - 1;				// set PWM period

    #if defined(__18f26j53) || defined(__18f46j53) || \
        defined(__18f27j53) || defined(__18f47j53)

    CCPTMRS1 = 0;                       // assign Timer2 to all CCP pins
    CCPTMRS2 = 0;

    #endif
    
    // 2- Set the PWM duty cycle by writing to the CCPRxL register and
    // CCPxCON<5:4> bits.

    switch (pin)
    {
        #if defined(__18f26j53) || defined(__18f46j53) || \
            defined(__18f27j53) || defined(__18f47j53)

        case CCP4:
            CCP4CON  = 0b00001100;
            CCPR4L   = ( duty >> 2 ) & 0xFF; // 8 LSB
            CCP4CON |= (duty & 0x300) >> 4;  // 2 MSB in <5:4>
            break;

        case CCP5:
            CCP5CON  = 0b00001100;
            CCPR5L   = ( duty >> 2 ) & 0xFF; // 8 LSB
            CCP5CON |= (duty & 0x300) >> 4;  // 2 MSB in <5:4>
            break;

        case CCP6:
            CCP6CON  = 0b00001100;
            CCPR6L   = ( duty >> 2 ) & 0xFF; // 8 LSB
            CCP6CON |= (duty & 0x300) >> 4;  // 2 MSB in <5:4>
            break;

        case CCP7:
            CCP7CON  = 0b00001100;
            CCPR7L   = ( duty >> 2 ) & 0xFF; // 8 LSB
            CCP7CON |= (duty & 0x300) >> 4;  // 2 MSB in <5:4>
            break;

        case CCP8:
            CCP8CON  = 0b00001100;
            CCPR8L   = ( duty >> 2 ) & 0xFF; // 8 LSB
            CCP8CON |= (duty & 0x300) >> 4;  // 2 MSB in <5:4>
            break;

        case CCP9:
            CCP9CON  = 0b00001100;
            CCPR9L   = ( duty >> 2 ) & 0xFF; // 8 LSB
            CCP9CON |= (duty & 0x300) >> 4;  // 2 MSB in <5:4>
            break;

        case CCP10:
            CCP10CON  = 0b00001100;
            CCPR10L   = ( duty >> 2 ) & 0xFF; // 8 LSB
            CCP10CON |= (duty & 0x300) >> 4;  // 2 MSB in <5:4>
            break;

        #else

        case CCP1:
            CCP1CON  = 0b00001100;
            CCPR1L   = ( duty >> 2 ) & 0xFF; // 8 LSB
            CCP1CON |= (duty & 0x300) >> 4;  // 2 MSB in <5:4>
            break;

        case CCP2:
            CCP2CON  = 0b00001100;
            CCPR2L   = ( duty >> 2 ) & 0xFF; // 8 LSB
            CCP1CON |= (duty & 0x300) >> 4;  // 2 MSB in <5:4>
            break;

        #endif
            
        default:
            #ifdef DEBUG
                #error "Invalid CCPx Pin"
            #endif
    }

    // 3- Make the CCPx pin an output by clearing the appropriate TRIS bit.

    pinmode(pin, OUTPUT);				// PWM pin as OUTPUT

    // 4- Set the TMR2 prescale value, then enable Timer2 by writing to T2CON

    T2CON = _t2con;						// Timer2 prescaler + ON

    // 5- Configure the CCPx module for PWM operation

    //PIR1bits.TMR2IF = 0;				// Reset interrupt flag
}

/*  --------------------------------------------------------------------
    PWM_setPercentDutyCycle
    --------------------------------------------------------------------
    Set a percentage duty cycle, allowing max 100 PWM steps.
    Allowed range: 0..100
    The duty cycle will be set to the specified percentage of the maximum
    for the current PWM frequency.
    Note: The number of available PWM steps can be lower than 100 with
    (very) high PWM frequencies.
    ------------------------------------------------------------------*/

void PWM_setPercentDutyCycle(u8 pin, u8 percent)
{
    u16 duty;

    if (percent == 0)
        duty = 0;
    else if (percent >= 100)
        duty = _pr2_plus1 - 1;
    else
        duty = percent * (_pr2_plus1 / 4) / 25;	// (factor PR2/100)

    PWM_setDutyCycle(pin, duty << 2);
}

/*  --------------------------------------------------------------------
    pwm_interrupt
    --------------------------------------------------------------------
    ------------------------------------------------------------------*/
/*
void pwm_interrupt()
{
    if (PIR1bits.TMR2IF)
        PIR1bits.TMR2IF = 0;
}
*/
#endif /* __PWM__ */
