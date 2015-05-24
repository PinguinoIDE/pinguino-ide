/*	--------------------------------------------------------------------
    FILE:			serial2.c
    PROJECT:		pinguino 32
    PURPOSE:		serial functions on UART2
    PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
    FIRST RELEASE:	01 Jan. 2011
    LAST RELEASE:	15 Jan. 2015
    --------------------------------------------------------------------
    18 Feb. 2012 jp mandon added support for PIC32-PINGUINO-220
    15 Jan. 2015 regis blanchot - renamed _PINGUINOSERIAL2_C_ in __SERIAL2__
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

#ifndef __SERIAL2__
#define __SERIAL2__

#include <stdarg.h>
#ifdef SERIALPRINTF
#include <printf.c>
#endif
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

#ifdef SERIALPRINTF
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
#endif

#if defined(SERIALPRINT) || defined(SERIALPRINTLN) || \
    defined(SERIALPRINTNUMBER) || defined(SERIALPRINTFLOAT)
void serial2print(char *string)
{
    #ifdef PIC32_PINGUINO_220
        SerialPrint(UART1, string);
    #else
        SerialPrint(UART2, string);
    #endif
}
#endif

#if defined(SERIALPRINTLN)
void serial2println(char *string)
{
    serial2print(string);
    serial2print("\r\n");
}
#endif

#if defined(SERIALPRINTNUMBER) || defined(SERIALPRINTFLOAT)
void serial2printNumber(long value, u8 base)
{
    #ifdef PIC32_PINGUINO_220
        SerialPrintNumber(UART1, value, base);
    #else
        SerialPrintNumber(UART2, value, base);
    #endif
}
#endif

#if defined(SERIALPRINTFLOAT)
void serial2printFloat(float number, u8 digits)
{ 
    #ifdef PIC32_PINGUINO_220
        SerialPrintFloat(UART1, number, digits);
    #else
        SerialPrintFloat(UART2, number, digits);
    #endif
}
#endif
    
    
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
        return(SerialClearRxError(UART1));
    #else
        return(SerialClearRxError(UART2));
    #endif
}

#endif /* __SERIAL2__ */

