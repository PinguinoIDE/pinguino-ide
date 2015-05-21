/*	----------------------------------------------------------------------------
    FILE:			io.c
    PROJECT:		pinguino
    PURPOSE:		PIC32 Peripheral Remappage and IOs Configuration
    PROGRAMER:		Jean-Pierre Mandon <jp.mandon@gmail.com>
                    Régis Blanchot <rblanchot@gmail.com>
    FIRST RELEASE:	16 Feb. 2012
    LAST RELEASE:	04 Mar. 2015
    ----------------------------------------------------------------------------
    CHANGELOG:
    20-02-12    Régis Blanchot  Exported from main32.c in this file
    25-02-12    Régis Blanchot  Added PWM remap. for PIC32 PINGUINO 220
    04-03-15    Régis Blanchot  Added AUDIO support
    20-03-15    Régis Blanchot  Removed call to system.c
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

#ifndef __IO_C
#define __IO_C

#include <p32xxxx.h>
#include <const.h>
#include <typedef.h>
//#include <system.c>

/***********************************************************************
 * Set all Analog Pins as Digital IOs.
 * The ANSELx register has a default value of 0xFFFF; therefore all pins
 * that share analog functions are analog (not digital) by default.
 **********************************************************************/
 
void IOsetDigital()
{
    #if defined(__32MX220F032D__) || defined(__32MX220F032B__) || \
        defined(__32MX250F128B__) || defined(__32MX270F256B__)
        CFGCONbits.JTAGEN=0;    // Disable the JTAG port, Port A as Digital Port
        ANSELA = 0;
        ANSELB = 0;
        #if defined(__32MX220F032D__)
            ANSELC = 0;
        #endif
    #else
        DDPCONbits.JTAGEN=0;  // Disable the JTAG port, Port A as Digital Port
        AD1PCFG = 0xFFFF;
    #endif
}

/***********************************************************************
 * Set all Pins as Output
 **********************************************************************/

void IOsetSpecial()
{
    #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
    TRISA  = 0;
    #endif
    
    TRISB  = 0;

    #if !defined(__32MX220F032B__) && \
        !defined(__32MX250F128B__) && !defined(__32MX270F256B__)

        TRISC  = 0;

        #if !defined(__32MX220F032D__)
            TRISD  = 0;
            TRISE  = 0;
            TRISF  = 0;
            TRISG  = 0;
        #endif

    #endif

    #if defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_OTG)
        TRISDbits.TRISD9  = INPUT;		// because PORTB is shared with SDA on Olimex board
        TRISDbits.TRISD10 = INPUT;		// because PORTB is shared with SCL on Olimex board
    #endif

    /* the followings are already declared in serial.c
    #if defined(PIC32_PINGUINO_220)
    TRISCbits.TRISC9 = OUTPUT;          // RC9 / U2TX output
    TRISCbits.TRISC8 = INPUT;           // RC8 / U2RX input
    #endif
    */
}

/***********************************************************************
 * PIC32 Peripheral Remappage
 **********************************************************************/

#if defined(__SERIAL__) || defined(__SPI__) || \
    defined(__PWM__)    || defined(__AUDIO__)

