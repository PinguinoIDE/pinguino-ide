/*	----------------------------------------------------------------------------
	FILE:			spi.c
	PROJECT:		pinguino
	PURPOSE:		Serial Peripheral Interface functions
	PROGRAMER:		Régis Blanchot <rblanchot@gmail.com>
					Marcus Fazzi <anunakin@gmail.com>
					Jean-Pierre Mandon <jp.mandon@gmail.com>
	FIRST RELEASE:	16 Mar 2011
	LAST RELEASE:	28 May 2012
	----------------------------------------------------------------------------
	CHANGELOG : 
	24 May 2011 - jp.mandon - fixed a bug in SPI_write, RX int flag must be called even for write
	20 Feb 2012 - r.blanchot - added PIC32_PINGUINO_220 support
	28 May 2012 - MFH - 	added PIC32_PINGUINO_MICRO support and fixed a bug
								in SPI_clock() identified by dk (KiloOne)
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
 *	Microchip making changes do SPI SFR numbers and changing 2A to 3, and 3A to 4
 * but it need a new processor.o and updated compiler libraries, not released at this time.
 *  
 *  MX440 and MX460 have only 2 SPI ports
 */
#ifndef __SPI__
#define __SPI__

#include <system.c>
#include <interrupt.c>

#ifndef SPIx				// Use SPI port 1, see PIC32 Datasheet
	#if defined(PIC32_PINGUINO_OTG) || defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_MICRO)  //dk MICRO added
		#define SPIx 2		// default SPI port is 2 for 32MX440F256H which has only one SPI port
	#elif defined(PIC32_PINGUINO_T795) //Olimex PIC32-T795 default on header
		#define SPIx 3		// default SPI port is 2 for 32MX440F256H which has only one SPI port
	#else
		#define SPIx 1		// default SPI port is 1
	#endif
#endif

#if defined(UBW32_460) || defined(EMPEROR460) || defined(PIC32_PINGUINO_220) || defined(PINGUINO32MX250) || defined(PINGUINO32MX220)
#if (SPIx == 1)
	#define BUFFER		SPI1BUF
	#define STATUS		SPI1STATbits.SPIROV	// Receive Overflow Flag bit
	#define STATRX  	SPI1STATbits.SPIRBF	// receive buffer full
	#define STATTX		SPI1STATbits.SPITBF	// transmit buffer full
	#define SPICONF		SPI1CON
	#define SPICONCLR	SPI1CONCLR
	#define CLKSPD		SPI1BRG
	#define INTFAULT	INT_SPI1_FAULT
	#define INTTXDONE 	INT_SPI1_TRANSFER_DONE
	#define INTRXDONE 	INT_SPI1_RECEIVE_DONE
	#define INTVECTOR 	INT_SPI1_VECTOR
#endif
#endif

#if (SPIx == 2)
	#define BUFFER		SPI2BUF
	#define STATUS		SPI2STATbits.SPIROV	// Receive Overflow Flag bit
	#define STATRX  	SPI2STATbits.SPIRBF	// Receive buffer full
	#define STATTX		SPI2STATbits.SPITBF	// Transmit buffer full
	#define SPICONF		SPI2CON
	#define SPICONCLR	SPI2CONCLR
	#define SPIENHBUF	SPI2CONbits.ENHBUF
	#define CLKSPD		SPI2BRG
	#define PULLUPS		0xF00 //Use CNPUE = PULLUPS for enable internal pullups 8,9,10,11
	#define INTFAULT	INT_SPI2_FAULT
	#define INTTXDONE 	INT_SPI2_TRANSFER_DONE
	#define INTRXDONE 	INT_SPI2_RECEIVE_DONE
	#define INTVECTOR 	INT_SPI2_VECTOR
#endif

