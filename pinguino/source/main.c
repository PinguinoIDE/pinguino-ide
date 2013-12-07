/*  -------------------------------------------------------------------------
    FILE:           main.c
    PROJECT:        pinguino
    PURPOSE:        application main function
    PROGRAMER:      Jean-pierre Mandon - Régis Blanchot
    FIRST RELEASE:  19 Sep 2008
    LAST RELEASE:   31 Jul 2013
    ----------------------------------------------------------------------------
    CHANGELOG :
    Originally based on a file by (c) 2006 Pierre Gaufillet <pierre.gaufillet@magic.fr>
    19 Sep 2008 - Jean-pierre Mandon - adapted to Pinguino  
    21 Apr 2012 - Régis Blanchot - added bootloader v4.x support
    20 Jun 2012 - Régis Blanchot - added io.c support (remapping)
    05 Feb 2013 - Régis Blanchot - added interrupt init
    11 Feb 2013 - Régis Blanchot - removed call to crt0iPinguino.c
                                   added reset_isr() instead
    28 Feb 2013 - Régis Blanchot - added stack pointer initialization
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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
    -------------------------------------------------------------------------*/

#include <pic18fregs.h>
#include <typedef.h>
#include <const.h>
#include <macro.h>

#ifdef noboot
    // runtime start code with variable initialisation
    //#include "crt0.c"
    //#include "crt0i.c"
    #include "config.h"
#endif

#ifdef boot4
    // runtime start code with variable initialisation
    //#include "crt0.c"
    //#include "crt0i.c"
    #include "crt0iz.c"
#endif

////////////////////////////////////////////////////////////////////////
#include "define.h"
////////////////////////////////////////////////////////////////////////

#include <pin.h>    // needs define.h to be included first
#include <io.c>     // needs define.h to be included first

////////////////////////////////////////////////////////////////////////
#include "user.c"   // user's .pde file translated to C
////////////////////////////////////////////////////////////////////////

#ifdef boot2

    #if !defined(__18f2455) && !defined(__18f4455) && \
        !defined(__18f2550) && !defined(__18f4550)
        #error "********************************"
        #error "*** Wrong Bootloader Version ***"
        #error "***  Press F9 to change it.  ***"
        #error "********************************"
        
    #endif
    
    // 2013-07-31 - A. Gentric - fix usb.c
    //#include <common_types.h>
    //#include <boot_iface.h>
    
    // only for compatibility with application_iface.o
    #ifndef __USB__
        void epap_in()      { return; }
        void epap_out()     { return; }
        void epapin_init()  { return; }
        void epapout_init() { return; }
    #endif

    // Application entry point called from bootloader v2.12
    void pinguino_main(void)

#elif defined (noboot) || defined(boot4)

    // Application entry point called from bootloader v4.x
    void main(void)

#endif

{
    #if defined(__18f25k50) || defined(__18f45k50) || \
        defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f26j53) || defined(__18f46j53) || \
        defined(__18f27j53) || defined(__18f47j53)

        u16 pll_startup_counter = 600;

    #endif
    
    /// ----------------------------------------------------------------
    /// If we start from a Power-on reset, set NOT_POR bit to 1
    /// ----------------------------------------------------------------

    if (RCONbits.NOT_POR == 0)
    {
        RCON |= 0b10010011;     // set all reset flag
                                // enable priority levels on interrupts
    }
    
    /// ----------------------------------------------------------------
    /// Perform a loop for some processors until their frequency is stable
    /// ----------------------------------------------------------------

    #if defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f26j53) || defined(__18f46j53) || \
        defined(__18f27j53) || defined(__18f47j53)

        // Enable the PLL and wait 2+ms until the PLL locks
        OSCTUNEbits.PLLEN = 1;
        while (pll_startup_counter--);
    
        // If Internal Oscillator is used
        // ???

    #elif defined(__18f2455) || defined(__18f4455) || \
          defined(__18f2550) || defined(__18f4550)
    
        // If Internal Oscillator is used
        if (OSCCONbits.SCS > 0x02)
            // wait INTOSC frequency is stable (IOFS=1) 
            while (!OSCCONbits.IOFS);

    #elif defined(__18f25k50) || defined(__18f45k50)
    
        // If Internal Oscillator is used
        if (OSCCONbits.SCS > 0x02)
            // wait HFINTOSC frequency is stable (HFIOFS=1) 
            while (!OSCCONbits.HFIOFS);

        // Enable the PLL and wait 2+ms until the PLL locks
        OSCCON2bits.PLLEN = 1;
        while (pll_startup_counter--);

    #endif

    /// ----------------------------------------------------------------
    /// Init. all flag/interrupt
    /// ----------------------------------------------------------------

    INTCONbits.GIEH     = 0;        // Disables all HP interrupts
    INTCONbits.GIEL     = 0;        // Disables all LP interrupts

/*
    INTCON = 0;
    INTCON2 = 0;
    INTCON3 = 0;
*/

/*
    // All peripheral interrupt disabled
    PIE1 = 0;
    PIE2 = 0;
    #if defined(__18f25k50) || defined(__18f45k50) || \
        defined(__18f26j50) || defined(__18f46j50)
    PIE3 = 0;
    #endif
*/

/*
    // All interrupts with low priority
    INTCON2bits.TMR0IP = 0;
    IPR1bits.TMR1IP = 0;
    IPR1bits.TMR2IP = 0;
    IPR1 = 0;
    IPR2 = 0;
    #if defined(__18f25k50) || defined(__18f45k50) || \
        defined(__18f26j50) || defined(__18f46j50)
    IPR3 = 0;
    #endif
*/

