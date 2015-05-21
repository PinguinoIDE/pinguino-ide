/*	----------------------------------------------------------------------------
    FILE:			printf.c
    PROJECT:		pinguino - http://www.pinguino.cc/
    PURPOSE:		alternative printf and sprintf functions
    PROGRAMERS:		regis blanchot <rblanchot@gmail.com>
                    mark harper <markfh@f2s.com>
    FIRST RELEASE:	10 Nov 2010
    LAST RELEASE:	16 Jan 2012
    ----------------------------------------------------------------------------
    TODO : thousands separator
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

#ifndef __PRINTF_C
#define __PRINTF_C

#include <stdarg.h>
#include <typedef.h>

#define PRINTF_BUF_LEN 12		// should be enough for 32 bits

typedef void (*funcout) (u8);	// type of void foo(u8)
static funcout pputchar;		// then void pputchar(u8)

/*	----------------------------------------------------------------------------
    pprintc = pinguino print char
    --------------------------------------------------------------------------*/

static void pprintc(u8 **str, u8 c)
{
    if (str)
    {
        **str = c;
        ++(*str);
    }
    else
    {
        pputchar(c);
    }
}

#define PAD_RIGHT	1
#define PAD_ZERO	2

/*	----------------------------------------------------------------------------
    pprints = pinguino print string
    --------------------------------------------------------------------------*/

static int pprints(u8 **out, const u8 *string, u8 width, u8 pad)
{
    register u8 pc = 0;
    u8 padchar = ' ';
    u8 len = 0;
    const u8 *ptr;

    if (width > 0)
    {
        for (ptr = string; *ptr; ++ptr)
            ++len;
        if (len >= width)
            width = 0;
        else
            width -= len;
        if (pad & PAD_ZERO) padchar = '0';
    }
    
    if (!(pad & PAD_RIGHT))
    {
        for ( ; width > 0; --width)
        {
            pprintc(out, padchar);
            ++pc;
        }
    }
    
    for ( ; *string ; ++string)
    {
        pprintc(out, *string);
        ++pc;
    }
    
    for ( ; width > 0; --width)
    {
        pprintc(out, padchar);
        ++pc;
    }

    return pc;
}

/*	----------------------------------------------------------------------------
    pprinti = pinguino print 32-bit signed or unsigned integer
    i:			32-bit number to convert into string
    base:		1 byte, 2 binary, 8 octal, 10 decimal, 16 hexadecimal
    sign:		0 unsigned, 1 signed
    width:		justify
    pad:		PAD_RIGHT or PAD_ZERO
    letterbase:	'a' or 'A' (lower or upper case)
    return:		string's length
    --------------------------------------------------------------------------*/

static u8 pprinti(u8 **out, u32 i, u8 islong, u8 base, u8 sign, u8 width, u8 pad, u8 separator, u8 letterbase)
{
    u8 buffer[PRINTF_BUF_LEN];
    u8 *string;
    u8 neg = 0, pc = 0;
    u32 t, uns32 = i;

    if (i == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return pprints(out, buffer, width, pad);
    }

    // Do we have a negative decimal number ?
    if  ( (sign) && (base == 10) )          // decimal signed number ?
    {
        if ( (islong) && ((s32)i < 0) )     // negative 32-bit ?
        {
            neg = 1;
            uns32 = - (s32)i;
        }
        if ( (!islong) && ((s16)i < 0) )    // negative 16-bit ?
        {
            neg = 1;
            uns32 = - (s16)i;
        }
    }
    
    // we start at the end
    string = buffer + PRINTF_BUF_LEN - 1;
    *string = '\0';

    while (uns32)
    {
        t = uns32 % base;
        if ( t >= 10 )
            t += letterbase - '0' - 10;
        *--string = t + '0';
        uns32 /= base;
    }

    if (neg)
    {
        if (width && (pad & PAD_ZERO))
        {
            pprintc(out, '-');
            ++pc;
            --width;
        }
        else
        {
            *--string = '-';
        }
    }

    return pc + pprints(out, string, width, pad);
}

/*	----------------------------------------------------------------------------
    pprintfl = pinguino print float
    --------------------------------------------------------------------------*/

