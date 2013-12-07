// UART Library for 8-bit Pinguino
// SDCC version / small device c compiler
// written by Jean-Pierre MANDON 2008 jp.mandon@free.fr
/*
	CHANGELOG:
	23-11-2012		regis blanchot		added __18f120,1320,14k22,2455,4455,46j50 support
	19-01-2013		regis blanchot		support of all clock frequency

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
*/
// this program is intended for an oscillator frequency of 20 Mhz


// local definition UART.C

#ifndef __SERIAL__
#define __SERIAL__

#include <pic18fregs.h>
#include <typedef.h>
//#include <stdlib.h>       // no longer used (09-11-2012)
#include <delay.c>
#include <oscillator.c>

#if defined(__SERIAL_PRINTF__) || defined(__SERIAL_PRINT__) || \
    defined(__SERIAL_PRINTLN__) || defined(__SERIAL_GETSTRING__)
    #include <stdio.c>
    #include <stdarg.h>
#endif

//extern u32 _cpu_clock_

#if defined(__SERIAL_GETSTRING__) || defined(__SERIAL_PRINT__) || defined(__SERIAL_PRINTLN__)
    #ifndef __SERIAL_PRINTF__
        #define __SERIAL_PRINTF__
    #endif
#endif

// rx buffer length
#ifndef RXBUFFERLENGTH
    #if defined(__18f1220) || defined(__18f1320) || defined(__18f14k22)
        #define RXBUFFERLENGTH 64
    #else
        #define RXBUFFERLENGTH 128
    #endif
#endif

char rx[RXBUFFERLENGTH];                // serial buffer
unsigned char wpointer,rpointer;        // write and read pointer

// setup PIC UART
void serial_begin(unsigned long baudrate)
{
	unsigned long spbrg;
	unsigned char highbyte,lowbyte;

    spbrg=(System_getCpuFrequency()/(4*baudrate))-1;
//    spbrg=(48000000/(4*baudrate))-1;
//    spbrg=(SystemGetClock()/(4*baudrate))-1;
//    spbrg=(_cpu_clock_/(4*baudrate))-1;

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

#elif defined(__18f2455) || \
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

//  RCONbits.IPEN = 1;                      //  enable interrupt priorities
    INTCONbits.GIEH = 1;                    // Enable global HP interrupts
    INTCONbits.GIEL = 1;                    // Enable global LP interrupts
 	INTCONbits.PEIE = 1;                    // Enable peripheral interrupts

    //Delayms(1000);                           // AG : 12-11-2012
}

// new character receive ?
unsigned char serial_available()
{
	return(wpointer!=rpointer);
}

// clear rx buffer
void serial_flush(void)
{
	wpointer=1;
	rpointer=1;
}

// serial_int is called by interruption service routine
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

// write char
void serial_putchar(unsigned char caractere)
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

// get char
unsigned char serial_read()
{
	unsigned char caractere=0;

	if (serial_available())
    {
		PIE1bits.RCIE=0;             // Atomic operation start
	    caractere=rx[rpointer++];
	    if (rpointer==RXBUFFERLENGTH)
		rpointer=1;
		PIE1bits.RCIE=1;             // Atomic operation end
    }
	return(caractere);
}

/*	----------------------------------------------------------------------------
	serial_printf()
	rblanchot@gmail.com
	write formated string on the serial port
	--------------------------------------------------------------------------*/

#if defined(__SERIAL_PRINTF__)  || defined(__SERIAL_PRINT__) || \
    defined(__SERIAL_PRINTLN__) || defined(__SERIAL_GETSTRING__)
void serial_printf(char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	pprintf(serial_putchar, fmt, args);
	va_end(args);
}
#endif /* __SERIAL_PRINTF__ */

/*	----------------------------------------------------------------------------
	serial_print()
	rblanchot@gmail.com
	16-08-2011: fixed bug in print - Régis Blanchot & Tiew Weng Khai
    11-09-2012: added FLOAT support - Régis Blanchot
	--------------------------------------------------------------------------*/

#if defined(__SERIAL_PRINT__) || defined(__SERIAL_PRINTLN__)
#	define serial_print(m,type)		{ serial_print_##type(m);  }
#	define serial_println(m,type)	{ serial_print_##type(m);  serial_printf("\r\n"); }
void serial_print_FLOAT(float m){ serial_printf("%f",m); }
void serial_print_DEC(u16 m)    { serial_printf("%d",m); }
void serial_print_HEX(u16 m)    { serial_printf("%x",m); }
void serial_print_BYTE(u16 m)   { serial_printf("%d",m); }
void serial_print_OCT(u16 m)    { serial_printf("%o",m); }
void serial_print_BIN(u16 m)    { serial_printf("%b",m); }
#endif

/* serial_print() function can not correctly support serial_print("some string") aka Serial.print("some string").
 * In case serial_print("some string"), va_arg(args, u32) will return a unexpected invalid value. 
 * by avrin */

#if 0
//void serial_print(char *fmt,...)
void serial_print(const char *fmt,...)
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
			serial_printf("%f", (u32)fmt);
			break;
		case DEC:
			serial_printf("%d",fmt);
			break;
		case HEX:
			serial_printf("%x",fmt);
			break;
		case BYTE:
			serial_printf("%d",fmt);
			break;
		case OCT:
			serial_printf("%o",fmt);
			break;
		case BIN:
			serial_printf("%b",fmt);
			break;
		default:
			serial_printf(fmt);
			break;
	}
	va_end(args);
}
#endif /* __SERIAL_PRINT__ */

/*	----------------------------------------------------------------------------
	serial_println()
	rblanchot@gmail.com
	Prints data to the serial port as human-readable ASCII text followed by
	a carriage return character (ASCII 13, or '\r') and a newline character
	(ASCII 10, or '\n').
	--------------------------------------------------------------------------*/
#if 0
#if defined(__SERIAL_PRINTLN__)
void serial_println(char *fmt,...)
{
	serial_print(fmt);
	serial_printf("\n\r");
}
#endif /* __SERIAL_PRINTLN__ */
#endif

/*	----------------------------------------------------------------------------
	serial_getkey()
	rblanchot@gmail.com
	--------------------------------------------------------------------------*/

#if defined(__SERIAL_GETKEY__)
u8 serial_getkey()
{
	u8 c;
	while (!(serial_available()));
	c = serial_read();
	serial_flush();
	return (c);
}
#endif /* __SERIAL_GETKEY__ */

/*	----------------------------------------------------------------------------
	serial_getstring()
	rblanchot@gmail.com
	--------------------------------------------------------------------------*/

#if defined(__SERIAL_GETSTRING__)
u8 * serial_getstring()
{
	static u8 buffer[80]; // Need static attribute, because a function can not return local array without
	                      // static attribute.
	u8 c;
	u8 i = 0;

	//buffer = (u8 *) malloc(80);
	do {
		c = serial_getkey();
		serial_printf("%c", c);
		buffer[i++] = c;
	} while (c != '\r');
	buffer[i] = '\0';
	return (buffer);
}
#endif /* __SERIAL_GETSTRING__ */

#endif
