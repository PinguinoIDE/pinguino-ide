/*-----------------------------------------------------------------------------------------------
   servo.c  LIBRARY FOR CONTROLLING UP OT ALL PINGUINO PINS AS SERVOS FOR ALL Pinguino 8bit PICs
   ==============================================================================================

    Version      : 4.2
    Author       : Jesús Carmona Esteban
    Last change  : 24/04/2014

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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
--------------------------------------------------------------------------------------*/

// Changes:
// -------------------------------------------------------------------------------------
// 16/05/2014 - Régis Blanchot - Changed Timer1 interrupt (used by DCF77 library) to Timer2 interrupt
// 24/04/2014 - Régis Blanchot - Ported to 32-bit Pinguino
// 15/11/2013 - Several bugs removed. Improved ServoAttach function.
// 12/11/2013 - Error on ServospulseUp function corrected. Code cleaned and compacted. Expanded to all 8 bit PICs available. 
// 20/10/2013 - Fixed interrupt handling for working TMR1 with new x.4 enviroment.
// 01/10/2013 - Tested and calibrated with oscilloscope for 18F4550, 18F2550 and EQUO_UNO for X.3 IDE.
// 28/09/2013 - Corrections on maths at servowrite funtion. 
// 02/09/2012 - Changes on ServoMinPulse and ServoMaxPulse functions to assemble Arduino ones in order to expand from 500us to 2500us pulses.
// 05/04/2012 - Expansion to versions 4550/PICUNO_EQUO using #defines in user program.
// -------------------------------------------------------------------------------------

// NOTES:
// - This library allows 250 positions for a servo.
//   Those 1-250 values are mapped from 0-180 degrees,
//   which is the input value by user at servo.write function.
// - There is a correspondence table where is stored maximum and minimum
//   values that any servo could reach in microseconds. But the value stored is from 1 to 250.
// - All servos are automatically refreshed by PIC in a parallel way.

// Values mapping between position and microseconds:
//
// TIMESLOT(byte value):
// 1                  62                  125                  192                  250
// |-------------------|-------------------|--------------------|--------------------|
// 500               1000                 1500                2000                 2500
// Time (microseconds)
//
// Defaul values now for SERVOMAX and SERVOMIN should be 64 and 192, 1000usec and 2000usec respectively.
// User can change 0 degrees up to 500 us pulse as absolute minumum, 
// and 180 degrees up to 2500 usec pulse as absolute maximum using the following functions:
// 
// - ServoMinimumPulse(unsigned int servo,int min_microseconds)
// - ServoMaximumPulse(unsigned int servo,int max_microseconds)
//
// -------------------------------------------------------------------------------------------------------


#ifndef __SERVOS__
#define __SERVOS__

//Includes for functions used internally in this lib.
//#include <stdlib.h>
#include <const.h>      // MIPS32
#include <digitalw.c>   // ports and mask definitions.
#include <delay.c>      // Delayus()
#include <system.c>     // GetPeripheralClock()
#include <interrupt.c>  // interrupts routines

// Max and Min values that correspond to 2000 usec and 1000 usec. 
#define DefaultSERVOMAX 192
#define DefaultSERVOMIN  64

// Current peripheral frequency
volatile unsigned int f500us, f20ms;

//library internal variables:
volatile unsigned int phase=0;
volatile unsigned int needreordering=0;

//-----------------------------------------------------------------------------------------------------------------------------
// Variable definition that depends on PIC type:
//-----------------------------------------------------------------------------------------------------------------------------

#define TotalPICpins   78
#define TotalPICports   6

//-----------------------------------------------------------------------------------------------------------------------------
// Variables and Matrix definitions for Any PIC
//-----------------------------------------------------------------------------------------------------------------------------

unsigned int timingindex;
unsigned int timedivision=0;
unsigned int loopvar;
unsigned int mascaratotal[TotalPICports];

unsigned int timevalue[TotalPICpins];              // This keeps values ordered for all pins.
unsigned int timings[TotalPICpins][TotalPICports]; // This keeps ports and pins activated for a specific timevalue (both matrix share index to make access easy).
unsigned int activatedservos[TotalPICports];       // This keeps masks for every port with the activated pins to be servos.
unsigned int servovalues[TotalPICpins];            // Entry table for values sets for every pin-servo.
unsigned int maxminpos[2][TotalPICpins];           // This table keeps minimum(0 degrees) and maximum(180 degrees) values(in ticks) that the servo can reach.