//Only 795 boards have SPI3 and SPI4
#if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
	#if (SPIx == 3)
		#define BUFFER		SPI3BUF
		#define STATUS		SPI3STATbits.SPIROV	// Receive Overflow Flag bit
		#define STATRX  	SPI3STATbits.SPIRBF	// receive buffer full
		#define STATTX		SPI3STATbits.SPITBF	// transmit buffer full
		#define SPICONF		SPI3CON
		#define SPICONCLR	SPI3CONCLR
		#define CLKSPD		SPI3BRG
		#define INTFAULT	INT_SPI3_FAULT
		#define INTTXDONE 	INT_SPI3_TRANSFER_DONE
		#define INTRXDONE 	INT_SPI3_RECEIVE_DONE
		#define INTVECTOR 	INT_SPI3_VECTOR
	#endif
	#if (SPIx == 4)
		#define BUFFER		SPI4ABUF
		#define STATUS		SPI4STATbits.SPIROV	// Receive Overflow Flag bit
		#define STATRX  	SPI4STATbits.SPIRBF	// receive buffer full
		#define STATTX		SPI4STATbits.SPITBF	// transmit buffer full
		#define SPICONF		SPI4CON
		#define SPICONCLR	SPI4CONCLR
		#define CLKSPD		SPI4BRG
		#define INTFAULT	INT_SPI4_FAULT
		#define INTTXDONE 	INT_SPI4_TRANSFER_DONE
		#define INTRXDONE 	INT_SPI4_RECEIVE_DONE
		#define INTVECTOR 	INT_SPI4_VECTOR
	#endif
#endif

// The SPI module offers the following operating modes:
// • 8-Bit, 16-Bit, and 32-bit data transmission modes
// • 8-Bit, 16-Bit, and 32-bit data reception modes
// • Master and Slave modes
// • Framed SPI modes

// SPIxCON.MSTEN
#define SPI_MASTER			1
#define SPI_SLAVE			0
#define SPI_PBCLOCK_DIV2	2
#define SPI_PBCLOCK_DIV4	4
#define SPI_PBCLOCK_DIV8	8
#define SPI_PBCLOCK_DIV16	16
#define SPI_PBCLOCK_DIV32	32
#define SPI_PBCLOCK_DIV64	64

void SPI_mode(u8 mode)
{
	// 7. Clear the SPIROV bit (SPIxSTAT<6>).
	STATUS = 0;					// clear the Overflow

	// 8. Write the desired settings to the SPIxCON register with MSTEN (SPIxCON<5>) = 1.
	// 9. Enable SPI operation by setting the ON bit (SPIxCON<15>).
	if (mode == SPI_MASTER)
		SPICONF=0x8120;		// or 0x8220 SPI ON, 8 bits transfer, SMP=1, Master mode
	if (mode == SPI_SLAVE)
		SPICONF=0x8000;		// SPI ON, 8 bits transfer, Slave mode
}

// Fsck = Fpb / (2 * (SPIxBRG + 1)
// SPIxBRG = (Fpb / (2 * Fsck)) - 1 
// speed must be in bauds
void SPI_clock(u32 speed)
{
	u32 Fpb;
	//u32 Fsck;
	u16 clk;

	Fpb = GetPeripheralClock();
	if (speed > (Fpb / 2))
	{
		CLKSPD = 0;			// use the maximum baud rate possible
		return;
	}
	else
	{
		clk = (Fpb / (2 * speed)) - 1;
		if (clk > 511)
			CLKSPD = 511;	// use the minimum baud rate possible
		else					// ** fix for bug identified by dk=KiloOne
			CLKSPD = clk;	// use calculated divider-baud rate
	}
}

void SPI_close() //(u8 num)
{
	unsigned char rData;
	// Disable all SPIx interrupts
	IntDisable(INTFAULT); 
	IntDisable(INTTXDONE); 
	IntDisable(INTRXDONE);
	// Stops and resets the SPIx
	SPICONF=0;
	// clears the receive buffer
	rData=BUFFER;
}

