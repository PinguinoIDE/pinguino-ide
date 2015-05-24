/*	--------------------------------------------------------------------
    FILE:			Serial.c
    PROJECT:		Pinguino
    PURPOSE:		UART Library for 8-bit Pinguino
                    using  2 wires or 4 wires
    PROGRAMER:		Jean-Pierre MANDON 2008 jp.mandon@free.fr
    CHANGELOG:
    23-11-2012		regis blanchot		added __18f120,1320,14k22,2455,4455,46j50 support
    19-01-2013		regis blanchot		support of all clock frequency
    14-04-2014		regis blanchot		added printNumber and printFloat function
    --------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2, or (at your option) any
    later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

    In other words, you are welcome to use, share and improve this program.
    You are forbidden to forbid anyone else to use, share and improve
    what you give them.   Help stamp out software-hoarding!
    ------------------------------------------------------------------*/

#ifndef __SERIAL__
#define __SERIAL__

#include <pic18fregs.h>
#include <typedef.h>
#include <macro.h>
//#include <stdlib.h>       // no longer used (09-11-2012)
#include <delayms.c>
#include <oscillator.c>

#if defined(SERIALPRINTF)
    #include <stdio.c>
    #include <stdarg.h>
#endif

//extern u32 _cpu_clock_

// RX buffer length
#ifndef RXBUFFERLENGTH
    #if defined(__18f1220) || defined(__18f1320) || defined(__18f14k22)
        #define RXBUFFERLENGTH 64
    #else
        #define RXBUFFERLENGTH 128
    #endif
#endif

char rx[RXBUFFERLENGTH];                // serial buffer
unsigned char wpointer,rpointer;        // write and read pointer

/***********************************************************************
 * Serial.begin()
 * Setup PIC18F UART
 **********************************************************************/

void Serial_begin(unsigned long baudrate)
{
    unsigned long spbrg;
    unsigned char highbyte,lowbyte;

    //spbrg=(48000000/(4*baudrate))-1;
    //spbrg=(_cpu_clock_/(4*baudrate))-1;
    //spbrg=(SystemGetClock()/(4*baudrate))-1;
    spbrg=(System_getCpuFrequency()/(4*baudrate))-1;

    highbyte=spbrg/256;
    lowbyte=spbrg%256;
    
    #if defined(__18f1220) || defined(__18f1320) || \
        defined(__18f14k22) || defined(__18lf14k22)
        TXSTAbits.BRGH=1;               	  	// set BRGH bit
        BAUDCTLbits.BRG16=1;					// set 16 bits SPBRG
        SPBRGH=highbyte;                        // set UART speed SPBRGH
        SPBRG=lowbyte;   						// set UART speed SPBRGL
        RCSTA=0x90;                             // set RCEN and SPEN
        BAUDCTLbits.RCIDL=1;			        // set receive active
        PIE1bits.RCIE=1;                        // enable interrupt on RX
        IPR1bits.RCIP=1;                        // define high priority for RX interrupt
        TXSTAbits.TXEN=1;                       // enable TX

    #elif defined(__18f2455) || defined(__18f4455) || \
          defined(__18f2550) || defined(__18f4550) || \
          defined(__18f25k50) || defined(__18f45k50)
        TRISCbits.TRISC7	= 1;					/* Rx1	set input */
        TXSTAbits.BRGH=1;               	  	// set BRGH bit
        BAUDCONbits.BRG16=1;					// set 16 bits SPBRG
        SPBRGH=highbyte;                        // set UART speed SPBRGH
        SPBRG=lowbyte;   						// set UART speed SPBRGL
        RCSTA=0x90;                             // set RCEN and SPEN
        BAUDCONbits.RCIDL=1;			        // set receive active
        TXSTAbits.TXEN=1;                       // enable TX

        PIR1bits.RCIF = 0;                      // Clear interrupt flag
        PIE1bits.RCIE=1;                        // enable interrupt on RX
        IPR1bits.RCIP=1;                        // define high priority for RX interrupt

    #elif defined(__18f26j50) || defined(__18f46j50) || \
          defined(__18f26j53) || defined(__18f46j53) || \
          defined(__18f27j53) || defined(__18f47j53)
        TRISCbits.TRISC7	= 1;					/* Rx1	set input */
        TXSTA1bits.BRGH=1;               	  	// set BRGH bit
        BAUDCON1bits.BRG16=1;					// set 16 bits SPBRG
        SPBRGH1=highbyte;                       // set UART speed SPBRGH
        SPBRG1=lowbyte;   						// set UART speed SPBRGL
        RCSTA1=0x90;                            // set RCEN and SPEN
        BAUDCON1bits.RCIDL=1;			        // set receive active
        TXSTA1bits.TXEN=1;                      // enable TX

        PIR1bits.RC1IF = 0;                     // Clear interrupt flag
        PIE1bits.RC1IE=1;                       // enable interrupt on RX
        IPR1bits.RC1IP=1;                       // define high priority for RX interrupt

        PIR1bits.TX1IF=0;  //
        PIE1bits.TX1IE=0;  //

    #else

        #error "Processor Not Yet Supported. Please, Take Contact with Developpers."

    #endif

    wpointer=1;                             // initialize write pointer
    rpointer=1;                             // initialize read pointer
    
    //RB: IPEN is managed both by bootloader and main.c
    //RCONbits.IPEN = 1;                      //  enable interrupt priorities
    INTCONbits.GIEH = 1;                    // Enable global HP interrupts
    INTCONbits.GIEL = 1;                    // Enable global LP interrupts
    INTCONbits.PEIE = 1;                    // Enable peripheral interrupts

    //Delayms(1000);                           // AG : 12-11-2012
}

