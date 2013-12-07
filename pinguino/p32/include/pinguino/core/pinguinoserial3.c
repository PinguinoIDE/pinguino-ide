/*	----------------------------------------------------------------------------
	FILE:			pinguinoserial3.c
	PROJECT:		pinguino32X
	PURPOSE:		
	PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
					UART3 by Marcus Fazzi <anunakin@ieee.org>
	FIRST RELEASE:	21 set. 2011
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

#ifndef __PINGUINOSERIAL3_C
#define __PINGUINOSERIAL3_C

#ifndef ENABLE_UART3
#define ENABLE_UART3
#endif

#include <stdarg.h>
#include <printf.c>
#include <serial.c>
#include <typedef.h>

void serial3init(u32 speed)
{
	SerialConfigure(UART3, UART_ENABLE,	UART_RX_TX_ENABLED,	speed);
}

void serial3printf(char *fmt, ...)		
{
	va_list args;

	va_start(args, fmt);
	pprintf(SerialUART3WriteChar, fmt, args);
	va_end(args);
}

/*
void serial3print(char *fmt,...)
{
	va_list args;
	va_start(args, fmt);

	switch (*args)
	{
		case DEC:
			serial3printf("%d",(int)fmt);
			break;
		case HEX:
			serial3printf("%x",(int)fmt);
			break;
		case BYTE:
			serial3printf("%d",(unsigned char)fmt);
			break;
		case OCT:
			serial3printf("%o",(int)fmt);
			break;
		case BIN:
			serial3printf("%b",(int)fmt);
			break;           
		default:
			serial3printf(fmt);
			break;
	}
	va_end(args);
}
*/

void serial3println(char *fmt,...)
{
	serial3printf(fmt);
	serial3printf("\n\r");
}

void serial3write(char c)
{
	SerialUART3WriteChar(c);
}

char serial3getkey(void)
{
	return SerialGetKey(UART3);
}

char * serial3getstring(void)
{
	return SerialGetString(UART3);
}

char serial3available(void)
{
	return SerialAvailable(UART3);
}

char serial3read(void)
{
	return SerialRead(UART3);
}

void serial3flush(void)
{
	SerialFlush(UART3);
}

BOOL serial3clearrxerror(void)
{
	return(ClearRxError(UART3));
}

#endif /* __PINGUINOSERIAL3_C */

