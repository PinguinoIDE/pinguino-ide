/*	----------------------------------------------------------------------------
    FILE:			stdlib.c
    PROJECT:		pinguino
    PURPOSE:		stdlib extended functions
                    converts float, long or integer to ascii
    PROGRAMER:		regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:	05 nov. 2010
    LAST RELEASE:	25 nov. 2011
    --------------------------------------------------------------------------*/

#ifndef __STDLIB_C
#define __STDLIB_C

#include <stdlib.h> // malloc

/**************************************************
 *
 *    itoa - converts int to string
 *
 **************************************************/

char * pitoa(int value, char *string, int base)
{
    char tmp[33];
    char *tp = tmp;
    int i;
    unsigned v;
    int sign;
    char *sp;

    if (base > 36 || base <= 1)
        return 0;

    sign = (base == 10 && value < 0);
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % base;
        v = v / base;
        if (i < 10)
            *tp++ = i+'0';
        else
            *tp++ = i + 'a' - 10;
    }

    if (string == 0)
        string = (char *)malloc((tp-tmp)+sign+1);
    sp = string;

    if (sign)
        *sp++ = '-';
    while (tp > tmp)
        *sp++ = *--tp;
    *sp = 0;

    return string;
}

/**************************************************
 *
 *    ltoa - converts long to string
 *
 **************************************************/

char * pltoa(long value, char *string, int base)
{
    char tmp[33];
    char *tp = tmp;
    long i;
    unsigned long v;
    int sign;
    char *sp;

    if (base > 36 || base <= 1)
        return 0;

    sign = (base == 10 && value < 0);
    if (sign)
        v = -value;
    else
        v = (unsigned long)value;
    while (v || tp == tmp)
    {
        i = v % base;
        v = v / base;
        if (i < 10)
            *tp++ = i+'0';
        else
            *tp++ = i + 'a' - 10;
    }

    if (string == 0)
        string = (char *)malloc((tp-tmp)+sign+1);
    sp = string;

    if (sign)
        *sp++ = '-';
    while (tp > tmp)
        *sp++ = *--tp;
    *sp = 0;

    return string;
}

/**************************************************
 *
 *    ultoa - converts unsigned long to string
 *
 **************************************************/

char * pultoa(unsigned long value, char *string, int base)
{
    char tmp[33];
    char *tp = tmp;
    long i;
    unsigned long v = value;
    char *sp;

    if (base > 36 || base <= 1)
        return 0;
 
    while (v || tp == tmp)
    {
        i = v % base;
        v = v / base;
        if (i < 10)
            *tp++ = i+'0';
        else
            *tp++ = i + 'a' - 10;
    }

    if (string == 0)
        string = (char *)malloc((tp-tmp)+1);
    sp = string;

 
    while (tp > tmp)
        *sp++ = *--tp;
    *sp = 0;
    return string;
}

/**************************************************
 *
 *    ftoa - converts float to string
 *
 **************************************************
 *
 * This is a simple implemetation with rigid parameters:
 * 	- buffer must be 8 chars long
 * 	- 3 digits precision max
 * 	- absolute range is -524,287 to 524,287 
 * 	- resolution (epsilon) is 0.125 and always rounds down
 *
 **************************************************/

void pftoa(float value, char* buffer)
{
    union
    {
        float f;
        struct
        {
           unsigned int mantissa_lo : 16;
           unsigned int mantissa_hi : 7;    
           unsigned int	exponent : 8;
           unsigned int	sign : 1;
        };
    } helper;
    
    unsigned long mantissa;
    signed char exponent;
    unsigned int int_part;
    char frac_part[3];
    int i, count = 0;
    
    helper.f = value;
    //mantissa is LS 23 bits
    mantissa = helper.mantissa_lo;
    mantissa += ((unsigned long) helper.mantissa_hi << 16);
    //add the 24th bit to get 1.mmmm^eeee format
    mantissa += 0x00800000;
    //exponent is biased by 127
    exponent = (signed char) helper.exponent - 127;
    
    //too big to shove into 8 chars
    if (exponent > 18)
    {
        buffer[0] = 'I';
        buffer[1] = 'n';
        buffer[2] = 'f';
        buffer[3] = '\0';
        return;
    }
    
    //too small to resolve (resolution of 1/8)
    if (exponent < -3)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    
    count = 0;
    
    //add negative sign (if applicable)
    if (helper.sign)
    {
        buffer[0] = '-';
        count++;
    }
    
    //get the integer part
    int_part = mantissa >> (23 - exponent);    
    //convert to string
    itoa(int_part, &buffer[count], 10);
    
    //find the end of the integer
    for (i = 0; i < 8; i++)
        if (buffer[i] == '\0')
        {
           count = i;
           break;
        }	   
 
    //not enough room in the buffer for the frac part    
    if (count > 5)
        return;
    
    //add the decimal point    
    buffer[count++] = '.';
    
    //use switch to resolve the fractional part
    switch (0x7 & (mantissa  >> (20 - exponent)))
    {
        case 0:
           frac_part[0] = '0';
           frac_part[1] = '0';
           frac_part[2] = '0';
           break;
        case 1:
           frac_part[0] = '1';
           frac_part[1] = '2';
           frac_part[2] = '5';		  
           break;
        case 2:
           frac_part[0] = '2';
           frac_part[1] = '5';
           frac_part[2] = '0';		  
           break;
        case 3:
           frac_part[0] = '3';
           frac_part[1] = '7';
           frac_part[2] = '5';		  
           break;
        case 4:
           frac_part[0] = '5';
           frac_part[1] = '0';
           frac_part[2] = '0';		  
           break;
        case 5:
           frac_part[0] = '6';
           frac_part[1] = '2';
           frac_part[2] = '5';		  
           break;
        case 6:
           frac_part[0] = '7';
           frac_part[1] = '5';
           frac_part[2] = '0';		  
           break;
        case 7:
           frac_part[0] = '8';
           frac_part[1] = '7';
           frac_part[2] = '5';				
           break;
    }
    
    //add the fractional part to the output string
    for (i = 0; i < 3; i++)
        if (count < 7)
           buffer[count++] = frac_part[i];
    
    //make sure the output is terminated
    buffer[count] = '\0';
}

#endif /* __STDLIB_C */
