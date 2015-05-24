/*	----------------------------------------------------------------------------
    FILE:			spi.c
    PROJECT:		pinguino
    PURPOSE:		Serial Peripheral Interface functions
    PROGRAMER:		Régis Blanchot <rblanchot@gmail.com>
                    Marcus Fazzi <anunakin@gmail.com>
                    Jean-Pierre Mandon <jp.mandon@gmail.com>
    FIRST RELEASE:	16 Mar 2011
    LAST RELEASE:	15 Mar 2014
    ----------------------------------------------------------------------------
    CHANGELOG : 
    24 May 2011 - jp.mandon  -  fixed a bug in SPI_write, RX int flag must be called even for write
    20 Feb 2012 - r.blanchot -  added PIC32_PINGUINO_220 support
    28 May 2012 - MFH        -  added PIC32_PINGUINO_MICRO support and fixed a bug
                                in SPI_clock() identified by dk (KiloOne)
    01 Mar 2014 - fcapozzi   -  added SPI32_init function
    15 Mar 2014 - rblanchot  -  fixed compatibility with 8-bit Pinguino SPI library 
    22 Aug 2014 - rblanchot  -  fixed SPI2 bug 
    13 Apr 2015 - rblanchot  -  added multi-module support (SPISW, SPI1, SPI2, SPI3 and SPI4) 
    14 Apr 2015 - rblanchot  -  moved #define and prototypes in spi.h 
    14 Apr 2015 - rblanchot  -  added SPI structure to store SPI features 
     ----------------------------------------------------------------------------
    TODO :
    * SPI SOFTWARE support
    * SLAVE MODE support
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

#ifndef __SPI__
#define __SPI__

#include <p32xxxx.h>
#include <typedef.h>
#include <spi.h>
#include <system.c>
#include <interrupt.c>

/**
 *  This function init the SPI module to default values
 *  Called from main32.c
 */

void SPI_init()
{
    u8 i;
    
    for (i=0; i<NUMOFSPI; i++)
    {
        SPI[i].mode     = SPI_MODE0;
        SPI[i].divider  = SPI_PBCLOCK_DIV64;
        SPI[i].role     = SPI_MASTER;
        SPI[i].bitorder = SPI_MSBFIRST;
        SPI[i].phase    = SPI_SMPEND;
    }
}

/**
 *  This function set SPI software pins
 */

void SPI_setPin(u8 module, u8 sda, u8 sck)
{
    if (module == SPISW)
    {
        SPI[module].sda = sda;
        SPI[module].sck = sck;
        output(SPI[module].sda);
        output(SPI[module].sck);
    }
}

/**
 *  This function select a SPI module
 *  RB20150429 : we can not defined only one SS (Slave Select) pin
 *  as we could have more than one slave on the same SPI bus.
 */

/*
void SPI_select(u8 module)
{
    switch(module)
    {
        case SPI1:
            #if  defined(PINGUINO32MX220) || defined(PINGUINO32MX250) || defined(PINGUINO32MX270)
            // RB7 is defined as SS1 pin
            LATBCLR = 1 << 7; // device selection
            #endif
            break;

        case SPI2:
            #if  defined(PINGUINO32MX220) || defined(PINGUINO32MX250) || defined(PINGUINO32MX270)
            // RB9 is defined as SS2 pin
            LATBCLR = 1 << 9; // device selection
            #endif
            break;
    }
}
*/

/**
 *  This function deselect a SPI module
 *  RB20150429 : we can not defined only one SS (Slave Select) pin
 *  as we could have more than one slave on the same SPI bus.
 */

/*
void SPI_deselect(u8 module)
{
    switch(module)
    {
        case SPI1:
            #if  defined(PINGUINO32MX220) || defined(PINGUINO32MX250) || defined(PINGUINO32MX270)
            // RB7 is defined as SS1 pin
            LATBSET = 1 << 7; // stops device selection
            #endif
            break;

        case SPI2:
            #if  defined(PINGUINO32MX220) || defined(PINGUINO32MX250) || defined(PINGUINO32MX270)
            // RB9 is defined as SS1 pin
            LATBSET = 1 << 9; // stops device selection
            #endif
            break;
    }
}
*/

/**
 * This function sets the order of the bits shifted out of and into the SPI bus,
 * either SPI_LSBFIRST (least-significant bit first) or
 * SPI_MSBFIRST (most-significant bit first). 
 */

