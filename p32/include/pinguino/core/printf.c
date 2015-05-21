/*	----------------------------------------------------------------------------
    FILE:			printf.c
    PROJECT:		pinguino - http://www.pinguino.cc/
    PURPOSE:		alternative printf and sprintf functions
    PROGRAMERS:		regis blanchot <rblanchot@gmail.com>
                    mark harper <markfh@f2s.com>
    FIRST RELEASE:	10 Nov 2010
    LAST RELEASE:	20 Dec 2014
    ----------------------------------------------------------------------------
    Changelog :
    2014-12-20 - RB - fixed long and float support
    2015-01-31 - RB - fixed cast issue in pprint
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

#define PRINT_BUF_LEN 12		// should be enough for 32 bits

typedef void (*funcout) (char);	// type of void foo(u8)
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
    sign:		0 positive, 1 negative
    width:		justify
    pad:		PAD_RIGHT or PAD_ZERO
    letterbase:	'a' or 'A' (lower or upper case)
    return:		string's length
    --------------------------------------------------------------------------*/

static u8 pprinti(u8 **out, u32 i, u8 base, u8 sign, u8 width, u8 pad, u8 separator, u8 letterbase)
{
    u8 buffer[PRINT_BUF_LEN];
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
    if ( (sign) && (base == 10) && ( (s32)i < 0 ) )
    {
        neg = 1;
        uns32 = - (s32)i;
    }

    // we start at the end
    string = buffer + PRINT_BUF_LEN - 1;
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
    ----------------------------------------------------------------------------
    The IEEE 754 standard specifies a binary32 (float) as having:
        Sign bit: 1 bit
        Exponent width: 8 bits
        Significand precision: 24 bits (23 explicitly stored)
    --------------------------------------------------------------------------*/

//static u8 pprintfl(u8 **out, double value, u8 width, u8 pad, u8 separator, u8 precision)
//static u8 pprintfl(u8 **out, int value, u8 width, u8 pad, u8 separator, u8 precision)
static u8 pprintfl(u8 **out, double value, u8 width, u8 pad, u8 separator, u8 precision)
{
    union
    {
        float	f;
        //s32		l;
        int 	l;
    } helper;
    
    u32 mantissa;
    u32 int_part  = 0;
    u32 frac_part = 0;
    s8 exponent;
    u8 buffer[PRINT_BUF_LEN], *string = buffer;
    u8 tmp[PRINT_BUF_LEN], *s = tmp;
    u8 count = 0, m, t;
    u8 length = PRINT_BUF_LEN - 1;

    helper.f = (float)value;
    //helper.l = (s32)value;
    //helper.l = value;

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
        int_part  = mantissa >> (23 - exponent);
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
/* TODO
            if ( separator && (m % 3 == 0) )
            {
                pprintc(out, ',');
                ++count;
                --width;
            }
*/
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
    u8 width, pad;
    register u8 pc = 0;
    u8 precision = 2; // default value is 2 digits fractional part
    u8 separator = 0; // no thousands separator
    u8 scr[2];

    for (; *format != 0; ++format)
    {
        if (*format == '%')
        {
            ++format;				// get the next format identifier
            width = pad = 0;

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
            }

            for ( ; *format >= '0' && *format <= '9'; ++format)
            {
                precision *= 10;
                precision += *format - '0';
            }

            if (*format == 'f') 	// float
            {
                pc += pprintfl(out, va_arg(args, double), width, pad, separator, precision);
                //pc += pprintfl(out, va_arg(args, int), width, pad, separator, precision);
                continue;
            }

            if (*format == 's')		// string
            {
                const u8 *s = va_arg(args, char*);
                //RB20150131
                //pc += pprints(out, s?s:"(null)", width, pad);
                if (s)
                    pc += pprints(out, s, width, pad);
                else
                    pc += pprints(out, (const u8 *)"(null)", width, pad);
                continue;
            }

            if (*format == 'l')		// long support (only for 8-bit compatibility)
            {
                ++format;
                //islong = 1;
            }

            if (*format == 'u')		// unsigned integer
            {
                pc += pprinti(out, va_arg(args, u32), 10, 0, width, pad, separator, 'a');
                continue;
            }

            if (*format == 'd' || *format == 'i')		// decimal signed integer
            {
                pc += pprinti(out, va_arg(args, u32), 10, 1, width, pad, separator, 'a');
                continue;
            }

            if (*format == 'x' || *format == 'p')	// lower hexa or pointer
            {
                pc += pprinti(out, va_arg(args, u32), 16, 0, width, pad, separator, 'a');
                continue;
            }

            if (*format == 'X' || *format == 'P')	// upper hexa or pointer
            {
                pc += pprinti(out, va_arg(args, u32), 16, 0, width, pad, separator, 'A');
                continue;
            }

            if (*format == 'b')		// binary
            {
                pc += pprinti(out, va_arg(args, u32), 2, 0, width, pad, separator, 'a');
                continue;
            }

            if (*format == 'o')		// octal
            {
                pc += pprinti(out, va_arg(args, u32), 8, 0, width, pad, separator, 'a');
                continue;
            }

            if (*format == 'c') 	// ascii
            {
                scr[0] = (u8)va_arg(args, u32);
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
    u8 r;
    va_list args;

    va_start(args, format);
    r = pprint(&out, format, args);
    va_end(args);// RB 2014/01/13
    return r;
}

#endif /* __PRINTF_C */
