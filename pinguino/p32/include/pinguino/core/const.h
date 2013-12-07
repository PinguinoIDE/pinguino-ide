/*	----------------------------------------------------------------------------
 *	const.h
 *	Régis Blanchot
 *	UBW32 and Emperor board revised - Marcus Fazzi SET/2011
 *	--------------------------------------------------------------------------*/
// 17 mar. 2012 [hgmvanbeek@gmail.com] added support for PIC32_PINGUINO_MICRO

#ifndef __CONST_H
	#define __CONST_H

	#define MIPS32	__attribute__((noinline,nomips16))

	#ifndef PI
		#define PI				3.1415926535897932384626433832795
	#endif

	#ifndef HALF_PI
		#define HALF_PI		1.5707963267948966192313216916398
	#endif

	#ifndef TWO_PI
		#define TWO_PI			6.283185307179586476925286766559
	#endif

	#ifndef DEG_TO_RAD
		#define DEG_TO_RAD	0.017453292519943295769236907684886
	#endif

	#ifndef RAD_TO_DEG
		#define RAD_TO_DEG	57.295779513082320876798154814105
	#endif

	#ifndef NULL
		#define NULL	0
	#endif

	#ifndef ON
		#define ON		1
	#endif

	#ifndef OFF
		#define OFF		0
	#endif

	#ifndef FALSE
		#define FALSE	0 
	#endif

	#ifndef TRUE
		#define TRUE	!FALSE
	#endif

	//lower case

	#ifndef false
		#define false	0
	#endif

	#ifndef true 
		#define true	!false
	#endif

	#ifndef False
		#define False	0
	#endif

	#ifndef True 
		#define True	!False
	#endif

	#ifndef INPUT
		#define INPUT	1
	#endif

	#ifndef OUTPUT
		#define OUTPUT	0
	#endif

	#ifndef HIGH
		#define HIGH	1
	#endif

	#ifndef LOW
		#define LOW		0
	#endif
	
	//Booleans

	#ifndef AND
		#define AND		&&
	#endif

	#ifndef OR 
		#define OR		||
	#endif

	//lower case (not compatible with c++)
	//#define and		&&
	//#define or		||	

	#ifndef BYTE
		#define BYTE	1
	#endif

	#ifndef BIN
		#define BIN		2
	#endif

	#ifndef OCT
		#define OCT		8 
	#endif

	#ifndef DEC
		#define DEC		10
	#endif

	#ifndef HEX
		#define HEX		16
	#endif

	#ifndef FLOAT
		#define FLOAT	32
	#endif

    #if defined(UBW32_460) || defined(UBW32_795)
		// UBW32 build-in leds
		#define LED1			63 //PORTEbits.RE2	// white led
		#define LED2			59 //PORTEbits.RE1	// red led
		#define LED3			58 //PORTEbits.RE0	// yellow led
		#define LED4			64 //PORTEbits.RE3	// green led	

		#define WHITELED		63 //PORTEbits.RE2	// led1
		#define GREENLED		59 //PORTEbits.RE3	// led2	
		#define USERLED		59 //PORTEbits.RE3	// led2	
		#define YELLOWLED		58 //PORTEbits.RE0	// led3
		#define REDLED			64 //PORTEbits.RE1	// led4

		// UBW32 build-in buttons
		#define PROGBUTTON	    68 //PORTEbits.RE7 = prog button
		#define USERBUTTON	    69 //PORTEbits.RE6 = user button

	#elif defined(PIC32_PINGUINO_T795)
		// Olimex PIC32-T795 build-in leds
		#define LED1			36 //PORTBbits.RB15	// green
		#define LED2			19 //PORTBbits.RB12	// yellow

		#define GREENLED		36 //PORTBbits.RB15	// green
		#define YELLOWLED		19 //PORTBbits.RB12	// yellow

		// UBW32 build-in buttons
		#define PROGBUTTON	    24 //PORTDbits.RD0 = prog button
		 
	#elif defined(EMPEROR460) || defined(EMPEROR795)
		// EMPEROR build-in leds
		#define LED1			49 //PORTEbits.RE2	// blue led
		#define LED2			48 //PORTEbits.RE3	// red led

		#define BLUELED		    49 //pin49 PORTEbits.RE2	// led1
		#define USERLED		    49 //pin49 PORTEbits.RE2	// led1
		#define REDLED			48 //pin48 PORTEbits.RE3	// led2

		// UBW32 build-in buttons
		#define USERBUTTON	    43 //PORTEbits.RE7 = user button

	#elif defined (PIC32_PINGUINO_MICRO)
		#define USERLED		    32	//PORTGbits.RG6	// led1
		#define GREENLED		32	//PORTGbits.RG6	// led1
		#define YELLOWLED		10	//PORTDbits.RD1	// led2

		#define LED1			GREENLED
		#define LED2			YELLOWLED

	 	// P1_1 refers to CON1 pin 1, P1_2 CON1 pin 2, and so on
		#define P1_1	31
		#define P1_2	30
		#define P1_3	29
		#define P1_4	28
		#define P1_5	27
		#define P1_6	26
		#define P1_7	25
		#define P1_8	24
		#define P1_9	23
		#define P1_10	22
		#define P1_11	21
		#define P1_12	20
		#define P1_13	19
		#define P1_14	18
		#define P1_15	17
		#define P1_16	16
		// CON2
		#define P2_1	15
		#define P2_2	14
		#define P2_3	13
		#define P2_4	12
		#define P2_5	11
		#define P2_6	10
		#define P2_7	9
		#define P2_8	8
		#define P2_9	7
		#define P2_10	6
		#define P2_11	5
		#define P2_12	4
		#define P2_13	3
		#define P2_14	2
		#define P2_15	1
		#define P2_16	0

	#elif defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_OTG)
		// Arduino's digital pins
		#define D0		0
		#define D1		1
		#define D2		2
		#define D3		3
		#define D4		4
		#define D5		5
		#define D6		6
		#define RTCC	6
		#define D7		7
		#define D8		8
		#define D9		9
		#define D10		10
		#define D11		11
		#define D12		12
		#define D13		13
		// Arduino's analogic pins
		#if defined(__ANALOG__)
			#define A0		0
			#define A1		1
			#define A2		2
			#define A3		3
			#define A4		4
			#define A5		5
			#define A6		6
			#define A7		7
		#else
			#define A0		14
			#define A1		15
			#define A2		16
			#define A3		17
			#define A4		18
			#define A5		19
			#define A6		20
			#define A7		21
		#endif
		// OLIMEX build-in leds
		#define LED1			13	//PORTGbits.RG6	// green led
		#define LED2			30	//PORTDbits.RD1	// yellow led

		#define USERLED		    13	//PORTGbits.RG6	// led1
		#define GREENLED		13	//PORTGbits.RG6	// led1
		#define YELLOWLED		30	//PORTDbits.RD1	// led2

		// OLIMEX build-in buttons
		#define USERBUTTON		2	//PORTDbits.RD0 = user button
	#elif defined(__32MX220F032B__)|| defined(__32MX250F128B__)
		#define USERLED			0
	#elif defined(PIC32_PINGUINO_220)
		#define	USERLED			13	// SCK2 // led1 green
	#endif

#endif /* __CONST_H */
