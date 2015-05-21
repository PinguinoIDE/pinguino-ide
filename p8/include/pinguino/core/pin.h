/*	----------------------------------------------------------------------------
	FILE:			pin.h
	PROJECT:		pinguino - http://www.pinguino.cc/
	PURPOSE:		pins name definition
	PROGRAMERS:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	10 Nov 2010
	LAST RELEASE:	26 Jun 2014
	----------------------------------------------------------------------------
	CHANGELOG:
	- rblanchot - fixed bug : RA4 name (ex.) in conflict with PORTAbits.RA4
	26 Jun 2014 - rblanchot - added Pinguino 45K50
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

/**********************************************************************/
#if defined(__18f4685)		// Added by Andrej Golac
/**********************************************************************/

	#define RA0	0		/*-----------*/
	#define AN0	0		/*           */
	#define CVREF	0		/*           */
	#define RA1		1	/*           */
	#define AN1		1	/*           */
	#define RA2	2		/*           */
	#define AN2	2		/*     P     */
	#define VREFL	2		/*           */
	#define RA3		3	/*     O     */
	#define AN3		3	/*           */
	#define VREFH		3	/*     R     */
	#define RA4	4		/*           */
	#define T0CKI	4		/*     T     */
	#define RA5		5	/*     _     */
	#define AN4		5	/*           */
	#define SS		5	/*     A     */
	#define HLVDIN		5	/*           */
	#define OSC2	6		/*           */
	#define CLKO	6		/*           */
	#define RA6	6		/*           */
	#define OSC1		7	/*           */
	#define CLKI		7	/*           */	
	#define RA7		7	/*-----------*/

	#define RB0	8		/*-----------*/
	#define INT0	8		/*           */
	#define FLT0	8		/*           */
	#define AN10	8		/*           */
	#define RB1		9	/*           */
	#define INT1		9	/*           */
	#define AN8		9	/*     P     */
	#define RB2	10		/*           */
	#define INT2	10		/*     O     */
	#define CANTX	10		/*           */
	#define RB3		11	/*     R     */
	#define CANRX		11	/*           */
	#define RB4	12		/*     T     */
	#define KBI0	12		/*     _     */
	#define AN9	12		/*           */
	#define RB5		13	/*     B     */
	#define KBI1		13	/*           */
	#define PGM		13	/*           */
	#define RB6	14		/*           */
	#define KBI2	14		/*           */
	#define PGC	14		/*           */
	#define RB7		15	/*           */
	#define KBI3		15	/*           */
	#define PGD		15	/*-----------*/

	#define RC0	16		/*-----------*/
	#define T1OSO	16		/*           */
	#define T13CKI	16		/*           */
	#define RC1		17	/*           */
	#define T1OSI		17	/*           */
	#define RC2	18		/*     P     */
	#define CCP1	18		/*           */
	#define RC3		19	/*     O     */
	#define SCK		19	/*           */
	#define SCL		19	/*     R     */
	#define RC4	20		/*           */
	#define SDI	20		/*     T     */
	#define SDA	20		/*     _     */
	#define RC5		21	/*           */
	#define SDO		21	/*     C     */
	#define RC6	22		/*           */
	#define TX	22		/*           */
	#define CK	22		/*           */
	#define RC7		23	/*           */
	#define RX		23	/*           */
	#define DT		23	/*-----------*/

	#define RD0	24		/*-----------*/
	#define PSP0	24		/*           */
	#define C1INH	24		/*           */
	#define RD1		25	/*           */
	#define PSP1		25	/*           */
	#define C1INL		25	/*           */
	#define RD2	26		/*           */
	#define PSP2	26		/*     p     */
	#define C2INH	26		/*           */
	#define RD3		27	/*     O     */
	#define PSP3		27	/*           */
	#define C2INL		27	/*     R     */
	#define RD4	28		/*           */
	#define PSP4	28		/*     T     */
	#define ECCP1	28		/*     _     */
	#define P1A	28		/*           */
	#define RD5		29	/*     D     */
	#define PSP5		29	/*           */
	#define P1B		29	/*           */
	#define RD6	30		/*           */
	#define PSP6	30		/*           */
	#define P1C	30		/*           */
	#define RD7		31	/*           */
	#define PSP7		31	/*           */
	#define P1D		31	/*-----------*/

	#define RE0	32		/*-----------*/
	#define RD	32		/*           */
	#define AN5	32		/*           */
	#define RE1		33	/*           */
	#define WR		33      /*     P     */
	#define AN6		33	/*     O     */
	#define C1OUT		33	/*     R     */
	#define RE2	34		/*     T     */
	#define CS	34		/*     -     */
	#define AN7	34		/*     E     */
	#define C2OUT	34		/*           */
	#define MCLR		35	/*           */
	#define VPP		35	/*           */
	#define RE3		35	/*-----------*/

