/*	----------------------------------------------------------------------------
	FILE:			pinguinoserial.c
	PROJECT:		pinguinoX
	PURPOSE:		
	PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
	FIRST RELEASE:	01 jan. 2011
	LAST RELEASE:	11 jun. 2013
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

	// 18 feb. 2012 jp mandon added support for PIC32-PINGUINO-220
	
#ifndef __PINGUINOSERIAL2_C
#define __PINGUINOSERIAL2_C

#include <stdarg.h>
#include <printf.c>
#include <serial.c>
#include <typedef.h>

void serial2init(u32 speed)
{
	#ifdef PIC32_PINGUINO_220
		SerialConfigure(UART1, UART_ENABLE,	UART_RX_TX_ENABLED,	speed);
	#else
		SerialConfigure(UART2, UART_ENABLE,	UART_RX_TX_ENABLED,	speed);
	#endif
}

void serial2printf(char *fmt, ...)		
{
	va_list args;

	va_start(args, fmt);
	#ifdef PIC32_PINGUINO_220
		pprintf(SerialUART1WriteChar, fmt, args);
	#else
		pprintf(SerialUART2WriteChar, fmt, args);
	#endif
	va_end(args);
}

/*******************************************************************************
	And For Compatibility Reasons ....
	16-08-2011: fixed bug in print - Régis Blanchot & Tiew Weng Khai
	29-10-2011: fixed uncompatible arg. *s / serial2printf - Régis Blanchot
*******************************************************************************/

void serial2print(char *fmt,...)
{
	//	unsigned char *s;
	unsigned char s;
	va_list args;					// list of arguments

	va_start(args, fmt);			// initialize the list
	// s = va_start(args, fmt);
	s = (unsigned char) va_arg(args, int);		// get the first variable arg.

	//switch (*s)
	switch (s)
	{
		case DEC:
			serial2printf("%d",fmt);
			break;
		case HEX:
			serial2printf("%x",fmt);
			break;
		case BYTE:
			serial2printf("%d",fmt);
			break;
		case OCT:
			serial2printf("%o",fmt);
			break;
		case BIN:
			serial2printf("%b",fmt);
			break;           
		default:
			serial2printf(fmt);
			break;
	}
}

/*
void serial2print(char *fmt,...)
{
	va_list args;
	va_start(args, fmt);

	switch (*args)
	{
		case DEC:
			serial2printf("%d",(int)fmt);
			break;
		case HEX:
			serial2printf("%x",(int)fmt);
			break;
		case BYTE:
			serial2printf("%d",(unsigned char)fmt);
			break;
		case OCT:
			serial2printf("%o",(int)fmt);
			break;
		case BIN:
			serial2printf("%b",(int)fmt);
			break;           
		default:
			serial2printf(fmt);
			break;
	}

	va_end(ap);
}
*/

void serial2println(char *fmt,...)
{
	serial2printf(fmt);
	serial2printf("\r\n");
}

void serial2write(char c)
{
#ifdef PIC32_PINGUINO_220
	SerialUART1WriteChar(c);
#else
	SerialUART2WriteChar(c);
#endif
}

char serial2getkey(void)
{
#ifdef PIC32_PINGUINO_220
	return SerialGetKey(UART1);
#else
	return SerialGetKey(UART2);
#endif
}

char * serial2getstring(void)
{
#ifdef PIC32_PINGUINO_220
	return SerialGetString(UART1);
#else
	return SerialGetString(UART2);
#endif
}

char serial2available(void)
{
#ifdef PIC32_PINGUINO_220
	return SerialAvailable(UART1);
#else
	return SerialAvailable(UART2);
#endif
}

char serial2read(void)
{
#ifdef PIC32_PINGUINO_220
	return SerialRead(UART1);
#else
	return SerialRead(UART2);
#endif
}

void serial2flush(void)
{
#ifdef PIC32_PINGUINO_220
	SerialFlush(UART1);
#else
	SerialFlush(UART2);
#endif
}

BOOL serial2clearrxerror(void)
{
#ifdef PIC32_PINGUINO_220
	return(ClearRxError(UART1));
#else
	return(ClearRxError(UART2));
#endif
}
#endif /* __PINGUINOSERIAL2_C */

