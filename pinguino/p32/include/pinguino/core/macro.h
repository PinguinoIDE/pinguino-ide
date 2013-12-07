/*	----------------------------------------------------------------------------
	macro.h
	Régis Blanchot
	--------------------------------------------------------------------------*/

#ifndef __MACRO_H
#define __MACRO_H

	/// ASM

	#define interrupts()	asm("ei")
	#define noInterrupts()	asm("di")
	#define nop()			asm("nop")

	/// BYTES

	#define low8(x)				((unsigned char) ((x) & 0xFF))
	#define high8(x)			((unsigned char) ((x) >> 8))
	// already defined in pinguino.pdl32
	//#define lowByte(x)			((unsigned char) ((x) & 0xFF))
	//#define highByte(x)			((unsigned char) ((x) >> 8))
	#define make16(low, high)	(low | (high << 8))
	#define make32(low, high)	(low | (high << 16))

	/// BITWISE OPERATION

	// Lire le bit n de octet
	#define BitRead(byte, n) \
		(byte = byte >> n & 1)

	// Met le bit n à 1 dans octet
	#define BitSet(byte, n) \
		(byte |= (1 << n))

	// Met le bit n à 0 dans octet
	#define BitClear(byte, n) \
		(byte = byte & !(1 << n))

	#define BitWrite(value, bit, bitvalue) \
		(bitvalue ? bitSet(value, bit) : bitClear(value, bit))
	
	#define Bit(b) \
		(1 << (b))

	#define BitTest(byte,n) \
	    ((byte&(1<<n))!=0)

	/// MATH

	#define min(a,b) ((a)<(b)?(a):(b))
	#define max(a,b) ((a)>(b)?(a):(b))
	#define abs(x) ((x)>0?(x):-(x))
	#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
	#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
	#define radians(deg) ((deg)*DEG_TO_RAD)
	#define degrees(rad) ((rad)*RAD_TO_DEG)
	#define sq(x) ((x)*(x))

#endif	/* __MACRO_H */

