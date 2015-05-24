/*      ----------------------------------------------------------------------------
        FILE:                   const.h
        PROJECT:                pinguino - http://www.pinguino.cc/
        PURPOSE:                definition of constants
        PROGRAMERS:             regis blanchot <rblanchot@gmail.com>
        FIRST RELEASE:          2010
        ----------------------------------------------------------------------------
        CHANGELOG:
        Sept 2011 - Marcus Fazzi - UBW32 and Emperor board revised
        17 mar. 2012 [hgmvanbeek@gmail.com] - added support for PIC32_PINGUINO_MICRO
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

#ifndef __CONST_H
    #define __CONST_H

    #define PINGUINO_MAJOR_VERSION 11
    #define PINGUINO_MINOR_VERSION 1

    // Tell the compiler the next function must be MIPS32.
    // Typically needed by all functions such as interrupt handlers
    // which cannot be MIPS16 functions when -mips16 option is enabled.
    // Usage : void MIPS32 myfunction(...)
    #define MIPS32          __attribute__((noinline,nomips16))

    #ifndef PI
        #define PI          3.1415926535897932384626433832795
    #endif

    #ifndef HALF_PI
        #define HALF_PI     1.5707963267948966192313216916398
    #endif

    #ifndef TWO_PI
        #define TWO_PI      6.283185307179586476925286766559
    #endif

    #ifndef DEG_TO_RAD
        #define DEG_TO_RAD  0.017453292519943295769236907684886
    #endif

    #ifndef RAD_TO_DEG
        #define RAD_TO_DEG  57.295779513082320876798154814105
    #endif

    #ifndef NULL
        #define NULL        0
    #endif

    #ifndef ON
        #define ON          1
    #endif

    #ifndef OFF
        #define OFF         0
    #endif

    #ifndef FALSE
        #define FALSE       0
    #endif

    #ifndef TRUE
        #define TRUE        !FALSE
    #endif

    //lower case

    #ifndef false
        #define false       0
    #endif

    #ifndef true 
        #define true        !false
    #endif

    #ifndef False
        #define False       0
    #endif

    #ifndef True 
        #define True        !False
    #endif

    #ifndef INPUT
        #define INPUT       1
    #endif

    #ifndef OUTPUT
        #define OUTPUT      0
    #endif

    #ifndef HIGH
        #define HIGH        1
    #endif

    #ifndef LOW
        #define LOW         0
    #endif
    
    //Booleans

    #ifndef AND
        #define AND         &&
    #endif

    #ifndef OR 
        #define OR          ||
    #endif

    //lower case (not compatible with c++)
    //#define and		&&
    //#define or		||	

    #ifndef BYTE
        #define BYTE        1
    #endif

    #ifndef BIN
        #define BIN         2
    #endif

    #ifndef OCT
        #define OCT         8 
    #endif

    #ifndef DEC
        #define DEC         10
    #endif

    #ifndef HEX
        #define HEX         16
    #endif

    #ifndef FLOAT
        #define FLOAT       32
    #endif

#endif /* __CONST_H */
