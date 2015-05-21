/*	--------------------------------------------------------------------
    FILE:			pwm.c
    PROJECT:		pinguino
    PURPOSE:		hardware PWM control functions
    PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
                    regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:	20 feb. 2011
    LAST RELEASE:	08 dec. 2011
    --------------------------------------------------------------------
    08 Dec. 2011    jp.mandon   fixed period compute in pwm_set_frequency
                                fixed pwm_set_percent_dutycycle
                                added resolution output to set_frequency function
    25 Feb. 2012    R.Blanchot  added PIC32_PINGUINO_220 support 
    17 Mar. 2012    [hgmvanbeek@gmail.com] added support for PIC32_PINGUINO_MICRO
    03 Mar. 2015    R.Blanchot  fixed prescaler (bit <6:4>) 
    --------------------------------------------------------------------
    * TODO : analogWriteResolution()
             to set the resolution of the analogWrite() function.
    --------------------------------------------------------------------
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
    ------------------------------------------------------------------*/

#ifndef __PWM_C
#define __PWM_C

#define __PWM__

#include <typedef.h>
#include <macro.h>
#include <const.h>
#include <system.c>
#include <digitalw.c>

/*  --------------------------------------------------------------------
    PWM_init
    --------------------------------------------------------------------
    Set PWM max frequency possible with 10-bit resolution
    ------------------------------------------------------------------*/
    
void PWM_init(void)
{
    PR3 = 1024;
    T3CON = 0x8000;
}

/*  --------------------------------------------------------------------
    PWM_setFrequency
    --------------------------------------------------------------------
    @param: frequency in hertz
    --------------------------------------------------------------------
    PB is the Peripheral Bus Clock
    let's say p = TMR Prescaler Value
    PWM Period  = (PR + 1) * TPB * p
    so (PR + 1) = PWM Period / (TPB * p)
    but PWM Period = 1 / PWM Frequency
    so (PR + 1) = (1/PWM Frequency) / (1/TPB * p)
    and (PR + 1) = FPB / (PWM Frequency * p)
    then (PR + 1) = FPB / PWM Frequency / p
    ------------------------------------------------------------------*/

void PWM_setFrequency(u32 freq)
{
    u32 prescaler;                           // shadow value of T3CON
    u32 period;                              // shadow value of PR3 set to max. + 1 

    // PR3+1 calculation
    period = GetPeripheralClock() / freq;    // FOSC /  PWM Frequency

    // Timer3 prescaler calculation
    // PWM max resolution is 10-bit
    // PR3 max value is then 1023
    // PR3+1 max value is 1024
    // highest prescaler value is 256
    // 256 * 1024 = 0x400000 :
    if (period <= 0x400000)                 // check if it's not too high
    {
        if (period <= 1024)                 // no needs of any prescaler
        {
            prescaler = 0b000;              // prescaler is 1
        }
        else if (period <= 2048)            // needs prescaler 1:2
        {
            period = period >> 1;           // divided by 2
            prescaler = 0b001;              // prescaler is 2
        }
        else if (period <= 4096)            // needs prescaler 1:4
        {
            period = period >> 2;           // divided by 4
            prescaler = 0b010;              // prescaler is 4
        }
        else if (period <= 8192)            // needs prescaler 1:8
        {
            period = period >> 3;           // divided by 8
            prescaler = 0b011;              // prescaler is 8
        }
        else if (period <= 16384)           // needs prescaler 1:16
        {
            period = period >> 4;           // divided by 16
            prescaler = 0b100;              // prescaler is 16
        }
        else if (period <= 32768)           // needs prescaler 1:32
        {
            period = period >> 5;           // divided by 32
            prescaler = 0b101;              // prescaler is 32
        }
        else if (period <= 65536)           // needs prescaler 1:64
        {
            period = period >> 6;           // divided by 64
            prescaler = 0b110;              // prescaler is 64
        }
        else                                // needs prescaler 1:256
        {
            period = period >> 8;           // divided by 256
            prescaler = 0b111;              // prescaler is 256
        }

        // Set the PWM period
        PR3 = period - 1;

        // Timer configuration
        T3CON = 0x8000 | (prescaler<<4);         // Set prescaler and enable Timer3

        //return(period);
    }
//  else return 0;
}

/*  --------------------------------------------------------------------
    analogwrite
    --------------------------------------------------------------------
    OCxCON (1 <= x <= 5) Register Config : 0x800E
    - PWM mode with Fault pin disabled
    - Timer3 is the clock source
    - OCxR<15:0> and OCxRS<15:0> are used for comparisons to the 16-bit
      timer source
    ------------------------------------------------------------------*/
    
