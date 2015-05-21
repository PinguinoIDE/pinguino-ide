/*	--------------------------------------------------------------------
	FILE:			digital.h
	PROJECT:		Pinguino 8
	PURPOSE:		Digital IO management
	PROGRAMER:		Régis Blanchot
	FIRST RELEASE:	15 Mar. 2014
	LAST RELEASE:	15 Mar. 2014
	----------------------------------------------------------------------------
	TODO : 
	----------------------------------------------------------------------------
    CHANGELOG :
        regis blanchot 15 Mar. 2014 : first release
	----------------------------------------------------------------------------
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
	------------------------------------------------------------------*/

#ifndef __DIGITAL_H__
#define __DIGITAL_H__

//#include <pic18fregs.h>
//#include <pin.h>
//#include <typedef.h>

#define pA	0
#define pB	1
#define pC	2
#define pD	3
#define pE	4
#define pF	5
#define pG	6

#define _0	0x01    // 1<<0
#define _1	0x02    // 1<<1 
#define _2	0x04    // 1<<2
#define _3	0x08    // 1<<3 
#define _4	0x10    // 1<<4 
#define _5	0x20    // 1<<5 
#define _6	0x40    // 1<<6 
#define _7	0x80    // 1<<7 

/**********************************************************************/
#if defined(PINGUINO1220) || defined(PINGUINO1320)
/**********************************************************************/
                                                    // Pinguino pin number
const unsigned char mask[19]={
                    _0,_1,_2,_3,_4,_5,_6,_7,        // 0 - 7
                    _0,_1,_2,_3,_4,_5};             // 8 - 13

const unsigned char port[19]={
                    pB, pB, pB, pB, pB, pB, pB, pB, // 0 - 7
                    pA, pA, pA, pA, pA, pA};        // 8 - 13

/**********************************************************************/
#elif defined(__18f14k22)
/**********************************************************************/
                                                    // Pinguino pin number
const unsigned char mask[19]={
                    _5,_4,_3,_5,_4,_3,_6,_7,_7,     // 0 - 8
                    _0,_1,_2,_0,_1,_2,_4,_5,_6};    // 9 - 17

const unsigned char port[19]={
                    pA, pA, pA, pC, pC, pC, pC, pC, pB, // 0 - 8
                    pA, pA, pA, pC, pC, pC, pB, pB, pB }; // 9 - 17

/**********************************************************************/
#elif defined(PINGUINO2455) || defined(PINGUINO2550) || defined(PINGUINO25K50)
/**********************************************************************/
                                                    // Pinguino pin number
const unsigned char mask[19]={
                    _0,_1,_2,_3,_4,_5,_6,_7,        // 0 - 7
                    _6,_7,_0,_1,_2,                 // 8 - 12
                    _0,_1,_2,_3,_5,_4};             // 13 - 18

const unsigned char port[19]={
                    pB, pB, pB, pB, pB, pB, pB, pB, // 0 - 7
                    pC, pC, pC, pC, pC,             // 8 - 12
                    pA, pA, pA, pA, pA, pA};        // 13 - 18

/**********************************************************************/
#elif defined(PINGUINO26J50) || defined(PINGUINO27J53)
/**********************************************************************/
                                                    // Pinguino pin number
const unsigned char mask[18]={
                    _0,_1,_2,_3,_4,_5,_6,_7,        // 0 - 7
                    _6,_7,_0,_1,_2,                 // 8 - 12
                    _0,_1,_2,_3,_5};                // 13 -17

const unsigned char port[18]={
                    pB, pB, pB, pB, pB, pB, pB, pB, // 0 -7
                    pC, pC, pC, pC, pC,             // 8 - 12
                    pA, pA, pA, pA, pA};            // 13 - 17

/**********************************************************************/
#elif defined(PINGUINO4550)
/**********************************************************************/
                                                    // Pinguino pin number
const unsigned char mask[30]={
                    _0,_1,_2,_3,_4,_5,_6,_7,		// 0 - 7
                    _6,_7,_0,_1,_2,					// 8 - 12
                    _0,_1,_2,_3,_5,				    // 13 - 17
                    _0,_1,_2,						// 18 - 20
                    _0,_1,_2,_3,_4,_5,_6,_7,  		// 21 - 28
                    _4                              // 29
                    };

const unsigned char port[30]={
                    pB, pB, pB, pB, pB, pB, pB, pB,
                    pC, pC, pC, pC, pC,
                    pA, pA, pA, pA, pA,
                    pE, pE, pE,
                    pD, pD, pD, pD, pD, pD, pD, pD,
                    pA};

/**********************************************************************/
#elif defined(PINGUINO45K50)
/**********************************************************************/
                                                    // Pinguino pin number
const unsigned char mask[36]={
                    _0,_1,_2,_3,_4,_5,_6,_7,		// 00 - 07
                    _0,_1,_2,_3,_4,_5,_6,_7,		// 08 - 15
                    _0,_1,_2,_3,_4,_5,_6,_7,		// 16 - 23
                    _0,_1,_2,_3,_4,_5,_6,_7,		// 24 - 31
                    _0,_1,_2,_3						// 32 - 35
                    };

