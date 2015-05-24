/* $Id: getsuart.c,v 1.1 2003/12/09 22:43:28 GrosbaJ Exp $ */
/* $Id: putsuart.c,v 1.1 2003/12/09 22:43:28 GrosbaJ Exp $ */
// Adaptation from Free Open Source Codes Forge and Sharing - www.codeforge.com
// Functions to get RS232 transmission without UART hardware
// RXD pin is defined on RB5, TXD on RB4
// other pairs can be chosen on digital pins
// OSC_FREQ = 48 MHz standard frequency for Pinguino Board 18fx550
// BAUDRATE = 9600 Bds
// For other configurations calculate 3 new delays according to following formulas
//DelayRXHalfBitUART = (((2*OSC_FREQ)/(8*BAUDRATE))+1)/2 - 9 Tcy
//DelayRXBitUART = (((2*OSC_FREQ)/(4*BAUDRATE))+1)/2 - 14 Tcy
//DelayTXBitUART = (((2*OSC_FREQ)/(4*BAUDRATE))+1)/2 - 12 Tcy
// change each Delay function :
// for example, here with 48 Mhz and 9600 Bds DelayRXHalfBitUART = 616 = 61 x 10Tcy + 6 Tcy
// -> therefore we find delay10tcy(61) (this is a SDCC function defined in delay.h header)
//                  and 3 nop (asm code nop = 2Tcy)

// 29/08/2014 adapted by A. Gentric for Pinguino Project
// Baud rates 115200, 2400, 1200 do not work perfectly

#ifndef __SWSERIAL__
#define __SWSERIAL__

#include <delay.h>
#include <stdio.c>
#include <stdarg.h>

char uartdata;
void swserial_printf(char *fmt, ...);

#if (SWBAUDRATE ==57600)
void DelayRXHalfBitUART(void)
{
//delay = (((2*OSC_FREQ)/(8*BAUDRATE))+1)/2 - 9 Tcy
    delay10tcy(9); //9+6nop
    __asm
    nop
    nop
    nop
    nop
    nop
    nop
    __endasm;
}

void DelayRXBitUART(void)
{
//delay = (((2*OSC_FREQ)/(4*BAUDRATE))+1)/2 - 14 Tcy
    delay10tcy(19);//19+6nop
    __asm
    nop
    nop
    nop
    nop
    nop
    nop
    __endasm;
}

void DelayTXBitUART(void)
{
    delay10tcy(19);//19+4nop
    __asm
    nop
    nop
    nop
    nop
    __endasm;
}
#elif (SWBAUDRATE ==38400)
void DelayRXHalfBitUART(void)
{
//delay = (((2*OSC_FREQ)/(8*BAUDRATE))+1)/2 - 9 Tcy
    delay10tcy(14); //14+8nop
    __asm
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    __endasm;
}

void DelayRXBitUART(void)
{
//delay = (((2*OSC_FREQ)/(4*BAUDRATE))+1)/2 - 14 Tcy
    delay10tcy(29); //29 + 8nop
    __asm
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    __endasm;
}

void DelayTXBitUART(void)
{
    delay10tcy(30);//30+1nop
    __asm
    nop
    __endasm;
}

#elif (SWBAUDRATE ==19200)
void DelayRXHalfBitUART(void)
{
//delay = (((2*OSC_FREQ)/(8*BAUDRATE))+1)/2 - 9 Tcy
    delay10tcy(30); //30 +4nop
    __asm
    nop
    nop
    nop
    nop
    __endasm;
}

void DelayRXBitUART(void)
{
//delay = (((2*OSC_FREQ)/(4*BAUDRATE))+1)/2 - 14 Tcy
    delay10tcy(61); //61 + 1 nop
    __asm
    nop
    __endasm;
}

void DelayTXBitUART(void)
{

    delay10tcy(61);//61 + 3 nop
    __asm
    nop
    nop
    nop
    __endasm;
}
#elif (SWBAUDRATE ==9600)
void DelayRXHalfBitUART(void)
{
//delay = (((2*OSC_FREQ)/(8*BAUDRATE))+1)/2 - 9 Tcy
    delay10tcy(61); //61 + 6nop
    __asm
    nop
    nop
    nop
    nop
    nop
    nop
    __endasm;
}

void DelayRXBitUART(void)
{
//delay = (((2*OSC_FREQ)/(4*BAUDRATE))+1)/2 - 14 Tcy
    delay10tcy(123); //123+6nop
    __asm
    nop
    nop
    nop
    nop
    nop
    nop
    __endasm;
}

void DelayTXBitUART(void)
{
    delay10tcy(123);//123 +8nop
    __asm
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    __endasm;
}
#elif (SWBAUDRATE ==4800)
void DelayRXHalfBitUART(void)
{
//delay = (((2*OSC_FREQ)/(8*BAUDRATE))+1)/2 - 9 Tcy
    delay10tcy(124); //124+1nop
    __asm
    nop
    __endasm;
}

void DelayRXBitUART(void)
{
//delay = (((2*OSC_FREQ)/(4*BAUDRATE))+1)/2 - 14 Tcy
    delay10tcy(248); //248 + 6nop
    __asm
    nop
    nop
    nop
    nop
    nop
    nop
    __endasm;
}