/*	-----------------------------------------------------------------------------
	The following code assumes that none of the SPIx input pins are shared with
	an analog input. If so, the AD1PCFG and corresponding TRIS registers have to 
	be properly configured.

	Perform the following steps to set up the SPI module for the Master mode operation:
	1.  Disable the SPI interrupts in the respective IEC0/1 register.
	2.  Stop and reset the SPI module by clearing the ON bit.
	3.  Clear the receive buffer.
	4.  Clear the ENHBUF bit (SPIxCON<16>) if using Standard Buffer mode or set the bit if using
		 Enhanced Buffer mode.
	5. If SPI interrupts are not going to be used, skip this step and continue to step 5. Otherwise
		 the following additional steps are performed:
		 a) Clear the SPIx interrupt flags/events in the respective IFS0/1 register.
		 b) Set the SPIx interrupt enable bits in the respective IEC0/1 register.
		 c) Write the SPIx interrupt priority and subpriority bits in the respective IPC5/7 register.
	6. Write the Baud Rate register, SPIxBRG.
	7. Clear the SPIROV bit (SPIxSTAT<6>).
	8. Write the desired settings to the SPIxCON register with MSTEN (SPIxCON<5>) = 1.
	9. Enable SPI operation by setting the ON bit (SPIxCON<15>).
	10. Write the data to be transmitted to the SPIxBUF register. Transmission (and reception) will
		 start as soon as data is written to the SPIxBUF register.
	---------------------------------------------------------------------------*/

void SPI_init()
{
	unsigned char rData;

	// 1.  Disable the SPI interrupts in the respective IEC0/1 register.
	IntDisable(INTFAULT); 
	IntDisable(INTTXDONE); 
	IntDisable(INTRXDONE);

	// 2.  Stop and reset the SPI module by clearing the ON bit.
	SPICONCLR = 0x8000; // bit 15

	// 3.  Clear the receive buffer.
	rData = BUFFER;

	// 4.  Clear the ENHBUF bit (SPIxCON<16>) if using Standard Buffer mode.
	// This bit can only be written when the ON bit = 0
	// SPIENHBUF = 0; // not available on all devices

	// 5. If SPI interrupts are not going to be used, skip this step and
	// continue to step 6. Otherwise the following additional steps are performed:
	//	 a) Clear the SPIx interrupt flags/events in the respective IFS0/1 register.
/*
	IntClearFlag(INTFAULT);
	IntClearFlag(INTTXDONE);
	IntClearFlag(INTRXDONE);
	//	 b) Set the SPIx interrupt enable bits in the respective IEC0/1 register.
	IntEnable(INTFAULT); 
	IntEnable(INTTXDONE); 
	IntEnable(INTRXDONE);
	//	 c) Write the SPIx interrupt priority and subpriority bits in the respective IPC5/7 register.
	IntSetVectorPriority(INTVECTOR, 3, 1);
*/
	// 6. Write the Baud Rate register, SPIxBRG.
	SPI_clock(250000); // 250 kbps (low data rate) or could be also 625000
	SPI_mode(SPI_MASTER);
}

// 10. Write the data to be transmitted to the SPIxBUF register.
// Transmission (and reception) will start as soon as data is written to the SPIxBUF register.
unsigned char SPI_write(unsigned char data_out)
{
	BUFFER = data_out;	// write to buffer for TX
	while (!STATRX);		// wait for the receive flag (transfer complete)
	return BUFFER;
}

unsigned char SPI_read(void)
{
	// return SPI_write(0);
	BUFFER = 0x00;			// dummy byte to capture the response
	while (!STATRX);		// wait until cycle complete
	return BUFFER;			// return with byte read
}

/*	----------------------------------------------------------------------------
	SPIInterrupt
	TODO: move this to interrupt library and add it to main32.c ?
	--------------------------------------------------------------------------*/

void SPIxInterrupt(void)
{
	unsigned char rData;

	// Is this an RX interrupt ?
	if (IntGetFlag(INTRXDONE))
	{
		rData = BUFFER;			// Read SPI data buffer
		IntClearFlag(INTRXDONE);
	}
	// Is this an TX interrupt ?
	if (IntGetFlag(INTTXDONE))
	{
		IntClearFlag(INTTXDONE);
	}
}

#endif	/* __SPI__ */