u8 analogwrite(u8 pin, u16 setpoint)
{
    ///*****************************************************************
    #if defined(PIC32_PINGUINO_220)
    ///*****************************************************************

    switch (pin)
    {
        case 2:
            pinmode(2, OUTPUT);
            OC3CON=0;       // PWM Off
            OC3R=setpoint;  // Timer3 will be compared to this values
            OC3RS=setpoint;
            OC3CON=0x800E;  // PWM On
            return 1;

        case 3:
            pinmode(3, OUTPUT);
            OC4CON=0;		// PWM Off
            OC4R=setpoint;	// Timer3 will be compared to this values
            OC4RS=setpoint;
            OC4CON=0x800E;	// PWM On
            return 1;

        case 11:
            pinmode(11, OUTPUT);
            OC2CON=0;		// PWM Off
            OC2R=setpoint;	// Timer3 will be compared to this values
            OC2RS=setpoint;
            OC2CON=0x800E;	// PWM On
            return 1;

        case 12:
            pinmode(12, OUTPUT);
            OC5CON=0;		// PWM Off
            OC5R=setpoint;	// Timer3 will be compared to this values
            OC5RS=setpoint;
            OC5CON=0x800E;  // PWM On
            return 1;

        case 13:
            pinmode(13, OUTPUT);
            OC1CON=0;		// PWM Off
            OC1R=setpoint;	// Timer3 will be compared to this values
            OC1RS=setpoint;
            OC1CON=0x800E;  // PWM On
            return 1;

        default:
            return 0;
    }

    ///*****************************************************************
    #elif defined(PINGUINO32MX220) || \
          defined(PINGUINO32MX250) || \
          defined(PINGUINO32MX270)
    ///*****************************************************************
    
    // D0-D7	: B15,B14,B13,B9,B8,B7,B5,A4
    // D8-D13	: B4,B3,B2,B1,B0,A0
    // D14 		: A1

    switch (pin)
    {

        case 1:                         // PWM4 = RB14 = pin 1
            TRISBCLR=1<<14;             // pinmode(1, OUTPUT);
            OC3CONCLR=0x8000;           // PWM Off
            OC3R=setpoint;              // Timer3 will be compared to this values
            OC3RS=setpoint;             // OCxR = Compare register, OCxRS = Secondary Compare register
            OC3CON=0x800E;              // PWM On, clock source = Timer3, PWM mode
            return 1;

        case 2:                         // PWM3 = RB13 = pin 2
            TRISBCLR=1<<13;             // pinmode(2, OUTPUT);
            OC4CONCLR=0x8000;           // PWM Off
            OC4R=setpoint;              // Timer3 will be compared to this values
            OC4RS=setpoint;             // OCxR = Compare register, OCxRS = Secondary Compare register
            OC4CON=0x800E;              // PWM On, clock source = Timer3, PWM mode
            return 1;

        case 6:                         // PWM2 = RB5 = pin 6
            TRISBCLR=1<<5;              // pinmode(6, OUTPUT);
            OC2CONCLR=0x8000;           // PWM Off
            OC2R=setpoint;              // Timer3 will be compared to this values
            OC2RS=setpoint;             // OCxR = Compare register, OCxRS = Secondary Compare register
            OC2CON=0x800E;              // PWM On, clock source = Timer3, PWM mode
            return 1;

        case 7:                         // PWM1 = RA4 = pin 7
            TRISACLR=1<<4;              // pinmode(7, OUTPUT);
            OC5CONCLR=0x8000;           // PWM Off
            OC5R=setpoint;              // Timer3 will be compared to this values
            OC5RS=setpoint;             // OCxR = Compare register, OCxRS = Secondary Compare register
            OC5CON=0x800E;              // PWM On, clock source = Timer3, PWM mode
            return 1;

        case 8:                         // PWM0 = RB4 = pin 8
            TRISBCLR=1<<4;              // pinmode(8, OUTPUT);
            OC1CONCLR=0x8000;           // PWM Off
            OC1R=setpoint;              // Timer3 will be compared to this values
            OC1RS=setpoint;             // OCxR = Compare register, OCxRS = Secondary Compare register
            OC1CON=0x800E;              // PWM On, clock source = Timer3, PWM mode
            return 1;

        default:
            return 0;
    }
    
    ///*****************************************************************
    #elif defined(PIC32_PINGUINO_MICRO)
    ///*****************************************************************

    switch (pin)
    {
        case 10:
            pinmode(pin, OUTPUT);
            OC2CON=0;		// PWM Off
            OC2R=setpoint;	// Timer3 will be compared to this values
            OC2RS=setpoint;
            OC2CON=0x800E;	// PWM On
            return 1;

        case 11:
            pinmode(pin, OUTPUT);
            OC3CON=0;		// PWM Off
            OC3R=setpoint;	// Timer3 will be compared to this values
            OC3RS=setpoint;
            OC3CON=0x800E;	// PWM On
            return 1;

        case 12:
            pinmode(pin, OUTPUT);
            OC4CON=0;		// PWM Off
            OC4R=setpoint;	// Timer3 will be compared to this values
            OC4RS=setpoint;
            OC4CON=0x800E;	// PWM On
            return 1;

        case 13:
            pinmode(pin, OUTPUT);
            OC5CON=0;		// PWM Off
            OC5R=setpoint;	// Timer3 will be compared to this values
            OC5RS=setpoint;
            OC5CON=0x800E;	// PWM On
            return 1;

        default:
            return 0;
    }

    ///*****************************************************************
    #elif defined(PIC32_PINGUINO) || \
          defined(PIC32_PINGUINO_OTG)
    ///*****************************************************************
          
    switch (pin)
    {
        case 2:
            TRISDSET=0x10;
            TRISDCLR=0x01;
            OC1CON=0;
            OC1R=setpoint;
            OC1RS=setpoint;
            OC1CON=0x800E;
            return 1;

        case 1:
            TRISDCLR=0x08;
            OC4CON=0;
            OC4R=setpoint;
            OC4RS=setpoint;
            OC4CON=0x800E;
            return 1;

        case 0:
            TRISDCLR=0x04;
            OC3CON=0;
            OC3R=setpoint;
            OC3RS=setpoint;
            OC3CON=0x800E;
            return 1;

        default:
            return 0;
    }

    ///*****************************************************************
    #elif defined(EMPEROR460) || \
          defined(EMPEROR795)
    ///*****************************************************************
          
    switch (pin)
    {
        case  0:
        case 72:
            TRISDCLR=0x01;
            OC1CON=0;
            OC1R=setpoint;
            OC1RS=setpoint;
            OC1CON=0x800E;
            return 1;

        case  1:
        case 69:
            TRISDCLR=0x02;
            OC2CON=0;
            OC2R=setpoint;
            OC2RS=setpoint;
            OC2CON=0x800E;
            return 1;

        case  2:
        case 68:
            TRISDCLR=0x04;
            OC3CON=0;
            OC3R=setpoint;
            OC3RS=setpoint;
            OC3CON=0x800E;
            return 1;

        case  3:
        case 67:
            TRISDCLR=0x08;
            OC4CON=0;
            OC4R=setpoint;
            OC4RS=setpoint;
            OC4CON=0x800E;
            return 1;

        case  4:
        case 66:
            TRISDCLR=0x10;
            OC5CON=0;
            OC5R=setpoint;
            OC5RS=setpoint;
            OC5CON=0x800E;
            return 1;

        default:
            return 0;
    }

    ///*****************************************************************
    #elif defined(UBW32_460) || \
          defined(UBW32_795) || \
          defined(PIC32_PINGUINO_T795)
    ///*****************************************************************

    switch (pin)
    {
        case  0:
        case 24:
        case 40:
            TRISDCLR=0x01;
            OC1CON=0;
            OC1R=setpoint;
            OC1RS=setpoint;
            OC1CON=0x800E;
            return 1;

        case  1:
        case  9:
        case 43:
            TRISDCLR=0x02;
            OC2CON=0;
            OC2R=setpoint;
            OC2RS=setpoint;
            OC2CON=0x800E;
            return 1;

        case  2:
        case  8:
        case 44:
            TRISDCLR=0x04;
            OC3CON=0;
            OC3R=setpoint;
            OC3RS=setpoint;
            OC3CON=0x800E;
            return 1;

        case  3:
        case  7:
        case 45:
            TRISDCLR=0x08;
            OC4CON=0;
            OC4R=setpoint;
            OC4RS=setpoint;
            OC4CON=0x800E;
            return 1;

        case  4:
        case 25:
        case 60:
            TRISDCLR=0x010;
            OC5CON=0;
            OC5R=setpoint;
            OC5RS=setpoint;
            OC5CON=0x800E;
            return 1;

        default:
            return 0;
    }
    #endif
}

/*	----------------------------------------------------------------------------
    PWM_setDutyCycle
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

void PWM_setDutyCycle(u8 pin, u16 setpoint)
{
    // Set the PWM duty cycle
    analogwrite(pin, setpoint);
}

/*	----------------------------------------------------------------------------
    PWM_setPercentDutyCycle
    ----------------------------------------------------------------------------
    Set a percentage duty cycle, allowing max 100 PWM steps.
    Allowed range: 0..100
    The duty cycle will be set to the specified percentage of the maximum
    for the current PWM frequency.
    Note: The number of available PWM steps can be lower than 100 with
    (very) high PWM frequencies.
    --------------------------------------------------------------------------*/

void PWM_setPercentDutyCycle(u8 pin, u8 duty)
{
    u16 setpoint;
    
    if (duty == 0)
        setpoint = 0;
    
    else if (duty >= 100)
        setpoint = PR3;
    
    else
        setpoint = duty * (PR3 + 1) / 100;
    
    // Set the PWM duty cycle
    //PWM_setDutyCycle(pin, duty << 2);
    //analogwrite(pin, duty << 2);
    analogwrite(pin, setpoint);
}
            
#endif	/* __PWM_C */
