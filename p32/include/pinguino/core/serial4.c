/*	--------------------------------------------------------------------
    FILE:			serial4.c
    PROJECT:		pinguino 32
    PURPOSE:		serial functions on UART4
    PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
    FIRST RELEASE:	01 Jan. 2011
    LAST RELEASE:	15 Jan. 2015
    --------------------------------------------------------------------
    15 Jan. 2015 regis blanchot - renamed _PINGUINOSERIAL4_C_ in __SERIAL4__
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

#ifndef __SERIAL4__
#define __SERIAL4__

#ifndef ENABLE_UART4
#define ENABLE_UART4
#endif

#include <stdarg.h>
#include <printf.c>
#include <serial.c>
#include <typedef.h>

void serial4init(u32 speed)
{
    SerialConfigure(UART4, UART_ENABLE,	UART_RX_TX_ENABLED,	speed);
}

void serial4printf(char *fmt, ...)		
{
    va_list args;

    va_start(args, fmt);
    pprintf(SerialUART4WriteChar, fmt, args);
    va_end(args);
}

/*
void serial4print(char *fmt,...)
{
    va_list args;
    va_start(args, fmt);

    switch (*args)
    {
        case DEC:
            serial4printf("%d",(int)fmt);
            break;
        case HEX:
            serial4printf("%x",(int)fmt);
            break;
        case BYTE:
            serial4printf("%d",(unsigned char)fmt);
            break;
        case OCT:
            serial4printf("%o",(int)fmt);
            break;
        case BIN:
            serial4printf("%b",(int)fmt);
            break;           
        default:
            serial4printf(fmt);
            break;
    }
    va_end(args);
}
*/

void serial4println(char *fmt,...)
{
    serial4printf(fmt);
    serial4printf("\n\r");
}

void serial4write(char c)
{
    SerialUART4WriteChar(c);
}

char serial4getkey(void)
{
    return SerialGetKey(UART4);
}

char * serial4getstring(void)
{
    return SerialGetString(UART4);
}

char serial4available(void)
{
    return SerialAvailable(UART4);
}

char serial4read(void)
{
    return SerialRead(UART4);
}

void serial4flush(void)
{
    SerialFlush(UART4);
}


BOOL serial4clearrxerror(void)
{
    return(SerialClearRxError(UART4));
}

#endif /* __SERIAL4__ */

