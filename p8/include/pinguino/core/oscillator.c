/*  --------------------------------------------------------------------
    FILE:           oscillator.c
    PROJECT:        pinguino
    PURPOSE:        pinguino system clock switching functions
    PROGRAMER:      regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:  5 Jan. 2011
    LAST RELEASE:   2 jan. 2013
    --------------------------------------------------------------------
    The Clock Switching mode is controlled by the SCS bits in the OSCCON
    register. These bits can be changed via software during run time to
    change the clock source.
    There are three choices that can be selected for the system clock
    via the clock switching feature. They include:
    * System Clock determined by the Fosc settings in the Configuration Word
    * INTOSC – Internal Oscillator
    * Timer1 External 32.768 Khz clock crystal
    --------------------------------------------------------------------
    CHANGELOG:
    21-11-2012      regis blanchot  added PINGUINO1220,1320,14k22 support
    07-12-2012      regis blanchot  added PINGUINO25K50 and 45K50 support
                                    added low power functions
    06-01-2015      regis blanchot  
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

#ifndef __OSCILLATOR_C
#define __OSCILLATOR_C

#include <pic18fregs.h>
#include <typedef.h>
#include <const.h>
#include <macro.h>
//#include <millis.c>

#ifndef CRYSTAL
#define CRYSTAL 8000000L
#endif

#if defined(_MILLIS_C_)
extern volatile u32 _reload_val;
#endif

volatile u32 _cpu_clock_;

// The indices are valid values for PLLDIV
//static const u8 plldiv[] = { 12, 10, 6, 5, 4, 3, 2, 1 };

#if defined(__18f14k22) || \
    defined(__18f25k50) || defined(__18f45k50)

    #define FINTOSC     16000000    // INTOSC Frequency
    // The indices are valid values for CPDIV
    static const u8 cpudiv[] = { 6, 3, 2, 1 };	/* TODO */
    static const u8 cpudiv_xtal[] = { 1, 1, 1, 1 };
    // The indices are valid values for IRCF
    static const u32 ircf[] = { 31250, 250000, 500000, 1000000, 2000000, 4000000, 8000000, 16000000 };

#elif   defined(__18f2455)  || defined(__18f4455)  || \
        defined(__18f2550)  || defined(__18f4550)   

    #define FINTOSC     8000000     // INTOSC Frequency
    // The indices are valid values for CPDIV
    static const u8 cpudiv[] = { 2, 3, 4, 6 };
    static const u8 cpudiv_xtal[] = { 1, 2, 3, 4 };
    // The indices are valid values for IRCF
    static const u32 ircf[] = { 31250, 125000, 250000, 500000, 1000000, 2000000, 4000000, 8000000 };

#elif   defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f27j53) || defined(__18f47j53)

    #define FINTOSC     8000000     // INTOSC Frequency
    // The indices are valid values for CPDIV
    static const u8 cpudiv[] = { 6, 3, 2, 1 }; // rblanchot 2014-03-27 - fixed 
    // The indices are valid values for IRCF
    static const u32 ircf[] = { 31250, 125000, 250000, 500000, 1000000, 2000000, 4000000, 8000000 };

#endif

// Different frequencies available
#define _64MHZ_     64000000
#define _48MHZ_     48000000
#define _32MHZ_     32000000
#define _16MHZ_     16000000
#define _8MHZ_       8000000
#define _4MHZ_       4000000
#define _2MHZ_       2000000
#define _1MHZ_       1000000
#define _500KHZ_      500000
#define _250KHZ_      250000
#define _125KHZ_      125000
#define _32K768HZ_     32768
#define _31K25HZ_      31250
#define _31KHZ_        31000

// 2014-09-05 RB -	Last versions of SDCC (>3.4.1) doesn't have
//					CONFIG words definition for some PIC ...

// CONFIG1H: CONFIGURATION REGISTER 1 HIGH (BYTE ADDRESS 300001h)    
#if !defined(__CONFIG1H)
#define __CONFIG1H 0x300001
#endif

// CONFIG1L: CONFIGURATION REGISTER 1 LOW (BYTE ADDRESS 300000h)
#if !defined(__CONFIG1L)
#define __CONFIG1L 0x300000
#endif

/*  --------------------------------------------------------------------
    SystemReadFlashMemory() read in all relevant clock settings
    ------------------------------------------------------------------*/

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

/*  --------------------------------------------------------------------
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
    ------------------------------------------------------------------*/