void SPI_setBitOrder(u8 module, u8 bitorder)
{
    SPI[module].bitorder = bitorder;
    //SPI_begin();
}

/**
 * This function sets the SPI data mode (clock polarity and phase)
 * Bits CKP (SPIxCON<6>) and CKE (SPIxCON<8>) determine on which edge of
 * the clock data transmission occurs.
 * Modes available are SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3
 * Mode    CKP    CKE
 * 0       0       0
 * 1       0       1
 * 2       1       0
 * 3       1       1
 */

//#ifdef SPISETDATAMODE
void SPI_setDataMode(u8 module, u8 mode)
{
    SPI[module].mode = mode;
    //SPI_begin();
}
//#endif

/**
 * This function sets the SPI mode.
 * Possible values are SPI_MASTER or SPI_SLAVE.
 * The default setting is SPI_MASTER.
 */

void SPI_setMode(u8 module, u8 role)
{
    SPI[module].role = role;
    //SPI_begin();
}

/**
 * Baud Rate generation
 * Fsck = Fpb / (2 * (SPIxBRG + 1)
 * Therefore, the maximum baud rate possible is Fpb/2 (SPIxBRG = 0),
 * and the minimum baud rate possible is Fpb/1024.
 * SPIxBRG = (Fpb / (2 * Fsck)) - 1
 * speed must be in bauds
 **/
 
u32 SPI_setClock(u8 module, u32 Fspi)
{
    u32 Fpb = GetPeripheralClock();
    
    if (Fspi > (Fpb / 2))
    {
        SPI[module].divider = 0;        // use the maximum baud rate possible
        return (Fpb / 2);               // and return the real speed
    }
    else
    {
        // divider baudrate
        SPI[module].divider = (Fpb / (2 * Fspi)) - 1;
        
        if (SPI[module].divider > 511)
        {
            SPI[module].divider = 511;  // use the minimum baud rate possible
            return (Fpb / 1024);        // and return the real speed
        }
        else                            // ** fix for bug identified by dk=KiloOne
        {
            // return the real speed
            return ( Fpb / ( 2 * SPI[module].divider + 1));
        }
    }

    //SPI_begin();
}

/**
 * This function sets the SPI clock divider relative to the peripheral clock.
 * The dividers available are 2, 4, 8, 16, 32 and 64.
 * The default setting is SPI_PBCLOCK_DIV64, which sets the SPI clock to
 * 1/64 of the frequency of the peripheral clock. 
 */

//#ifdef SPISETCLOCKDIVIDER
void SPI_setClockDivider(u8 module, u32 divider)
{
    if (divider > 1024)
        SPI[module].divider = 511;
    else
        SPI[module].divider = divider / 2 - 1;
}
//#endif

/**
 * Disable all SPIx interrupts
 * Stops and resets the SPIx
 * Clears the receive buffer
 **/
 
void SPI_close(u8 module)
{
    u8 rData;
    
    switch(module)
    {
        #if !defined(__32MX440F256H__)
        
        case SPI1:
            // 1.  Disable the SPI interrupts in the respective IEC0/1 register.
            IntDisable(INT_SPI1_FAULT); 
            IntDisable(INT_SPI1_TRANSFER_DONE); 
            IntDisable(INT_SPI1_RECEIVE_DONE);
            // 2.  Stop and reset the SPI module by clearing the ON bit.
            SPI1CON = 0;
            // 3.  Clear the receive buffer.
            rData=SPI1BUF;
            break;
        
        #endif
        
        case SPI2:
            // 1.  Disable the SPI interrupts in the respective IEC0/1 register.
            IntDisable(INT_SPI2_FAULT); 
            IntDisable(INT_SPI2_TRANSFER_DONE); 
            IntDisable(INT_SPI2_RECEIVE_DONE);
            // 2.  Stop and reset the SPI module by clearing the ON bit.
            SPI2CON = 0;
            // 3.  Clear the receive buffer.
            rData=SPI2BUF;
            break;

        #if defined(__32MX795F512L__) || \
            defined(__32MX795F512H__)

        case SPI3:
            // 1.  Disable the SPI interrupts in the respective IEC0/1 register.
            IntDisable(INT_SPI3_FAULT); 
            IntDisable(INT_SPI3_TRANSFER_DONE); 
            IntDisable(INT_SPI3_RECEIVE_DONE);
            // 2.  Stop and reset the SPI module by clearing the ON bit.
            SPI3CON = 0;
            // 3.  Clear the receive buffer.
            rData=SPI3BUF;
            break;

        case SPI4:
            // 1.  Disable the SPI interrupts in the respective IEC0/1 register.
            IntDisable(INT_SPI4_FAULT); 
            IntDisable(INT_SPI4_TRANSFER_DONE); 
            IntDisable(INT_SPI4_RECEIVE_DONE);
            // 2.  Stop and reset the SPI module by clearing the ON bit.
            SPI4CON = 0;
            // 3.  Clear the receive buffer.
            rData=SPI4BUF;
            break;

        #endif
    }
}