//-----------------------------------------------------------------------------------------------------------------------------
//  Functions for SERVO library
//-----------------------------------------------------------------------------------------------------------------------------

void servos_init()
{
    unsigned int a;
    unsigned int fpb;

    // Filling up the servovalues table to 255. 

    for(a=0;a<TotalPICpins;a++)
    {
        servovalues[a]=255;               // Filling up the servovalues table to 255.
        maxminpos[0][a]= DefaultSERVOMIN; // Setting min servo position to 1000 usec.
        maxminpos[1][a]= DefaultSERVOMAX; // Setting max servo position to 2000 usec.
    }
    
    // Filling up the activated servos matrix.

    for(a=0;a<TotalPICports;a++)
        activatedservos[a]=0x00;  // Setting all pins as deactivated as servo.

    // Timer2 Configuration
    // The Timer2 clock prescale (TCKPS) is 1:64
    // TMR2 count register increments on every Peripheral clock cycle
    // TMR2 increments every 64 * 1/Fpb
    //  500 us =>  500 / (64 / Fpb) = (  500 * Fpb ) / 64 cycles
    // 2500 us => 2500 / (64 / Fpb) = ( 2500 * Fpb ) / 64 cycles

    fpb    = GetPeripheralClock() / 1000 / 1000;
    f500us = (   500 * fpb ) / 64; 
    f20ms  = ( 20000 * fpb ) / 64;
    
    IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    // bit 6-4 TCKPS<2:0>: Timer Input Clock Prescale Select bits
    // 0 = 1:1   default prescale value
    // 1 = 1:2   prescale value
    // 2 = 1:4   prescale value
    // 3 = 1:8   prescale value
    // 4 = 1:16  prescale value
    // 5 = 1:32  prescale value
    // 6 = 1:64  prescale value
    // 7 = 1:256 prescale value

    T2CON    = 6 << 4;  				// prescaler 1:64, internal peripheral clock
    TMR2     = 0;						// clear timer register
    PR2      = f500us;					// load period register
    IntSetVectorPriority(INT_TIMER2_VECTOR, 7, 3);
    IntClearFlag(INT_TIMER2);
    IntEnable(INT_TIMER2);
    T2CONSET = 0x8000;					// start timer 1
}

static void ServosPulseDown()
{
    volatile timingindex = 0;

    for(timedivision = 0; timedivision < 251; timedivision++)
    {
        if (timevalue[timingindex] == timedivision)
        {
            #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
            PORTA ^= timings[timingindex][pA];
            #endif
            PORTB ^= timings[timingindex][pB];
            #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
            PORTC ^= timings[timingindex][pC];
            #endif
            #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
                !defined(__32MX220F032B__)
            PORTD ^= timings[timingindex][pD];
            PORTE ^= timings[timingindex][pE];
            PORTF ^= timings[timingindex][pF];
            PORTG ^= timings[timingindex][pG];
            #endif

            timingindex++;
        }
        
        Delayus(8);
        // NEW: Every round on this "for" loop must last 8 microseconds.
        // So the following asm code is to adjust to that exact time.
        /*
        __asm
            movlw 7
            movwf _loopvar
        bucle:
            NOP
            NOP
            NOP
            NOP
            NOP
            NOP
            NOP
            decfsz _loopvar,1
            goto bucle
        __endasm;
        */
    }
}

// This function starts up pulses for all activated servos.
static void ServosPulseUp()
{
    #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
    PORTA |= activatedservos[pA];
    #endif
    PORTB |= activatedservos[pB];
    #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
    PORTC |= activatedservos[pC];
    #endif
    #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
        !defined(__32MX220F032B__)
    PORTD |= activatedservos[pD];
    PORTE |= activatedservos[pE];
    PORTF |= activatedservos[pF];
    PORTG |= activatedservos[pG];
    #endif
}

