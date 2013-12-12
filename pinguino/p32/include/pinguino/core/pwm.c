/*	----------------------------------------------------------------------------
	FILE:			pwm.c
	PROJECT:		pinguinoX
	PURPOSE:		hardware PWM control functions
	PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
					regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	20 feb. 2011
	LAST RELEASE:	08 dec. 2011
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

// 08 dec. 2011 fixed _pr3_plus1 compute in pwm_set_frequency
// fixed pwm_set_percent_dutycycle
// added resolution output to set_frequency function
// jp.mandon

// 25 feb. 2012 added PIC32_PINGUINO_220 support - regis blanchot
// 17 mar. 2012 [hgmvanbeek@gmail.com] added support for PIC32_PINGUINO_MICRO

#ifndef __PWM__
#define __PWM__

#include <typedef.h>
#include <macro.h>
#include <const.h>
#include <system.c>
#include <digitalw.c>

/*	----------------------------------------------------------------------------
	GLOBAL VARIABLES
	--------------------------------------------------------------------------*/

u32 _pr3_plus1 = 0xffff + 1;	// shadow value of PR3 set to max. + 1 
u32 _t3con;							// shadow value of T3CON

/*	----------------------------------------------------------------------------
	init_pwm
	PWM use Timer 3 with a fixed value of 20 khz
	resolution is 10 bits.
	----------------------------------------------------------------------------
	OCxCON (1 <= x <= 5) Register Config :
	- PWM mode with Fault pin disabled
	- Timer3 is the clock source
	- OCxR<15:0> and OCxRS<15:0> are used for comparisons to the 16-bit timer source
 --------------------------------------------------------------------------*/
	
void PWM_init(void)
{
	T3CON|=0x8010;		// timer 3 use a 1:4 prescaler for PWM 20 khz
	PR3=1023;			// resolution is 10 bits ( 0..1023 )
}