/**
 * The following code assumes that none of the SPIx input pins are
 * shared with an analog input. If so, the AD1PCFG and corresponding
 * TRIS registers have to be properly configured.
 * 
 * Perform the following steps to set up the SPI module for the Master
 * mode operation:
 *  1.  Disable the SPI interrupts in the respective IEC0/1 register.
 *  2.  Stop and reset the SPI module by clearing the ON bit.
 *  3.  Clear the receive buffer.
 *  4.  Clear the ENHBUF bit (SPIxCON<16>) if using Standard Buffer
 *      mode or set the bit if using Enhanced Buffer mode.
 *  5.  If SPI interrupts are not going to be used, skip this step and
 *      continue to step 5. Otherwise the following additional steps
 *      are performed:
 *       a) Clear the SPIx interrupt flags/events in the respective IFS0/1 register.
 *       b) Set the SPIx interrupt enable bits in the respective IEC0/1 register.
 *       c) Write the SPIx interrupt priority and subpriority bits in the respective IPC5/7 register.
 *  6. Write the Baud Rate register, SPIxBRG.
 *  7. Clear the SPIROV bit (SPIxSTAT<6>).
 *  8. Write the desired settings to the SPIxCON register with MSTEN (SPIxCON<5>) = 1.
 *  9. Enable SPI operation by setting the ON bit (SPIxCON<15>).
 **/

