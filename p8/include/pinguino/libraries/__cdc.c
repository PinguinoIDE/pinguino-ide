/*	--------------------------------------------------------------------
    FILE:         __cdc.c
    PROJECT:      pinguino
    PURPOSE:      USB CDC routines for use with pinguino board, 
    PROGRAMER:    Jean-Pierre Mandon 2010
    CHANGELOG:
    --------------------------------------------------------------------
    14 Jun 2011 - Regis Blanchot (rblanchot@gmail.com) added :
                  printf, println, print, write, getKey, getString
    05 Feb 2013 - Regis Blanchot (rblanchot@gmail.com) moved :
                  interrupt routine
    04 Mar 2014 - Regis Blanchot (rblanchot@gmail.com) added :
                  print, printNumber and printFloat
    10 Mar 2014 - Regis Blanchot (rblanchot@gmail.com) fixed :
                  printNumber, getKey and getString
                  printFloat still to do
                  init optimized
    ------------------------------------------------------------------*/

#ifndef __USBCDC
#define __USBCDC

#define USB_USE_CDC

#include <pic18fregs.h>
#include <usb/usb_cdc.h>
#include <usb/usb_config.c>
#include <usb/picUSB.c>
#include <usb/usb_cdc.c>
#include <typedef.h>

//#ifdef boot2
    #include <delayms.c>
//#endif

#if defined(CDCPRINTF)
    #include <stdio.c>                  // Pinguino printf
    #include <stdarg.h>
#endif

// CDC buffer length
#ifndef _CDCBUFFERLENGTH_
#define _CDCBUFFERLENGTH_ 64
#endif

u8 _cdc_buffer[_CDCBUFFERLENGTH_];  // usb buffer

/***********************************************************************
 * USB CDC init routine
 * called from main.c
 **********************************************************************/

void CDC_init(void)
{
    u32 counter=1;

    // Disable global interrupts
    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;

    #ifdef boot2
    
    UCON=0;
    UCFG=0;

    UEP1=0;UEP2=0;UEP3=0;UEP4=0;UEP5=0;
    UEP6=0;UEP7=0;UEP8=0;UEP9=0;UEP10=0;UEP11=0;
    UEP12=0;UEP13=0;UEP14=0;UEP15=0;
    
    // and wait 2 seconds
    Delayms(2000);

    #endif

    // Initialize USB for CDC
    UCFG = 0x14;            // Enable pullup resistors; full speed mode
    deviceState = DETACHED;
    remoteWakeup = 0x00;
    currentConfiguration = 0x00;

    // Enable USB module
    #if 0
    while (deviceState != CONFIGURED)
    {
        EnableUSBModule();
        ProcessUSBTransactions();
    }
    #else
    while ( counter++ && deviceState != CONFIGURED )
    {
        EnableUSBModule();
        ProcessUSBTransactions();
    }
    #endif

    // Enable Interrupt
    #if defined(__18f25k50) || defined(__18f45k50)
        PIR3bits.USBIF = 0;     // clear usb interrupt flag
        PIE3bits.USBIE = 1;     // enable usb interrupt
        IPR3bits.USBIP = 1;     // high priority interrupt
    #else
        PIR2bits.USBIF = 0;     // clear usb interrupt flag
        PIE2bits.USBIE = 1;     // enable usb interrupt
        IPR2bits.USBIP = 1;     // high priority interrupt
    #endif
    
    INTCONbits.GIEH = 1;   // Enable global HP interrupts
    INTCONbits.GIEL = 1;   // Enable global LP interrupts
}

/***********************************************************************
 * USB CDC write routine (CDC.write)
 * added by regis blanchot 14/06/2011
 * write 1 char on CDC port
 **********************************************************************/

#define CDCwrite(c) CDCputc(c)

/***********************************************************************
 * USB CDC print routine (CDC.print)
 * added by regis blanchot 04/03/2014
 * write a string on CDC port
 **********************************************************************/

