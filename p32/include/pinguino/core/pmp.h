/*    ----------------------------------------------------------------------------
    FILE:           pmp.h
    PROJECT:        pinguino
    PURPOSE:        PIC32 Parallel Master Port functions
    PROGRAMER:      Regis Blanchot
    FIRST RELEASE:  07 Apr. 2012
    LAST RELEASE:   26 Mar. 201'
    ----------------------------------------------------------------------------
    CHANGELOG : 

    Apr  07 2012  - initial release
    ----------------------------------------------------------------------------
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    --------------------------------------------------------------------------*/

//http://www.ps3devwiki.com/files/flash/Tools/PIC32MX/D32_v1_5/MemInterface.c

#ifndef __PMP_H
#define __PMP_H

#include <typedef.h>

// 1 = Port is busy
// 0 = Port is not busy
//#define PMP_busy()                (PMMODE & 0x8000)
#define PMP_wait()                while(PMMODEbits.BUSY)
//#define PMP_wait()                  while(PMMODE & 0x8000)

#define PMP_MODE_IRQ_OFF            0b00
#define PMP_MODE_BUF_IRQ            0b10
#define PMP_MODE_CY_IRQ             0b01

#define PMP_MODE_INC_BUFFER         0b11
#define PMP_MODE_DEC_ADDR           0b10
#define PMP_MODE_INC_ADDR           0b01
#define PMP_MODE_INC_OFF            0b00

#define    PMP_MODE_8BIT            0        // PMP<15:8> not controlled by the PMP module
#define    PMP_MODE_16BIT           1

#define PMP_MODE_MASTER1            0b11    // Read & Write on same pin
#define PMP_MODE_MASTER2            0b10    // Read & Write on separate pins
#define PMP_MODE_ESLAVE             0b01    // Enhanced
#define PMP_MODE_SLAVE              0b00    // Legacy

#if defined(PINGUINO32MX250) || defined(PINGUINO32MX270) || defined(PINGUINO32MX220)
    #define PMP_MUX_8X8             0b11    // Lower 8 bits of address are multiplexed on PMD<7:0> pins; upper 8 bits are not used
    #define PMP_MUX_16X8            0b10    // All 16 bits of address are multiplexed on PMD<7:0> pins
    #define PMP_MUX_16X16           0b01    // Lower 8 bits of address are multiplexed on PMD<7:0> pins, upper bits are on PMA<10:8> and PMA<14>
    #define PMP_MUX_OFF             0b00    // Address and data appear on separate pins
#else
    #define PMP_MUX_16X16           0b11    // Fully multiplexed (16 data pins PMD<15:0>)
    #define PMP_MUX_16X8            0b10    // Fully multiplexed (lower eight data pins PMD<7:0>)
    #define PMP_MUX_8X16            0b01    // Partially multiplexed (lower eight data pins PMD<7:0>)
    #define PMP_MUX_OFF             0b00    // Demultiplexed (separate data and address lines)
#endif

#if defined(PINGUINO32MX250) || defined(PINGUINO32MX270) || defined(PINGUINO32MX220)
    #define PMP_CS1                 0b10    // PMCS1 as CS
    #define PMP_CS_OFF              0b00    // PMCS1 as bit address
#else
    #define PMP_CS1CS2              0b10    // PMCS1 and PMCS2 function as Chip Select
    #define PMP_CS2                 0b01    // PMCS2 as CS, PMCS1 as address bit 14
    #define PMP_CS_OFF              0b00    // PMCS1 and PMCS2 function as address bit 14 and 15
#endif

#define PMP_ACTIVE_LOW              0
#define PMP_ACTIVE_HIGH             1

#define PMA0                        1<<0
#define PMA1                        1<<1
#define PMA2                        1<<2
#define PMA3                        1<<3
#define PMA4                        1<<4
#define PMA5                        1<<5
#define PMA6                        1<<6
#define PMA7                        1<<7
#define PMA8                        1<<8
#define PMA9                        1<<9
#define PMA10                       1<<10
#define PMA11                       1<<11
#define PMA12                       1<<12
#define PMA13                       1<<13
#define PMA14                       1<<14
#define PMA15                       1<<15

#define PMCS                       1<<14    // PMA14 or PMCS
#define PMCS1                      1<<14    // PMA14 or PMCS1
#define PMCS2                      1<<15    // PMA15 or PMCS2
#define PMRD                       1<<13    // Master 2 (arbitrary value)
#define PMRD_PMWR                  1<<13    // Master 1 (arbitrary value)
#define PMWR                       1<<12    // Master 2 (arbitrary value)
#define PMENB                      1<<12    // Master 1 (arbitrary value)
#define PMBE                       1<<11    // Master 1 (arbitrary value)

/** --------------------------------------------------------------------
    Prototypes
    ------------------------------------------------------------------*/

void PMP_setMode(u8);
void PMP_setAddress(u16);
void PMP_setControl(u16);
void PMP_setPolarity(u8);
void PMP_setMux(u8);
void PMP_setWidth(u8);
void PMP_setWaitStates(u8 beg, u8 mid, u8 end);
void PMP_autoIncrement(s8 inc);
void PMP_init();
void PMP_sendAddress(u16);
void PMP_write(u16);
u16  PMP_read();

#endif /* __PMP_H */
