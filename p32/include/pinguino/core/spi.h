/*	----------------------------------------------------------------------------
    FILE:			spi.h
    PROJECT:		pinguino
    PURPOSE:		Serial Peripheral Interface functions
    PROGRAMER:		Régis Blanchot <rblanchot@gmail.com>
    FIRST RELEASE:	14 Apr 2015
    LAST RELEASE:	14 Apr 2015
    ----------------------------------------------------------------------------
    CHANGELOG : 
    15 Apr 2015 - rblanchot  -  created from spi.c
    15 Apr 2015 - rblanchot  -  added SPI structure
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

/**
 *  MX795 are the only boards with SPI3 and SPI4
 *  all others have only 2 SPI ports
 */
 
#ifndef __SPI_H
#define __SPI_H

#include <typedef.h>

// SPI modules
#define SPISW   0
#define SPI1    1
#define SPI2    2
#define SPI3    3
#define SPI4    4

// Modes
#define SPI_SLAVE			0
#define SPI_MASTER			1
#define SPI_MASTER8			1
#define SPI_MASTER16		2
#define SPI_MASTER32		3

#define SPI_LSBFIRST        0
#define SPI_MSBFIRST        1

#define SPI_SMPMID          0
#define SPI_SMPEND          1

#define SPI_PBCLOCK_DIV2	2
#define SPI_PBCLOCK_DIV4	4
#define SPI_PBCLOCK_DIV8	8
#define SPI_PBCLOCK_DIV16	16
#define SPI_PBCLOCK_DIV32	32
#define SPI_PBCLOCK_DIV64	64
#define SPI_PBCLOCK_DIV128	128
#define SPI_PBCLOCK_DIV256	256
#define SPI_PBCLOCK_DIV512	512
#define SPI_PBCLOCK_DIV1024	1024

#define SPI_MODE0           0
#define SPI_MODE1           1
#define SPI_MODE2           2
#define SPI_MODE3           3

// Typedef
typedef struct
{
    u8  mode;
    u32 divider;
    u8  role;
    u8  bitorder;
    u8  phase;
    u8  sda;
    u8  sck;
} spi_t;

// Prototypes
void SPI_init();
void SPI_setPin(u8 module, u8 sda, u8 sck);
void SPI_select(u8 module);
void SPI_deselect(u8 module);
void SPI_setBitOrder(u8 module, u8 bitorder);
void SPI_setDataMode(u8 module, u8 mode);
void SPI_setMode(u8 module, u8 mode);
u32 SPI_setClock(u8 module, u32 Fspi);
void SPI_setClockDivider(u8 module, u32 divider);
void SPI_close(u8 module);
void SPI_begin(u8 module);
u8 SPI_write(u8 module, u8 data_out);
u8 SPI_read(u8 module);

// Globals
#if defined(__32MX795F512L__) || defined(__32MX795F512H__)
#define NUMOFSPI 5
#else
#define NUMOFSPI 3
#endif

spi_t SPI[NUMOFSPI];

#endif	/* __SPI_H */