// This funtion analyses servovalues table and creates and ordered table(timings)
// from smaller to bigger of all the values, asociating to each
// position of the table the servos that matches that timing.
static void SortServoTimings()
{
    volatile unsigned int s,t,totalservos,numservos;

    // table initialization:
    for(t=0;t<TotalPICpins;t++)
    {
        timevalue[t]=255; 
        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
        timings[t][pA]=0x00;
        #endif
        timings[t][pB]=0x00;
        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
        timings[t][pC]=0x00;
        #endif
        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
            !defined(__32MX220F032B__)
        timings[t][pD]=0x00;
        timings[t][pE]=0x00;
        timings[t][pF]=0x00;
        timings[t][pG]=0x00;
        #endif
    }
    
    // mascaratotal table initialization:
    for(t=0;t<TotalPICports;t++)
        mascaratotal[t]=0x00;

    totalservos=0; // Total servos revised. This helps to keep within "while"
    t=0;           // Index to go through timevalue and timings tables.

    while(totalservos<TotalPICpins)
    {
        numservos=1;

        for(s=0;s<TotalPICpins;s++)
        {
            
            switch (portmask[s])
            {
                #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
                case pA:
                    if (pinmask[s] & mascaratotal[pA] & activatedservos[pA]){
                        break;
                    }
                    else if (servovalues[s] < timevalue[t]){
                        timevalue[t]=servovalues[s];
                        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
                        timings[t][pA]=pinmask[s];
                        #endif
                        timings[t][pB]=0x00;
                        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
                        timings[t][pC]=0x00;
                        #endif
                        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
                            !defined(__32MX220F032B__)
                        timings[t][pD]=0x00;
                        timings[t][pE]=0x00;
                        timings[t][pF]=0x00;
                        timings[t][pG]=0x00;
                        #endif
                        numservos=1;
                    }
                    else if (servovalues[s] == timevalue[t]){
                        timings[t][pA] |= pinmask[s];
                        numservos++;
                    }
                    break;      		
                #endif

                case pB: 
                    if (pinmask[s] & mascaratotal[pB] & activatedservos[pB]){
                        break;
                    }
                    else if (servovalues[s] < timevalue[t]){
                        timevalue[t]=servovalues[s];
                        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
                        timings[t][pA]=0x00;
                        #endif
                        timings[t][pB]=pinmask[s];
                        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
                        timings[t][pC]=0x00;
                        #endif
                        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
                            !defined(__32MX220F032B__)
                        timings[t][pD]=0x00;
                        timings[t][pE]=0x00;
                        timings[t][pF]=0x00;
                        timings[t][pG]=0x00;
                        #endif
                        numservos=1;
                    }
                    else if (servovalues[s] == timevalue[t]){
                        timings[t][pB] |= pinmask[s];
                        numservos++;
                    }
                    break;

                #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
                case pC:
                    if (pinmask[s] & mascaratotal[pC] & activatedservos[pC]){
                        break;
                    }
                    else if (servovalues[s] < timevalue[t]){
                        timevalue[t]=servovalues[s];
                        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
                        timings[t][pA]=0x00;
                        #endif
                        timings[t][pB]=0x00;
                        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
                        timings[t][pC]=pinmask[s];
                        #endif
                        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
                            !defined(__32MX220F032B__)
                        timings[t][pD]=0x00;
                        timings[t][pE]=0x00;
                        timings[t][pF]=0x00;
                        timings[t][pG]=0x00;
                        #endif
                        numservos=1;
                    }
                    else if (servovalues[s] == timevalue[t]){
                        timings[t][pC] |= pinmask[s];
                        numservos++;
                    }
                    break;
                #endif

                #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
                    !defined(__32MX220F032B__)
                case pD:
                    if (pinmask[s] & mascaratotal[pD] & activatedservos[pD]){
                        break;
                    }
                    else if (servovalues[s] < timevalue[t]){
                        timevalue[t]=servovalues[s];
                        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
                        timings[t][pA]=0x00;
                        #endif
                        timings[t][pB]=0x00;
                        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
                        timings[t][pC]=0x00;
                        #endif
                        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
                            !defined(__32MX220F032B__)
                        timings[t][pD]=pinmask[s];
                        timings[t][pE]=0x00;
                        timings[t][pF]=0x00;
                        timings[t][pG]=0x00;
                        #endif
                        numservos=1;
                    }
                    else if (servovalues[s] == timevalue[t]){
                        timings[t][pD] |= pinmask[s];
                        numservos++;
                    }
                    break;

                case pE:
                    if (pinmask[s] & mascaratotal[pE] & activatedservos[pE]){
                        break;
                    }
                    else if (servovalues[s] < timevalue[t]){
                        timevalue[t]=servovalues[s];
                        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
                        timings[t][pA]=0x00;
                        #endif
                        timings[t][pB]=0x00;
                        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
                        timings[t][pC]=0x00;
                        #endif
                        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
                            !defined(__32MX220F032B__)
                        timings[t][pD]=0x00;
                        timings[t][pE]=pinmask[s];
                        timings[t][pF]=0x00;
                        timings[t][pG]=0x00;
                        #endif
                        numservos=1;
                    }
                    else if (servovalues[s] == timevalue[t]){
                        timings[t][pE] |= pinmask[s];
                        numservos++;
                    }
                    break;

                case pF:
                    if (pinmask[s] & mascaratotal[pF] & activatedservos[pF]){
                        break;
                    }
                    else if (servovalues[s] < timevalue[t]){
                        timevalue[t]=servovalues[s];
                        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
                        timings[t][pA]=0x00;
                        #endif
                        timings[t][pB]=0x00;
                        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
                        timings[t][pC]=0x00;
                        #endif
                        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
                            !defined(__32MX220F032B__)
                        timings[t][pD]=0x00;
                        timings[t][pE]=0x00;
                        timings[t][pF]=pinmask[s];
                        timings[t][pG]=0x00;
                        #endif
                        numservos=1;
                    }
                    else if (servovalues[s] == timevalue[t]){
                        timings[t][pF] |= pinmask[s];
                        numservos++;
                    }
                    break;

                case pG:
                    if (pinmask[s] & mascaratotal[pG] & activatedservos[pG]){
                        break;
                    }
                    else if (servovalues[s] < timevalue[t]){
                        timevalue[t]=servovalues[s];
                        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
                        timings[t][pA]=0x00;
                        #endif
                        timings[t][pB]=0x00;
                        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
                        timings[t][pC]=0x00;
                        #endif
                        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
                            !defined(__32MX220F032B__)
                        timings[t][pD]=0x00;
                        timings[t][pE]=0x00;
                        timings[t][pF]=0x00;
                        timings[t][pG]=pinmask[s];
                        #endif
                        numservos=1;
                    }
                    else if (servovalues[s] == timevalue[t]){
                        timings[t][pG] |= pinmask[s];
                        numservos++;
                    }
                    break;
                #endif
            }
        }

        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
        mascaratotal[pA] |= timings[t][pA];
        #endif
        mascaratotal[pB] |= timings[t][pB];
        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
        mascaratotal[pC] |= timings[t][pC];
        #endif
        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
            !defined(__32MX220F032B__)
        mascaratotal[pD] |= timings[t][pD];
        mascaratotal[pE] |= timings[t][pE];
        mascaratotal[pF] |= timings[t][pF];
        mascaratotal[pG] |= timings[t][pG];
        #endif
                                        
        totalservos += numservos;
        t++;

    }
    needreordering=0;  // This indicates that servo timings are sorted.
}

