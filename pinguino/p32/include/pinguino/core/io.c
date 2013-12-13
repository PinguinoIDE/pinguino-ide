/*	----------------------------------------------------------------------------
	FILE:			io.c
	PROJECT:		pinguino
	PURPOSE:		PIC32 Peripheral Remappage and IOs Configuration
	PROGRAMER:		Jean-Pierre Mandon <jp.mandon@gmail.com>
 					Régis Blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	16 feb. 2012
	LAST RELEASE:	16 feb. 2012
	----------------------------------------------------------------------------
	CHANGELOG:
	[20-02-12][Régis Blanchot][Exported from main32.c in this file]
	[25-02-12][Régis Blanchot][Added PWM remap. for PIC32 PINGUINO 220]
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

#ifndef __REMAP_C
#define __REMAP_C

#include <p32xxxx.h>
#include <const.h>
#include <typedef.h>
#include <system.c>

void IOsetSpecial()
{
	#if defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_OTG)
	TRISDbits.TRISD9  = INPUT;		// because PORTB is shared with SDA on Olimex board
	TRISDbits.TRISD10 = INPUT;		// because PORTB is shared with SCL on Olimex board
	#endif	
	
	#if defined(PIC32_PINGUINO_220)
	//the followings are already declared in serial.c
	//TRISCbits.TRISC9 = OUTPUT;	// RC9 / U2TX output
	//TRISCbits.TRISC8 = INPUT;		// RC8 / U2RX input			
	#endif	
}

// All Analog Pins as Digital IOs
void IOsetDigital()
{
	#if defined(__32MX220F032D__)||defined(__32MX250F128B__)||defined(__32MX220F032B__)
			DDPCONbits.JTAGEN=0;		// check : already in system.c
			ANSELA = 0;
			ANSELB = 0;
			#if defined(__32MX220F032D__)
				ANSELC = 0;
			#endif
	#else
	AD1PCFG = 0xFFFF;
	#endif
}

// PIC32 Peripheral Remappage
void IOsetRemap()
{
#if defined(PIC32_PINGUINO_220)
	SystemUnlock();
	CFGCONbits.IOLOCK=0;			// unlock configuration
	CFGCONbits.PMDLOCK=0;
	#ifdef __SERIAL__
		U2RXRbits.U2RXR=6;			// Define U2RX as RC8 ( D0 )
		RPC9Rbits.RPC9R=2;			// Define U2TX as RC9 ( D1 )
		U1RXRbits.U1RXR=2;			// Define U1RX as RA4 ( UEXT SERIAL )
		RPB4Rbits.RPB4R=1;			// Define U1TX as RB4 ( UEXT SERIAL )
	#endif
	#ifdef __SPI__
		SDI1Rbits.SDI1R=5;			// Define SDI1 as RA8 ( UEXT SPI )
		RPA9Rbits.RPA9R=3;			// Define SDO1 as RA9 ( UEXT SPI )
	#endif
	#ifdef __PWM__
		RPC2Rbits.RPC2R  =0b0101;	// PWM0 = OC3 as D2  = RC2
		RPC3Rbits.RPC3R  =0b0101;	// PWM1 = OC4 as D3  = RC3
		RPB5Rbits.RPB5R  =0b0101;	// PWM2 = OC2 as D11 = RB5
		RPB13Rbits.RPB13R=0b0110;	// PWM3 = OC5 as D12 = RB13
		RPB15Rbits.RPB15R=0b0101;	// PWM4 = OC1 as D13 = RB15
	#endif
	CFGCONbits.IOLOCK=1;			// relock configuration
	CFGCONbits.PMDLOCK=1;	
	SystemLock();
#endif

// Thanks to danirobin
#if defined(PINGUINO32MX250) || defined(PINGUINO32MX220)
	SystemUnlock();
	CFGCONbits.IOLOCK=0;			// unlock configuration
	CFGCONbits.PMDLOCK=0;
	#ifdef __SERIAL__
		U2RXRbits.U2RXR =0b0010;    // Define U2RX as RB1
		RPB0Rbits.RPB0R =0b0010;    // Define U2TX as RB0 _
		U1RXRbits.U1RXR =0b0100;	// Define U1RX as RB2 ( D10 )
		RPB3Rbits.RPB3R =0b0001;	// Define U1TX as RB3 ( D9 )
	#endif
	#ifdef __SPI__
		SDI1Rbits.SDI1R=1;			// Define SDI1 as RB5  [Overlap!]
		RPA4Rbits.RPA4R=3;			// Define SDO1 as RA4  [Overlap!]
	#endif
    #ifdef __PWM__
        RPB4Rbits.RPB4R =0b0101;    // PWM0 = OC1 = RB4
        RPA4Rbits.RPA4R =0b0110;    // PWM1 = OC4 = RA4
        RPB5Rbits.RPB5R =0b0101;    // PWM2 = OC2 = RB5
        RPB13Rbits.RPB13R=0b0101;   // PWM3 = OC5 = RB13
        RPB14Rbits.RPB14R=0b0101;   // PWM4 = OC3 = RB14
    #endif
	CFGCONbits.IOLOCK=1;			// relock configuration
	CFGCONbits.PMDLOCK=1;	
	SystemLock();
#endif
}

#endif /* __REMAP_C */
