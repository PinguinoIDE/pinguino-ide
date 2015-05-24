/*	----------------------------------------------------------------------------
    FILE:			digitalw.c
    PROJECT:		pinguino
    PURPOSE:
    PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
    FIRST RELEASE:	02 Dec. 2010
    LAST RELEASE:	02 Dec. 2013
    ----------------------------------------------------------------------------
    CHANGELOG:
    [02-12-10][jp.mandon@gmail.com][tested from 0 to 31 OK except PORTA pin]
    [02-12-10][rblanchot@gamil.com][PORTA pin issue fixed in system.c (JTAGEN)]
    [13-02-11][jp.mandon@gmail.com][updated for EMPEROR 460 and PIC32_PINGUINO]
    [15-02-11][Marcus Fazzi][UBW32 mask fixed]
    [23-02-11][Marcus Fazzi][PGC/PGD order for UBW32 mask fixed]
    [18-03-11][Marcus Fazzi][Added support for MX795]
    [20-03-11][Marcus Fazzi][IO mapping for Emperor boards]
    [31-03-11][rblanchot@gmail.com][fixed conditional compilation for board support]
    [30-01-12][rblanchot@gmail.com][added PIC32-PINGUINO-220 et PIC32-PINGUINO-MICRO]
    [03-03-12][jp.mandon@gmail.com][added UEXT pin for PIC32-PINGUINO-MX220]
    [29-03-12][hgmvanbeek@gmail.com][extended PIC32-PINGUINO-MICRO pinning]
    [13-05-12][jp.mandon@gmail.com][extended PINGUINO32MX250 and PINGUINO32MX22 pinning]
    [02-12-13][rblanchot@gmail.com][updated toggle function]
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

#ifndef __DIGITALW_C
#define __DIGITALW_C

#include <p32xxxx.h>
//#include <typedef.h>

#define pA	0
#define pB	1
#define pC	2
#define pD	3
#define pE	4
#define pF	5
#define pG	6

#define _0	1<<0	// 0x0001
#define _1	1<<1	// 0x0002
#define _2	1<<2	// 0x0004
#define _3	1<<3	// 0x0008
#define _4	1<<4	// 0x0010
#define _5	1<<5	// 0x0020
#define _6	1<<6	// 0x0040
#define _7	1<<7	// 0x0080
#define _8	1<<8	// 0x0100
#define _9	1<<9	// 0x0200
#define _10	1<<10	// 0x0400
#define _11	1<<11	// 0x0800
#define _12	1<<12	// 0x1000
#define _13	1<<13	// 0x2000
#define _14	1<<14	// 0x4000
#define _15	1<<15	// 0x8000

//----------------------------------------------------------------------

#if defined(PIC32_PINGUINO)

const int portmask[]=
                 {pD,pD,pD,pD,pD,pD,pD,pD,	// 0-7
                  pB,pB,pG,pG,pG,pG,pB,pB,	// 8-15
                  pB,pB,pD,pD,pB,pB,pE,pE,	// 16-23
                  pE,pE,pE,pE,pE,pE,pD,pF,	// 24-31
                  pF};				// 32

const int pinmask[]={_2,_3,_0,_5,_6,_7,_8,_11,		// 0-7
                  _14,_15,_9,_8,_7,_6,_1,_2,	// 8-15
                  _3,_4,_9,_10,_11,_10,_0,_1,		// 16-23
                  _2,_3,_4,_5,_6,_7,_1,_1,			// 24-31
                  _0};											// 32

//----------------------------------------------------------------------

#elif defined(PIC32_PINGUINO_T795)

const int portmask[]= {
                  pB,pB,pB,pB,pB,pB,pD,pD,	// 0-7
                  pD,pD,pE,pE,pE,pE,pE,pE,	// 8-15
                  pE,pE,pB,pB,pF,pD,pD,pD,	// 16-23
                  pD,pD,pD,pD,pD,pF,pF,pF,	// 24-31
                  pF,pB,pB,pB,pB,pG,pG,pG,	// 32-39
                  pG,pB,pB,pB,pB};          // 40-44

const int pinmask[]= {
                  _3,_4,_6,_7,_9,_10,_11,_3,	// 0-7
                  _2,_1,_0,_1,_2,_3,_4,_5,	    // 8-15
                  _6,_7,_11,_12,_3,_8,_9,_10,   // 16-23
                  _0,_4,_5,_6,_7,_0,_1,_5,      // 24-31
                  _4,_8,_14,_13,_15,_6,_7,_8,   // 32-39
                  _9,_5,_2,_1,_0};              // 40-44

//----------------------------------------------------------------------

#elif defined(PIC32_PINGUINO_OTG)

const int portmask[]= {
                  pD,pD,pD,pD,pD,pD,pD,pD,	// 0-7
                  pB,pB,pG,pG,pG,pG,pB,pB,	// 8-15
                  pB,pB,pD,pD,pB,pB,pE,pE,	// 16-23
                  pE,pE,pE,pE,pE,pE,pD,pF,	// 24-31
                  pF};				// 32

const int pinmask[]= {
                  _2,_3,_0,_5,_6,_7,_8,_11,		// 0-7
                  _13,_14,_9,_8,_7,_6,_1,_2,	// 8-15
                  _3,_4,_9,_10,_11,_10,_0,_1,		// 16-23
                  _2,_3,_4,_5,_6,_7,_1,_1,			// 24-31
                  _0};											// 32

//----------------------------------------------------------------------

#elif defined(PIC32_PINGUINO_MICRO)

const int portmask[]= {	 pB,  pB,  pB,  pB,  pB,  pB,  pB,  pB,	// 0-7
                         pB,  pB,  pD,  pD,  pD,  pD,  pD,  pD,	// 8-15
                         pB,  pD,  pD,  pD,  pG,  pF,  pE,  pE,	// 16-23
                         pE,  pE,  pE,  pE,  pE,  pE,  pC,  pC,	// 24-31
                         pG,  pF,  pF,  pD,  pD,  pG,  pG,  pF, // 32-39
                         pB,  pD };								// 40-41  P41 strapped with P13

const int pinmask[]=  {  _1,  _2,  _3,  _4,  _8,  _9, _10, _11,	// 0-7
                        _12, _14,  _1,  _2,  _3,  _4,  _5,  _6,	// 8-15
                         _0,  _7,  _8, _11,  _9,  _1,  _7,  _6,	// 16-23
                         _5,  _4,  _3,  _2,  _1,  _0, _13, _14,	// 24-31
                         _6,  _4,  _5,  _9, _10,  _8,  _7,  _0, // 32-39
                        _13,  _0 };								// 40-41  P41 strapped with P13

//----------------------------------------------------------------------

#elif (PIC32_PINGUINO_220)

const int portmask[]={	pC,pC,pC,pC,pC,pC,pC,pC,	// D0-D7	: C8,C9,C2,C3,C4,C5,C6,C7
                        pB,pA,pA,pB,pB,pB,			// D8-D13	: B7,A10,A1,B5,B13,B15
                        pC,pC,pB,pB,pB,pB,			// D14-D19
                        pA,pB,pB };					// D20 (UEXT_CS),D21 (SDA1 UEXT), D22 (SCL1 UEXT)

const int pinmask[]={	_8,_9,_2,_3,_4,_5,_6,_7,	// D0-D7	: C8,C9,C2,C3,C4,C5,C6,C7
                        _7,_10,_1,_5,_13,_15,		// D8-D13	: B7,A10,A1,B5,B13,B15
                        _0,_1,_0,_1,_2,_3,			// A0-A5	: C0,C1,B0,B1,B2,B3
                        _7,_9,_8 };					// D20		: A7,B9,B8

//----------------------------------------------------------------------

#elif defined(PINGUINO32MX220) || defined(PINGUINO32MX250) || defined(PINGUINO32MX270)

const int portmask[]={	pB,pB,pB,pB,pB,pB,pB,pA,	// D0-D7	: B15,B14,B13,B9,B8,B7,B5,A4
                        pB,pB,pB,pB,pB,pA,			// D8-D13	: B4,B3,B2,B1,B0,A0
                        pA };						// D14 		: A1

const int pinmask[]={	_15,_14,_13,_9,_8,_7,_5,_4,	// D0-D7	: B15,B14,B13,B9,B8,B7,B5,A4
                        _4,_3,_2,_1,_0,_0,			// D8-D13	: B4,B3,B2,B1,B0,A0
                        _1 };						// D14 		: A1

//----------------------------------------------------------------------

#elif defined(EMPEROR460) || defined(EMPEROR795)

const int portmask[]=
                 {pC,pG,pG,pG,pG,pA,pE,pE,			//0-7
                  pB,pB,pB,pB,pB,pB,pA,pA,			//8-15
                  pB,pB,pB,pB,pA,pF,pF,pB,			//16-23
                  pB,pB,pB,pD,pD,pF,pF,pF,			//24-31
                  pF,pF,pA,pA,pA,pA,pA,pA,			//32-39
                  pC,pC,pC,pE,pE,pE,pG,pE,			//40-47
                  pE,pE,pG,pG,pG,pE,pE,pA,			//48-55
                  pA,pG,pG,pF,pF,pD,pD,pD,			//56-63
                  pD,pD,pD,pD,pD,pD,pC,pC,			//64-71
                  pD,pD,pD,pD,pD,pB,pB};			//72-78

const int pinmask[]={_4,_6,_7,_8,_9,_0,_8,_9,		//0-7		: E8=6,E9=7
                  _5,_4,_3,_2,_1,_0,_9,_10,			//8-15
                  _8,_9,_10,_11,_1,_13,_12,_12,		//16-23
                  _13,_14,_15,_14,_15,_4,_5,_3,		//24-31
                  _2,_8,_2,_3,_4,_5,_14,_15,		//32-39
                  _3,_2,_1,_7,_6,_5,_15,_4,			//40-47		: C3=40,C2=41,C1=42,E7=43,E6=44,E5=45,G15=46,E4=47
                  _3,_2,_13,_12,_14,_1,_0,_7,		//48-55		: E3=48,E2=49,G13=50,G12=51,G14=52,E1=53,E0=54,A7=55
                  _6,_0,_1,_1,_0,_7,_6,_5,			//56-63
                  _4,_13,_12,_3,_2,_1,_14,_13,		//64-71
                  _0,_11,_10,_9,_8,_7,_6};			//72-78

//----------------------------------------------------------------------
// define for UBW32 460/795 boards, same for Pinguino Minimum

#elif defined(UBW32_460) || defined(UBW32_795)

const int portmask[]=
                 {pC,pG,pG,pG,pG,pA,pE,pE, 			//0-7
                  pB,pB,pB,pA,pA,pB,pB,pB, 			//8-15
                  pB,pA,pF,pF,pB,pB,pB,pB,			//16-23
                  pD,pD,pF,pF,pF,pF,pA,pA,			//24-31
                  pA,pA,pA,pA,pD,pD,pD,pD,			//32-39
                  pD,pC,pC,pD,pD,pD,pD,pD,			//40-47
                  pD,pD,pD,pD,pF,pF,pG,pG,			//48-55
                  pA,pA,pE,pE,pG,pG,pG,pE,			//56-63
                  pE,pE,pG,pE,pE,pE,pC,pC,			//64-71
                  pC,pB,pB,pB,pB,pB,pF};			//72-78


const int pinmask[]={_4,_6,_7,_8,_9,_0,_8,_9,		//0-7
                  _5,_4,_3,_9,_10,_8,_9,_10,		//8-15
                  _11,_1,_13,_12,_12,_13,_14,_15, 	//16-23
                  _14,_15,_4,_5,_2,_8,_2,_3,		//24-31
                  _4,_5,_14,_15,_8,_9,_10,_11,		//32-39
                  _0,_13,_14,_1,_2,_3,_12,_13,		//40-47
                  _4,_5,_6,_7,_0,_1,_1,_0,			//48-55
                  _6,_7,_0,_1,_14,_12,_13,_2,		//56-63
                  _3,_4,_15,_5,_6,_7,_1,_2,			//64-71
                  _3,_7,_6,_2,_1,_0,_3 };			//72-78

#endif

//----------------------------------------------------------------------

//#if defined(PINMODE)
void pinmode(int pin, int state)
{
    switch (portmask[pin])
    {
        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
        case pA:
            if (state) TRISASET=pinmask[pin];
            else TRISACLR=pinmask[pin];
            break;
        #endif

        case pB:
            if (state) TRISBSET=pinmask[pin];
            else TRISBCLR=pinmask[pin];
            break;

        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
        case pC:
            if (state) TRISCSET=pinmask[pin];
            else TRISCCLR=pinmask[pin];
            break;
        #endif

        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
            !defined(__32MX220F032B__)
        case pD:
            if (state) TRISDSET=pinmask[pin];
            else TRISDCLR=pinmask[pin];
            break;
        case pE:
            if (state) TRISESET=pinmask[pin];
            else TRISECLR=pinmask[pin];
            break;
        case pF:
            if (state) TRISFSET=pinmask[pin];
            else TRISFCLR=pinmask[pin];
            break;
        case pG:
            if (state) TRISGSET=pinmask[pin];
            else TRISGCLR=pinmask[pin];
            break;
        #endif
    }
}
//#endif

//#if defined(DIGITALOUTPUT)
void output(int pin)
{
    switch (portmask[pin])
    {
        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
        case pA: TRISACLR=pinmask[pin]; break;
        #endif

        case pB: TRISBCLR=pinmask[pin]; break;

        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
        case pC: TRISCCLR=pinmask[pin]; break;
        #endif

        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
            !defined(__32MX220F032B__)
        case pD: TRISDCLR=pinmask[pin]; break;
        case pE: TRISECLR=pinmask[pin]; break;
        case pF: TRISFCLR=pinmask[pin]; break;
        case pG: TRISGCLR=pinmask[pin]; break;
        #endif
    }
}
//#endif

//#if defined(DIGITALINPUT)
void input(int pin)
{
    switch (portmask[pin])
    {
        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
        case pA: TRISASET=pinmask[pin]; break;
        #endif

        case pB: TRISBSET=pinmask[pin]; break;

        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
        case pC: TRISCSET=pinmask[pin]; break;
        #endif

        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
            !defined(__32MX220F032B__)
        case pD: TRISDSET=pinmask[pin]; break;
        case pE: TRISESET=pinmask[pin]; break;
        case pF: TRISFSET=pinmask[pin]; break;
        case pG: TRISGSET=pinmask[pin]; break;
        #endif
    }
}
//#endif

//#if defined(PINREAD)
int pinread(int pin)
{
    int reg = 0;

    switch (portmask[pin])
    {
        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
            case pA:
                reg = TRISA;
                break;
        #endif
        case pB:
            reg = TRISB;
            break;
        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
            case pC:
                reg = TRISC;
                break;
        #endif
        #if !defined(__32MX220F032B__) && !defined(__32MX220F032D__) && \
            !defined(__32MX250F128B__) && !defined(__32MX270F256B__)
			case pD:
				reg = TRISD;
				break;
			case pE:
				reg = TRISE;
				break;
			case pF:
				reg = TRISF;
				break;
			case pG:
				reg = TRISG;
				break;
        #endif
    }
    if ((reg & pinmask[pin])!=0)
        return 1;
    else
        return 0;
}
//#endif

//#if defined(DIGITALWRITE)
void digitalwrite(int pin, int state)
{
    switch (portmask[pin])
    {
        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
        case pA:
            if (state) PORTASET=pinmask[pin];
            else PORTACLR=pinmask[pin];
            break;
        #endif
        
        case pB:
            if (state) PORTBSET=pinmask[pin];
            else PORTBCLR=pinmask[pin];
            break;
            
        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
        case pC:
            if (state) PORTCSET=pinmask[pin];
            else PORTCCLR=pinmask[pin];
            break;
        #endif

        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
            !defined(__32MX220F032B__)
        case pD:
            if (state) PORTDSET=pinmask[pin];
            else PORTDCLR=pinmask[pin];
            break;
        case pE:
            if (state) PORTESET=pinmask[pin];
            else PORTECLR=pinmask[pin];
            break;
        case pF:
            if (state) PORTFSET=pinmask[pin];
            else PORTFCLR=pinmask[pin];
            break;
        case pG:
            if (state) PORTGSET=pinmask[pin];
            else PORTGCLR=pinmask[pin];
            break;
        #endif
    }
}
//#endif

//#if defined(DIGITALHIGH)
void high(int pin)
{
    switch (portmask[pin])
    {
        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
        case pA: PORTASET=pinmask[pin]; break;
        #endif
        
        case pB: PORTBSET=pinmask[pin]; break;
            
        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
        case pC: PORTCSET=pinmask[pin]; break;
        #endif

        #if !defined(__32MX220F032D__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && \
            !defined(__32MX220F032B__)
        case pD: PORTDSET=pinmask[pin]; break;
        case pE: PORTESET=pinmask[pin]; break;
        case pF: PORTFSET=pinmask[pin]; break;
        case pG: PORTGSET=pinmask[pin]; break;
        #endif
    }
}
//#endif

//#if defined(DIGITALLOW)
void low(int pin)
{
    switch (portmask[pin])
    {
        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
        case pA: PORTACLR=pinmask[pin]; break;
        #endif
        
        case pB: PORTBCLR=pinmask[pin]; break;
            
        #if !defined(__32MX220F032B__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__)
        case pC: PORTCCLR=pinmask[pin]; break;
        #endif

        #if !defined(__32MX220F032D__) && !defined(__32MX220F032B__) && \
			!defined(__32MX250F128B__) && !defined(__32MX270F256B__)
        case pD: PORTDCLR=pinmask[pin]; break;
        case pE: PORTECLR=pinmask[pin]; break;
        case pF: PORTFCLR=pinmask[pin]; break;
        case pG: PORTGCLR=pinmask[pin]; break;
        #endif
    }
}
//#endif

//#if defined(DIGITALREAD)
u8 digitalread(int pin)
{
    switch (portmask[pin])
    {
        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
            case pA: return((PORTA&pinmask[pin])!=0);
                break;
        #endif
        case pB: return((PORTB&pinmask[pin])!=0);
                break;
        #if !defined(__32MX220F032B__) && !defined(__32MX250F128B__) && !defined(__32MX270F256B__)
            case pC: return((PORTC&pinmask[pin])!=0);
                    break;
        #endif
        #if !defined(__32MX220F032D__) && !defined(__32MX220F032B__) && \
			!defined(__32MX250F128B__) && !defined(__32MX270F256B__)
			case pD: return((PORTD&pinmask[pin])!=0);
					break;
			case pE: return((PORTE&pinmask[pin])!=0);
					break;
			case pF: return((PORTF&pinmask[pin])!=0);
					break;
			case pG: return((PORTG&pinmask[pin])!=0);
					break;
        #endif
        default: return 255;
    }
    return 255;
}
//#endif

//#if defined(TOGGLE)
void toggle(int pin)
{
    //digitalwrite(pin, digitalread(pin)^1);

    switch (portmask[pin])
    {
        #if !defined(__32MX440F256H__) && !defined(__32MX795F512H__)
        case pA: PORTAINV=pinmask[pin]; break;
        #endif

        case pB: PORTBINV=pinmask[pin]; break;

        #if !defined(__32MX250F128B__) && !defined(__32MX270F256B__) && !defined(__32MX220F032B__)
        case pC: PORTCINV=pinmask[pin]; break;
        #endif

        #if !defined(__32MX220F032B__) && !defined(__32MX220F032D__) && \
            !defined(__32MX250F128B__) && !defined(__32MX270F256B__)
        case pD: PORTDINV=pinmask[pin]; break;
        case pE: PORTEINV=pinmask[pin]; break;
        case pF: PORTFINV=pinmask[pin]; break;
        case pG: PORTGINV=pinmask[pin]; break;
        #endif
    }
}
//#endif

#endif	/* __DIGITALW_C */
