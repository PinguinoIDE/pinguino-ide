#ifndef __SPI_C__
#define __SPI_C__

#ifndef __SPI__
#define __SPI__
#endif

//#define SPIINIT // requested because V11 IDE doesn't generate #define from .pde applications

#include <spi.h>
#include <delayms.c>
#include <digitalp.c>
#include <digitalw.c>

/**
 * This function initializes the SPI hardware configuring polarity and edge
 * of transition using Standard SPI Mode Terminology (datasheet Table 19-1 p206)
 *
 *    Mode0,Mode1 CKP   CKE
 *    0,0         0     1
 *    0,1         0     0
 *    1,0         1     1
 *    1,1         1     0
 *
 * Also is possible to use LOW or HIGH for mode0 to indicate the idle state
 * clock level, and RISING or FALLING to indicate when the transmit should
 * take place.
 *
 * The sync_mode param could be any of SPI_MASTER, MASTER_FOSC_4, MASTER_FOSC_16,
 * MASTER_FOSC_64, SLAVE_SS or SLAVE, where MASTER_FOSC_X indicates the SPI
 * clock speed used. If you want to use the /SS pin in slave mode you should
 * initialize the SPI using SPI_SLAVE_SS.
 sync_mode   description
   0       SPI_MASTER mode, clock = FOSC_4
   1       SPI_MASTER, clock = FOSC_16
   2       SPI_MASTER, clock = FOSC_64
   3       SPI_MASTER, clock = TMR2 output/2
   4       SPI_SLAVE mode, /SS pin enabled
   5       SPI_SLAVE, /SS pin disabled
 * smp_phase = Sample bit
   0  in slave mode
   0  in master mode : Input data sampled at middle of data output time
   1  in master mode : Input data sampled at end of data output time
   Nota bene : enter 128 as decimal value or 0x80 as hexadecimal value for the below smp_phase parameter
 */

/** -----------------------------------------------------------------**/
#ifdef SPIINIT
/** -----------------------------------------------------------------**/

#define SELECT()	SD_CS = 0 // device selection
#define DESELECT()	SD_CS = 1 // it stops device selection
/* Port Controls  (Platform dependent) */
#define LA7 LATBbits.LATB5
#define LA6 LATBbits.LATB4

void SPI_init(u8 sync_mode, u8 bus_mode, u8 smp_phase)
{
    DESELECT();
    SSPCON1bits.SSPEN = 0;
    SSPSTAT &= 0x3F;                // power on state
    SSPSTAT |= smp_phase;           // select data input sample phase
    SSPCON1 = sync_mode;            // select serial mode

    switch( bus_mode )
    {
        case 0:                       // SPI bus mode 0,0
            CKE = 1;        // data transmitted on falling edge
            break;    
        case 2:                       // SPI bus mode 1,0
            CKE = 1;        // data transmitted on rising edge
            CKP = 1;        // clock idle state high
            break;
        case 3:                       // SPI bus mode 1,1
            CKP = 1;        // clock idle state high
            break;
        default:                      // default SPI bus mode 0,1
            break;
    }

    switch( sync_mode )
    {
        case 4:                       // slave mode w /SS enable
            SSPIN = 1;       // define /SS pin as input
            SCKPIN = 1;       // define clock pin as input
            break;

        case 5:                       // slave mode w/o /SS enable
            SCKPIN = 1;       // define clock pin as input
            break;

        default:                      // master mode, define clock pin as output
            SCKPIN = 0;       // define clock pin as input
            break;
    }
    
    SDIPIN = 1;       // define SDI pin as input  
    SDOPIN = 0;       // define SDO pin as output
    SSPCON1bits.SSPEN = 1;
    Delayms(30);

    #ifdef SPIINT
    PIE1bits.SSPIE = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    INTCONbits.PEIE = 1;
    //IPR1bits.SSPIP = 1;
    PIR1bits.SSP1IF = 0;
    #endif
}

/** -----------------------------------------------------------------**/
#else
/** -----------------------------------------------------------------**/

/**
 *  This function init the SPI module to default values
 *  Called from main.c
 */

