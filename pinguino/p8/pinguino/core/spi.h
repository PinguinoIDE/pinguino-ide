/**
 * SPI Hardware library for pinguino project
 *
 * Author: Francisco J. Sánchez Rivas (FJ_Sanchez)
 * fran [at] mipixel [dot] com
 *
 * http://www.mipixel.com
 *
 */
 
#ifndef __SPI_H__
#define __SPI_H__

// pins
#if defined(__18f2455) || defined(__18f4455) || \
    defined(__18f2550) || defined(__18f4550)
    #define SD_CS			PORTAbits.RA5		// Chip Select
    #define SSPIN		TRISAbits.TRISA5	// Chip Select TRIS
    #define SDIPIN   	TRISBbits.TRISB0	// SDI Master input/SDO Slave output TRIS
    #define SCKPIN   	TRISBbits.TRISB1	// SCK Clock TRIS
    #define SDOPIN  	TRISCbits.TRISC7	// SDO Master output/SDI Slave input TRIS
    #define BUFFER          SSPBUF
    #define CONFIG          SSPCON1
    #define STATUS          SSPSTAT
    #define WCOL            SSPCON1bits.WCOL
	#define CKP             SSPCON1bits.CKP
	#define CKE             SSPSTATbits.CKE
    #define SSPIE           PIE1bits.SSPIE
    #define FLAG            PIR1bits.SSPIF
	#define SSPHPE          IPR1bits.SSPIP
    #define ENABLE          SSPCON1bits.SSPEN
    
	
#elif defined(__18f26j50)|| defined(__18f46j50) || \
      defined(__18f27j53)|| defined(__18f47j53)
// Pinguinos 18F26J50 / 18F47J53  uses SPI2 (cf. io.c)
// because SPI1 share same pins as UART1
    #define SD_CS			PORTBbits.RB0		// SPI2 Chip Select
    #define SSPIN		    TRISBbits.TRISB0	// SPI2 Chip Select TRIS
    #define SDIPIN      	TRISBbits.TRISB3	// SPI2 SDO Master input/Slave output TRIS
    #define SCKPIN      	TRISBbits.TRISB2	// SPI2 SCK Clock TRIS
    #define SDOPIN      	TRISBbits.TRISB1	// SPI2 SDI Master output/Slave input TRIS
    #define BUFFER          SSP2BUF
    #define CONFIG          SSP2CON1
    #define STATUS          SSP2STAT
    #define WCOL            SSP2CON1bits.WCOL
	#define CKP             SSP2CON1bits.CKP
	#define CKE             SSP2STATbits.CKE
    #define SSPIE           PIE3bits.SSP2IE
    #define FLAG            PIR3bits.SSP2IF
	#define SSPHPE          IPR3bits.SSP2IP
    #define ENABLE          SSP2CON1bits.SSPEN

#else
    #error "This library is only for Pinguino x455, x550, x6j50 or x7j53"
#endif
/*	----------------------------------------------------------------------------
	---------- *** MODES ***
	----------------------------------------------------------------------------
	SSPM<3:0>: Synchronous Serial Port Mode Select bits
	0000 = SPI Master mode, clock = FOSC/4
	0001 = SPI Master mode, clock = FOSC/16
	0010 = SPI Master mode, clock = FOSC/64
	0011 = SPI Master mode, clock = TMR2 output/2
	0100 = SPI Slave mode, clock = SCK pin, SS pin control enabled
	0101 = SPI Slave mode, clock = SCK pin, SS pin control disabled, SS can be used as I/O pin
	---------------------------------------------------------------------------*/

#define SPI_MASTER			0x00
#define SPI_MASTER_FOSC_4	0x00
#define SPI_MASTER_FOSC_16	0x01
#define SPI_MASTER_FOSC_64	0x02
#define SPI_MASTER_FOSC_T2	0x03
#define SPI_SLAVE_SS		0x04
#define SPI_SLAVE			0x05

#define SPI_CLOCK_DIV4		0x00
#define SPI_CLOCK_DIV16		0x01
#define SPI_CLOCK_DIV64		0x02
#define SPI_CLOCK_TIMER2	0x03

#define SPI_MODE0			0x00
#define SPI_MODE1			0x04
#define SPI_MODE2			0x08
#define SPI_MODE3			0x0C


#define SPI_FALLING_EDGE	0x00  // negated
#define SPI_RISING_EDGE		0x01  // negated

#endif   /* __SPI_H__ */
