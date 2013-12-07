/*  ----------------------------------------------------------------------------
    FILE:           oscillator.c
    PROJECT:        pinguino
    PURPOSE:        pinguino system functions
    PROGRAMER:      regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:  5 Jan. 2011
    LAST RELEASE:   2 jan. 2013
    ----------------------------------------------------------------------------
    CHANGELOG:
    21-11-2012        regis blanchot        added PINGUINO1220,1320,14k22 support
    07-12-2012        regis blanchot        added PINGUINO25K50 and 45K50 support
                                            added low power functions
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

#ifndef __OSCILLATOR_C
#define __OSCILLATOR_C

#include <pic18fregs.h>
#include <typedef.h>
#include <const.h>
#include <macro.h>

#ifndef CRYSTAL
#define CRYSTAL 8000000L
#endif

u32 _cpu_clock_;

// The indices are valid values for PLLDIV
//static const u8 plldiv[] = { 12, 10, 6, 5, 4, 3, 2, 1 };

// The indices are valid values for IRCF
#if defined(__18f14k22) || \
    defined(__18f25k50) || defined(__18f45k50)
// The indices are valid values for CPDIV
static const u8 cpudiv[] = { 6, 3, 2, 1 };	/* TODO */
static const u8 cpudiv_xtal[] = { 1, 1, 1, 1 };
static const u32 ircf[] = { 31250, 250000, 500000, 1000000, 2000000, 4000000, 8000000, 16000000 };

#elif   defined(__18f2455)  || defined(__18f4455)  || \
        defined(__18f2550)  || defined(__18f4550)   
// The indices are valid values for CPDIV
static const u8 cpudiv[] = { 2, 3, 4, 6 };
static const u8 cpudiv_xtal[] = { 1, 2, 3, 4 };
static const u32 ircf[] = { 31250, 125000, 250000, 500000, 1000000, 2000000, 4000000, 8000000 };

#elif   defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f27j53) || defined(__18f47j53)
// The indices are valid values for CPDIV
static const u8 cpudiv[] = { 6, 3, 2, 1 };
static const u32 ircf[] = { 31250, 125000, 250000, 500000, 1000000, 2000000, 4000000, 8000000 };

#endif

#if defined(__18f14k22)

    #define _64MHZ_     0b10000111  // 7  NB = 16 x PLL
    #define _32MHZ_     0b10000110  // 7  NB =  8 x PLL
    #define _16MHZ_     0b111       // 7
    #define _8MHZ_      0b110       // 6
    #define _4MHZ_      0b101       // 5
    #define _2MHZ_      0b100       // 4
    #define _1MHZ_      0b011       // 3
    #define _500KHZ_    0b010       // 2
    #define _250KHZ_    0b001       // 1
    #define _31KHZ_     0b000       // 0

#elif defined(__18f25k50) || defined(__18f45k50)

    #define _16MHZ_     0b111       // 7
    #define _8MHZ_      0b110       // 6
    #define _4MHZ_      0b101       // 5
    #define _2MHZ_      0b100       // 4
    #define _1MHZ_      0b011       // 3
    #define _500KHZ_    0b010       // 2
    #define _250KHZ_    0b001       // 1
    #define _31KHZ_     0b000       // 0

#elif defined(__18f2455)  || defined(__18f4455)  || \
      defined(__18f2550)  || defined(__18f4550)  || \
      defined(__18f26j50) || defined(__18f46j50) || \
      defined(__18f27j53) || defined(__18f47j53)

    #define _48MHZ_     0b10000111  // 7  NB = PLL 96MHz / 2
    #define _8MHZ_      0b111       // 7
    #define _4MHZ_      0b110       // 6
    #define _2MHZ_      0b101       // 5
    #define _1MHZ_      0b100       // 4
    #define _500KHZ_    0b011       // 3
    #define _250KHZ_    0b010       // 2
    #define _125KHZ_    0b001       // 1
    #define _31KHZ_     0b000       // 0
    
#endif

/*  ----------------------------------------------------------------------------
    Switch on Primary clock source
    --------------------------------------------------------------------------*/

    #define System_setExtOsc()      do { OSCCONbits.SCS = 0b00; } while (0)

/*  ----------------------------------------------------------------------------
    Switch on Timer1 oscillator, assumes this is a watch crystal
    --------------------------------------------------------------------------*/

    #define System_setTimer1Osc()   do { OSCCONbits.SCS = 0b01; } while (0)