#if defined(CDCPRINT) || defined(CDCPRINTLN)
void CDCprint(char *string)
{
    CDCputs(string, strlen(string));
}
#endif

/***********************************************************************
 * USB CDC print routine (CDC.println)
 * added by regis blanchot 04/03/2014
 * write a string followed by a carriage return character (ASCII 13, or '\r')
 * and a newline character (ASCII 10, or '\n') on CDC port
 **********************************************************************/

#if defined(CDCPRINTLN)
void CDCprintln(char *string)
{
    CDCputs(string, strlen(string));
    Delayms(1);
    CDCputc('\r');
    Delayms(1);
    CDCputc('\n');
    Delayms(1);
    //CDCprint(string);
    //CDCprint("\n\r");
}
#endif

/***********************************************************************
 * USB CDC printNumber routine (CDC.printNumber)
 * added by regis blanchot 14/06/2011
 * write a number on CDC port
 * base : see const.h (DEC, BIN, HEXA, OCTO, ...)
 **********************************************************************/

#if defined(CDCPRINTNUMBER) || defined(CDCPRINTFLOAT)
void CDCprintNumber(long value, u8 base)
{  
    u8 sign;
    u8 length;

    long i;
    unsigned long v;    // absolute value

    u8 tmp[12];
    u8 *tp = tmp;       // pointer on tmp

    u8 string[12];
    u8 *sp = string;    // pointer on string

    if (value==0)
    {
        //CDCputs("0", 1);
        CDCputc('0');
        return;
    }
    
    sign = ( (base == 10) && (value < 0) );

    if (sign)
        v = -value;
    else
        v = (unsigned long)value;

    //while (v || tp == tmp)
    while (v)
    {
        i = v % base;
        v = v / base;
        
        if (i < 10)
            *tp++ = i + '0';
        else
            *tp++ = i + 'A' - 10;
    }

    // start of string
    if (sign)
        *sp++ = '-';

    length = sign + tp - tmp + 1;

    // backwards writing 
    while (tp > tmp)
        *sp++ = *--tp;

    // end of string
    *sp = 0;

    CDCputs(string, length);
}
#endif

/***********************************************************************
 * USB CDC printFloat routine (CDC.printFloat)
 * added by regis blanchot 14/06/2011
 * write a float number on CDC port
 * base : see const.h (DEC, BIN, HEXA, OCTO, ...)
 **********************************************************************/

#if defined(CDCPRINTFLOAT)
void CDCprintFloat(float number, u8 digits)
{ 
    u8 i, toPrint;
    u16 int_part;
    float rounding, remainder;

    // Handle negative numbers
    if (number < 0.0)
    {
        //CDCputs('-', 1);
        CDCputc('-');
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"  
    rounding = 0.5;
    for (i=0; i<digits; ++i)
        rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it  
    int_part = (u16)number;
    remainder = number - (float)int_part;
    CDCprintNumber(int_part, 10);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0)
    {
        //CDCputs('.', 1); 
        CDCputc('.'); 
    }
    
    // Extract digits from the remainder one at a time
    while (digits-- > 0)
    {
        remainder *= 10.0;
        toPrint = (unsigned int)remainder; //Integer part without use of math.h lib, I think better! (Fazzi)
        CDCprintNumber(toPrint, 10);
        remainder -= toPrint; 
    }
}
#endif

/***********************************************************************
 * USB CDC printf routine (CDC.printf)
 * added by regis blanchot 14/06/2011
 * write a formated string on CDC port
 **********************************************************************/

#if defined(CDCPRINTF)
void CDCprintf(const u8 *fmt, ...)
{
    u8 length;
    va_list	args;

    va_start(args, fmt);
    length = psprintf2(_cdc_buffer, fmt, args);
    CDCputs(_cdc_buffer, length);
    va_end(args);
}
#endif