void spi_init()
{
    u8 i;

    for (i=0; i<NUMOFSPI; i++)
    {
        SPI[i].mode     = SPI_MODE0;
        SPI[i].divider  = SPI_CLOCK_DIV4;
        SPI[i].role     = SPI_MASTER;
        SPI[i].bitorder = SPI_MSBFIRST;
        SPI[i].phase    = SPI_SMPEND;
    }
}

/**
 *  This function set SPI software pins
 */

void SPI_setPin(u8 module, u8 sda, u8 sck)//, u8 cs)
{
    if (module == SPISW)
    {
        SPI[module].sda = sda;
        SPI[module].sck = sck;
        //SPI[module].cs  = cs;
        pinmode(SPI[module].sda, OUTPUT);
        pinmode(SPI[module].sck, OUTPUT);
        //pinmode(SPI[module].cs,  OUTPUT);
    }
}

/**
 *  This function select a SPI module
 */

void SPI_select(u8 module)
{
    switch(module)
    {
        case SPI1:
            SD_CS = LOW;
            digitalwrite(SPI[SPI1].cs, LOW);
            break;

        #if defined(__18f26j50)|| defined(__18f46j50) || \
            defined(__18f27j53)|| defined(__18f47j53)
        case SPI2:
            SD_CS2 = LOW;
            digitalwrite(SPI[SPI2].cs, LOW);
            break;
        #endif
    }
}

/**
 *  This function deselect a SPI module
 */

void SPI_deselect(u8 module)
{
    switch(module)
    {
        case SPI1:
            SD_CS = HIGH;
            digitalwrite(SPI[SPI1].cs, HIGH);
            break;

        #if defined(__18f26j50)|| defined(__18f46j50) || \
            defined(__18f27j53)|| defined(__18f47j53)
        case SPI2:
            SD_CS2 = HIGH;
            digitalwrite(SPI[SPI2].cs, HIGH);
            break;
        #endif
    }
}

