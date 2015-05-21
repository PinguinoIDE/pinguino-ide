/*	----------------------------------------------------------------------------
	FILE:			ctype.c
	PROJECT:		pinguino
	PURPOSE:		alternative port of libc
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	7 feb. 2011
	LAST RELEASE:	7 feb. 2011
	----------------------------------------------------------------------------
	TODO :
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

#ifndef __CTYPE_C
	#define __CTYPE_C

#include <typedef.h>

u8 isalnum (u8);
u8 isalpha (u8);
u8 iscntrl (u8);
u8 isdigit (u8);
u8 isgraph (u8);
u8 islower (u8);
u8 isprint (u8);
u8 ispunct (u8);
u8 isspace (u8);
u8 isupper (u8);
u8 isxdigit(u8);
u8 tolower (u8);
u8 toupper (u8);
u8 isblank (u8);
u8 isascii (u8);
u8 toascii (u8);

u8 isalnum(u8 c)
{
	if ((isalpha(c)) || (isdigit(c)))
		return 1;
	return 0;
}

u8 isalpha(u8 c)
{
	if ((isupper(c)) || (islower(c)))
	  return 1;
	return 0;
}

/* Not sure which characters are actually control characters */
u8 iscntrl(u8 c)
{
	if ((c >= 0) && (c <= 31))
		return 1;
	return 0;
}

u8 isdigit(u8 c)
{
	if ((c >= 48) && (c <= 57))
		return 1;
	return 0;
}

/*  Not quite sure this one is correct either */
u8 isgraph(u8 c)
{
	if ((c >= 33) && (c <= 255))
	  return 1;
	return 0;
}


u8 islower(u8 c)
{
	if ((c >= 97) && (c <= 122))
		return 1;
	return 0;
}

/*  Not quite sure this one is correct either */
u8 isprint(u8 c)
{
	if ((c >= 32) && (c < 126))
	  return 1;
	return 0;
}


u8 ispunct(u8 c)
{
	if ((!isalnum(c)) && (!isspace(c)) && (isprint(c)))
		return 1;
	return 0;
}

u8 isspace(u8 c)
{
	if (c == 32)
		return 1;
	return 0;
}

u8 isupper(u8 c)
{
	if ((c >= 65) && (c <= 90))
		return 1;
	return 0;
}

/* Returns a nonzero value if c is a hex digit (0-9,a-f,A-F), zero otherwise */
u8 isxdigit(u8 c)
{
	if (((c >= 48) && (c<=57)) || ((c >= 65) && (c <= 70)) || ((c >= 97) && (c <= 102)))
		return 1;
	return 0;
}

u8 tolower(u8 c)
{
	if (!islower(c))
		return c + ('a' - 'A');
	else
		return c;
}

u8 toupper(u8 c)
{
	if (islower(c))
		return c - ('a' - 'A');
	else
		return c;
}

/*  Not quite sure this one is correct either */
u8 isblank(u8 c)
{
	//return ((__ctype_ptr__[c+1] & _B) || (c == '\t')); ???
	return (c == '\t');
}

u8 isascii(u8 c)
{
	return c >= 0 && c< 128;
}

u8 toascii(u8 c)
{
	return (c)&0177;
}

#endif
