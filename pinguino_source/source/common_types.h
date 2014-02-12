/*-------------------------------------------------------------------------
  common_types.h - Common declarations and types

             (c) 2005 Pierre Gaufillet <pierre.gaufillet@magic.fr> 

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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
-------------------------------------------------------------------------*/

/* $Id: common_types.h,v 1.1 2006/11/11 14:08:34 gaufille Exp $ */

#ifndef VASCO_H_
#define VASCO_H_

//#ifndef __TYPEDEF_H
//#define __TYPEDEF_H
//#endif

/* 8 bits */
//typedef unsigned char			u8;                    // conflict with typedef.h
//typedef char						s8;
typedef unsigned char			uchar;
typedef unsigned char			UCHAR;
//typedef unsigned char			byte;
typedef unsigned char 			BYTE;
//typedef unsigned char			BOOL;
//typedef unsigned char			bool;					// not compatible with c++
//typedef unsigned char			boolean;	
typedef signed char				schar;
typedef signed char				CHAR;
//typedef unsigned char 			uint8_t;            // conflict with stdint.h
//typedef signed char 				int8_t;
typedef union
{
    uchar uc;
    schar sc;
    struct
    {
        unsigned b0:1;
        unsigned b1:1;
        unsigned b2:1;
        unsigned b3:1;
        unsigned b4:1;
        unsigned b5:1;
        unsigned b6:1;
        unsigned b7:1;
    };
} Char;

/* 16 bits */
//typedef unsigned int				u16;                    // conflict with typedef.h
//typedef int							s16;
typedef int							INT;
typedef unsigned int				UINT;
typedef unsigned int				uint;
typedef signed int				sint;
//typedef unsigned int				word;
typedef short						SHORT;
typedef unsigned short			USHORT;
typedef unsigned short			WORD;
typedef unsigned short			WCHAR;
//typedef int 						int16_t;            // conflict with stdint.h
//typedef unsigned int 			uint16_t;
typedef union
{
    uint ui;
    sint si;
    Char c[2];
} Int;


/* 32 bits */
//typedef unsigned long			u32;                    // conflict with typedef.h
//typedef long						s32;
typedef unsigned long			ulong;
typedef signed long				slong;
//typedef unsigned long			dword; 
typedef long						LONG;
typedef unsigned long			ULONG;
typedef unsigned long			DWORD;
//typedef long 						int32_t;            // conflict with stdint.h
//typedef unsigned long 			uint32_t;
typedef union
{
    ulong ul;
    slong sl;
    Int   i[2];
    Char  c[4];
} Long;

//#define TRUE 1
//#define FALSE 0

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif

#ifndef NULL
#define NULL 0
#endif

#endif /*VASCO_H_*/