static u8 pprintfl(u8 **out, float value, u8 width, u8 pad, u8 separator, u8 precision)
{
    union
    {
        float	f;
        s32		l;
    } helper;
    
    u32 mantissa;
    u32 int_part  = 0;
    u32 frac_part = 0;
    s8 exponent;
    u8 buffer[PRINTF_BUF_LEN], *string = buffer;
    u8 tmp[PRINTF_BUF_LEN], *s = tmp;
    u8 count = 0, m, t;
    u8 length = PRINTF_BUF_LEN - 1;

    helper.f = value;

    // add negative sign if applicable
    if (helper.l < 0)
    {
        if (width && (pad & PAD_ZERO))
        {
            pprintc(out, '-');
            ++count;
            --width;
        }
        else
        {
            *string++ = '-';
            length--;
        }
    }

    // shifts the 23 bits of mantissa out,
    // takes the next 8 bits then subtracts 127 to get actual exponent
    exponent = ((helper.l >> 23) & 0xFF) - 127;	

    // takes last 23 bits and adds the implicit 1
    mantissa = (helper.l & 0x7FFFFF) | 0x800000;

/*
    if ( (exponent >= 31) || (exponent < -23) )
    {
        buffer[0] = 'i';
        buffer[1] = 'n';
        buffer[2] = 'f';
        buffer[3] = '\0';
        return pprints(out, buffer, width, pad);
    }
*/

    if (exponent >= 31)
    {
        buffer[0] = '+';
        buffer[1] = 'i';
        buffer[2] = 'n';
        buffer[3] = 'f';
        buffer[4] = '\0';
        return pprints(out, buffer, width, pad);
    }

    else if (exponent < -23)
    {
        /*
        buffer[0] = '-';
        buffer[1] = 'i';
        buffer[2] = 'n';
        buffer[3] = 'f';
        buffer[4] = '\0';
        return pprints(out, buffer, width, pad);
        */
        int_part  = 0;
        frac_part = 0;
    }

    else if (exponent >= 23)
    {
        int_part = mantissa << (exponent - 23);
    }

    else if (exponent >= 0) 
    {
        int_part = mantissa >> (23 - exponent);
        frac_part = (mantissa << (exponent + 1)) & 0xFFFFFF; // mfh
    }

    else // if (exponent < 0)
        frac_part = (mantissa & 0xFFFFFF) >> -(exponent + 1);

    // add integer part to string
    if (int_part == 0)
    {
        *string++ = '0';
        length--;
    }
    else
    {
        m = 0;
        // the string is more easily written backwards
        while (int_part)
        {
            t = int_part % 10;		// decimal base
            *s++ = t + '0';
            int_part /= 10;
            m++;					// string's length counter
            length--;
        }
        // now the string is written in the right direction
        while (m--)
        {
            *string++ = *--s;
/*	--- TODO -------------------------------------------------------------------
            if ( separator && (m % 3 == 0) )
            {
                pprintc(out, ' ');
                ++count;
                --width;
            }
    --------------------------------------------------------------------------*/
        }
    }
    
    // add fractional part to string
    if (precision > 6)
        precision = 6;

    // check if we have enough space
    if (precision > length)
        precision = length;
 
    // otherwise, number has no fractional part
    if (precision >= 1)
    {
        // add the decimal point to string
        *string++ = '.';

        // print binary-coded decimal (BCD)
        for (m = 0; m < precision; m++)
        {
            // multiplies frac_part by 10 by adding 8 times and 2 times; 
            frac_part = (frac_part << 3) + (frac_part << 1); 
            // converts leading digits to number character
            *string++ = (frac_part >> 24) + '0';
            // strips off leading digits
            frac_part &= 0xFFFFFF;
        }
    }

    // end of string
    *string++ = '\0';

    return count + pprints(out, buffer, width, pad);
}

/*	----------------------------------------------------------------------------
    pprint = pinguino print
    --------------------------------------------------------------------------*/