/*
    // All peripheral interrupts flags cleared
    PIR1 = 0;
    PIR2 = 0;
    #if defined(__18f25k50) || defined(__18f45k50) || \
        defined(__18f26j50) || defined(__18f46j50)
    PIR3 = 0;
    #endif
*/

    /// ----------------------------------------------------------------
    /// I/O init 
    /// ----------------------------------------------------------------

    IO_init();
    IO_digital();
    
    #if defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f26j53) || defined(__18f46j53) || \
        defined(__18f27j53) || defined(__18f47j53)

    IO_remap();

    #endif

    /// ----------------------------------------------------------------
    /// Various Init.
    /// ----------------------------------------------------------------

    #ifdef __USB__
    usb_init();
    #endif

    #ifdef __USBCDC
    CDC_init();
    #endif    

    #ifdef __USBBULK
    bulk_init();
    #endif

    #if defined(ANALOGREFERENCE) || defined(ANALOGREAD)
    analog_init();
    #endif

    #ifdef ANALOGWRITE
    analogwrite_init();
    #endif

    #ifdef __MILLIS__           // Use Timer 0
    millis_init();
    #endif

    #ifdef SERVOSLIBRARY        // Use Timer 1
    servos_init();
    #endif

    #ifdef __PS2KEYB__
    keyboard_init()
    #endif

////////////////////////////////////////////////////////////////////////
    setup();
////////////////////////////////////////////////////////////////////////

    #if defined(TMR0INT) || defined(TMR1INT) || \
        defined(TMR2INT) || defined(TMR3INT) || \
        defined(TMR4INT) || defined(TMR5INT) || \
        defined(TMR6INT) || defined(TMR8INT) 

    IntTimerStart();            // Enable all defined timers interrupts
                                // at the same time

    #endif

    #ifdef ON_EVENT
    //IntInit();
    INTCONbits.GIEH = 1;   // Enable global HP interrupts
    INTCONbits.GIEL = 1;   // Enable global LP interrupts
    #endif

    while (1)
    {
////////////////////////////////////////////////////////////////////////
        loop();
////////////////////////////////////////////////////////////////////////
    }
}

#if  defined(__USBCDC)      || defined(__USBBULK)   || defined(__USB__)     || \
     defined(USERINT)       || defined(INT0INT)     || defined(I2CINT)      || \
     defined(__SERIAL__)    || defined(ON_EVENT)    || defined(__MILLIS__)  || \
     defined(SERVOSLIBRARY) || defined(__PS2KEYB__) || defined(__DCF77__)   || \
     defined(RTCCALARMINTENABLE)

/*  ----------------------------------------------------------------------------
    High Interrupt Vector
    --------------------------------------------------------------------------*/

#ifdef boot2
#pragma code high_priority_isr 0x2020
#endif

// boot4 : ENTRY + 0x08
// noboot: 0x08
void high_priority_isr(void) __interrupt 1
{
    __asm
        MOVFF   TBLPTRL, POSTDEC1
        MOVFF   TBLPTRH, POSTDEC1
        MOVFF   TBLPTRU, POSTDEC1
        MOVFF   TABLAT,  POSTDEC1
    __endasm;

    #ifdef __USBCDC
    CDC_interrupt();
    #endif
    
    #if defined(__USBBULK)
    bulk_interrupt();
    #endif

    #ifdef __USB__
    usb_interrupt();
    #endif

    #ifdef __SERIAL__
    serial_interrupt();
    #endif

    #ifdef __MILLIS__
    millis_interrupt();
    #endif

    #ifdef I2CINT
    I2C_interrupt();
    #endif

    #ifdef SERVOSLIBRARY
    servos_interrupt();
    #endif

    #ifdef INT0INT
    userhighinterrupt();
    #endif

    #ifdef __PS2KEYB__
    keyboard_interrupt();
    #endif

    #ifdef __DCF77__
    dcf77_interrupt();
    #endif

    #ifdef RTCCALARMINTENABLE
    rtcc_interrupt();
    #endif

    __asm 
        MOVFF   PREINC1, TABLAT
        MOVFF   PREINC1, TBLPTRU
        MOVFF   PREINC1, TBLPTRH
        MOVFF   PREINC1, TBLPTRL
    __endasm;

}

/*  ----------------------------------------------------------------------------
    Low Interrupt Vector
    --------------------------------------------------------------------------*/

#ifdef boot2
#pragma code low_priority_isr 0x4000
#endif

// boot4 : ENTRY + 0x18
// noboot: 0x18
void low_priority_isr(void) __interrupt 2
{

    __asm
        MOVFF   TBLPTRL, POSTDEC1
        MOVFF   TBLPTRH, POSTDEC1
        MOVFF   TBLPTRU, POSTDEC1
        MOVFF   TABLAT,  POSTDEC1
    __endasm;

    #ifdef USERINT
    userinterrupt();
    #endif

    #ifdef ON_EVENT
    userlowinterrupt();
    #endif

    __asm 
        MOVFF   PREINC1, TABLAT
        MOVFF   PREINC1, TBLPTRU
        MOVFF   PREINC1, TBLPTRH
        MOVFF   PREINC1, TBLPTRL
    __endasm;

}

#endif

/*  ----------------------------------------------------------------------------
    Reset Interrupt Vector
    --------------------------------------------------------------------------*/
/*
#if defined (noboot) || defined(boot4)

// boot4 : ENTRY + 0x00
// noboot: 0x00
void reset_isr(void) __naked __interrupt 0
{
    // Call the Pinguino main routine.
    main();
}

#endif
*/