//#ifdef SPIBEGIN
void SPI_begin(u8 module)
{
    SPI_deselect(module);
    
    switch(module)
    {
        case SPI1:
            SSPCON1bits.SSPEN = 0;
            SSPSTAT = 0x3F;                 // power on state (SMP=0) 
            SSPCON1 = SPI[SPI1].divider;      // select serial mode

            if (SPI[SPI1].phase == SPI_SMPEND)
            {
                SSPSTATbits.SMP = 1;         // input data sampled at end of data output time
            }

            switch(SPI[SPI1].mode)
            {
                case SPI_MODE0:              // SPI bus mode 0,0
                  SSPCON1bits.CKP = 0;       // clock idle state low
                  SSPSTATbits.CKE = 0;       // data transmitted on falling edge
                  break;    
                case SPI_MODE1:              // default SPI bus mode 0,1
                  SSPCON1bits.CKP = 0;       // clock idle state low
                  SSPSTATbits.CKE = 1;       // data transmitted on falling edge
                  break;
                case SPI_MODE2:              // SPI bus mode 1,0
                  SSPCON1bits.CKP = 1;       // clock idle state high
                  SSPSTATbits.CKE = 0;       // data transmitted on rising edge
                  break;
                case SPI_MODE3:              // SPI bus mode 1,1
                  SSPCON1bits.CKP = 1;       // clock idle state high
                  SSPSTATbits.CKE = 1;       // data transmitted on falling edge
                  break;
            }

            switch(SPI[SPI1].role)
            {
                case SPI_SLAVE_SS:           // slave mode w /SS enable
                    SSPIN = 1;               // define /SS pin as input
                    SCKPIN = 1;              // define clock pin as input
                    break;

                case SPI_SLAVE:              // slave mode w/o /SS enable
                    SCKPIN = 1;              // define clock pin as input
                    break;

                default:                     // master mode, define clock en SS pin as output
                    SSPIN  = 0;              // define SS  pin as output
                    SCKPIN = 0;              // define clock pin as output
                    break;
            }

            SDIPIN = 1;                    // define SDI pin as input
            SDOPIN = 0;                    // define SDO pin as output
            SSPCON1bits.SSPEN = 1;
            Delayms(30);

            #ifdef SPIINT
                PIE1bits.SSPIE = 1;
                INTCONbits.GIEH = 1;
                INTCONbits.GIEL = 1;
                INTCONbits.PEIE = 1;
                //IPR1bits.SSPIP = 1;
                PIR1bits.SSP1IF = 0;
            #endif
            break;

        #if defined(__18f26j50)|| defined(__18f46j50) || \
            defined(__18f27j53)|| defined(__18f47j53)
        case SPI2:
            SSP2CON1bits.SSPEN = 0;
            SSP2STAT = 0x3F;                 // power on state (SMP=0) 
            SSP2CON1 = SPI[SPI2].divider;    // select serial mode

            if (SPI[SPI2].phase == SPI_SMPEND)
            {
                SSP2STATbits.SMP = 1;       // input data sampled at end of data output time
            }
            
            switch(SPI[SPI2].mode)
            {
                case SPI_MODE0:             // SPI bus mode 0,0
                  SSP2CON1bits.CKP = 0;     // clock idle state low
                  SSP2STATbits.CKE = 0;     // data transmitted on falling edge
                  break;    
                case SPI_MODE1:             // default SPI bus mode 0,1
                  SSP2CON1bits.CKP = 0;     // clock idle state low
                  SSP2STATbits.CKE = 1;     // data transmitted on falling edge
                  break;
                case SPI_MODE2:             // SPI bus mode 1,0
                  SSP2CON1bits.CKP = 1;     // clock idle state high
                  SSP2STATbits.CKE = 0;     // data transmitted on rising edge
                  break;
                case SPI_MODE3:             // SPI bus mode 1,1
                  SSP2CON1bits.CKP = 1;     // clock idle state high
                  SSP2STATbits.CKE = 1;     // data transmitted on falling edge
                  break;
            }

            switch(SPI[SPI2].role)
            {
                case SPI_SLAVE_SS:          // slave mode w /SS enable
                    SS2PIN = 1;             // define /SS pin as input
                    SCK2PIN = 1;            // define clock pin as input
                    break;

                case SPI_SLAVE:             // slave mode w/o /SS enable
                    SCK2PIN = 1;            // define clock pin as input
                    break;

                default:                    // master mode, define clock en SS pin as output
                    SS2PIN  = 0;            // define SS  pin as output
                    SCK2PIN = 0;            // define clock pin as output
                    break;
            }

            SDI2PIN = 1;                    // define SDI pin as input
            SDO2PIN = 0;                    // define SDO pin as output
            SSP2CON1bits.SSPEN = 1;
            Delayms(30);

            #ifdef SPIINT
                PIE3bits.SSP2IE = 1;
                INTCONbits.GIEH = 1;
                INTCONbits.GIEL = 1;
                INTCONbits.PEIE = 1;
                //IPR1bits.SSPIP = 1;
                PIR3bits.SSP1IF = 0;
            #endif
            break;
        #endif
    }
}
//#endif

/**
 * This function sets the order of the bits shifted out of and into the SPI bus,
 * either LSBFIRST (least-significant bit first) or MSBFIRST (most-significant bit first). 
 */

//#ifdef SPISETBITORDER
void SPI_setBitOrder(u8 module, u8 bitorder)
{
    switch(module)
    {
        case SPI1:
            SPI[SPI1].bitorder = bitorder;
            break;

        #if defined(__18f26j50)|| defined(__18f46j50) || \
            defined(__18f27j53)|| defined(__18f47j53)
        case SPI2:
            SPI[SPI2].bitorder = bitorder;
            break;
        #endif
    }
}

/**
 * This function sets the SPI data mode (clock polarity and phase)
 * Modes available are SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3
 * Mode    CKP    CKE
 * 0       0       0
 * 1       0       1
 * 2       1       0
 * 3       1       1
 */
//#endif