/***********************************************************************
 * Serial.available()
 * return true if a new character has been received, otherwise false
 **********************************************************************/

unsigned char Serial_available()
{
    return (wpointer!=rpointer);
}

/***********************************************************************
 * Serial.flush()
 * Clear RX buffer
 **********************************************************************/

void Serial_flush(void)
{
    wpointer=1;
    rpointer=1;
}

/***********************************************************************
 * Interruption routine called by main.c
 **********************************************************************/

void serial_interrupt(void)
{
    char caractere;
    unsigned char newwp;

    #if defined(__18f1220)  || defined(__18f1320)   || \
        defined(__18f14k22) || defined(__18lf14k22) || \
        defined(__18f2550)  || defined(__18f4550)   || \
        defined(__18f25k50) || defined(__18f45k50)  || \
        defined(__18f2455)  || defined(__18f4455)

    if (PIR1bits.RCIF)
    { 
        PIR1bits.RCIF=0;				// clear RX interrupt flag
        caractere=RCREG;				// take received char

    #elif defined(__18f26j50) || defined(__18f46j50) || \
          defined(__18f26j53) || defined(__18f46j53) || \
          defined(__18f27j53) || defined(__18f47j53)

    if (PIR1bits.RC1IF) 
    {
        PIR1bits.RC1IF=0;				// clear RX interrupt flag
        caractere=RCREG1;				// take received char

    #else

        #error "Processor Not Yet Supported. Please, Take Contact with Developpers."

    #endif

        if (wpointer!=RXBUFFERLENGTH-1)	// if not last place in buffer
            newwp=wpointer+1;			// place=place+1

        else
            newwp=1;					// else place=1

        if (rpointer!=newwp)			// if read pointer!=write pointer
            rx[wpointer++]=caractere;	// store received char

        if (wpointer==RXBUFFERLENGTH)	// if write pointer=length buffer
            wpointer=1;					// write pointer = 1
    }
}

/***********************************************************************
 * Serial.write()
 * Write a char on Serial port
 **********************************************************************/

void Serial_putchar(unsigned char caractere)
{
    #if defined(__18f1220) || defined(__18f1320)    || \
        defined(__18f14k22) || defined(__18lf14k22) || \
        defined(__18f2455)                          || \
        defined(__18f2550) || defined(__18f4550)    || \
        defined(__18f25k50) || defined(__18f45k50)

        while (!TXSTAbits.TRMT);
        TXREG=caractere;		        // yes, send char

    #elif defined(__18f26j50) || defined(__18f46j50) || \
          defined(__18f27j53) || defined(__18f47j53)

        while (!TXSTA1bits.TRMT);
        TXREG1=caractere;		        // yes, send char

    #else

        #error "Processor Not Yet Supported. Please, Take Contact with Developpers."

    #endif
}

/***********************************************************************
 * Serial.read()
 * Get a char from Serial port
 **********************************************************************/

#if defined(SERIALREAD) || defined(SERIALGETKEY) || defined(SERIALGETSTRING)
unsigned char Serial_read()
{
    unsigned char caractere=0;

    if (Serial_available())
    {
        PIE1bits.RCIE=0;             // Atomic operation start
        caractere=rx[rpointer++];
        if (rpointer==RXBUFFERLENGTH)
        rpointer=1;
        PIE1bits.RCIE=1;             // Atomic operation end
    }
    return(caractere);
}
#endif /* SERIALREAD */

/***********************************************************************
 * USB SERIAL print routine (SERIAL.print)
 * 16-08-2011: fixed bug in print - Régis Blanchot & Tiew Weng Khai
 * 11-09-2012: added FLOAT support - Régis Blanchot
 * 04-03-2014: updated (no more printf call) - Régis Blanchot 
 * write a string on SERIAL port
 **********************************************************************/

#if defined(SERIALPRINTSTRING) || defined(SERIALPRINTLN) || \
    defined(SERIALPRINTNUMBER) || defined(SERIALPRINTFLOAT)

void Serial_print(char *string)
{
    u8 i;
    for( i=0; string[i]; i++)
        Serial_putchar(string[i]);
}
#endif /* SERIALPRINTSTRING */

/* Avrin : 
 * Serial_print() function can not correctly support
 * Serial_print("some string") aka Serial.print("some string").
 * In case Serial_print("some string"), va_arg(args, u32) will return
 * an unexpected invalid value. 
 */

#if 0
#if defined(SERIALPRINTSTRING) || defined(SERIALPRINTLN)
#define Serial_print(m,type)	{ Serial_print_##type(m);  }
#define Serial_println(m,type)	{ Serial_print_##type(m);  Serial_printf("\r\n"); }
void Serial_print_FLOAT(float m){ Serial_printf("%f",m); }
void Serial_print_DEC(u16 m)    { Serial_printf("%d",m); }
void Serial_print_HEX(u16 m)    { Serial_printf("%x",m); }
void Serial_print_BYTE(u16 m)   { Serial_printf("%d",m); }
void Serial_print_OCT(u16 m)    { Serial_printf("%o",m); }
void Serial_print_BIN(u16 m)    { Serial_printf("%b",m); }
#endif
#endif

#if 0
//void Serial_print(char *fmt,...)
void Serial_print(const char *fmt,...)
{
    //u8 *s;
    u8 s;
    va_list args;							// a list of arguments
    va_start(args, fmt);					// initialize the list
    //s = va_start(args, fmt);
    s = (u8) va_arg(args, u32);				// get the first variable arg.

    //switch (*s)
    switch (s)
    {
        case FLOAT:
            Serial_printf("%f", (u32)fmt);
            break;
        case DEC:
            Serial_printf("%d",fmt);
            break;
        case HEX:
            Serial_printf("%x",fmt);
            break;
        case BYTE:
            Serial_printf("%d",fmt);
            break;
        case OCT:
            Serial_printf("%o",fmt);
            break;
        case BIN:
            Serial_printf("%b",fmt);
            break;
        default:
            Serial_printf(fmt);
            break;
    }
    va_end(args);
}
#endif

/***********************************************************************
 * USB SERIAL print routine (SERIAL.println)
 * added by regis blanchot 04/03/2014
 * write a string followed by a carriage return character (ASCII 13, or '\r')
 * and a newline character (ASCII 10, or '\n') on SERIAL port
 **********************************************************************/

#if defined(SERIALPRINTLN)
void Serial_println(char *string)
{
    Serial_print(string);
    Serial_print("\n\r");
}
#endif /* SERIALPRINTLN */

/***********************************************************************
 * USB SERIAL printNumber routine (SERIAL.printNumber)
 * added by regis blanchot 14/06/2011
 * write a number on SERIAL port
 * base : see const.h (DEC, BIN, HEXA, OCTO, ...)
 **********************************************************************/

#if defined(SERIALPRINTNUMBER) || defined(SERIALPRINTFLOAT)
void Serial_printNumber(long value, u8 base)
{  
    u8 sign;

    long i;
    unsigned long v;    // absolute value

    u8 tmp[12];
    u8 *tp = tmp;       // pointer on tmp

    u8 string[12];
    u8 *sp = string;    // pointer on string

    if (value==0)
    {
        Serial_putchar('0');
        return;
    }
    
    sign = ( (base == 10) && (value < 0) );

    if (sign)
        v = -value;
    else
        v = (unsigned long)value;

    //while (v || tp == tmp)
    while (v)
    {
        i = v % base;
        v = v / base;
        
        if (i < 10)
            *tp++ = i + '0';
        else
            *tp++ = i + 'A' - 10;
    }

    // start of string
    if (sign)
        *sp++ = '-';

    // backwards writing 
    while (tp > tmp)
        *sp++ = *--tp;

    // end of string
    *sp = 0;

    Serial_print(string);
}
#endif /* SERIALPRINTNUMBER */

/***********************************************************************
 * USB SERIAL printFloat routine (SERIAL.printFloat)
 * added by regis blanchot 14/06/2011
 * write a float number on SERIAL port
 * base : see const.h (DEC, BIN, HEXA, OCTO, ...)
 **********************************************************************/

#if defined(SERIALPRINTFLOAT)
void Serial_printFloat(float number, u8 digits)
{ 
	u8 i, toPrint;
	u16 int_part;
	float rounding, remainder;

	// Handle negative numbers
	if (number < 0.0)
	{
		Serial_puts('-', 1);
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"  
	rounding = 0.5;
	for (i=0; i<digits; ++i)
		rounding /= 10.0;

	number += rounding;

	// Extract the integer part of the number and print it  
	int_part = (u16)number;
	remainder = number - (float)int_part;
	Serial_printNumber(int_part, 10);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0)
		Serial_puts('.', 1); 

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		toPrint = (unsigned int)remainder; //Integer part without use of math.h lib, I think better! (Fazzi)
		Serial_printNumber(toPrint, 10);
		remainder -= toPrint; 
	}
}
#endif /* SERIALPRINTFLOAT */