u32 System_getCpuFrequency() 
{
    u8 CPUDIV;

    #if defined(__18f2455)  || defined(__18f4455)  || \
        defined(__18f2550)  || defined(__18f4550)

    u8 fosc, CPUDIV_XTAL;

    #endif

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

            #else // xxJ5x

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
                  defined(__18f26j53) || defined(__18f46j53) || \
                  defined(__18f27j53) || defined(__18f47j53)

            if (OSCTUNEbits.PLLEN)
            {
                return 48000000UL / CPUDIV;
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

/*  --------------------------------------------------------------------
    Calculates the Peripheral frequency.
    On PIC18F, Peripheral Freq. = CPU. Freq. / 4
    TODO : replace with #define ?
    ------------------------------------------------------------------*/

#define SystemGetInstructionClock()		System_getPeripheralFrequency()	

u32 System_getPeripheralFrequency() 
{
    return System_getCpuFrequency() >> 2;
}

/*  --------------------------------------------------------------------
    Functions to change clock source and frequency
    --------------------------------------------------------------------

    OSCCON: OSCILLATOR CONTROL REGISTER
    --------------------------------------------------------------------
    OSTS: Oscillator Start-up Time-out Status bit
    1 = Device is running from the clock defined by FOSC<3:0> of the CONFIG1H register
    0 = Device is running from the internal oscillator (HFINTOSC or INTRC)

    SCS<1:0>: System Clock Select bit
    1x = Internal oscillator block
    01 = Secondary (SOSC) oscillator
    00 = Primary clock (determined by FOSC<3:0> in CONFIG1H).

    OSCCON2: OSCILLATOR CONTROL REGISTER 2
    --------------------------------------------------------------------
    SOSCRUN: SOSC Run Status bit
    1 = System clock comes from secondary SOSC
    0 = System clock comes from an oscillator, other than SOSC

    SOSCDRV: SOSC Drive Control bit
    1 = T1OSC/SOSC oscillator drive circuit is selected by Configuration bits, CONFIG2L <4:3>
    0 = Low-power T1OSC/SOSC circuit is selected
 
    SOSCGO: Secondary Oscillator Start Control bit
    1 = Secondary oscillator is enabled.
    0 = Secondary oscillator is shut off if no other sources are requesting it.

    PRISD: Primary Oscillator Drive Circuit Shutdown bit
    1 = Oscillator drive circuit on
    0 = Oscillator drive circuit off (zero power)

    INTSRC: HFINTOSC Divided by 512 Enable bit
    1 = HFINTOSC used as the 31.25 kHz system clock reference – high accuracy
    0 = INTRC used as the 31.25 kHz system clock reference – low power.

    OSCTUNE: OSCILLATOR TUNING REGISTER (PICxxJxx only)
    --------------------------------------------------------------------
    INTSRC: Internal Oscillator Low-Frequency Source Select bit
    1 = 31.25 kHz device clock derived from 8 MHz INTOSC source (divide-by-256 enabled)
    0 = 31 kHz device clock derived directly from INTRC internal oscillator
    ------------------------------------------------------------------*/

#if defined(__18f14k22) || defined(__18f2455)  || \
    defined(__18f2550)  || defined(__18f4550)  || \
    defined(__18f25k50) || defined(__18f45k50) || \
    defined(__18f26j50) || defined(__18f46j50) || \
    defined(__18f26j53) || defined(__18f46j53) || \
    defined(__18f27j53) || defined(__18f47j53)

/*  --------------------------------------------------------------------
    Switch to System Clock determined by the Fosc settings
    in the Configuration Word.
    When the SCS bits = 00, this can be the Internal Oscillator,
    External Crystal/Resonator or External Clock.
    Pinguino's Configuration Word are set to External Crystal.
    ------------------------------------------------------------------*/

#if defined(SYSTEMSETEXTOSC) || defined(SYSTEMSETCPUFREQUENCY) || defined(SYSTEMSETPERIPHERALFREQUENCY)
void System_setExtOsc()
{
    OSCCONbits.SCS  = 0b00;
    // Wait while Oscillator Start-up Timer time-out is running
    // which means the primary oscillator is not yet ready
    //while (!OSCCONbits.OSTS);

    //#if defined(__18f25k50) || defined(__18f45k50)
    //OSCCON2bits.INTSRC = ;
    //#else
    //OSCTUNEbits.INTSRC = ;
    //#endif
}
#endif /* defined(SYSTEMSETEXTOSC) || defined(SYSTEMSETPERIPHERALFREQUENCY) */

/*  --------------------------------------------------------------------
    Switch on Timer1 External 32768Hz clock crystal
    With help from Mike Hall
    --------------------------------------------------------------------
    When the SCS bits = 01, the system clock is switched to the Secondary
    Oscillator which is an external 32768Hz crystal that controls
    the Timer1 Peripheral. The external clock crystal is an optional
    clock source that must be part of the Timer1 design circuit.
    ------------------------------------------------------------------*/

#if defined(SYSTEMSETTIMER1OSC) || defined(SYSTEMSETCPUFREQUENCY) || defined(SYSTEMSETPERIPHERALFREQUENCY)
void System_setTimer1Osc()
{
    TMR1H = 0x80;
    TMR1L = 0x00;

    #if defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f26j53) || defined(__18f46j53) || \
        defined(__18f27j53) || defined(__18f47j53) || \
        defined(__18f25k50) || defined(__18f45k50)
        
    T1GCONbits.TMR1GE = 0;      // Timer1 is not gated, set to 1 if timer1 doesnt work
    
    #endif
    
    // Set the timer1 to increment asynchronously to the internal
    // phase clocks. Running asynchronously allows the external clock
    // source to continue incrementing the timer during Sleep and can
    // generate an interrupt on overflow.

    T1CONbits.NOT_T1SYNC = 1;   // Make the external clock un-synchronized.   
    T1CONbits.TMR1CS = 0b10;    // Timer1 Source is the External Oscillator
    T1CONbits.T1OSCEN = 1;      // Enable Timer1 as a Source Clock
    T1CONbits.TMR1ON = 1;       // Starts Timer1

    #if defined(__18f26j53) || defined(__18f46j53) || \
        defined(__18f27j53) || defined(__18f47j53)
        
    OSCCON2bits.SOSCDRV = 1;    // Osc. high drive level
    
    #endif
    
    #if defined(__18f26j53) || defined(__18f46j53) || \
        defined(__18f27j53) || defined(__18f47j53) || \
        defined(__18f25k50) || defined(__18f45k50)
        
    OSCCON2bits.SOSCRUN = 0;    // System clock not from ext. Osc
    OSCCON2bits.SOSCGO = 1;     // turn on Osc.
    OSCCON2bits.PRISD = 1;      // turn on Osc.
    
    #endif
    
    OSCCONbits.SCS = 0b01;      // Switch System Clock to the Timer1
}
#endif /* defined(SYSTEMSETTIMER1OSC) || defined(SYSTEMSETPERIPHERALFREQUENCY) */

/*  --------------------------------------------------------------------
    Switch to Internal Oscillator (INTOSC)
    When the SCS bits = 0b10 or 0b11, then the system clock is switched
    to Internal Oscillator independent of the Fosc configuration bit
    settings. The IRCF bits of the OSCCON register will select the
    internal oscillator frequency.
    ------------------------------------------------------------------*/

#if defined(SYSTEMSETINTOSC) || defined(SYSTEMSETCPUFREQUENCY) || defined(SYSTEMSETPERIPHERALFREQUENCY)
void System_setIntOsc(u32 freq)
{
    u8 _save_gie;
    u8 sel=0;
    u8 i;
    
    #if defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f26j53) || defined(__18f46j53) || \
        defined(__18f27j53) || defined(__18f47j53)
        
    u16 pll_startup_counter = 600;

    #endif

    /// -----------------------------------------------------------------
    /// 1- Save status and Disable Interrupt  
    /// -----------------------------------------------------------------

    _save_gie = INTCONbits.GIE;
    INTCONbits.GIE = 0;

    /// -----------------------------------------------------------------
    /// 2- if freq > FINTOSC
    /// -----------------------------------------------------------------

    if (freq > FINTOSC)
    {
        #if defined(__18f14k22) || \
            defined(__18f26j50) || defined(__18f46j50) || \
            defined(__18f27j53) || defined(__18f47j53)
            
            OSCTUNEbits.PLLEN = 1;
            // Enable the PLL and wait 2+ms until the PLL locks
            while (pll_startup_counter--);
            //OSCCONbits.SCS  = 0b00; // Select PLL.

            if (freq = 32000000)
            {
                _cpu_clock_ = 32000000; 
                OSCCONbits.IRCF = 0b110;
            }
       
        #elif defined(__18f2455)  || defined(__18f4455)  || \
              defined(__18f2550)  || defined(__18f4550)  || \
              defined(__18f26j50) || defined(__18f46j50) || \
              defined(__18f27j53) || defined(__18f47j53)

            if (freq == 48000000)
            {
                _cpu_clock_ = 48000000;
                OSCCONbits.IRCF = 0b111;
            }
            
        #elif defined(__18f14k22)
        
            if (freq == 64000000)
            {
                _cpu_clock_ = 64000000;
                OSCCONbits.IRCF = 0b111;
            }

        #else // invalid freq

            _cpu_clock_ = FINTOSC;

        #endif
    }

    /// -----------------------------------------------------------------
    /// 3- if freq <= FINTOSC
    /// -----------------------------------------------------------------

    if (freq <= FINTOSC)
    {
        //INTSRC: Internal Oscillator Low-Frequency Source Select bit
        //0 = 31 kHz device clock derived directly from INTRC internal oscillator
        if (freq == 31000)
        {
            _cpu_clock_ = 31000;
            
            #if defined(__18f25k50) || defined(__18f45k50)

            OSCCON2bits.PLLEN = 0;      // PLL disabled
            OSCCON2bits.INTSRC = 0;     // select INTOSC as a 31 KHz clock source

            #elif defined(__18f2455) || defined(__18f4550) || \
                  defined(__18f2550) || defined(__18f4550)

            OSCTUNEbits.INTSRC = 0;     // select INTOSC as a 31 KHz clock source

            #else

            OSCTUNEbits.PLLEN = 0;      // PLL disabled
            OSCTUNEbits.INTSRC = 0;     // select INTOSC as a 31 KHz clock source

            #endif

            OSCCONbits.IRCF = 0;
        }

        //INTSRC: Internal Oscillator Low-Frequency Source Select bit
        //1 = 31.25 kHz device clock derived from INTOSC source
        else if (freq == 31250)
        {
            _cpu_clock_ = 31250;
            
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

            OSCCONbits.IRCF = 0;
        }

        else
        {
            
            // calculate a valid freq (must be a multiple of FINTOSC)
            // and get the corresponding bits (IRCF) to select
            while ( ( (FINTOSC / freq) << sel++ ) < 0x80 );
            _cpu_clock_ = ircf[sel];

            OSCCONbits.IRCF = sel;
        }

    }
    
    /// -----------------------------------------------------------------
    /// 4- Switch to Internal Oscillator (INTOSC)  
    /// -----------------------------------------------------------------

    OSCCONbits.SCS  = 0b11;

    /// -----------------------------------------------------------------
    /// 5- wait INTOSC frequency is stable (HFIOFS=1)
    /// -----------------------------------------------------------------

    #if defined(__18f25k50) || defined(__18f45k50)

    while (!OSCCONbits.HFIOFS); 

    #endif

    // RB : Can not work because this function call System_getPeripheralFrequency()
    //updateMillisReloadValue();
    #if defined(_MILLIS_C_)
    
    //INTCONbits.TMR0IE = 0; //INT_DISABLE;
    _reload_val = 0xFFFF - (_cpu_clock_ / 4 / 1000) ;
    //INTCONbits.TMR0IE = 1; //INT_ENABLE;
    
    #endif
    
    /// -----------------------------------------------------------------
    /// 6- Get back to Interrupt status
    /// -----------------------------------------------------------------

    INTCONbits.GIE = _save_gie;
}
#endif /* defined(SYSTEMSETINTOSC) || defined(SYSTEMSETPERIPHERALFREQUENCY) */

/*  --------------------------------------------------------------------
    Set the CPU frequency
    ------------------------------------------------------------------*/

#if defined(SYSTEMSETCPUFREQUENCY) 
void System_setCpuFrequency(u32 freq) 
{
    if (freq == 32768)
        System_setTimer1Osc();
    else if (freq == 48000000)
        System_setExtOsc();
    else
        System_setIntOsc(freq);
}
#endif

/*  --------------------------------------------------------------------
    Set the Peripheral frequency (Fosc/4)
    ------------------------------------------------------------------*/

#if defined(SYSTEMSETPERIPHERALFREQUENCY)
void System_setPeripheralFrequency(u32 freq)
{
    if (freq > 12000000UL)
    {
        freq = 12000000UL;
    }

    if (OSCCONbits.SCS == 0b00)
    {
        // TODO
    }

    else if (OSCCONbits.SCS == 0b11)
    {
        System_setIntOsc(freq*4);
    }

    else
    {
        System_setExtOsc(freq*4);
    }
}
#endif /* defined(SYSTEMSETPERIPHERALFREQUENCY) */

#else

    #error "This library doesn't support your processor."
    #error "Please contact a developper."

#endif /* defined(__18f14k22) ... */

#endif /* __OSCILLATOR_C */