void ServoAttach(unsigned int pin)
{
    if(pin>=TotalPICpins) return;

    switch (portmask[pin])
    {
        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
        case pA: 
                activatedservos[pA] |= pinmask[pin];  // list pin as servo driver.
                TRISACLR = pinmask[pin];// set as output pin
                break;
        #endif
        case pB: 
                activatedservos[pB] |= pinmask[pin];  // list pin as servo driver.
                TRISBCLR = pinmask[pin];// set as output pin
                break;
        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
        case pC: 
                activatedservos[pC] |= pinmask[pin];  // list pin as servo driver.
                TRISCCLR = pinmask[pin];// set as output pin
                break;
        #endif
        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
            !defined(__32MX220F032B__)
        case pD: 
                activatedservos[pD] |= pinmask[pin];  // list pin as servo driver.
                TRISDCLR = pinmask[pin];// set as output pin
                break;
        case pE: 
                activatedservos[pE] |= pinmask[pin];  // list pin as servo driver.
                TRISECLR = pinmask[pin];// set as output pin
                break;	            	            
        case pF: 
                activatedservos[pF] |= pinmask[pin];  // list pin as servo driver.
                TRISFCLR = pinmask[pin];// set as output pin
                break;	            	            
        case pG: 
                activatedservos[pG] |= pinmask[pin];  // list pin as servo driver.
                TRISGCLR = pinmask[pin];// set as output pin
                break;	            	            
        #endif
    }
}