/***********************************************************************
 * USB CDC getKey routine (CDC.getKey)
 * added by regis blanchot 14/06/2011
 * wait and return a char from CDC port
 **********************************************************************/

#if defined(CDCGETKEY) || defined(CDCGETSTRING)
u8 CDCgetkey(void)
{
    u8 buffer[_CDCBUFFERLENGTH_];		// always get a full packet

    while (!CDCgets(buffer));
    return (buffer[0]);	// return only the first character
}
#endif

/***********************************************************************
 * USB CDC getString routine (CDC.getString)
 * added by regis blanchot 14/06/2011
 * wait and return a string from CDC port
 **********************************************************************/

#if defined(CDCGETSTRING)
u8 * CDCgetstring(void)
{
    u8 c, i = 0;
    static u8 buffer[_CDCBUFFERLENGTH_];	// Needs static buffer at least.
    
    do {
        c = CDCgetkey();
        buffer[i++] = c;
        //CDCputs(&buffer[i-1], 1);
        CDCputc(buffer[i-1]);
    } while (c != '\r');
    buffer[i] = '\0';
    return buffer;
}
#endif

/***********************************************************************
 * USB CDC interrupt routine
 * added by regis blanchot 05/02/2013
 * called from main.c
 **********************************************************************/
 
void CDC_interrupt(void)
{
    #if defined(__18f25k50) || defined(__18f45k50)
    if(PIR3bits.USBIF)
    {
        PIR3bits.USBIF = 0;
    #else
    if(PIR2bits.USBIF)
    {
        PIR2bits.USBIF = 0;
    #endif
        ProcessUSBTransactions();
        UIRbits.SOFIF = 0;
        UIRbits.URSTIF = 0;
        UEIR = 0;
    }
}

#endif /* __USBCDC */

/*
// CDC.print
#define CDCprint(m,type)	{ CDCprint_##type(m);  }
#define CDCprintln(m,type)	{ CDCprint_##type(m);  CDCprintf("\r\n"); }
void CDCprint_FLOAT(float m){ CDCprintf("%f",m); }
void CDCprint_DEC(u16 m)    { CDCprintf("%d",m); }
void CDCprint_HEX(u16 m)    { CDCprintf("%x",m); }
void CDCprint_BYTE(u16 m)   { CDCprintf("%d",m); }
void CDCprint_OCT(u16 m)    { CDCprintf("%o",m); }
void CDCprint_BIN(u16 m)    { CDCprintf("%b",m); }
*/

/* CDCprint() function can not correctly support CDCprint("some string") aka CDC.print("some string").
 * In case CDCprint("some string"), va_arg(args, u32) will return a unexpected invalid value. 
 * by avrin */

/*
#if defined(CDCPRINT)
void CDCprint(const u8 *fmt, ...)
{
    u8 s;
    va_list args;							// a list of arguments
    va_start(args, fmt);					// initialize the list
    s = (u8) va_arg(args, u32);				// get the first variable arg.

    // last is a string (char *) or an integer
    //switch (*args)
    switch (s)
    {
        case FLOAT:
            CDCprintf("%f", (u32)fmt);
            break;
        case DEC:
            CDCprintf("%d", (u32)fmt);
            break;
        case HEX:
            CDCprintf("%x", (u32)fmt);
            break;
        case BYTE:
            //CDCprintf("%d", (u8)fmt);
            CDCprintf("%d", (u32)fmt);
            break;
        case OCT:
            CDCprintf("%o", (u32)fmt);
            break;
        case BIN:
            CDCprintf("%b", (u32)fmt);
            break;           
        default:
            CDCprintf(fmt);
            break;
    }
    va_end(args);
}
#endif

//CDC.println
#if defined(CDCPRINTLN)
void CDCprintln(const u8 *fmt, ...)
{
    va_list args;							// a list of arguments
    va_start(args, fmt);					// initialize the list

    CDCprintf(fmt, args);
    CDCprintf("\n\r");
}
#endif
*/

