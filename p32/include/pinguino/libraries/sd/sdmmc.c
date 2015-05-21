/*
** SDMMC.c SD card interface
**
** 7/20/06 v1.4 LDJ
** 7/14/07 v2.0 LDJ
**
** 30 Mar 2012  [hgmvanbeek@gmail.com][Some cards have no card detect and no write protect]
** 07 May 2012	Changes made to allow SD card library to support
**				PIC32 Pinguino and PIC32 Pinguino Micro that have neither
**				SD card presence nor WP switch detection switches.
**				see functions getCD() and getWP() below.
** 14 Apr 2015  Replaces writeSPI with SPI_write form the SPI lib.
*/

#ifndef __SDMMC_C__
#define __SDMMC_C__

#include <typedef.h>
#include <spi.h>        // in order to use default SPI port
#include <spi.c>        // in order to use default SPI port
#include <system.c>
#include <digitalw.c>
#include <sd/sdmmc.h>
#include <sd/diskio.h>
#include <sd/ff.h>

void disableSD(u8 module)
{
    digitalwrite(SD[module].cs, HIGH);  // Deselected = CS high
    SPI_write(module, 0xFF);
}

void enableSD(u8 module)
{
    digitalwrite(SD[module].cs, LOW);   // Selected = CS low
}

// c    command code
// a    byte address of data block
int sendSDCmd(u8 module, u8 c, unsigned a)
{
    int i, r;

    // enable SD card
    // CS low
    enableSD(module);

    // send a comand packet (6 bytes)
    SPI_write(module, c | 0x40);    // send command
    SPI_write(module, a>>24);       // msb of the address
    SPI_write(module, a>>16);
    SPI_write(module, a>>8);
    SPI_write(module, a);           // lsb
    SPI_write(module, 0x95);        // send CMD0 CRC

    // now wait for a response, allow for up to 8 bytes delay
    for(i=0; i<8; i++)
    {
        r = SPI_read(module);
        if (r != 0xFF)
            break;
    }
    return (r);

    /* return response
    FF - timeout
    00 - command accepted
    01 - command received, card in idle state after RESET

    other codes:
    bit 0 = Idle state
    bit 1 = Erase Reset
    bit 2 = Illegal command
    bit 3 = Communication CRC error
    bit 4 = Erase sequence error
    bit 5 = Address error
    bit 6 = Parameter error
    bit 7 = Always 0
    */
    // NOTE CSCD is still low!
} // sendSDCmd


// a        LBA of sector requested
// p        pointer to sector buffer
// returns  TRUE if successful
int readSECTOR(u8 module, LBA a, char *p)
{
    int r, i;

    #ifdef READ_LED
    digitalwrite(READ_LED, 0);
    #endif

    // 1. send READ command
    r = sendSDCmd(module, READ_SINGLE, (a << 9));
    if (r == 0)    // check if command was accepted
    {
        // 2. wait for a response
        for(i=0; i<R_TIMEOUT; i++)
        {
            r = SPI_read(module);
            if (r == DATA_START)
            break;
        }

        // 3. if it did not timeout, read 512 byte of data
        if (i != R_TIMEOUT)
        {
            i = 512;
            do{
                *p++ = SPI_read(module);
            } while (--i>0);

            // 4. ignore CRC
            SPI_read(module);
            SPI_read(module);

        } // data arrived

    } // command accepted

    // 5. remember to disable the card
    disableSD(module);

    #ifdef READLED
    digital(READ_LED, 1);
    #endif

    return (r == DATA_START);    // return TRUE if successful
} // readSECTOR


// a        LBA of sector requested
// p        pointer to sector buffer
// returns  TRUE if successful
int writeSECTOR(u8 module, LBA a, char *p)
{
    unsigned r, i;

    // 0. check Write Protect
    if (getWP(module))
        return FALSE;

    // 1. send WRITE command
    r = sendSDCmd(module, WRITE_SINGLE, (a << 9));
    if (r == 0)    // check if command was accepted
    {
        // 2. send data
        SPI_write(module, DATA_START);

        // send 512 bytes of data
        for(i=0; i<512; i++)
        SPI_write(module, *p++);

        // 3. send dummy CRC
        SPI_write(module, 0xFF);
        SPI_write(module, 0xFF);

        // 4. check if data accepted
        r = SPI_read(module);
        if ((r & 0xf) == DATA_ACCEPT)
        {
            #ifdef WRITE_LED
            digitalwrite(WRITE_LED, 0);
            #endif

            // 5. wait for write completion
            for(i=0; i<W_TIMEOUT; i++)
            {
                r = SPI_read(module);
                if (r != 0 )
                    break;
            }
            #ifdef WRITE_LED
            digitalwrite(WRITE_LED, 1);
            #endif
        } // accepted
        else
        {
            r = FALSE;
        }
    } // command accepted

    // 6. disable the card
    disableSD(module);

    return (r);      // return TRUE if successful
} // writeSECTOR


// SD card connector presence detection switch
// returns  TRUE card present
//          FALSE card not present
int getCD(u8 module)
{
    #if defined(__ST7735__)

    return TRUE;

    // 07 May 2012 ** Added specific support for PIC32 Pinguino and Micro
    #elif defined(PIC32_PINGUINO)  || defined(PIC32_PINGUINO_OTG) || defined(PIC32_PINGUINO_MICRO) || \
          defined(EMPEROR460)      || \
          defined(PINGUINO32MX220) || defined(PINGUINO32MX250)    || defined(PINGUINO32MX270)

    return TRUE;

    #else

    return TRUE; //(SD[module].cd);

    #endif
}

// card Write Protect tab detection switch
// returns  TRUE write protect tab on LOCK
//          FALSE write protection tab OPEN
int getWP(u8 module)
{
    #if defined(__ST7735__)

    return FALSE;

    // 07 May 2012 ** Added specific support for PIC32 Pinguino and Micro
    #elif defined(PIC32_PINGUINO)  || defined(PIC32_PINGUINO_OTG) || defined(PIC32_PINGUINO_MICRO) || \
          defined(EMPEROR460)      || \
          defined(PINGUINO32MX220) || defined(PINGUINO32MX250)    || defined(PINGUINO32MX270)

    return FALSE;

    #else

    return FALSE; //(SD[module].wp);

    #endif
}

#endif /* __SDMMC_C__ */