u8 analogwrite(u8 pin, u16 setpoint)
{
#if defined(PIC32_PINGUINO_220)
	switch (pin)
	{
		case 2:
			pinmode(2, OUTPUT);
			OC3CON=0;		// PWM Off
			OC3R=setpoint;	// Timer3 will be compared to this values
			OC3RS=setpoint;
			OC3CON=0x000E;
			OC3CON|=0x8000;	// PWM On
			return 1;
		case 3:
			pinmode(3, OUTPUT);
			OC4CON=0;		// PWM Off
			OC4R=setpoint;	// Timer3 will be compared to this values
			OC4RS=setpoint;
			OC4CON=0x000E;
			OC4CON|=0x8000;	// PWM On
			return 1;
		case 11:
			pinmode(11, OUTPUT);
			OC2CON=0;		// PWM Off
			OC2R=setpoint;	// Timer3 will be compared to this values
			OC2RS=setpoint;
			OC2CON=0x000E;
			OC2CON|=0x8000;	// PWM On
			return 1;
		case 12:
			pinmode(12, OUTPUT);
			OC5CON=0;		// PWM Off
			OC5R=setpoint;	// Timer3 will be compared to this values
			OC5RS=setpoint;
			OC5CON=0x000E;
			OC5CON|=0x8000;	// PWM On
			return 1;
		case 13:
			pinmode(13, OUTPUT);
			OC1CON=0;		// PWM Off
			OC1R=setpoint;	// Timer3 will be compared to this values
			OC1RS=setpoint;
			OC1CON=0x000E;
			OC1CON|=0x8000;	// PWM On
			return 1;
		default:
			return 0;
	}
#endif

#if defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
	switch (pin)
	{
		case 1:// PWM4
			pinmode(1, OUTPUT);
			OC3CON=0;		// PWM Off
			OC3R=setpoint;	// Timer3 will be compared to this values
			OC3RS=setpoint;
			OC3CON=0x000E;
			OC3CON|=0x8000;	// PWM On
			return 1;
		case 2:// PWM3
			pinmode(2, OUTPUT);
			OC4CON=0;		// PWM Off
			OC4R=setpoint;	// Timer3 will be compared to this values
			OC4RS=setpoint;
			OC4CON=0x000E;
			OC4CON|=0x8000;	// PWM On
			return 1;
		case 6:// PWM2
			pinmode(6, OUTPUT);
			OC2CON=0;		// PWM Off
			OC2R=setpoint;	// Timer3 will be compared to this values
			OC2RS=setpoint;
			OC2CON=0x000E;
			OC2CON|=0x8000;	// PWM On
			return 1;
		case 7:// PWM1
			pinmode(7, OUTPUT);
			OC5CON=0;		// PWM Off
			OC5R=setpoint;	// Timer3 will be compared to this values
			OC5RS=setpoint;
			OC5CON=0x000E;
			OC5CON|=0x8000;	// PWM On
			return 1;
		case 8:// PWM0
			pinmode(8, OUTPUT);
			OC1CON=0;		// PWM Off
			OC1R=setpoint;	// Timer3 will be compared to this values
			OC1RS=setpoint;
			OC1CON=0x000E;
			OC1CON|=0x8000;	// PWM On
			return 1;
		default:
			return 0;
	}
#endif
	
#if defined(PIC32_PINGUINO_MICRO)
	switch (pin)
	{
		case 10:
			pinmode(pin, OUTPUT);
			OC2CON=0;		// PWM Off
			OC2R=setpoint;	// Timer3 will be compared to this values
			OC2RS=setpoint;
			OC2CON=0x000E;
			OC2CON|=0x8000;	// PWM On
			return 1;
		case 11:
			pinmode(pin, OUTPUT);
			OC3CON=0;		// PWM Off
			OC3R=setpoint;	// Timer3 will be compared to this values
			OC3RS=setpoint;
			OC3CON=0x000E;
			OC3CON|=0x8000;	// PWM On
			return 1;
		case 12:
			pinmode(pin, OUTPUT);
			OC4CON=0;		// PWM Off
			OC4R=setpoint;	// Timer3 will be compared to this values
			OC4RS=setpoint;
			OC4CON=0x000E;
			OC4CON|=0x8000;	// PWM On
			return 1;
		case 13:
			pinmode(pin, OUTPUT);
			OC5CON=0;		// PWM Off
			OC5R=setpoint;	// Timer3 will be compared to this values
			OC5RS=setpoint;
			OC5CON=0x000E;
			OC5CON|=0x8000;	// PWM On
			return 1;
		default:
			return 0;
	}
#endif

#if defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_OTG)
	switch (pin)
	{
		case 2:
			TRISDSET=0x10;
			TRISDCLR=0x01;
			OC1CON=0;
			OC1R=setpoint;
			OC1RS=setpoint;
			OC1CON=0x000E;
			OC1CON|=0x8000;
			return 1;
			break;
		case 1:
			TRISDCLR=0x08;
			OC4CON=0;
			OC4R=setpoint;
			OC4RS=setpoint;
			OC4CON=0x000E;
			OC4CON|=0x8000;
			return 1;
			break;
		case 0:
			TRISDCLR=0x04;
			OC3CON=0;
			OC3R=setpoint;
			OC3RS=setpoint;
			OC3CON=0x000E;
			OC3CON|=0x8000;
			return 1;
			break;
		default:
			return 0;
	}		
#endif
#if defined(EMPEROR460) || defined(EMPEROR795)
	switch (pin)
		{
			case  0:
			case 72: TRISDCLR=0x01;
					 OC1CON=0;
					 OC1R=setpoint;
					 OC1RS=setpoint;
					 OC1CON=0x000E;
					 OC1CON|=0x8000;
					 return 1;
					 break;
			case  1:
			case 69: TRISDCLR=0x02;
					 OC2CON=0;
					 OC2R=setpoint;
					 OC2RS=setpoint;
					 OC2CON=0x000E;
					 OC2CON|=0x8000;
					 return 1;
					 break;
			case  2:
			case 68: TRISDCLR=0x04;
					 OC3CON=0;
					 OC3R=setpoint;
					 OC3RS=setpoint;
					 OC3CON=0x000E;
					 OC3CON|=0x8000;
					 return 1;
					 break;
			case  3:
			case 67: TRISDCLR=0x08;
					 OC4CON=0;
					 OC4R=setpoint;
					 OC4RS=setpoint;
					 OC4CON=0x000E;
					 OC4CON|=0x8000;
					 return 1;
					 break;
			case  4:
			case 66: TRISDCLR=0x10;
					 OC5CON=0;
					 OC5R=setpoint;
					 OC5RS=setpoint;
					 OC5CON=0x000E;
					 OC5CON|=0x8000;
					 return 1;
					 break;
			default: return 0;
		}		
#endif
#if defined(UBW32_460) || defined(UBW32_795) || defined(PIC32_PINGUINO_T795)
	switch (pin)
		{
			case  0:
			case 24:
			case 40: TRISDCLR=0x01;
					 OC1CON=0;
					 OC1R=setpoint;
					 OC1RS=setpoint;
					 OC1CON=0x000E;
					 OC1CON|=0x8000;
					 return 1;
					 break;
			case  1:
			case  9:
			case 43: TRISDCLR=0x02;
					 OC2CON=0;
					 OC2R=setpoint;
					 OC2RS=setpoint;
					 OC2CON=0x000E;
					 OC2CON|=0x8000;
					 return 1;
					 break;
			case  2:
			case  8:
			case 44: TRISDCLR=0x04;
					 OC3CON=0;
					 OC3R=setpoint;
					 OC3RS=setpoint;
					 OC3CON=0x000E;
					 OC3CON|=0x8000;
					 return 1;
					 break;
			case  3:
			case  7:
			case 45: TRISDCLR=0x08;
					 OC4CON=0;
					 OC4R=setpoint;
					 OC4RS=setpoint;
					 OC4CON=0x000E;
					 OC4CON|=0x8000;
					 return 1;
					 break;
			case  4:
			case 25:
			case 60: TRISDCLR=0x010;
					 OC5CON=0;
					 OC5R=setpoint;
					 OC5RS=setpoint;
					 OC5CON=0x000E;
					 OC5CON|=0x8000;
					 return 1;
					 break;
			default: return 0;
		}		
#endif
}