void SPI_begin(u8 module)
{
    // Reset the module
    SPI_close(module);
    
    // Configure the module
    switch(module)
    {
        case SPISW:
            break;
            
        #if !defined(__32MX440F256H__)

        case SPI1:
            // IO's
            #if defined(PINGUINO32MX220) || defined(PINGUINO32MX250) || defined(PINGUINO32MX270)
            TRISBCLR = 1<<14;                   // SCK1 is on RB14 ( D1 )
            //TRISBCLR = 1<<7;                    // SS1  is on RB7  ( D5 )
            TRISBSET = 1<<5;                    // SDI1 is on RB5  ( D6 )
            TRISACLR = 1<<4;                    // SDO1 is on RA4  ( D7 )
            #endif
            
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
            SPI1BRG = SPI[SPI1].divider; // Default SPI_PBCLOCK_DIV64

            // 7. Clear the SPIROV bit (SPIxSTAT<6>).
            SPI1STATbits.SPIROV = 0;                         // clear the Overflow

            // 8. Write the desired settings to the SPIxCON register.

            /**
             * Mode           CKP     CKE
             * SPI_MODE0       0       0
             * SPI_MODE1       0       1
             * SPI_MODE2       1       0
             * SPI_MODE3       1       1
             */

            switch (SPI[SPI1].mode)
            {
                case SPI_MODE0:                 // SPI bus mode 0,0
                  SPI1CONbits.CKP = 0;
                  SPI1CONbits.CKE = 0;          // data transmitted on falling edge
                  break;    
                case SPI_MODE1:                 // default SPI bus mode 0,1
                  SPI1CONbits.CKP = 0;
                  SPI1CONbits.CKE = 1;          // data transmitted on falling edge
                  break;
                case SPI_MODE2:                 // SPI bus mode 1,0
                  SPI1CONbits.CKP = 1;          // clock idle state high
                  SPI1CONbits.CKE = 0;          // data transmitted on rising edge
                  break;
                case SPI_MODE3:                 // SPI bus mode 1,1
                  SPI1CONbits.CKP = 1;          // clock idle state high
                  SPI1CONbits.CKE = 1;          // data transmitted on falling edge
                  break;
            }
            
            SPI1CONbits.SMP = SPI[SPI1].phase;  // Input data sampled at end of middle of data output time

            switch (SPI[SPI1].role)
            {
                case SPI_MASTER8:
                    SPI1CONbits.MODE16 = 0;
                    SPI1CONbits.MODE32 = 0;
                    SPI1CONbits.MSTEN  = 1;     // Master
                    break;
                case SPI_MASTER16:
                    SPI1CONbits.MODE16 = 1;
                    SPI1CONbits.MODE32 = 0;
                    SPI1CONbits.MSTEN  = 1;     // Master
                    break;
                case SPI_MASTER32:
                    SPI1CONbits.MODE16 = 0;
                    SPI1CONbits.MODE32 = 1;
                    SPI1CONbits.MSTEN  = 1;     // Master
                    break;
                case SPI_SLAVE:
                    SPI1CONbits.MSTEN  = 0;     // Slave
                    break;
            }

            // The SSx pin is not driven by the SPI Master.
            // User have to drive the bit himself and pulse it before the SPI
            // transmission takes place.
            // He can therefore use any other I/O pin for that purpose,
            // but we provide one ;-)
            /*
            if (SPI[SPI1].role != SPI_SLAVE)
            {
                #if defined(PINGUINO32MX250) || defined(PINGUINO32MX270) || defined(PINGUINO32MX220)
                    TRISBCLR = 1<<7;            // RB7 is defined as SS1 pin (cf. io.c)
                #endif
            }
            */
            
            // 9. Enable SPI operation by setting the ON bit (SPIxCON<15>).
            SPI1CONSET = 0x8000;                 // Set bit 15
            break;

        #endif

        case SPI2:
            // IO's
            #if defined(PINGUINO32MX220) || defined(PINGUINO32MX250) || defined(PINGUINO32MX270)
            TRISBCLR = 1<<15;                   // SCK2 is on RB15 ( D0 )
            TRISBSET = 1<<13;                   // SDI2 is on RB13 ( D2 )
            //TRISBCLR = 1<<9;                    // SS2  is on RB9  ( D3 )
            TRISBCLR = 1<<8;                    // SDO2 is on RB8  ( D4 )
            #endif
            
            // 6. Write the Baud Rate register, SPIxBRG.
            SPI2BRG = SPI[SPI2].divider;        // Default SPI_PBCLOCK_DIV64

            // 7. Clear the SPIROV bit (SPIxSTAT<6>).
            SPI2STATbits.SPIROV = 0;            // clear the Overflow

            // 8. Write the desired settings to the SPIxCON register.
            switch (SPI[SPI2].mode)
            {
                case SPI_MODE0:                 // SPI bus mode 0,0
                  SPI2CONbits.CKP = 0;
                  SPI2CONbits.CKE = 0;          // data transmitted on falling edge
                  break;    
                case SPI_MODE1:                 // default SPI bus mode 0,1
                  SPI2CONbits.CKP = 0;
                  SPI2CONbits.CKE = 1;          // data transmitted on falling edge
                  break;
                case SPI_MODE2:                 // SPI bus mode 1,0
                  SPI2CONbits.CKP = 1;          // clock idle state high
                  SPI2CONbits.CKE = 0;          // data transmitted on rising edge
                  break;
                case SPI_MODE3:                 // SPI bus mode 1,1
                  SPI2CONbits.CKP = 1;          // clock idle state high
                  SPI2CONbits.CKE = 1;          // data transmitted on falling edge
                  break;
            }
            
            SPI2CONbits.SMP = SPI[SPI2].phase;       // Input data sampled at end of middle of data output time

            switch (SPI[SPI2].role)
            {
                case SPI_MASTER8:
                    SPI2CONbits.MODE16 = 0;
                    SPI2CONbits.MODE32 = 0;
                    SPI2CONbits.MSTEN  = 1; // Master
                    break;
                case SPI_MASTER16:
                    SPI2CONbits.MODE16 = 1;
                    SPI2CONbits.MODE32 = 0;
                    SPI2CONbits.MSTEN  = 1; // Master
                    break;
                case SPI_MASTER32:
                    SPI2CONbits.MODE16 = 0;
                    SPI2CONbits.MODE32 = 1;
                    SPI2CONbits.MSTEN  = 1; // Master
                    break;
                case SPI_SLAVE:
                    SPI2CONbits.MSTEN  = 0; // Slave
                    break;
            }
            
            // 9. Enable SPI operation by setting the ON bit (SPIxCON<15>).
            SPI2CONSET = 0x8000;                 // Set bit 15
            break;

        #if defined(__32MX795F512L__) || \
            defined(__32MX795F512H__)

        case SPI3:
            // 6. Write the Baud Rate register, SPIxBRG.
            SPI3BRG = SPI[SPI3].divider; // Default SPI_PBCLOCK_DIV64

            // 7. Clear the SPIROV bit (SPIxSTAT<6>).
            SPI3STATbits.SPIROV = 0;                         // clear the Overflow

            // 8. Write the desired settings to the SPIxCON register.
            switch (SPI[SPI3].mode)
            {
                case SPI_MODE0:                 // SPI bus mode 0,0
                  SPI3CONbits.CKP = 0;
                  SPI3CONbits.CKE = 0;          // data transmitted on falling edge
                  break;    
                case SPI_MODE1:                 // default SPI bus mode 0,1
                  SPI3CONbits.CKP = 0;
                  SPI3CONbits.CKE = 1;          // data transmitted on falling edge
                  break;
                case SPI_MODE2:                 // SPI bus mode 1,0
                  SPI3CONbits.CKP = 1;          // clock idle state high
                  SPI3CONbits.CKE = 0;          // data transmitted on rising edge
                  break;
                case SPI_MODE3:                 // SPI bus mode 1,1
                  SPI3CONbits.CKP = 1;          // clock idle state high
                  SPI3CONbits.CKE = 1;          // data transmitted on falling edge
                  break;
            }
            
            SPI1CONbits.SMP = SPI[SPI3].phase;  // Input data sampled at end of middle of data output time

            switch (SPI[SPI3].role)
            {
                case SPI_MASTER8:
                    SPI3CONbits.MODE16 = 0;
                    SPI3CONbits.MODE32 = 0;
                    SPI3CONbits.MSTEN  = 1; // Master
                    break;
                case SPI_MASTER16:
                    SPI3CONbits.MODE16 = 1;
                    SPI3CONbits.MODE32 = 0;
                    SPI3CONbits.MSTEN  = 1; // Master
                    break;
                case SPI_MASTER32:
                    SPI3CONbits.MODE16 = 0;
                    SPI3CONbits.MODE32 = 1;
                    SPI3CONbits.MSTEN  = 1; // Master
                    break;
                case SPI_SLAVE:
                    SPI3CONbits.MSTEN  = 0; // Slave
                    break;
            }
            
            // 9. Enable SPI operation by setting the ON bit (SPIxCON<15>).
            SPI3CONSET = 0x8000;                 // Set bit 15
            break;

        case SPI4:
            // 6. Write the Baud Rate register, SPIxBRG.
            SPI4BRG = SPI[SPI4].divider; // Default SPI_PBCLOCK_DIV64

            // 7. Clear the SPIROV bit (SPIxSTAT<6>).
            SPI4STATbits.SPIROV = 0;                         // clear the Overflow

            // 8. Write the desired settings to the SPIxCON register.
            switch (SPI[SPI4].mode)
            {
                case SPI_MODE0:                 // SPI bus mode 0,0
                  SPI4CONbits.CKP = 0;
                  SPI4CONbits.CKE = 0;          // data transmitted on falling edge
                  break;    
                case SPI_MODE1:                 // default SPI bus mode 0,1
                  SPI4CONbits.CKP = 0;
                  SPI4CONbits.CKE = 1;          // data transmitted on falling edge
                  break;
                case SPI_MODE2:                 // SPI bus mode 1,0
                  SPI4CONbits.CKP = 1;          // clock idle state high
                  SPI4CONbits.CKE = 0;          // data transmitted on rising edge
                  break;
                case SPI_MODE3:                 // SPI bus mode 1,1
                  SPI4CONbits.CKP = 1;          // clock idle state high
                  SPI4CONbits.CKE = 1;          // data transmitted on falling edge
                  break;
            }
            
            SPI4CONbits.SMP = SPI[SPI4].phase; // Input data sampled at end of middle of data output time

            switch (SPI[SPI4].role)
            {
                case SPI_MASTER8:
                    SPI4CONbits.MODE16 = 0;
                    SPI4CONbits.MODE32 = 0;
                    SPI4CONbits.MSTEN  = 1; // Master
                    break;
                case SPI_MASTER16:
                    SPI4CONbits.MODE16 = 1;
                    SPI4CONbits.MODE32 = 0;
                    SPI4CONbits.MSTEN  = 1; // Master
                    break;
                case SPI_MASTER32:
                    SPI4CONbits.MODE16 = 0;
                    SPI4CONbits.MODE32 = 1;
                    SPI4CONbits.MSTEN  = 1; // Master
                    break;
                case SPI_SLAVE:
                    SPI4CONbits.MSTEN  = 0; // Slave
                    break;
            }
            
            // 9. Enable SPI operation by setting the ON bit (SPIxCON<15>).
            SPI4CONSET = 0x8000;                 // Set bit 15
            break;

        #endif
    }
}