/**********************************************************************/
#elif defined(PICUNO_EQUO)
/**********************************************************************/

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
    #if defined(PICUNOV1) // Early version of PICUNO_EQUO (BLUE PCB)
	#define USERLED	2
    #else                 // Second and last version (BLACK PCB)
	#define RUNLED	20
	#define LED1	13
	#define USERLED	13
    #endif
    
/**********************************************************************/
#elif defined(PINGUINO2455)  || defined(PINGUINO4455)  || \
      defined(PINGUINO2550)  || defined(PINGUINO4550)  || \
      defined(PINGUINO25K50) || defined(PINGUINO45K50) || \
      defined(PINGUINO26J50) || defined(PINGUINO46J50) || \
      defined(PINGUINO27J53) || defined(CHRP3)
/**********************************************************************/

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

/**********************************************************************/
    #if defined(PINGUINO2550) || defined(PINGUINO25K50) || defined(PINGUINO2455)
/**********************************************************************/
    
        #define A4          18
        //#define RA4       18
        #define RUNLED      18
        #define LED1        18
        #define USERLED     18
        #define CCP1        12
        #define CCP2        11

/**********************************************************************/
    #elif defined(CHRP3)
/**********************************************************************/

        #define USERLED 17
        #define BACKLIGHT 17 // RA4
        #define LED12   8
        #define LED11   20
        #define LED10   0
        #define LED9    1
        #define LED8    2
        #define LED7    3
        #define LED6    4
        #define LED5    5
        #define LED4    6
        #define LED3    7
        #define LED2    17
        #define SERVO1  10 // RC0
        #define SERVO2  11 // RC1
        #define SERVO3  12 // RC2
        #define SERVO4  12 // RC3 ???
        #define BUTTONS2 0 
        #define BUTTONS3 1 
        #define BUTTONS4 2 
        #define BUTTONS5 3 
        #define BUTTONS6 4 
        #define BUTTONS7 5
        #define LIGHTSENSOR1 0
        #define LIGHTSENSOR2 1
        #define POSITIONSENSOR 2
        #define TEMPSENSOR 3
        #define POWERSENSOR 4
        #define IRSENSOR 9  // RC7
        #define BUZZER  19
        #define CCP1        12
        #define CCP2        11
        
/**********************************************************************/
    #elif defined(PINGUINO4455) || defined(PINGUINO4550)
/**********************************************************************/
    
        #define A4          29
        //#define RA4       29
        #define RUNLED      29
        #define LED1        29
        #define USERLED     29
        #define CCP1        12
        #define CCP2        11

/**********************************************************************/
    #elif defined(PINGUINO45K50)
/**********************************************************************/
    
        #define A4          12
        //#define RA4       29
        #define RUNLED      12
        #define LED1        12
        #define USERLED     12
        #define CCP1        18  // RC2
        #define CCP2        17  // RC1
        
/**********************************************************************/
    #elif defined(PINGUINO26J50) || defined(PINGUINO27J53)
/**********************************************************************/

        #define RTCC        1
        #define CCP1        10  // cf. io.c
        #define CCP2        11
        #define LED1        12
        #define USERLED     12

    #endif

/**********************************************************************/
#elif defined(PINGUINO46J50) || defined(PINGUINO47J53)
/**********************************************************************/

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
    /*
    #define PMD0        24
    #define PMD1        25
    #define PMD2        26
    #define PMD3        27
    #define PMD4        28
    #define PMD5        29
    #define PMD6        30
    #define PMD7        31

    #define PMWR        14 // RE1
    #define PMRD        13 // RE0
    #define PMBE        1  // RB1
    #define PMCS        15 // RE2
    */

#endif

#endif /* __PIN_H */

