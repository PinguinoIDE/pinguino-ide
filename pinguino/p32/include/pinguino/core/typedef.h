/*	----------------------------------------------------------------------------
	typedef.c
	Régis Blanchot
	--------------------------------------------------------------------------*/

#ifndef __TYPEDEF_H
#define __TYPEDEF_H

/*	----------------------------------------------------------------------------
	pinguino types
	--------------------------------------------------------------------------*/

	typedef signed char				s8;
	typedef signed short int		s16;
	typedef signed long	int			s32;
	typedef signed long long 		s64;

	typedef unsigned char			u8;
	typedef unsigned short int		u16;
	typedef unsigned long int		u32;
	typedef unsigned long long 		u64;
	
/*	----------------------------------------------------------------------------
	gcc types
	--------------------------------------------------------------------------*/

	//#include <types.h>

	typedef unsigned char			byte;
	typedef unsigned char			BYTE;

	typedef unsigned char			BOOL;//bool;					// not compatible with c++
	typedef unsigned char			boolean;	

	typedef signed char 			int8_t;
	typedef short int				int16_t;
	typedef long int				int32_t;
	typedef long long 				int64_t;

	typedef unsigned char 			uint8_t;
	typedef unsigned short int		uint16_t;
	typedef unsigned long int		uint32_t;
	typedef unsigned long long 		uint64_t;

	// 8 bits
	typedef unsigned char			uchar;
	typedef signed char				schar;
	typedef unsigned char			UCHAR;
	typedef signed char				CHAR;

	// 16 bits
	typedef short int				INT;
	typedef unsigned short int		UINT;
/*
 * 2012-04-28 Regis Blanchot - types.h: error: conflicting types
 */ 
//	typedef unsigned short int		uint;
	typedef signed short int		sint;
	typedef unsigned short int		word;
	typedef short int				SHORT;
	typedef unsigned short int		USHORT;
	typedef unsigned short int		WORD;
	typedef unsigned short int		WCHAR;

	// 32 bits
//	typedef unsigned long int		ulong;
	typedef unsigned long int		ULONG;
	typedef signed long int			slong;
	typedef unsigned long int		dword; 
	typedef unsigned long int		DWORD;
	typedef long int				LONG;

#endif	/* __TYPEDEF_H */
