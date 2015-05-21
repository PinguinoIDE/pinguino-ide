/*-----------------------------------------------------------------------------------------------
   servo.c  LIBRARY FOR CONTROLLING UP OT ALL PINGUINO PINS AS SERVOS FOR ALL Pinguino 8bit PICs
   ==============================================================================================

    Version      : 4.2
    Author       : Jesús Carmona Esteban
    Last change  : 15/11/2013

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
// 15/11/2013 - Several bugs removed. Improved ServoAttach function.
// 12/11/2013 - Error on ServospulseUp function corrected. Code cleaned and compacted. Expanded to all 8 bit PICs available. 
// 20/10/2013 - Fixed interrupt handling for working TMR1 with new x.4 enviroment.
// 01/10/2013 - Tested and calibrated with oscilloscope for 18F4550, 18F2550 and EQUO_UNO for X.3 IDE.
// 28/09/2013 - Corrections on maths at servowrite funtion. 
// 02/09/2012 - Changes on ServoMinPulse and ServoMaxPulse functions to assemble Arduino ones in order to expand from 500us to 2500us pulses.
// 05/04/2012 - Expansion to versions 4550/PICUNO_EQUO using #defines in user program.
// -------------------------------------------------------------------------------------

// NOTES:
// - Fosc 48Mhz => 12 MIPS (Fosc/4). Lesser clock frecuencies are not compatible with this library.
// - This library allows 250 positions for a servo.
//   Those 1-250 values are mapped from 0-180 degrees,
//   which is the input value by user at servo.write function.
// - There is a correspondence table where is stored maximum and minimum
//   values that any servo could reach in microseconds. But the value stored is from 1 to 250.
// - All servos are automatically refreshed by PIC in a parallel way.
//
//
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
// - ServoMinimumPulse(unsigned char servo, int min_microseconds)
// - ServoMaximumPulse(unsigned char servo, int max_microseconds)
//
// -------------------------------------------------------------------------------------------------------


#ifndef SERVOSLIBRARY
#define SERVOSLIBRARY

//Includes for functions used internally in this lib.
//#include <stdlib.h>
#include <digital.h>  //includes ports and mask definitions.

// Max and Min values that correspond to 2000 usec and 1000 usec. 
#define DefaultSERVOMAX 192
#define DefaultSERVOMIN  64

#define MINUS 500
#define MAXUS 2500
#define MIDUS ((MINUS+MAXUS)/2)

//library internal variables:
volatile unsigned char phase=0;
volatile unsigned char needreordering=0;

//-----------------------------------------------------------------------------------------------------------------------------
// Variable definition that depends on PIC type:
//-----------------------------------------------------------------------------------------------------------------------------
#if defined(PINGUINO1220) || defined(PINGUINO1320)
#define TotalPICpins   19
#define TotalPICports   2

#elif defined(__18f14k22)
#define TotalPICpins   19
#define TotalPICports   3

#elif  defined(PINGUINO2455) || defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(CHRP3)
#define TotalPICpins   19
#define TotalPICports   3

#elif defined(PINGUINO26J50)
#define TotalPICpins   18
#define TotalPICports   3

#elif defined(PINGUINO47J53)
#define TotalPICpins   32
#define TotalPICports   5

#elif defined(PINGUINO4550) || defined(PINGUINO45K50)
#define TotalPICpins   30
#define TotalPICports   5

#elif defined(FREEJALDUINO)
#define TotalPICpins   19
#define TotalPICports   3

#elif defined(PICUNO_EQUO)
#define TotalPICpins   14
#define TotalPICports   4

#endif


// sumary of ports per PIC
// ---------------------------------------------------
// 2 ports:
// #if defined (PINGUINO1220) || defined(PINGUINO1320)
// 3 ports:
// #elif defined(__18f14k22) || defined(PINGUINO2455) || defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(CHRP3) || defined(PINGUINO26J50) || defined(PINGUINO4550) || defined(PICUNO_EQUO) || defined(FREEJALDUINO) 
// 4 ports:
// #elif defined(PICUNO_EQUO)
// 5 ports:
// #elif defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50)
//
// #endif

//-----------------------------------------------------------------------------------------------------------------------------
// Variables and Matrix definitions for Any PIC
//-----------------------------------------------------------------------------------------------------------------------------
unsigned char timingindex;
unsigned char timedivision=0;
unsigned char loopvar;
unsigned char mascaratotal[TotalPICports];

unsigned char timevalue[TotalPICpins];              // This keeps values ordered for all pins.
unsigned char timings[TotalPICpins][TotalPICports]; // This keeps ports and pins activated for a specific timevalue (both matrix share index to make access easy).
unsigned char activatedservos[TotalPICports];       // This keeps masks for every port with the activated pins to be servos.

unsigned char servovalues[TotalPICpins]; // Entry table for values sets for every pin-servo.

unsigned char maxminpos[2][TotalPICpins]; // This table keeps minimum(0 degrees) and maximum(180 degrees) values(in ticks) that the servo can reach.

//-----------------------------------------------------------------------------------------------------------------------------
//  Functions for SERVO library
//-----------------------------------------------------------------------------------------------------------------------------
void servos_init()
{
    unsigned char a;

    // Filling up the servovalues table to 255. 
    for(a=0;a<TotalPICpins;a++)
    {
        servovalues[a]=255;  //Filling up the servovalues table to 255.
        maxminpos[0][a]= DefaultSERVOMIN; // Setting min servo position to 1000 usec.
        maxminpos[1][a]= DefaultSERVOMAX; // Setting max servo position to 2000 usec.
    }
    
    // Filling up the activated servos matrix.
    for(a=0;a<TotalPICports;a++)
        activatedservos[a]=0x00;  // Setting all pins as deactivated as servo.

    INTCONbits.GIEH    = 0; // Disable global HP interrupts
    INTCONbits.GIEL    = 0; // Disable global LP interrupts
      
    T1CON=0x01; 			//timer 1 prescaler 1 source is internal oscillator
    TMR1H=0xFF; 			// First value on timer to start up...
    TMR1L=0x00; 			// ...now the first interrupt will be generated by timer after 9 ms.
    IPR1bits.TMR1IP = 1; 	// INT_HIGH_PRIORITY
    PIR1bits.TMR1IF = 0; 	// Setting flag to 0
    PIE1bits.TMR1IE = 1; 	// INT_ENABLE
    T1CONbits.TMR1ON   = 1; // Starting TMR1
    
    INTCONbits.GIEH    = 1; // Enable global HP interrupts
    INTCONbits.GIEL    = 1; // Enable global LP interrupts

}


static void ServosPulseDown()
{
    volatile timingindex = 0;

    for(timedivision=0;timedivision < 251;timedivision++)
    {
        if (timevalue[timingindex] == timedivision)
        {
            PORTA = PORTA ^ timings[timingindex][pA];
            PORTB = PORTB ^ timings[timingindex][pB];
            #if defined(__18f14k22) || defined(PINGUINO2455) || defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(CHRP3) || defined(PINGUINO26J50) || defined(FREEJALDUINO) || defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)
            PORTC = PORTC ^ timings[timingindex][pC];
            #endif
            #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)			
            PORTD = PORTD ^ timings[timingindex][pD];
            #endif
            #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50)
            PORTE = PORTE ^ timings[timingindex][pE];
            #endif
            timingindex++;
        }
        
        // NEW: Every round on this "for" loop must last 8 microseconds. So the following asm code is to adjust to that exact time.
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
    }
}

// This function starts up pulses for all activated servos.
static void ServosPulseUp()
{
    PORTA = PORTA | activatedservos[pA];
    PORTB = PORTB | activatedservos[pB];
    #if defined(__18f14k22) || defined(PINGUINO2455) || defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(CHRP3) || defined(PINGUINO26J50) || defined(FREEJALDUINO) || defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)
    PORTC = PORTC | activatedservos[pC];
    #endif
    #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)			
    PORTD = PORTD | activatedservos[pD];
    #endif
    #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50)
    PORTE = PORTE | activatedservos[pE];
    #endif
}


// This funtion analyses servovalues table and creates and ordered table(timings)
// from smaller to bigger of all the values, asociating to each
// position of the table the servos that matches that timing.
static void SortServoTimings()
{
    volatile unsigned char s,t,totalservos,numservos;

    // table initialization:
    for(t=0;t<TotalPICpins;t++)
    {
        timevalue[t]=255; 
        timings[t][pA]=0x00;
        timings[t][pB]=0x00;
        #if defined(__18f14k22) || defined(PINGUINO2455) || defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(CHRP3) || defined(PINGUINO26J50) || defined(FREEJALDUINO) || defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)
        timings[t][pC]=0x00;
        #endif
        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)			
        timings[t][pD]=0x00;
        #endif
        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50)
        timings[t][pE]=0x00;
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
            
            switch (port[s])
            {
                        case pA:
                                    if (mask[s] & mascaratotal[pA] & activatedservos[pA]){
                                        break;
                                    }
                                    else if (servovalues[s] < timevalue[t]){
                                        timevalue[t]=servovalues[s];
                                        timings[t][pA]=mask[s];
                                        timings[t][pB]=0x00;
                                        #if defined(__18f14k22) || defined(PINGUINO2455) || defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(CHRP3) || defined(PINGUINO26J50) || defined(FREEJALDUINO) || defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)
                                        timings[t][pC]=0x00;
                                        #endif
                                        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)			
                                        timings[t][pD]=0x00;
                                        #endif
                                        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50)
                                        timings[t][pE]=0x00;
                                        #endif
                                        numservos=1;
                                    }
                                    else if (servovalues[s] == timevalue[t]){
                                        timings[t][pA] |= mask[s];
                                        numservos++;
                                    }
                                    break;      		

                        case pB: 
                                    if (mask[s] & mascaratotal[pB] & activatedservos[pB]){
                                        break;
                                    }
                                    else if (servovalues[s] < timevalue[t]){
                                        timevalue[t]=servovalues[s];
                                        timings[t][pA]=0x00;
                                        timings[t][pB]=mask[s];
                                        #if defined(__18f14k22) || defined(PINGUINO2455) || defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(CHRP3) || defined(PINGUINO26J50) || defined(FREEJALDUINO) || defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)
                                        timings[t][pC]=0x00;
                                        #endif
                                        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)			
                                        timings[t][pD]=0x00;
                                        #endif
                                        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50)
                                        timings[t][pE]=0x00;
                                        #endif
                                        numservos=1;
                                    }
                                    else if (servovalues[s] == timevalue[t]){
                                        timings[t][pB] |= mask[s];
                                        numservos++;
                                    }
                                    break;

                        #if defined(__18f14k22) || defined(PINGUINO2455) || defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(CHRP3) || defined(PINGUINO26J50) || defined(FREEJALDUINO) || defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)
                        case pC:
                                    if (mask[s] & mascaratotal[pC] & activatedservos[pC]){
                                        break;
                                    }
                                    else if (servovalues[s] < timevalue[t]){
                                        timevalue[t]=servovalues[s];
                                        timings[t][pA]=0x00;
                                        timings[t][pB]=0x00;
                                        timings[t][pC]=mask[s];
                                        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)			
                                        timings[t][pD]=0x00;
                                        #endif
                                        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50)
                                        timings[t][pE]=0x00;
                                        #endif
                                        numservos=1;
                                    }
                                    else if (servovalues[s] == timevalue[t]){
                                        timings[t][pC] |= mask[s];
                                        numservos++;
                                    }
                                    break;
                        #endif

                        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)			
                        case pD:
                                    if (mask[s] & mascaratotal[pD] & activatedservos[pD]){
                                        break;
                                    }
                                    else if (servovalues[s] < timevalue[t]){
                                        timevalue[t]=servovalues[s];
                                        timings[t][pA]=0x00;
                                        timings[t][pB]=0x00;
                                        timings[t][pC]=0x00;
                                        timings[t][pD]=mask[s];
                                        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50)
                                        timings[t][pE]=0x00;
                                        #endif
                                        numservos=1;
                                    }
                                    else if (servovalues[s] == timevalue[t]){
                                        timings[t][pD] |= mask[s];
                                        numservos++;
                                    }
                                    break;
                        #endif

                        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50)
                        case pE:
                                    if (mask[s] & mascaratotal[pE] & activatedservos[pE]){
                                        break;
                                    }
                                    else if (servovalues[s] < timevalue[t]){
                                        timevalue[t]=servovalues[s];
                                        timings[t][pA]=0x00;
                                        timings[t][pB]=0x00;
                                        timings[t][pC]=0x00;
                                        timings[t][pD]=0x00;
                                        timings[t][pE]=mask[s];
                                        numservos=1;
                                    }
                                    else if (servovalues[s] == timevalue[t]){
                                        timings[t][pE] |= mask[s];
                                        numservos++;
                                    }
                                    break;
                        #endif
            }
        }
        mascaratotal[pA] |= timings[t][pA];
        mascaratotal[pB] |= timings[t][pB];
        #if defined(__18f14k22) || defined(PINGUINO2455) || defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(CHRP3) || defined(PINGUINO26J50)  || defined(FREEJALDUINO) || defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)
        mascaratotal[pC] |= timings[t][pC];
        #endif
        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)			
        mascaratotal[pD] |= timings[t][pD];
        #endif
        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50)
        mascaratotal[pE] |= timings[t][pE];
        #endif
                                        
        totalservos += numservos;
        t++;

    }
    needreordering=0;  // This indicates that servo timings are sorted.
}

void ServoAttach(unsigned char pin)
{
    if(pin>=TotalPICpins) return;

    switch (port[pin])
    {
        case pA: 
                activatedservos[pA] = activatedservos[pA] | mask[pin];  // list pin as servo driver.
                TRISA = TRISA & (~mask[pin]); 					// set as output pin
                break;
        case pB: 
                activatedservos[pB] = activatedservos[pB] | mask[pin];  // list pin as servo driver.
                TRISB = TRISB & (~mask[pin]); 					// set as output pin
                break;
        #if defined(__18f14k22) || defined(PINGUINO2455) || defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(CHRP3) || defined(PINGUINO26J50) || defined(FREEJALDUINO) || defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)
        case pC: 
                activatedservos[pC] = activatedservos[pC] | mask[pin];  // list pin as servo driver.
                TRISC = TRISC & (~mask[pin]); 					// set as output pin
                break;
        #endif
        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)				    
        case pD: 
                activatedservos[pD] = activatedservos[pD] | mask[pin];  // list pin as servo driver.
                TRISD = TRISD & (~mask[pin]); 					// set as output pin
                break;
        #endif
        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50)	    
        case pE: 
                activatedservos[pE] = activatedservos[pE] | mask[pin];  // list pin as servo driver.
                TRISE = TRISE & (~mask[pin]); 					// set as output pin
                break;	            	            
        #endif
    }
}

void ServoDetach(unsigned char pin)
{
    if(pin>=TotalPICpins) return;

    switch (port[pin])
    {
        case pA: activatedservos[pA] = activatedservos[pA] ^ mask[pin];
                break;
        case pB: activatedservos[pB] = activatedservos[pB] ^ mask[pin];
                break;
        #if defined(__18f14k22) || defined(PINGUINO2455) || defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(CHRP3) || defined(PINGUINO26J50) || defined(FREEJALDUINO) || defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO)
        case pC: activatedservos[pC] = activatedservos[pC] ^ mask[pin];
                break;
        #endif
        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PICUNO_EQUO) 
        case pD: activatedservos[pD] = activatedservos[pD] ^ mask[pin];
                break;
        #endif
        #if defined(PINGUINO47J53) || defined(PINGUINO47J53B) || defined(PINGUINO4550) || defined(PINGUINO45K50)	    
        case pE: activatedservos[pE] = activatedservos[pE] ^ mask[pin];
                break;
        #endif
    }
}

void ServoWrite(unsigned char servo,unsigned char degrees)
{
    unsigned char range;
    unsigned char ticksperdegree;
    unsigned char value;

    // Check if number of servo is valid
    if(servo>=TotalPICpins)
        return;

    // limitting degrees:
    if(degrees>180) degrees=180;

    // Converts degrees to timeslots
    range = (maxminpos[1][servo]  - maxminpos[0][servo]);
    value = (degrees*range) / 180 + maxminpos[0][servo];

    // Storage of that new position to servovalues positions table:
    // it should be added the min value for that servo
    servovalues[servo]= value;

    needreordering=1;  // This indicates servo timings must be reordered.
}

unsigned char ServoRead(unsigned char servo)
{
    if(servo>=TotalPICpins)        // test if numservo is valid
        return 0;
        
    return servovalues[servo];
}

void ServoMinimumPulse(unsigned char servo,int min_microseconds)
{
    // Check if number of servo is valid:
    if(servo>=TotalPICpins)
        return;

    // test if microseconds are within range:
    if (min_microseconds < MINUS) min_microseconds = MINUS;
    if (min_microseconds > MIDUS) min_microseconds = MIDUS;

    // The following formula converts min. microseconds to min. timeslot
    maxminpos[0][servo]=(min_microseconds - MINUS)>>3;   // 0 < final_min < 125
}

void ServoMaximumPulse(unsigned char servo,int max_microseconds)
{
    // Check if number of servo is valid:
    if(servo>=TotalPICpins)
        return;
        
    // test if microseconds are within range:
    if (max_microseconds < MIDUS) max_microseconds = MIDUS;
    if (max_microseconds > MAXUS) max_microseconds = MAXUS;

    // The following formula converts max. microseconds to max. timeslot
    maxminpos[1][servo]=(max_microseconds - MINUS)>>3;   // 125 < final_max < 250
}

//interrupt handler that handles servos
void servos_interrupt(void)
{
    if (PIR1bits.TMR1IF)
    {
        //T1CON=0x00;

        //case before 500 microseconds:

        if (phase)
        {
            ServosPulseUp();
            // Load at TMR1 (65535d - 6000d (-54usec for adjusments ) = 59481d = 0xE859 (after some calibration 0xe959)
            TMR1H= 0xe9;//0xe9;
            TMR1L= 0x59;
            // timer 1 prescaler 1 source is internal oscillator Fosc/4 (CPU clock or Fosc=48Mhz).
            T1CON=1;
            phase = 0;
        }

        //case before 2500 microseconds:

        else
        {
            //The following call takes 2 ms aprox.:
            ServosPulseDown();
            // After fisrt 2,5 ms we need a delay of 17,5 ms to complete a 20 ms cycle.
            // Loading at TMR1 65535d - (4,375 x 12000(=1ms)=) 52500d = 13035d = 0x32EB => 4,375 ms
            // This is 4,375ms x 4 (prescaler) = 17,5 ms
            TMR1H= 0x32;
            TMR1L= 0xeb;
            // timer 1 prescaler 1 source is internal oscillator Fosc/4 (recordemos que Fosc=48Mhz).
            if (needreordering)
                SortServoTimings(); // This takes more than 1 ms, but it's call only if needed.
            T1CON= ( 1 | 2 << 4 ) ; // activate timer1 and prescaler = 1:4
            phase = 1;  			//This indicates that after next interrupt it will start the servos cycle.
        }

        // enable interrupt again
        PIR1bits.TMR1IF=0;
    }
    return;
}

#endif

//-----------------------------------------------------------------------------------------------------------------------------
// End of SERVOLIBRARY
//----------------------------------------------------------------------------------------------------------------------------------
