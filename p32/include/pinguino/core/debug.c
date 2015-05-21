/*	----------------------------------------------------------------------------
	---------- debug.c
	----------------------------------------------------------------------------
	@author		Régis Blanchot <rblanchot@gmail.com>
	@descr		Enable Formated Debug Function
				Debug Mode is triggered from the IDE > Pinguino > Debug Menu
	--------------------------------------------------------------------------*/

#ifndef __DEBUG_C
#define __DEBUG_C

#include <stdarg.h>

void debug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    
    #ifdef USBCDCDEBUG
        #include <__cdc.c>
        CDCprintf("debug: ");
        CDCprintf(format, args);
        CDCprintf("\r\n");

    #elif UART1DEBUG
        #include <serial1.c>
        serial1init(9600);
        serial1printf("debug: ");
        serial1printf(format, args);
        serial1printf("\r\n");

    #elif UART2DEBUG
        #include <serial2.c>
        serial2init(9600);
        serial2printf("debug: ");
        serial2printf(format, args);
        serial2printf("\r\n");
    #endif

    va_end(args);
}

#endif	/* __DEBUG_C */

