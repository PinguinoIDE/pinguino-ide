/*	----------------------------------------------------------------------------
	FILE:			pinguinoserial6.c
	PROJECT:		pinguino32X
	PURPOSE:		
	PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
					UART6 by Marcus Fazzi <anunakin@ieee.org>
	FIRST RELEASE:	23 set. 2011
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

#ifndef __PINGUINOSERIAL6_C
#define __PINGUINOSERIAL6_C

#ifndef ENABLE_UART6
#define ENABLE_UART6
#endif

#include <stdarg.h>
#include <printf.c>
#include <serial.c>
#include <typedef.h>

void serial6init(u32 speed)
{
	SerialConfigure(UART6, UART_ENABLE,	UART_RX_TX_ENABLED,	speed);
}

void serial6printf(char *fmt, ...)		
{
	va_list args;

	va_start(args, fmt);
	pprintf(SerialUART6WriteChar, fmt, args);
	va_end(args);
}

/*
void serial6print(char *fmt,...)
{
	va_list args;
	va_start(args, fmt);

	switch (*args)
	{
		case DEC:
			serial6printf("%d",(int)fmt);
			break;
		case HEX:
			serial6printf("%x",(int)fmt);
			break;
		case BYTE:
			serial6printf("%d",(unsigned char)fmt);
			break;
		case OCT:
			serial6printf("%o",(int)fmt);
			break;
		case BIN:
			serial6printf("%b",(int)fmt);
			break;           
		default:
			serial6printf(fmt);
			break;
	}
	va_end(args);
}
*/

void serial6println(char *fmt,...)
{
	serial6printf(fmt);
	serial6printf("\n\r");
}

void serial6write(char c)
{
	SerialUART6WriteChar(c);
}

char serial6getkey(void)
{
	return SerialGetKey(UART6);
}

char * serial6getstring(void)
{
	return SerialGetString(UART6);
}

char serial6available(void)
{
	return SerialAvailable(UART6);
}

char serial6read(void)
{
	return SerialRead(UART6);
}

void serial6flush(void)
{
	SerialFlush(UART6);
}

BOOL serial6clearrxerror(void)
{
	return(ClearRxError(UART6));
}

#endif /* __PINGUINOSERIAL6_C */