void IOsetRemap()
{
    #if defined(PIC32_PINGUINO_220)
    
        //SystemUnlock();
        SYSKEY = 0;                     // ensure OSCCON is locked
        SYSKEY = 0xAA996655;            // Write Key1 to SYSKEY
        SYSKEY = 0x556699AA;            // Write Key2 to SYSKEY
        CFGCONbits.IOLOCK=0;            // unlock configuration
        CFGCONbits.PMDLOCK=0;

        #ifdef __SERIAL__
            U2RXRbits.U2RXR=6;          // Define U2RX as RC8 ( D0 )
            RPC9Rbits.RPC9R=2;          // Define U2TX as RC9 ( D1 )
            U1RXRbits.U1RXR=2;          // Define U1RX as RA4 ( UEXT SERIAL )
            RPB4Rbits.RPB4R=1;          // Define U1TX as RB4 ( UEXT SERIAL )
        #endif

        #ifdef __SPI__
            SDI1Rbits.SDI1R=5;          // Define SDI1 as RA8 ( UEXT SPI )
            RPA9Rbits.RPA9R=3;          // Define SDO1 as RA9 ( UEXT SPI )
        #endif

        #if defined(__PWM__) || defined(__AUDIO__)
            RPC2Rbits.RPC2R  =0b0101;   // PWM0 = OC3 as D2  = RC2
            RPC3Rbits.RPC3R  =0b0101;   // PWM1 = OC4 as D3  = RC3
            RPB5Rbits.RPB5R  =0b0101;   // PWM2 = OC2 as D11 = RB5
            RPB13Rbits.RPB13R=0b0110;   // PWM3 = OC5 as D12 = RB13
            RPB15Rbits.RPB15R=0b0101;   // PWM4 = OC1 as D13 = RB15
        #endif

        CFGCONbits.IOLOCK=1;            // relock configuration
        CFGCONbits.PMDLOCK=1;
        //SystemLock();
        SYSKEY = 0x12345678;            // Write any value other than Key1 or Key2

    #endif

    // Thanks to danirobin
    #if defined(PINGUINO32MX220) || \
        defined(PINGUINO32MX250) || \
        defined(PINGUINO32MX270)
        
        //SystemUnlock();
        SYSKEY = 0;                     // ensure OSCCON is locked
        SYSKEY = 0xAA996655;            // Write Key1 to SYSKEY
        SYSKEY = 0x556699AA;            // Write Key2 to SYSKEY
        CFGCONbits.IOLOCK=0;			// unlock configuration
        CFGCONbits.PMDLOCK=0;

        #ifdef __SERIAL__
            // Serial 1
            U1RXRbits.U1RXR = 0b0100;   // Define U1RX as RB2 ( D10 )
            RPB3Rbits.RPB3R = 0b0001;   // Define U1TX as RB3 ( D9 )
            // Serial 2
            U2RXRbits.U2RXR = 0b0010;   // Define U2RX as RB1 (D11)
            RPB0Rbits.RPB0R = 0b0010;   // Define U2TX as RB0 (D12)
        #endif

        // SDI2 : 0011 = RPB13 (D2) or 0100 = RPB2 (D10)
        // SDO2 : RA1 (D14), RA4 (D7), RB1 (D11), RB2 (D10), RB5 (D8), RB8 (D4)
        #ifdef __SPI__
            // SPI1
            SDI1Rbits.SDI1R = 0b0001;   // Define SDI1 as RB5 ( D6 )
            RPA4Rbits.RPA4R = 0b0011;   // Define SDO1 as RA4 ( D7 )
            // SCK1 is not remappable             SCK1 = RB14 ( D1 )
            
            // SPI2
            SDI2Rbits.SDI2R = 0b0011;   // Define SDI2 as RB13 ( D2 )
            RPB8Rbits.RPB8R = 0b0100;   // Define SDO2 as RB8  ( D4 )
            // SCK2 is not remappable             SCK2 = RB15  ( D0 )

            //SS1Rbits.SS1R   = 0b0100;   // Define SS1  as RB7 ( D5 )
            //RPB7Rbits.RPB7R = 0b0011;   // Define SS1  as RB7 ( D5 )
        #endif

        #if defined(__PWM__) || defined(__AUDIO__)
            RPB4Rbits.RPB4R   = 0b0101; // PWM0 = OC1 = RB4  = D8
            RPA4Rbits.RPA4R   = 0b0110; // PWM1 = OC4 = RA4  = D7
            RPB5Rbits.RPB5R   = 0b0101; // PWM2 = OC2 = RB5  = D6
            RPB13Rbits.RPB13R = 0b0101; // PWM3 = OC5 = RB13 = D2
            RPB14Rbits.RPB14R = 0b0101; // PWM4 = OC3 = RB14 = D1
        #endif

        CFGCONbits.IOLOCK=1;			// relock configuration
        CFGCONbits.PMDLOCK=1;	
        //SystemLock();
        SYSKEY = 0x12345678;            // Write any value other than Key1 or Key2

    #endif
}

#endif // defined(__SERIAL__) || defined(__SPI__) || \
       // defined(__PWM__)    || defined(__AUDIO__)

#endif /* __IO_C */