/**
 * Write the data to be transmitted to the SPIxBUF register.
 * Transmission (and reception) will start as soon as data is written
 * to the SPIxBUF register.
 **/
 
u8 SPI_write(u8 module, u8 data_out)
{
    u8 i;
    u8 bitMask;

    switch(module)
    {
        case SPISW:
            for (i = 0; i < 8; i++)
            {
                // MSB first
                if (SPI[module].bitorder == SPI_MSBFIRST)
                    bitMask = 0x80 >> i;

                // LSB first
                else
                    bitMask = 1 << i;

                // Send bit
                digitalwrite(SPI[module].sda, (data_out & bitMask) ? 1 : 0);

                // pulse
                high(SPI[module].sck);
                low(SPI[module].sck);
            }
            return data_out;

        #if !defined(__32MX440F256H__)

        case SPI1:
            SPI1BUF = data_out;             // write to buffer for TX
            while (!SPI1STATbits.SPIRBF);   // wait for the receive flag (transfer complete)
            return SPI1BUF;

        #endif

        case SPI2:
            SPI2BUF = data_out;             // write to buffer for TX
            while (!SPI2STATbits.SPIRBF);   // wait for the receive flag (transfer complete)
            return SPI2BUF;

        #if defined(__32MX795F512L__) || \
            defined(__32MX795F512H__)

        case SPI3:
            SPI3BUF = data_out;             // write to buffer for TX
            while (!SPI3STATbits.SPIRBF);   // wait for the receive flag (transfer complete)
            return SPI3BUF;

        case SPI4:
            SPI4BUF = data_out;             // write to buffer for TX
            while (!SPI4STATbits.SPIRBF);   // wait for the receive flag (transfer complete)
            return SPI4BUF;

        #endif
    }
}