/*  ----------------------------------------------------------------------------
    SystemReadFlashMemory() read in all relevant clock settings
    --------------------------------------------------------------------------*/

static u16 System_readFlashMemory(u32 address)
{
	u8 h8,l8;

	TBLPTRU=address>>16;
	TBLPTRH=address>>8;
	TBLPTRL=address;
	__asm tblrd*+ __endasm;
	l8=TABLAT;
	__asm tblrd*+ __endasm;
	h8=TABLAT;
	return((h8<<8)+l8);
}

/*  ----------------------------------------------------------------------------
    Calculates the CPU frequency.

    - if PLL is enabled
        * CPU Freq. = 48MHz / CPUDIV
        * Incoming Freq. = 4 * PLLDIV (unused) (x3 or x4 on 25k50)
    - if PLL is disabled
        * if OSCCONbits.SCS == 0, Incoming Freq. = External Oscillator
                CPU Freq. = Incoming Freq. / CPUDIV
                Inc. Freq. is unknown, must be defined by user (for ex. #define CRYSTAL = 8000000L)
        * if OSCCONbits.SCS == 1, Incoming Freq. = Timer1
                CPU Freq. = Incoming Freq.
                how to get freq. from Timer 1 ?
        * if OSCCONbits.SCS >= 2, Incoming Freq. = Internal Oscillator
                CPU Freq. = Incoming Freq.
                Inc. Freq. = IRCF bits

    TODO : 18f14k22
    --------------------------------------------------------------------------*/

u32 System_getCpuFrequency() 
{
    u8 CPUDIV,CPUDIV_XTAL,fosc;

    switch (OSCCONbits.SCS)
    {
        case 0: // primary osc. (internal or external / CPUDIV)

            #if defined(__18f25k50) || defined(__18f45k50)
            
            if (OSCCON2bits.PLLEN)
            {
                return 48000000L;
            } 
			else
            {
                return CRYSTAL;
            }
            
            #endif
            
            // CPUDIV ?
            
            #if defined(__18f2455)  || defined(__18f4455)  || \
                defined(__18f2550)  || defined(__18f4550)
                
            CPUDIV  = ( System_readFlashMemory(__CONFIG1L) & 0b00011000 ) >> 3;
			CPUDIV_XTAL = cpudiv_xtal[CPUDIV];
            fosc  = System_readFlashMemory(__CONFIG1H) & 0b00001111;

            #else

            CPUDIV  = System_readFlashMemory(__CONFIG1H) & 0b00000011;

            #endif

            CPUDIV = cpudiv[CPUDIV];
            
            // PLL ?
            
            #if defined(__18f2455) || defined(__18f4455) || \
                  defined(__18f2550) || defined(__18f4550)
                  
			fosc >>=1;
            if( (fosc==0) || (fosc==2) || (fosc==6) )
            {
                return CRYSTAL / CPUDIV_XTAL;
            }
            else
            {
                return 96000000UL / CPUDIV;
            }
            
            #elif defined(__18f26j50) || defined(__18f46j50) || \
                  defined(__18f27j53) || defined(__18f47j53)

            if (OSCTUNEbits.PLLEN)
            {
                return 48000000L / CPUDIV;
            } 
			else
            {
                return CRYSTAL / CPUDIV;
            }
            
            #endif
            
        case 1: // secondary osc. (timer 1, most of the time 32768 Hz)
            return 32768;
            
        case 2: // reserved or postscaled internal clock
        case 3: // postscaled internal clock (IRCF)
            return ircf[OSCCONbits.IRCF];
    }
}

/*  ----------------------------------------------------------------------------
    Calculates the Peripheral frequency.
    On PIC18F, Peripheral Freq. = CPU. Freq. / 4
    TODO : replace with #define
    --------------------------------------------------------------------------*/
#define SystemGetInstructionClock()		System_getPeripheralFrequency()	
u32 System_getPeripheralFrequency() 
{
    return System_getCpuFrequency() >> 2;
}

/*  ----------------------------------------------------------------------------
    Switch on postscaled internal oscillator
    * 18f14k22 : PLL enabled = INTOSC x 4 only if INTOSC = 8 or 16 MHz
    --------------------------------------------------------------------------*/