/***********************************************************************
 * Serial.printf
 * updated by regis blanchot <rblanchot@gmail.com> - 14/04/2014
 * write a formated string on Serial port
 **********************************************************************/

#if defined(SERIALPRINTF)
void Serial_printf(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    pprintf(Serial_putchar, fmt, args);
    va_end(args);
}
#endif /* SERIALPRINTF__ */

/***********************************************************************
 * Serial.getKey
 * updated by regis blanchot <rblanchot@gmail.com> - 14/04/2014
 * get a key from Serial port
 **********************************************************************/

#if defined(SERIALGETKEY) || defined(SERIALGETSTRING)
u8 Serial_getkey()
{
    u8 c;
    while (!(Serial_available()));
    c = Serial_read();
    Serial_flush();
    return (c);
}
#endif /* SERIALGETKEY__ */

/***********************************************************************
 * Serial.getString
 * updated by regis blanchot <rblanchot@gmail.com> - 14/04/2014
 * get a string from Serial port
 **********************************************************************/

#if defined(SERIALGETSTRING)
u8 * Serial_getstring()
{
    static u8 buffer[80]; // Need static attribute, because a function can not return local array without
                          // static attribute.
    u8 c;
    u8 i = 0;

    //buffer = (u8 *) malloc(80);
    do {
        c = Serial_getkey();
        Serial_putchar(c);
        buffer[i++] = c;
    } while (c != '\r');
    buffer[i] = '\0';
    return (buffer);
}
#endif /* SERIALGETSTRING__ */

/**********************************************************************/

#endif /* __SERIAL__ */
