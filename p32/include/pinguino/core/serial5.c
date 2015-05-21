/*	--------------------------------------------------------------------
    FILE:			serial5.c
    PROJECT:		pinguino 32
    PURPOSE:		serial functions on UART5
    PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
    FIRST RELEASE:	01 Jan. 2011
    LAST RELEASE:	15 Jan. 2015
    --------------------------------------------------------------------
    15 Jan. 2015 regis blanchot - renamed _PINGUINOSERIAL5_C_ in __SERIAL5__
    --------------------------------------------------------------------
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
    ------------------------------------------------------------------*/

#ifndef __SERIAL5__
#define __SERIAL5__

#ifndef ENABLE_UART5
#define ENABLE_UART5
#endif

#include <stdarg.h>
#include <printf.c>
#include <serial.c>
#include <typedef.h>

void serial5init(u32 speed)
{
    SerialConfigure(UART5, UART_ENABLE,	UART_RX_TX_ENABLED,	speed);
}

void serial5printf(char *fmt, ...)		
{
    va_list args;

    va_start(args, fmt);
    pprintf(SerialUART5WriteChar, fmt, args);
    va_end(args);
}

/*
void serial5print(char *fmt,...)
{
    va_list args;
    va_start(args, fmt);

    switch (*args)
    {
        case DEC:
            serial5printf("%d",(int)fmt);
            break;
        case HEX:
            serial5printf("%x",(int)fmt);
            break;
        case BYTE:
            serial5printf("%d",(unsigned char)fmt);
            break;
        case OCT:
            serial5printf("%o",(int)fmt);
            break;
        case BIN:
            serial5printf("%b",(int)fmt);
            break;           
        default:
            serial5printf(fmt);
            break;
    }
    va_end(args);
}
*/

void serial5println(char *fmt,...)
{
    serial5printf(fmt);
    serial5printf("\n\r");
}

void serial5write(char c)
{
    SerialUART5WriteChar(c);
}

char serial5getkey(void)
{
    return SerialGetKey(UART5);
}

char * serial5getstring(void)
{
    return SerialGetString(UART5);
}

char serial5available(void)
{
    return SerialAvailable(UART5);
}

char serial5read(void)
{
    return SerialRead(UART5);
}

void serial5flush(void)
{
    SerialFlush(UART5);
}

BOOL serial5clearrxerror(void)
{
    return(SerialClearRxError(UART5));
}

#endif /* __SERIAL5__ */