/*	----------------------------------------------------------------------------
	PWM_set_frequency
	----------------------------------------------------------------------------
	@param:	frequency in hertz (range 3kHz .. 12MHz)
	----------------------------------------------------------------------------
	PB is Peripheral Bus Clock
	let's say p = TMR Prescale Value
	PWM Period 	= (PR + 1) * TPB * p
	so		(PR + 1) = PWM Period / (TPB * p)
	but PWM Period 	= 1 / PWM Frequency
	so		(PR + 1) = (1/PWM Frequency) / (1/TPB * p)
	and	(PR + 1) = FPB / (PWM Frequency * p)
	then	(PR + 1) = FPB / PWM Frequency / p
	--------------------------------------------------------------------------*/

void PWM_set_frequency(u32 freq)
{
	// PR3+1 calculation
	_pr3_plus1 = GetPeripheralClock() / freq;	// FOSC /  PWM Frequency

	// Timer3 prescaler calculation
	// PR3 max value is 0xffff, so PR3+1 max value is 0x10000 = 65536
	// highest prescaler value is 256
	// 256 * 65536 = 0x1000000 = 16777216 so :
	if (_pr3_plus1 <= 0x1000000)			// check if it's not too high
	{
		if (_pr3_plus1 <= 256)				// no needs of any prescaler
		{
			_t3con = 0b000;					// prescaler is 1
		}
		else if (_pr3_plus1 <= 512)		// needs prescaler 1:2
		{
			_pr3_plus1 = _pr3_plus1 >> 1;	// divided by 2
			_t3con = 0b001;					// prescaler is 2
		}
		else if (_pr3_plus1 <= 1024)		// needs prescaler 1:4
		{
			_pr3_plus1 = _pr3_plus1 >> 2;	// divided by 4
			_t3con = 0b010;					// prescaler is 4
		}
		else if (_pr3_plus1 <= 2048)		// needs prescaler 1:8
		{
			_pr3_plus1 = _pr3_plus1 >> 3;	// divided by 8
			_t3con = 0b011;					// prescaler is 8
		}
		else if (_pr3_plus1 <= 4096)		// needs prescaler 1:16
		{
			_pr3_plus1 = _pr3_plus1 >> 4;	// divided by 16
			_t3con = 0b100;					// prescaler is 16
		}
		else if (_pr3_plus1 <= 8192)		// needs prescaler 1:32
		{
			_pr3_plus1 = _pr3_plus1 >> 5;	// divided by 32
			_t3con = 0b101;					// prescaler is 32
		}
		else if (_pr3_plus1 <= 16384)		// needs prescaler 1:64
		{
			_pr3_plus1 = _pr3_plus1 >> 6;	// divided by 64
			_t3con = 0b110;					// prescaler is 64
		}
		else										// needs prescaler 1:256
		{
			_pr3_plus1 = _pr3_plus1 >> 8;	// divided by 256
			_t3con = 0b111;					// prescaler is 256
		}
//		return(_pr3_plus1);
	}
//	else return 0;
}

