/*	----------------------------------------------------------------------------
    ---------- const.h
    ---------- rblanchot@gmail.com
    --------------------------------------------------------------------------**/

#ifndef __CONST_H
    #define __CONST_H

    #define PINGUINO_MAJOR_VERSION 11
    #define PINGUINO_MINOR_VERSION 1

    // already defined in math.h
    //#define PI        3.1415926535897932384626433832795
    //#define HALF_PI   1.5707963267948966192313216916398
    //#define TWO_PI    6.2831853071795864769252867665590
    //#define SQR_PI    9.8696044010893586188344909998761
    #define DEG_TO_RAD  0.0174532925199432957692369076848  // PI/180
    #define RAD_TO_DEG  57.295779513082320876798154814105  // 180/PI
    #define rad(x)      (x * DEG_TO_RAD)
    #define deg(x)      (x * RAD_TO_DEG)
     
    // already defined in common_types.h
    #ifndef FALSE
        #define FALSE		0
    #endif
    #define false			0 
    #define False			0 
    
    #ifndef TRUE
        #define TRUE		!FALSE
    #endif
    #define true			!false
    #define True			!false

    #ifndef NULL
      #define NULL          (void *)0
    #endif

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