const unsigned char port[36]={
                    pB, pB, pB, pB, pB, pB, pB, pB,
                    pA, pA, pA, pA, pA, pA, pA, pA,
                    pC, pC, pC, pC, pC, pC, pC, pC,
                    pD, pD, pD, pD, pD, pD, pD, pD,
                    pE, pE, pE, pE
                    };

/**********************************************************************/
#elif defined(PINGUINO46J50) || defined(PINGUINO47J53) || defined(PINGUINO47J53B)
/**********************************************************************/
                                                    // Pinguino pin number
const unsigned char mask[32]={
                    _0,_1,_2,_3,_4,_5,_6,_7,        // 00 - 07
                    _0,_1,_2,_3,_4,_0,_1,_2,        // 08 - 15
                    _0,_1,_2,_3,_4,_5,_6,_7,        // 16 - 23
                    _0,_1,_2,_3,_4,_5,_6,_7,        // 24 - 31
                    };            

const unsigned char port[32]={
                    pB, pB, pB, pB, pB, pB, pB, pB, // 00 - 07
                    pA, pA, pA, pA, pA, pE, pE, pE, // 08 - 15
                    pC, pC, pC, pC, pC, pC, pC, pC, // 16 - 23
                    pD, pD, pD, pD, pD, pD, pD, pD  // 24 - 31
                    };            

/**********************************************************************/
#elif defined(FREEJALDUINO)
/**********************************************************************/

const unsigned char mask[19]={
                    _7,_6,_4,_0,_1,_2,_3,_4,_5,_6,_7,_0,_1,_2,_0,_1,_2,_3, _5};

const unsigned char port[19]={
                    1,1,2,0,0,0,0,0,0,0,0,1,1,1,2,2,2,2,2};

/**********************************************************************/
#elif defined(PICUNO_EQUO)
/**********************************************************************/

#if defined(PICUNOV1) // Early version of PICUNO_EQUO (BLUE PCB)
const unsigned char mask[22]={
                    _7,_6,_4,_0,_1,_2,_3,_4,        // 0 - 7
                    _5,_6,_7,_0,_1,_2,              // 8 - 13
                    _0,_1,_2,_3,_5,_0,_1,           // 14 - 21
                    };
                    
const unsigned char port[23]={
                    pC, pC, pA, pB, pB, pB, pB, pB, // 0 - 7
                    pB, pB, pB, pC, pC, pC,         // 8 - 13
                    pA, pA, pA, pA, pA, pE, pE, pE  // 14 - 21
                    };
#else                // Second and last version (BLACK PCB)
const unsigned char mask[21]={
                    _7,_6,_2,_3,_0,_2,_1,_1,        // 0 - 7
                    _2,_3,_4,_5,_6,_7,              // 8 - 13
                    _0,_1,_2,_5,_0,_1,_4            // 14 - 20
                    };
                    
const unsigned char port[21]={
                    pC, pC, pB, pB, pD, pC, pC, pD,
                    pD, pD, pD, pD, pD, pD,
                    pA, pA, pA, pA, pE, pE, pA
                    };
#endif

/**********************************************************************/
#elif defined(CHRP3)
/**********************************************************************/
                                                    // Pinguino pin number
const unsigned char mask[21]={
                    _0,_1,_2,_3,_4,_5,_6,_7,        // 00 - 07
                    _6,_7,_0,_1,_2,                 // 08 - 12
                    _0,_1,_2,_3,_4,_5,_6,_7};       // 13 - 20

const unsigned char port[21]={
                    pB, pB, pB, pB, pB, pB, pB, pB, // 00 - 07
                    pC, pC, pC, pC, pC,             // 08 - 12
                    pA, pA, pA, pA, pA, pA, pA, pA  // 13 - 20
                    };

/**********************************************************************/
#elif defined(__18f4685)				// Added by Andrej Golac
/**********************************************************************/

const unsigned char mask[36] = {
			_0, _1, _2, _3, _4, _5, _6, _7,	// PORT A pins
			_0, _1, _2, _3, _4, _5, _6, _7,	// PORT B pins
			_0, _1, _2, _3, _4, _5, _6, _7,	// PORT C pins
			_0, _1, _2, _3, _4, _5, _6, _7,	// PORT D pins
			_0, _1, _2, _3			// PORT E pins
			};
const unsigned char port[36] = {
			pA, pA, pA, pA, pA, pA, pA, pA,	// PORT A
			pB, pB, pB, pB, pB, pB, pB, pB,	// PORT B
			pC, pC, pC, pC, pC, pC, pC, pC,	// PORT C
			pD, pD, pD, pD, pD, pD, pD, pD,	// PORT D
			pE, pE, pE, pE			// PORT E
			};

/**********************************************************************/
#else
/**********************************************************************/

    #error "Board/Processor Not Yet Supported."
    #error "Please, Take Contact with Developpers."

#endif

#endif /* __DIGITAL_H__ */