/*	----------------------------------------------------------------------------
	PWM_set_dutycycle
	----------------------------------------------------------------------------
	1.	Set the PWM period by writing to the selected timer period register (PRy).
	2.	Set the PWM duty cycle by writing to the OCxRS register.
	3.	Write the OxCR register with the initial duty cycle.
	4.	Enable interrupts, if required, for the timer and output compare modules. The output
	compare interrupt is required for PWM Fault pin utilization.
	5.	Configure the Output Compare module for one of two PWM Operation modes by writing
	to the Output Compare mode bits, OCM<2:0> (OCxCON<2:0>).
	6.	Set the TMRy prescale value and enable the time base by setting TON
	(TxCON<15>) = ‘1’.
	--------------------------------------------------------------------------*/

void PWM_set_dutycycle(u8 pin, u16 duty)
{
	//u8 temp;

	// PWM period
	PR3 = _pr3_plus1 - 1;						// set PR3 (max or PWM_set_frequency)

	analogwrite(pin, duty);

	// TMR2 configuration
	// TODO:intUsed[INT_TMR3] = INT_USED;		// tell interrupt.c we use TMR3
	//PIR1bits.TMR2IF = 0;							// reset this flag for the next test
	T3CONbits.TCKPS = _t3con;						// Timer3 prescaler
	T3CONbits.TON = ON;									// enable Timer3
	//while (PIR1bits.TMR3IF == 0);				// Wait until TMR2 overflows
}

/*	----------------------------------------------------------------------------
	PWM_set_percent_dutycycle
	----------------------------------------------------------------------------
	Set a percentage duty cycle, allowing max 100 PWM steps.
	Allowed range: 0..100
	The duty cycle will be set to the specified percentage of the maximum
	for the current PWM frequency.
	Note: The number of available PWM steps can be lower than 100 with
	(very) high PWM frequencies.
	--------------------------------------------------------------------------*/

void PWM_set_percent_dutycycle(u8 pin, u8 percent)
{
	u16 duty;
	if (percent == 0)
		duty = 0;
	else if (percent >= 100)
		duty = _pr3_plus1 - 1;
	else
		duty=(percent*_pr3_plus1)/100;
	PWM_set_dutycycle(pin,duty);
}
			
#endif	/* __PWM__ */