void ServoDetach(unsigned int pin)
{
    if(pin>=TotalPICpins) return;

    switch (portmask[pin])
    {
        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
        case pA: activatedservos[pA] ^= pinmask[pin]; break;
        #endif
        case pB: activatedservos[pB] ^= pinmask[pin]; break;
        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
        case pC: activatedservos[pC] ^= pinmask[pin]; break;
        #endif
        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
            !defined(__32MX220F032B__)
        case pD: activatedservos[pD] ^= pinmask[pin]; break;
        case pE: activatedservos[pE] ^= pinmask[pin]; break;
        case pF: activatedservos[pF] ^= pinmask[pin]; break;
        case pG: activatedservos[pG] ^= pinmask[pin]; break;
        #endif
    }
}

void ServoWrite(unsigned int servo, unsigned int degrees)
{
    unsigned int range;
    unsigned int ticksperdegree;
    unsigned int value;

    // Check if number of servo is valid
    if(servo>=TotalPICpins)
        return;

    // limitting degrees:
    if(degrees>180) degrees=180;

    // Converts degrees to timeslots
    range = (maxminpos[1][servo]  - maxminpos[0][servo]);
    value = (degrees*range) / 180 + maxminpos[0][servo];

    // Store the new position
    servovalues[servo]= value;

    needreordering=1;  // This indicates servo timings must be reordered.
}

unsigned int ServoRead(unsigned int servo)
{
    if(servo>=TotalPICpins)        // test if numservo is valid
        return 0;
        
    return servovalues[servo];
}

void ServoMinimumPulse(unsigned int servo,int min_microseconds)
{
    // Check if number of servo is valid:
    if(servo>=TotalPICpins)
        return;

    // test if microseconds are within range:
    if (min_microseconds<500)  min_microseconds=500;
    if (min_microseconds>1500) min_microseconds=1500;

    // The following formula converts min. microseconds to min. timeslot
    maxminpos[0][servo]=(min_microseconds-500)>>3;   // 0 < final_min < 125
}

void ServoMaximumPulse(unsigned int servo,int max_microseconds)
{
    // Check if number of servo is valid:
    if(servo>=TotalPICpins)
        return;
        
    // test if microseconds are within range:
    if (max_microseconds<1500) max_microseconds=1500;
    if (max_microseconds>2500) max_microseconds=2500;

    // The following formula converts max. microseconds to max. timeslot
    maxminpos[1][servo]=(max_microseconds-500)>>3;   // 125 < final_max < 250
}

/***********************************************************************
 * Interrupt handler that handles servos
 * Timer 2 interrupt (Vector 8)
 **********************************************************************/
/*
// Put the ISR_wrapper in the good place
void MIPS32 ISR_wrapper_vector_8(void) __attribute__ ((section (".vector_8")));

// servo_interrupt is declared as an interrupt routine
void MIPS32 servo_interrupt(void) __attribute__ ((interrupt));

// ISR_wrapper will call the servo_interrupt()
void MIPS32 ISR_wrapper_vector_8(void)
{
    servo_interrupt();
}
*/
//void servo_interrupt()
void Timer2Interrupt(void)
{
    // is this an TMR1 interrupt ?
    if (IntGetFlag(INT_TIMER2))
    {

        if (phase)
        {
            ServosPulseUp();
            PR2 = f500us;           // load period register
            phase = 0;
        }

        else
        {
            ServosPulseDown();      // 2 ms
            PR2 = f20ms;            // load period register
            if (needreordering)
                SortServoTimings(); // 1 ms
            phase = 1;              // will start the servos cycle on next interrupt
        }

        // enable interrupt again
        IntClearFlag(INT_TIMER2);
    }
}

#endif

//-----------------------------------------------------------------------------------------------------------------------------
// End of SERVOLIBRARY
//----------------------------------------------------------------------------------------------------------------------------------
