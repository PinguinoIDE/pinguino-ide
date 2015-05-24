/*	----------------------------------------------------------------------------
    typedef.h
    Régis Blanchot
    ----------------------------------------------------------------------------
    SDCC types:
    char (8 bits, 1 byte)
    short (16 bits, 2 bytes)
    int (16 bits, 2 bytes)
    long (32 bits, 4 bytes)
    long long (64 bits, 8 bytes)
    --------------------------------------------------------------------------*/

#ifndef __TYPEDEF_H
    #define __TYPEDEF_H

    #include <stdint.h>                     // ISO C99 7.18 Integer types

/*	----------------------------------------------------------------------------
    pinguino types
    --------------------------------------------------------------------------*/

    typedef signed char			s8;
    typedef signed int			s16;
    typedef signed long			s32;
    //typedef signed long long 	s64;        // SDCC doesn't support 64-bit type

    typedef unsigned char		u8;
    typedef unsigned int		u16;
    typedef unsigned long		u32;
    //typedef unsigned long long 	u64;    // SDCC doesn't support 64-bit type
    
/*	----------------------------------------------------------------------------
    avr-gcc types
    --------------------------------------------------------------------------*/

    typedef unsigned char		byte;
    typedef unsigned char		BOOL;//bool;					// not compatible with c++
    typedef unsigned char		boolean;	

    typedef unsigned int		word;	

    typedef unsigned long 		dword;

#endif
