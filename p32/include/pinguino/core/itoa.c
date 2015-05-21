/*	----------------------------------------------------------------------------
	FILE:			itoa.c
	PROJECT:		pinguino
	PURPOSE:		converts integer, long or unsigned long to ascii
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	05 nov. 2010
	LAST RELEASE:	29 oct. 2011
	--------------------------------------------------------------------------*/

#include <stdlib.h>

#define HEXA	16
#define DECIMAL	10
#define OCTAL	8
#define BINARY	2

char * itoa(int value, char *string, int base)
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


char * ltoa(long value, char *string, int base)
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

char * ultoa(unsigned long value, char *string, int base)
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
			*tp++ = i + '0';
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