static u8 pprint(u8 **out, const u8 *format, va_list args)
{
    u32 val;
    u8 width, pad, islong;
    register u8 pc = 0;
    u8 precision = 2; // default value is 2 digits fractional part
    u8 separator = 0; // no thousands separator
    u8 scr[2];

    for (; *format != 0; ++format)
    {
        val = 0;
        islong = 0;                 // default is 16-bit
        
        if (*format == '%')
        {
            width = pad = 0;		// default is left justify, no zero padded
            ++format;				// get the next format identifier

            if (*format == '\0')	// end of line
                break;

            if (*format == '%')		// error
                goto abort;

            if (*format == '-')		// right justify
            {
                ++format;
                pad = PAD_RIGHT;
            }

            while (*format == '0')	// field is padded with 0's instead of blanks
            {
                ++format;
                pad |= PAD_ZERO;
            }
                                    // how many 0 ?
            for ( ; *format >= '0' && *format <= '9'; ++format)
            {
                width *= 10;
                width += *format - '0';
            }

/* TODO
            if (*format == '\'')	// thousands separator
            {
                separator = 1;
                ++format;
            }
*/
            if (*format == '.')		// float precision
            {
                ++format;
                precision = 0;
                // get number of digits after decimal point
                for ( ; *format >= '0' && *format <= '9'; ++format)
                {
                    precision *= 10;
                    precision += *format - '0';
                }
            }

            if (*format == 'f') 	// float
            {
                pc += pprintfl(out, va_arg(args, float), width, pad, separator, precision);
                continue;
            }

            if (*format == 's')		// string
            {
                u8 *s = va_arg(args, u8*);
                pc += pprints(out, s?s:"(null)", width, pad);
                continue;
            }
            
            if (*format == 'l')		// long support
            {
                ++format;
                islong = 1;
            }
            
            if (*format == 'u')		// decimal (10) unsigned (0) integer
            {
                val = (islong) ? va_arg(args, u32) : va_arg(args, u16);
                pc += pprinti(out, val, islong, 10, 0, width, pad, separator, 'a');
                continue;
            }

            if (*format == 'd' || *format == 'i') // decimal (10) signed (1) integer
            {
                val = (islong) ? va_arg(args, u32) : va_arg(args, u16);
                pc += pprinti(out, val, islong, 10, 1, width, pad, separator, 'a');
                continue;
            }

            if (*format == 'x' || *format == 'p')	// unsigned (0) lower ('a') hexa (16) or pointer
            {
                val = (islong) ? va_arg(args, u32) : va_arg(args, u16);
                pc += pprinti(out, val, islong, 16, 0, width, pad, separator, 'a');
                continue;
            }

            if (*format == 'X' || *format == 'P')	// unsigned (0) upper ('A') hexa (16) or pointer
            {
                val = (islong) ? va_arg(args, u32) : va_arg(args, u16);
                pc += pprinti(out, val, islong, 16, 0, width, pad, separator, 'A');
                continue;
            }

            if (*format == 'b')		// binary
            {
                val = (islong) ? va_arg(args, u32) : va_arg(args, u16);
                pc += pprinti(out, val, islong, 2, 0, width, pad, separator, 'a');
                continue;
            }

            if (*format == 'o')		// octal
            {
                val = (islong) ? va_arg(args, u32) : va_arg(args, u16);
                pc += pprinti(out, val, islong, 8, 0, width, pad, separator, 'a');
                continue;
            }

            if (*format == 'c') 	// ascii
            {
                scr[0] = va_arg(args, u16);
                scr[1] = '\0';
                pc += pprints(out, scr, width, pad);
                continue;
            }

        }
        else
        {
            abort:
            pprintc(out, *format);
            ++pc;
        }
    }
    if (out) **out = '\0';
    return pc;
}

/*	----------------------------------------------------------------------------
    pprintf = pinguino print formatted
    --------------------------------------------------------------------------*/

//int pprintf(funcout func, const char *format, ...)
u8 pprintf(funcout func, const u8 *format, va_list args)
{
    //va_list args;
    
    pputchar = func;
    //va_start( args, format );
    return pprint(0, format, args);
}

/*	----------------------------------------------------------------------------
    psprintf = pinguino print formatted data to string
    --------------------------------------------------------------------------*/

// to work with CDC.printf
u8 psprintf2(u8 *out, const u8 *format, va_list args)
{
    return pprint(&out, format, args);
}

// to work with Sprintf
u8 psprintf(u8 *out, const u8 *format, ...)
{
    va_list args;

    va_start(args, format);
    return pprint(&out, format, args);
}

#endif /* __PRINTF_C */