//#ifdef SPISETDATAMODE
void SPI_setDataMode(u8 module, u8 mode)
{
    switch(module)
    {
        case SPI1:
            SPI[SPI1].mode = mode;
            break;

        #if defined(__18f26j50)|| defined(__18f46j50) || \
            defined(__18f27j53)|| defined(__18f47j53)
        case SPI2:
            SPI[SPI2].mode = mode;
            break;
        #endif
    }
}
//#endif

/**
 * This function sets the SPI mode.
 * Possible values are SPI_MASTER or SPI_SLAVE.
 * The default setting is SPI_MASTER.
 */

//#ifdef SPISETMODE
void SPI_setMode(u8 module, u8 mode)
{
    switch(module)
    {
        case SPI1:
            SPI[SPI1].role  = mode;
            SPI[SPI1].divider = mode;
            break;

        #if defined(__18f26j50)|| defined(__18f46j50) || \
            defined(__18f27j53)|| defined(__18f47j53)
        case SPI2:
            SPI[SPI2].role  = mode;
            SPI[SPI2].divider = mode;
            break;
        #endif
    }
}
//#endif

/**
 * This function sets the SPI clock divider relative to the system clock.
 * The dividers available are 4, 8, 16, 64.
 * The default setting is SPI_CLOCK_DIV4, which sets the SPI clock to one-quarter
 * the frequency of the system clock. 
 */

//#ifdef SPISETCLOCKDIVIDER
void SPI_setClockDivider(u8 module, u8 divider)
{
    switch(module)
    {
        case SPI1:
            SPI[SPI1].divider = divider;
            SPI[SPI1].role  = divider;
            break;

        #if defined(__18f26j50)|| defined(__18f46j50) || \
            defined(__18f27j53)|| defined(__18f47j53)
        case SPI2:
            SPI[SPI2].divider = divider;
            SPI[SPI2].role  = divider;
            break;
        #endif
    }
}
//#endif

/** -----------------------------------------------------------------**/
#endif /* SPIINIT */
/** -----------------------------------------------------------------**/

//#ifdef SPIWRITE
u8 SPI_write(u8 module, u8 datax)
{
    u8 clear;

    switch(module)
    {
        case SPI1:
            clear = SSP1BUF;        // clear BF
            PIR1bits.SSP1IF = 0;              // enable SPI2 interrupt
            SSPCON1bits.WCOL = 0;
            SSP1BUF = datax;        // send data

            if (SSPCON1bits.WCOL)
                return (1);
            else
                while (!PIR1bits.SSP1IF);
            return (0);

        #if defined(__18f26j50)|| defined(__18f46j50) || \
            defined(__18f27j53)|| defined(__18f47j53)
        case SPI2:
            clear = SSP2BUF;        // clear BF
            PIR3bits.SSP2IF = 0;              // enable SPI2 interrupt
            SSP2CON1bits.WCOL = 0;
            SSP2BUF = datax;        // send data

            if (SSP2CON1bits.WCOL)
                return (1);
            else
                while (!PIR3bits.SSP2IF);
            return (0);

        #endif
    }
}
//#endif

#define SPI_read(module) SPI_write(module, 0xFF)

/**
 *  Interrupt routines 
 */

void spi1_interrupt()
{
    u8 c;
    if (PIR1bits.SSP1IF)
    {
        c = SSP1BUF;
        SSP1BUF = SPI1_onEvent_func(c);
        PIR1bits.SSP1IF = 0;
    }
}

static void SPI1_onEvent(u8(*func)(u8))
{
  SPI1_onEvent_func = func;
}

#if defined(__18f26j50)|| defined(__18f46j50) || \
    defined(__18f27j53)|| defined(__18f47j53)

void spi2_interrupt()
{
    u8 c;
    if (PIR3bits.SSP2IF)
    {
        c = SSP2BUF;
        SSP2BUF = SPI2_onEvent_func(c);
        PIR3bits.SSP2IF = 0;
    }
}

static void SPI2_onEvent(u8(*func)(u8))
{
  SPI2_onEvent_func = func;
}

#endif // __18f47j53__ ...

#endif // __SPI_C__
