/*	----------------------------------------------------------------------------
	---------- const.h
	---------- rblanchot@gmail.com
	--------------------------------------------------------------------------**/

#ifndef __CONST_H
    #define __CONST_H

	// already defined in math.h
	//#define PI				3.1415926535897932384626433832795
	//#define HALF_PI		1.5707963267948966192313216916398
	//#define TWO_PI			6.283185307179586476925286766559
	#define DEG_TO_RAD	0.017453292519943295769236907684886
	#define RAD_TO_DEG	57.295779513082320876798154814105

	// already defined in common_types.h
    #ifndef FALSE
	#define FALSE			0
    #endif
	#define false			0 
	#define False			0 
    
    #ifndef TRUE
	#define TRUE			!FALSE
    #endif
	#define true			!false
	#define True			!false

	#define INPUT			1
	#define OUTPUT			0

	#define HIGH			1
	#define LOW				0

	#define ON				1
	#define OFF				0

	#define BYTE			1
	#define BIN				2
	#define OCT				8 
	#define DEC				10
	#define HEX				16
	#define FLOAT			32

#endif

