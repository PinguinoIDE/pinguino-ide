/*	----------------------------------------------------------------------------
	FILE:			pin.h
	PROJECT:		pinguino - http://www.pinguino.cc/
	PURPOSE:		pins name definition
	PROGRAMERS:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	10 Nov 2010
	LAST RELEASE:	09 Feb 2012
	----------------------------------------------------------------------------
	CHANGELOG:
 	- rblanchot - fixed bug : RA4 name (ex.) in conflict with PORTAbits.RA4
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

#ifndef __PIN_H
#define __PIN_H

#if defined(PICUNO_EQUO)

	#define C7 		0
//	#define RC7 	0
	#define RX 		0
	#define DT 		0
	#define SDO 	0
	#define MISO 	0

	#define C6 		1
//	#define RC6 	1
	#define TX 		1
	#define CK 		1

	#define B2 		2
//	#define RB2 	2
	#define AN8 	2
	#define INT2 	2
	#define VMO 	2
	
	#define B3 		3
//	#define RB3 	3
	#define AN9 	3
	#define VPO		3
//	#define	CCP2_ALT	3	//Digital Pin 3 = RB3. 
							//If the Configuration bit CCP2MX = 0, CCP2 is multiplexed with RB3.
							//But this can only be done if user can change the Configuration bit in bootloader firmware.

//	#define RD0 	4
	#define SPP0	4
	
	#define C2 		5
//	#define RC2 	5
	#define CCP1 	5

	#define C1 		6
//	#define RC1 	6
	#define T1OSI 	6
	#define CCP2 	6
	#define UOE 	6
	
//	#define RD1 	7
	#define SPP1 	7

//	#define RD2 	8
	#define SPP2 	8

//	#define RD3 	9
	#define SPP3 	9

//	#define RD4 	10
	#define SPP4 	10

//	#define RD5 	11
	#define SPP5 	11

//	#define RD6 	12
	#define SPP6 	12
	
//	#define RD7 	13
	#define SPP7 	13

	#define A0 		14
//	#define RA0 	14
	#define AN0 	14

	#define A1 		15
//	#define RA1 	15
	#define AN1 	15

	#define A2 		16
//	#define RA2 	16
	#define AN2 	16

	#define A3 		17
//	#define RA5 	17
	#define AN4 	17
	#define SS 		17
	#define HLVDIN 	17
	#define C2OUT 	17
	
	#define A4		18
//	#define RE0 	18
	#define AN5 	18
	#define CK1SPP 	18

	#define A5 		19
//	#define RE1 	19
	#define AN6 	19
	#define CK2SPP 	19

	// run led pin
//	#define RA4		20
	#define RUNLED	20
	#define LED1	20
	#define USERLED	20

#elif defined(PINGUINO2455)  || defined(PINGUINO4455)  || \
      defined(PINGUINO2550)  || defined(PINGUINO4550)  || \
      defined(PINGUINO25K50) || defined(PINGUINO45K50) || \
      defined(PINGUINO26J50) || defined(PINGUINO46J50) || \
      defined(PINGUINO27J53)

	#define B0 0
//	#define RB0 0
	#define AN12 0
	#define INT0 0
	#define FLT0 0
	#define SDI 0
	#define SDA 0

	#define B1 1
//	#define RB1 1
	#define AN10 1
	#define INT1 1
	#define SCK 1
	#define SCL 1

	#define B2 2
//	#define RB2 2
	#define AN8 2
	#define INT2 2
	#define VMO 2

	#define B3 3
//	#define RB3 3
	#define AN9 3
	#define VPO 3

	#define B4 4
//	#define RB4 4
	#define AN11 4
	#define KBI0 4

	#define B5 5
//	#define RB5 5
	#define KBI1 5
	#define PGM 5

	#define B6 6
//	#define RB6 6
	#define KBI2 6
	#define PGC 6

	#define B7 7
//	#define RB7 7
	#define KBI3 7
	#define PGD 7

	#define C6 8
//	#define RC6 8
	#define TX 8
	#define CK 8

	#define C7 9
//	#define RC7 9
	#define RX 9
	#define DT 9
	#define SDO 9

	#define C0 10
//	#define RC0 10
	#define T1OSO 10
	#define T13CK 10

	#define C1 11
//	#define RC1 11
	#define T1OSI 11
//	#define CCP2 11
	#define UOE 11

	#define C2 12
//	#define RC2 12
//	#define CCP1 12

	#define A0 13
//	#define RA0 13
	#define AN0 13

	#define A1 14
//	#define RA1 14
	#define AN1 14

	#define A2 15
//	#define RA2 15
	#define AN2 15

	#define A3 16
//	#define RA3 16
	#define AN3 16

	#define A5 17
//	#define RA5 17
	#define AN4 17
	#define SS 17
	#define HLVDIN 17
	#define C2OU 17

    #if defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(PINGUINO2455) || defined(CHRP3)
    
        #define A4          18
        //#define RA4       18
        #define RUNLED      18
        #define LED1        18
        #define USERLED     18
        #define CCP1        12
        #define CCP2        11

        #if defined(CHRP3)
            #define LED11   0 // ???
            #define LED10   0
            #define LED9    1
            #define LED8    2
            #define LED7    3
            #define LED6    4
            #define LED5    5
            #define LED4    6
            #define LED3    7
        #endif
        
    #elif defined(PINGUINO4550) || defined(PINGUINO45K50) || defined(PINGUINO4455)
    
        #define A4          29
        //#define RA4       29
        #define RUNLED      29
        #define LED1        29
        #define USERLED     29
        #define CCP1        12
        #define CCP2        11
        
    #elif defined(PINGUINO26J50) || defined(PINGUINO46J50) || defined(PINGUINO27J53)

        #define RTCC        1
        #define CCP1        10  // cf. io.c
        #define CCP2        11
        #define LED1        12
        #define USERLED     12

    #endif

#elif defined(PINGUINO47J53A) || defined(PINGUINO47J53B)

    //#error "*** BREAKPOINT ***"

    #define B0          0

    #define B1          1
    #define RTCC        1  // RB1

    #define B2          2
    #define B3          3

    #define B4          4
    #define CCP4        4  // RB4

    #define B5          5
    #define CCP5        5  // RB5

    #define B6          6
    #define CCP6        6  // RB6

    #define B7          7
    #define CCP7        7  // RB7

    #define A0          8
    #define A1          9
    #define A2          10
    #define A3          11
    #define A4          12
    #define A5          13
    #define A6          14
    #define A7          15

    #define C0          16

    #define C1          17
    #define CCP8        17 // RC1

    #define C2          18
    #define LED1        18 // RC2
    #define USERLED     18 // RC2

    #define C3          19
    #define C4          20
    #define C5          21

    #define C6          22
    #define CCP9        22 // RC6

    #define C7          23
    #define CCP10       23 // RC7

    #define D0          24
    #define D1          25
    #define D2          26
    #define D3          27
    #define D4          28
    #define D5          29
    #define D6          30
    #define D7          31

#endif

#endif /* __PIN_H */