void DelayTXBitUART(void)
{
    delay10tcy(248);//248+8nop
    __asm
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    __endasm;
}
#endif

/* **********************************************************************
*      Function Name:  ReadUART                                        *
*      Return Value:   char: received data                             *
*      Parameters:     void                                            *
*      Description:    This routine waits for a start bit, and then    *
*                      reads all 8-bits.                               *
*      Special Note:   The user must provide two routines:             *
*                      DelayRXHalfBit():                               *
*                        DelayRXHalfBit should have:                   *
*                              5 Tcy for overhead                      *
*                              2 Tcy call to DelayRXHalfBit            *
*                              ? Tcy                                   *
*                              2 Tcy for return from DelayRXHalfBit    *
*                              = (((2*OSC_FREQ)/(8*BAUDRATE))+1)/2 Tcy *
*                      DelayRXBit():                                   *
*                        DelayRXBit should have:                       *
*                              10 Tcy for overhead                     *
*                              2 Tcy call to DelayRXBit                *
*                              ? Tcy                                   *
*                              2 Tcy for return from DelayRXBit        *
*                              = (((2*OSC_FREQ)/(4*BAUDRATE))+1)/2 Tcy *
*********************************************************************** */

unsigned char getcUART(void)
{
extern unsigned char uartdata;
int i;
//unsigned char temp;
uartdata=0;
while (SW_PORT_RXDpin){
	while (SW_PORT_RXDpin);
	DelayRXHalfBitUART();
}
/*      __asm
	      banksel _PORTB
	beginloop:
		  btfsc _PORTB,5
		  goto beginloop
		  call DelayRXHalfBitUART
	  __endasm;
*/
for(i=0; i<8; i++){	// data reading
    DelayRXBitUART();
//    uartdata = uartdata | (SW_PORT_RXDpin<<i);	//bits are written one by one
      __asm
        banksel _SW_PORT
        btfss   _SW_PORT,RXDpin         ; Set or clear the carry bit
        bcf             STATUS,0        ; according to the state of the
        btfsc   _SW_PORT,RXDpin         ; RXD I/O pin
        bsf             STATUS,0
        banksel _uartdata
        rrcf    _uartdata,F              ; Rotate the carry into data
	  __endasm;
}
      __asm
        banksel _uartdata
        movf   _uartdata,W,B
	  __endasm;
DelayRXBitUART(); // stop bit
//		swserial_printf("%c", uartdata);
return uartdata;
}

/* **********************************************************************
*      Function Name:  WriteUART                                       *
*      Return Value:   void                                            *
*      Parameters:     data: byte to send out the UART                 *
*      Description:    This routine sends a byte out the TXD pin.      *
*      Special Note:   The user must provide a routine:                *
*                      DelayTXBit():                                   *
*                        DelayTXBit should have:                       *
*                              8 Tcy for overhead                      *
*                              2 Tcy call to DelayTXBit                *
*                              ? Tcy                                   *
*                              2 Tcy for return from DelayTXBit        *
*                      = (((2*OSC_FREQ)/(4*BAUDRATE))+1)/2  Tcy        *
*********************************************************************** */


void putcUART (unsigned char uartdat)
{
int i;
//unsigned char temp;
uartdata=uartdat;
SW_PORT_TXDpin=1;
DelayTXBitUART();
SW_PORT_TXDpin=0; //start bit
DelayTXBitUART();
for(i=0;i < 8; i++) {
      __asm
        banksel _uartdata
        rrcf    _uartdata,F              ; Rotate next bit into carry
        banksel _SW_PORT
        btfss   STATUS,0                ; Set or clear TXD pin according
        bcf     _SW_PORT,TXDpin  ; to what is in the carry
        btfsc   STATUS,0
        bsf     _SW_PORT,TXDpin
      __endasm;
//    SW_PORT_TXDpin= (uartdata>>i) & 0x01;
	DelayTXBitUART();
}
SW_PORT_TXDpin=1;
DelayTXBitUART();
}

void swserial_printf(char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	pprintf(putcUART, fmt, args);
	va_end(args);
}

/********************************************************************
*    Function Name:  getsUART                                       *
*    Return Value:   void                                           *
*    Parameters:     buffer: pointer to string                      *
*                    len: length of characters to receive           *
*    Description:    This routine receives a string of characters   *
*                    from the SW UART of length specified by len.   *
********************************************************************/
unsigned char * getsUART(void)
{
	static unsigned char buffer[80]; // Need static attribute, because a function can not return
	                      // local array without static attribute.
	unsigned char c;
	unsigned char i = 0;

	do {
		c = getcUART();
		swserial_printf("%c", c);
		buffer[i++] = c;
	} while (c != '\r');
	buffer[i] = '\0';
	return (buffer);

}

/**********************************************************************
*    Function Name:  putsUART                                         *
*    Return Value:   void                                             *
*    Parameters:     data: pointer to string of data                  *
*    Description:    This routine transmits a string of characters    *
*                    to the UART data byte including the null.        *
**********************************************************************/
void putsUART( char *datx)
{
  do
  {    // Transmit a byte
    putcUART(*datx);
  } while( *datx++ );
}
#endif