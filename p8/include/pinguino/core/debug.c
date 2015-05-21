/*	----------------------------------------------------------------------------
	---------- debug.c
	----------------------------------------------------------------------------
	@author		Régis Blanchot <rblanchot@gmail.com>
	@descr		Enable Debug Mode
	--------------------------------------------------------------------------*/

#ifndef __DEBUG_C
#define __DEBUG_C
#define DEBUG

#include <stdarg.h>

#if defined(USBCDCDEBUG)
#include <__cdc.c>
#endif

#if defined(UART1DEBUG)
#include <serial.c>
#endif

void debug(const char *format, ...)
{
	va_list args;
	va_start(args, format);

	#if defined(NODEBUG)
	#endif

	#if defined(USBCDCDEBUG)
		CDCprintf("debug: ");
		CDCprintf(format, args);
		CDCprintf("\r\n");
	#endif

	#if defined(UART1DEBUG)
		#include <serial.c>
		serial_begin(9600);
		serial_printf("debug: ");
		serial_printf(format, args);
		serial_printf("\r\n");
	#endif

	va_end(args);
}

#endif	/* __DEBUG_C */