#if defined(__18f14k22) || defined(__18f2455)  || \
    defined(__18f2550)  || defined(__18f4550)  || \
    defined(__18f25k50) || defined(__18f45k50) || \
    defined(__18f26j50) || defined(__18f46j50) || \
    defined(__18f27j53) || defined(__18f47j53)


#if defined(SYSTEMSETINTOSC) || defined(SYSTEMSETPERIPHERALFREQUENCY)
void System_setIntOsc(u8 speed)
{
    u8 flag=0;
    u8 _save_gie;
    
    #if defined(__18f26j50) || defined(__18f46j50)
    u16 pll_startup_counter = 600;
    #endif

    _save_gie = INTCONbits.GIE;
	INTCONbits.GIE = 0;

    OSCCONbits.SCS  = 0b11;     // Select Internal Osc.
    
    #if defined(__18f25k50) || defined(__18f45k50)
    OSCCON2bits.PLLEN = 0;      // PLL disabled
    OSCCON2bits.INTSRC = 1;     // select INTOSC as a 31.25 KHz clock source
    #elif defined(__18f2455) || defined(__18f4550) || \
          defined(__18f2550) || defined(__18f4550)
    OSCTUNEbits.INTSRC = 1;     // select INTOSC as a 31.25 KHz clock source
    #else
    OSCTUNEbits.PLLEN = 0;      // PLL disabled
    OSCTUNEbits.INTSRC = 1;     // select INTOSC as a 31.25 KHz clock source
    #endif

    #if defined(__18f14k22)
    if (speed == _64MHZ_) {
		cpu_clock_ = 64000000; 
		flag = 1; 
        OSCTUNEbits.PLLEN = 1;
        // Enable the PLL and wait 2+ms until the PLL locks
        while (pll_startup_counter--);
		OSCCONbits.SCS  = 0b00; // Select PLL.
    }
    #elif defined(__18f26j50) || defined(__18f46j50) || \
          defined(__18f27j53) || defined(__18f47j53)

    if (speed == _48MHZ_) {
        _cpu_clock_ = 48000000;
        OSCTUNEbits.PLLEN = 1;
        // Enable the PLL and wait 2+ms until the PLL locks
        while (pll_startup_counter--);
		OSCCONbits.SCS  = 0b00; // Select PLL.
    }
    #elif defined(__18f14k22)
    if (speed == _32MHZ_) {
		_cpu_clock_ = 32000000; 
        OSCTUNEbits.PLLEN = 1;
        // Enable the PLL and wait 2+ms until the PLL locks
        while (pll_startup_counter--);
		OSCCONbits.SCS  = 0b00; // Select PLL.
    }
    #endif

    #if defined(__18f14k22) || defined(__18f25k50) || defined(__18f45k50)
    if (speed == _16MHZ_)  _cpu_clock_ = 16000000;
    #endif

    if (speed == _8MHZ_)   _cpu_clock_ =  8000000;
    if (speed == _4MHZ_)   _cpu_clock_ =  4000000;
    if (speed == _2MHZ_)   _cpu_clock_ =  2000000;
    if (speed == _1MHZ_)   _cpu_clock_ =  1000000;
    if (speed == _500KHZ_) _cpu_clock_ =   500000;
    if (speed == _250KHZ_) _cpu_clock_ =   250000;

    #if defined(__18f2455)  || defined(__18f4455)  || \
        defined(__18f2550)  || defined(__18f4550)  || \
        defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f27j53) || defined(__18f47j53)

    if (speed == _125KHZ_) _cpu_clock_ =   125000;
    #endif

    if (speed == _31KHZ_)  _cpu_clock_ =    31250;

    if (speed >= 128)       speed -= 128;
    OSCCONbits.IRCF = speed;

    #if defined(__18f25k50) || defined(__18f45k50)
    while (!OSCCONbits.HFIOFS); // wait INTOSC frequency is stable (HFIOFS=1) 
    #endif

	updateMillisReloadValue();
	INTCONbits.GIE = _save_gie;
}

#ifdef SYSTEMSETPERIPHERALFREQUENCY
void System_setPeripheralFrequency(u8 speed)
{
    SystemSetIntOsc(speed*4);
}
#endif /* SYSTEMSETPERIPHERALFREQUENCY */

#endif /* defined(SYSTEMSETINTOSC) || defined(SYSTEMSETPERIPHERALFREQUENCY) */

#endif /* defined(__18f14k22) ... */

#endif /* __OSCILLATOR_C */

