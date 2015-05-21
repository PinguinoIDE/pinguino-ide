/*      ----------------------------------------------------------------------------
        FILE:           pin.h
        PROJECT:        pinguino - http://www.pinguino.cc/
        PURPOSE:        pins name definition
        PROGRAMERS:     regis blanchot <rblanchot@gmail.com>
        FIRST RELEASE:  20 Mar 2014
        ----------------------------------------------------------------------------
        CHANGELOG:
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
    #if defined(UBW32_460) || defined(UBW32_795)
/**********************************************************************/

            // UBW32 build-in leds
            #define USERLED                 59 //PORTEbits.RE3      // led2

            #define LED1                    63 //PORTEbits.RE2      // white led
            #define LED2                    59 //PORTEbits.RE1      // red led
            #define LED3                    58 //PORTEbits.RE0      // yellow led
            #define LED4                    64 //PORTEbits.RE3      // green led    

            #define WHITELED                63 //PORTEbits.RE2      // led1
            #define GREENLED                59 //PORTEbits.RE3      // led2
            #define YELLOWLED               58 //PORTEbits.RE0      // led3
            #define REDLED                  64 //PORTEbits.RE1      // led4

            // UBW32 build-in buttons
            #define PROGBUTTON              68 //PORTEbits.RE7 = prog button
            #define USERBUTTON              69 //PORTEbits.RE6 = user button

/**********************************************************************/
    #elif defined(PIC32_PINGUINO_T795)
/**********************************************************************/

            // Olimex PIC32-T795 build-in leds
            #define USERLED                 36 //PORTBbits.RB15     // green

            #define LED1                    36 //PORTBbits.RB15     // green
            #define LED2                    19 //PORTBbits.RB12     // yellow

            #define GREENLED                36 //PORTBbits.RB15     // green
            #define YELLOWLED               19 //PORTBbits.RB12     // yellow

            // UBW32 build-in buttons
            #define PROGBUTTON              24 //PORTDbits.RD0 = prog button
             
/**********************************************************************/
    #elif defined(EMPEROR460) || defined(EMPEROR795)
/**********************************************************************/

            // EMPEROR build-in leds
            #define USERLED                 49 //pin49 PORTEbits.RE2    // led1

            #define LED1                    49 //PORTEbits.RE2      // blue led
            #define LED2                    48 //PORTEbits.RE3      // red led

            #define BLUELED                 49 //pin49 PORTEbits.RE2    // led1
            #define REDLED                  48 //pin48 PORTEbits.RE3        // led2

            // UBW32 build-in buttons
            #define USERBUTTON          43 //PORTEbits.RE7 = user button

/**********************************************************************/
    #elif defined (PIC32_PINGUINO_MICRO)
/**********************************************************************/

            #define USERLED                 32  //PORTGbits.RG6 // led1

            #define LED1                    GREENLED
            #define LED2                    YELLOWLED

            #define GREENLED                32      //PORTGbits.RG6 // led1
            #define YELLOWLED               10      //PORTDbits.RD1 // led2

            // P1_1 refers to CON1 pin 1, P1_2 CON1 pin 2, and so on
            #define P1_1    31
            #define P1_2    30
            #define P1_3    29
            #define P1_4    28
            #define P1_5    27
            #define P1_6    26
            #define P1_7    25
            #define P1_8    24
            #define P1_9    23
            #define P1_10   22
            #define P1_11   21
            #define P1_12   20
            #define P1_13   19
            #define P1_14   18
            #define P1_15   17
            #define P1_16   16
            
            // CON2
            #define P2_1    15
            #define P2_2    14
            #define P2_3    13
            #define P2_4    12
            #define P2_5    11
            #define P2_6    10
            #define P2_7    9
            #define P2_8    8
            #define P2_9    7
            #define P2_10   6
            #define P2_11   5
            #define P2_12   4
            #define P2_13   3
            #define P2_14   2
            #define P2_15   1
            #define P2_16   0

/**********************************************************************/
    #elif defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_OTG)
/**********************************************************************/

            // Digital pins
            #define D0              0
            #define D1              1
            #define D2              2
            #define D3              3
            #define D4              4
            #define D5              5
            #define D6              6
            #define RTCC    6
            #define D7              7
            #define D8              8
            #define D9              9
            #define D10             10
            #define D11             11
            #define D12             12
            #define D13             13

            // Analog pins
            #if defined(__ANALOG__)
                    #define A0              0
                    #define A1              1
                    #define A2              2
                    #define A3              3
                    #define A4              4
                    #define A5              5
                    #define A6              6
                    #define A7              7
            #else
                    #define A0              14
                    #define A1              15
                    #define A2              16
                    #define A3              17
                    #define A4              18
                    #define A5              19
                    #define A6              20
                    #define A7              21
            #endif

            // OLIMEX build-in leds
            #define USERLED            	    13	    //PORTGbits.RG6 // led1

            #define LED1                    13      //PORTGbits.RG6 // green led
            #define LED2                    30      //PORTDbits.RD1 // yellow led

            #define GREENLED                13      //PORTGbits.RG6 // led1
            #define YELLOWLED               30      //PORTDbits.RD1 // led2

            // OLIMEX build-in buttons
            #define USERBUTTON              2       //PORTDbits.RD0 = user button

/**********************************************************************/
    #elif defined(PINGUINO32MX220) || \
          defined(PINGUINO32MX250) || \
          defined(PINGUINO32MX270)
/**********************************************************************/

            #define USERLED         13
            #define USERBUTTON      14
   
            #define PWM0            8
            #define PWM1            7
            #define PWM2            6
            #define PWM3            2
            #define PWM4            1
           
            #define RTCC            9
           
            //#define PMCS1           0
            //#define PMRD            2
            //#define PMWR            9
            //#define PMBE            9
           
            #define PMD0            12
            #define PMD1            11
            #define PMD2            10
            #define PMD3            3
            #define PMD4            4
            #define PMD5            5
            #define PMD6            14
            #define PMD7            13
   
/**********************************************************************/
    #elif defined(PIC32_PINGUINO_220)
/**********************************************************************/

            #define USERLED                 13      //PORTGbits.RG6 // LED1 // green LED

            // OLIMEX build-in leds
            #define LED1                    USERLED
            #define LED2                    9      //PORTAbits.RA10 // red LED

            #define GREENLED                USERLED
            #define REDLED                  LED2

            // OLIMEX build-in buttons
            #define USERBUTTON              8       //PORTBbits.RB7 = user button

            // Arduino's digital pins
            #define D0              		 0		//RC8
            #define D1              		 1		//RC9
            #define D2              		 2		//RC2
            #define D3              		 3		//RC3
            #define D4              		 4		//RC4
            #define D5              		 5		//RC5
            #define D6              		 6		//RC6
            #define D7              		 7		//RC7
            #define D8              		 8		//RB7
            #define D9              		 9		//RA10
            #define D10             		10		//RA1
            #define D11             		11		//RB5
            #define D12             		12		//RB13
            #define D13             		13		//RB15

            // Arduino's analog pins
            #if defined(__ANALOG__)
                    #define A0               0		//AN6
                    #define A1               1		//AN7
                    #define A2               2		//AN2
                    #define A3               3		//AN3
                    #define A4               4		//AN4
                    #define A5               5		//AN5
            #endif

    #endif

#endif /* __PIN_H */
