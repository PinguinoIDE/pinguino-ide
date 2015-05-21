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

// SPI modules
#define SPISW   0
#define SPI1    1
#define SPI2    2

// pins
#if defined(__18f2455)  || defined(__18f4455)  || \
    defined(__18f2550)  || defined(__18f4550)
    
    #define SD_CS           PORTAbits.RA5       // Chip Select
    #define SSPIN           TRISAbits.TRISA5    // Chip Select TRIS
    #define SDIPIN          TRISBbits.TRISB0    // SDI Master input/SDO Slave output TRIS
    #define SCKPIN          TRISBbits.TRISB1    // SCK Clock TRIS
    #define SDOPIN          TRISCbits.TRISC7    // SDO Master output/SDI Slave input TRIS
    
#elif defined(__18f25k50) || defined(__18f45k50)

    #define SD_CS           PORTAbits.RA5       // Chip Select
    #define SSPIN           TRISAbits.TRISA5    // Chip Select TRIS
    #define SDIPIN          TRISBbits.TRISB0    // SDI Master input/SDO Slave output TRIS
    #define SCKPIN          TRISBbits.TRISB1    // SCK Clock TRIS
    /// RB 2014-02-07 : C7 (could be RC7 but if CONFIG3H.SDOMX = 0)
    #define SDOPIN          TRISBbits.TRISB3    // SDO Master output/SDI Slave input TRIS
    
#elif defined(__18f26j50)|| defined(__18f46j50) || \
      defined(__18f27j53)|| defined(__18f47j53)

    // SPI1 (!!! share the same pins as UART1 !!!)
    #define SD_CS           PORTBbits.RB0       // SPI2 Chip Select
    #define SSPIN           TRISBbits.TRISB0    // SPI2 Chip Select TRIS
    #define SDIPIN          TRISBbits.TRISB3    // SPI2 SDO Master input/Slave output TRIS
    #define SCKPIN          TRISBbits.TRISB2    // SPI2 SCK Clock TRIS
    #define SDOPIN          TRISBbits.TRISB1    // SPI2 SDI Master output/Slave input TRIS

    // SPI2
    #define SD_CS2          PORTBbits.RB0       // SPI2 Chip Select
    #define SS2PIN          TRISBbits.TRISB0    // SPI2 Chip Select TRIS
    #define SDI2PIN         TRISBbits.TRISB3    // SPI2 SDO Master input/Slave output TRIS
    #define SCK2PIN         TRISBbits.TRISB2    // SPI2 SCK Clock TRIS
    #define SDO2PIN         TRISBbits.TRISB1    // SPI2 SDI Master output/Slave input TRIS

#else

    #error "This library is only for Pinguino x455, x550, x6j50 or x7j53"

#endif

/*  ----------------------------------------------------------------------------
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

#define SPI_MASTER          0b0000
#define SPI_MASTER_FOSC_4   0b0000
#define SPI_MASTER_FOSC_8   0b1010  // xxj5x only
#define SPI_MASTER_FOSC_16  0b0001
#define SPI_MASTER_FOSC_64  0b0010
#define SPI_MASTER_FOSC_T2  0b0011
#define SPI_SLAVE_SS        0b0100
#define SPI_SLAVE           0b0101

#define SPI_CLOCK_DIV4      SPI_MASTER_FOSC_4
#define SPI_CLOCK_DIV8      SPI_MASTER_FOSC_8
#define SPI_CLOCK_DIV16     SPI_MASTER_FOSC_16
#define SPI_CLOCK_DIV64     SPI_MASTER_FOSC_64
#define SPI_CLOCK_TIMER2    SPI_MASTER_FOSC_T2

#define SPI_MODE0           0x00
#define SPI_MODE1           0x04
#define SPI_MODE2           0x08
#define SPI_MODE3           0x0C

#define SPI_FALLING_EDGE    0x00  // negated
#define SPI_RISING_EDGE     0x01  // negated

#define SPI_SMPEND          1<<7

#define SPI_LSBFIRST        0
#define SPI_MSBFIRST        1

/// Typedef

typedef struct
{
    u8  mode;
    u32 divider;
    u8  role;
    u8  bitorder;
    u8  phase;
    u8  sda;
    u8  sck;
    u8  cs;
} spi_t;

/// Prototypes

void SPI_begin(u8 module);
void SPI_init(u8 module, u8 sync_mode, u8 bus_mode, u8 smp_phase);
void SPI_setMode(u8 module, u8 mode);
void SPI_setBitOrder(u8 module, u8 bitorder);
void SPI_setDataMode(u8 module, u8 mode);
void SPI_setClockDivider(u8 module, u8 clock);
u8 SPI_write(u8 module, u8 datax);
u8 SPI_read(u8 module);
void SPI1_interrupt();
void SPI2_interrupt();
static void SPI1_onEvent (u8(*func)(u8));
static void SPI2_onEvent (u8(*func)(u8));
static u8 (*SPI1_onEvent_func)( u8);
static u8 (*SPI2_onEvent_func)( u8);

/// Globals

#if defined(__18f26j50)|| defined(__18f46j50) || \
    defined(__18f27j53)|| defined(__18f47j53)
    
    #define NUMOFSPI 3

#else

    #define NUMOFSPI 2

#endif

spi_t SPI[NUMOFSPI];

#endif   /* __SPI_H__ */