// send dummy byte to capture the response
#define SPI_read(module) SPI_write(module, 0xFF)

/**
 * SPI1Interrupt
 **/

#if !defined(__32MX440F256H__)

void SPI1Interrupt(void)
{
    u8 rData;

    // Is this an RX interrupt ?
    if (IntGetFlag(INT_SPI1_RECEIVE_DONE))
    {
        rData = SPI1BUF;			// Read SPI data buffer
        IntClearFlag(INT_SPI1_RECEIVE_DONE);
    }
    // Is this an TX interrupt ?
    if (IntGetFlag(INT_SPI1_TRANSFER_DONE))
    {
        IntClearFlag(INT_SPI1_TRANSFER_DONE);
    }
}

#endif

/**
 * SPI2Interrupt
 **/

void SPI2Interrupt(void)
{
    u8 rData;

    // Is this an RX interrupt ?
    if (IntGetFlag(INT_SPI2_RECEIVE_DONE))
    {
        rData = SPI2BUF;			// Read SPI data buffer
        IntClearFlag(INT_SPI2_RECEIVE_DONE);
    }
    // Is this an TX interrupt ?
    if (IntGetFlag(INT_SPI2_TRANSFER_DONE))
    {
        IntClearFlag(INT_SPI2_TRANSFER_DONE);
    }
}

#if defined(__32MX795F512L__) || \
    defined(__32MX795F512H__)

void SPI3Interrupt(void)
{
    u8 rData;

    // Is this an RX interrupt ?
    if (IntGetFlag(INT_SPI3_RECEIVE_DONE))
    {
        rData = SPI3BUF;			// Read SPI data buffer
        IntClearFlag(INT_SPI3_RECEIVE_DONE);
    }
    // Is this an TX interrupt ?
    if (IntGetFlag(INT_SPI3_TRANSFER_DONE))
    {
        IntClearFlag(INT_SPI3_TRANSFER_DONE);
    }
}

void SPI4Interrupt(void)
{
    u8 rData;

    // Is this an RX interrupt ?
    if (IntGetFlag(INT_SPI4_RECEIVE_DONE))
    {
        rData = SPI4BUF;			// Read SPI data buffer
        IntClearFlag(INT_SPI4_RECEIVE_DONE);
    }
    // Is this an TX interrupt ?
    if (IntGetFlag(INT_SPI4_TRANSFER_DONE))
    {
        IntClearFlag(INT_SPI4_TRANSFER_DONE);
    }
}

#endif /* __32MX795F512x__ */

#endif	/* __SPI__ */
