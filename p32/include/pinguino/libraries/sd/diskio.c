/*------------------------------------------------------------------------/
 /  MMCv3/SDv1/SDv2 (in SPI mode) control module
 /-------------------------------------------------------------------------/
 /
 /  Copyright (C) 2010, ChaN, all right reserved.
 /
 / * This software is a free software and there is NO WARRANTY.
 / * No restriction on use. You can use, modify and redistribute it for
 /   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
 / * Redistributions of source code must retain the above copyright notice.
 /
 /-------------------------------------------------------------------------*/

// 07 May 2012 Changes made to allow SD card library to support
//					PIC32 Pinguino Micro and potentially other cards that
//					do not support the use of the RTCC library.
// 25 May 2012 Added includes for delay.c and digitalw.c

#include <typedef.h>
#include <sd/ff.h>
#include <sd/fileio.h>
#include <sd/diskio.h>
#include <delay.c>
#include <digitalw.c>
#include <system.c>
#include <spi.h>        // in order to use default SPI port
#include <spi.c>        // in order to use default SPI port
//#include <millis.c>

// For boards known to support the RTCC library ***Added 07 May 2012
// to allow SD Library to support PIC32 Pinguino Micro, which
// which does not have a RTCC crystal and associated components,
// and would not operate if #include <rtcc.c> is included.
// See also changes to get_fattime() below.

#if defined (PIC32_PINGUINO) || defined (PIC32_PINGUINO_OTG)
    #include <rtcc.c>
#endif

/* Definitions for MMC/SDC command */
#define CMD0   (0)			/* GO_IDLE_STATE */
#define CMD1   (1)			/* SEND_OP_COND */
#define ACMD41 (41|0x80)	/* SEND_OP_COND (SDC) */
#define CMD8   (8)			/* SEND_IF_COND */
#define CMD9   (9)			/* SEND_CSD */
#define CMD10  (10)			/* SEND_CID */
#define CMD12  (12)			/* STOP_TRANSMISSION */
#define ACMD13 (13|0x80)	/* SD_STATUS (SDC) */
#define CMD16  (16)			/* SET_BLOCKLEN */
#define CMD17  (17)			/* READ_SINGLE_BLOCK */
#define CMD18  (18)			/* READ_MULTIPLE_BLOCK */
#define CMD23  (23)			/* SET_BLOCK_COUNT */
#define ACMD23 (23|0x80)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24  (24)			/* WRITE_BLOCK */
#define CMD25  (25)			/* WRITE_MULTIPLE_BLOCK */
#define CMD41  (41)			/* SEND_OP_COND (ACMD) */
#define CMD55  (55)			/* APP_CMD */
#define CMD58  (58)			/* READ_OCR */

/* Port Controls  (Platform dependent) */
//#define SOCKPORT	PORTB		/* Socket contact port */
//#define SOCKWP	(1<<10)		/* Write protect switch (RB10) */
//#define SOCKINS	(1<<11)		/* Card detect switch (RB11) */

//#define	FCLK_SLOW()			/* Set slow clock (100k-400k) */
//#define	FCLK_FAST()			/* Set fast clock (depends on the CSD) */


/*--------------------------------------------------------------------------

 Module Private Functions

 ---------------------------------------------------------------------------*/

static volatile DSTATUS Stat = STA_NOINIT; /* Disk status */

static volatile UINT Timer1, Timer2; /* 1000Hz decrement timer */

static UINT CardType;

/*-----------------------------------------------------------------------*/
/* Exchange a byte between PIC and MMC via SPI  (Platform dependent)     */
/*-----------------------------------------------------------------------*/

//#define xchg_spi(module, dat)   SPI_write(module, dat)
//#define CS_H(module)            high(SD[module].cs)
//#define CS_L(module)          enableSD(module)

void rcvr_spi_m(u8 module, PF_BYTE *p)
{
    *(p) = (PF_BYTE) (SPI_read(module));
}

/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static int wait_ready(u8 module)
{
    UINT tmr;

    /* Wait for ready in timeout of 500ms */
    for (tmr = 5000; tmr; tmr--)
    {
        if (SPI_read(module) == 0xFF)
            return 1;
        Delayus(100);
    }

    return 0;
}

/*-----------------------------------------------------------------------*/
/* Select the card and wait ready                                        */
/*-----------------------------------------------------------------------*/

static int select(u8 module) /* 1:Successful, 0:Timeout */
{
    enableSD(module);
    SPI_write(module, 0xFF); /* Dummy clock (force DO enabled) */

    if (wait_ready(module))
        return 1; /* OK */
        
    disableSD(module);
    return 0; /* Timeout */
}

/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/* Returns : 1:OK, 0:Failed                                              */
/* PF_BYTE *buff : Data buffer to store received data                    */
/* UINT btr : Byte count (must be multiple of 4)                         */
/*-----------------------------------------------------------------------*/

static int rcvr_datablock(u8 module, PF_BYTE *buff, UINT btr)
{
    PF_BYTE d;
    UINT tmr;

    /* Wait for data packet in timeout of 100ms */
    for (tmr = 1000; tmr; tmr--)
    {
        d = SPI_read(module);
        #ifdef SD_DEBUG
        debugf("return 0x%X\r\n",d);
        #endif
        if (d != 0xFF)
            break;
        Delayus(100);
    }

    /* If not valid data token, return with error */
    if (d != 0xFE)
        return 0;

    /* Receive the data block into buffer */
    do {
        rcvr_spi_m(module, buff++);
        rcvr_spi_m(module, buff++);
        rcvr_spi_m(module, buff++);
        rcvr_spi_m(module, buff++);
    } while (btr -= 4);

    /* Send Dummy CRC */
    SPI_write(module, 0xFF);
    SPI_write(module, 0xFF);

    /* Return with success */
    return 1;
}

/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/* Returns : 1:OK, 0:Failed                                              */
/* PF_BYTE *buff : 512 byte data block to be transmitted                 */
/* PF_BYTE token : Data token                                            */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
static int xmit_datablock(u8 module, const PF_BYTE *buff, PF_BYTE token)
{
    PF_BYTE resp;
    UINT bc = 512;

    if (!wait_ready(module))
        return 0;

    /* Xmit a token */
    SPI_write(module, token);

    /* Not StopTran token */
    if (token != 0xFD)
    {

        /* Xmit the 512 byte data block to the MMC */
        do {
            SPI_write(module, *buff++);
            SPI_write(module, *buff++);
        } while (bc -= 2);
        
        /* Send dummy CRC */
        SPI_write(module, 0xFF);
        SPI_write(module, 0xFF);

        /* Receive a data response */
        resp = SPI_read(module);

        /* If not accepted, return with error */
        if ((resp & 0x1F) != 0x05)
            return 0;
    }

    return 1;
}
#endif	/* _READONLY */

/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/* PF_BYTE cmd : Command byte                                            */
/* DWORD arg : Argument                                                  */
/*-----------------------------------------------------------------------*/

static PF_BYTE send_cmd(u8 module, PF_BYTE cmd, DWORD arg)
{
    PF_BYTE n, crc, res;

    #ifdef SD_DEBUG
    debugf("Send command %d\r\n",cmd);
    #endif

    /* ACMD<n> is the command sequense of CMD55-CMD<n> */
    if (cmd & 0x80)
    {
        cmd &= 0x7F;
        res = send_cmd(module, CMD55, 0);
        if (res > 1)
            return res;
    }

    /* Select the card and wait for ready */
    disableSD(module);
    if (!select(module))
        return 0xFF;
    
    /* Send command */
    SPI_write(module, 0x40 | cmd);           /* Start + Command index */

    /* Send argument */
    SPI_write(module, (PF_BYTE)(arg >> 24)); /* Argument[31..24]      */
    SPI_write(module, (PF_BYTE)(arg >> 16)); /* Argument[23..16]      */
    SPI_write(module, (PF_BYTE)(arg >> 8));  /* Argument[15..8]       */
    SPI_write(module, (PF_BYTE)arg);         /* Argument[7..0]        */

    /* Send CRC */
    crc = 0x01;     /* Dummy CRC + Stop */

    if (cmd == CMD0)
        crc = 0x95; /* Valid CRC for CMD0(0) */
    
    if (cmd == CMD8)
        crc = 0x87; /* Valid CRC for CMD8(0x1AA) */

    SPI_write(module, crc);

    /* Receive command response */
    if (cmd == CMD12)
        SPI_write(module, 0xFF); /* Skip a stuff byte when stop reading */

    /* Wait for a valid response in timeout of 10 attempts */
    /* return response :
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
    n = 10;
    do
    {
        res = SPI_read(module);
        #ifdef SD_DEBUG
        debugf("return 0x%X\r\n",res);
        #endif
    }
    while ((res & 0x80) && --n);


    return res; /* Return with the response value */
}

/*--------------------------------------------------------------------------

 Public Functions

 ---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/* PF_BYTE drv : Physical drive number (0)                               */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(u8 module, PF_BYTE drv)
{
    PF_BYTE n, ty, cmd, buf[4];
    UINT tmr;
    DSTATUS s = 0;

    if (drv)
        return STA_NOINIT; /* Supports only single drive */
        
    if (Stat & STA_NODISK)
        return Stat; /* No card in the socket */

    disableSD(module); /* Force socket power on */

    //FCLK_SLOW();
    for (n = 80; n; n--)
        SPI_write(module, 0xFF); /* 80 dummy clocks */

    high(SD[module].cs);
    
    for (n = 80; n; n--)
        SPI_write(module, 0xFF); /* 80 dummy clocks */

    ty = 0;

    /* Enter Idle state */
 
    if (send_cmd(module, CMD0, 0) == 1)
    {
        //serial1printf("Idle state\r\n");
        /* SDv2? */
        if (send_cmd(module, CMD8, 0x1AA) == 1)
        {
            for (n = 0; n < 4; n++)
            {
                buf[n] = SPI_write(module, 0xFF); /* Get trailing return value of R7 resp */
                //serial1printf("return 0x%X\r\n", buf[n]);
            }
            
            /* The card can work at vdd range of 2.7-3.6V */
            if (buf[2] == 0x01 && buf[3] == 0xAA)
            {
                /* Wait for leaving idle state (ACMD41 with HCS bit) */
                for (tmr = 1000; tmr; tmr--)
                {
                    if (send_cmd(module, ACMD41, 1UL << 30) == 0)
                        break;
                    Delayus(1000);
                }

                /* Check CCS bit in the OCR */
                if (tmr && send_cmd(module, CMD58, 0) == 0)
                {
                    for (n = 0; n < 4; n++)
                    {
                        buf[n] = SPI_write(module, 0xFF);
                        //serial1printf("return 0x%X\r\n", buf[0]);
                    }
                    ty = (buf[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2; /* SDv2 */
                }
            }
        }
        
        /* SDv1 or MMCv3 */
        else
        {
            //serial1printf("SDv1 or MMCv3\r\n");
            if (send_cmd(module, ACMD41, 0) <= 1)
            {
                ty = CT_SD1;
                cmd = ACMD41; /* SDv1 */
            }
            
            else
            {
                ty = CT_MMC;
                cmd = CMD1; /* MMCv3 */
            }
            
            /* Wait for leaving idle state */
            for (tmr = 1000; tmr; tmr--)
            {
                if (send_cmd(module, ACMD41, 0) == 0)
                    break;
                Delayus(1000);
            }

            /* Set R/W block length to 512 */
            if (!tmr || send_cmd(module, CMD16, 512) != 0)
                ty = 0;
        }
    }

    CardType = ty;
    #ifdef SD_DEBUG
    debugf("CardType=%d\r\n",CardType);
    #endif
    
    /* Initialization succeded */
    if (ty)
    {
        s &= ~STA_NOINIT;
        
        // 6. increase speed to the max. baud rate possible = FPB/2
        SPI_close(module);
        SPI_setMode(module, SPI_MASTER8);   // Master mode (0x20)
        SPI_setDataMode(module, SPI_MODE3); // CKE=1 (0x100), CKP=0

        u8 fpb, fsd, fspimax, i;
        u16 div;
        
        // SPI send 1 bit per clock tick
        fsd = CardType * 8; // Class 4 = 4 MB/s = 32 Mb/s
        fpb = GetPeripheralClock()/1000000;
        fspimax = fpb/2;

        if (fsd < fspimax)
        {
            for (i=1; i<=10; i++)
            {
                div = 1<<i;
                if ( (fpb/div) < fsd )
                    break;
            }
            SPI_setClockDivider(module, div);
        }
        else // Fspi can be set to the max
        {
            SPI_setClockDivider(module, SPI_PBCLOCK_DIV2);
        }
        
        SPI_begin(module);
    }
    
    /* Initialization failed */
    else
    {
        s |= STA_NOINIT;
    }

    Stat = s;

    disableSD(module);

    return Stat;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/* PF_BYTE drv : Physical drive number (0)                               */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(PF_BYTE drv)
{
    if (drv)
        return STA_NOINIT; /* Supports only single drive */
    return Stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/* PF_BYTE drv : Physical drive nmuber (0)                               */
/* PF_BYTE *buff : Pointer to the data buffer to store read data         */
/* DWORD sector : Start sector number (LBA)                              */
/* PF_BYTE count : Sector count (1..255)                                 */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(u8 module, PF_BYTE drv, PF_BYTE *buff, DWORD sector, PF_BYTE count)
{
    if (drv || !count)
        return RES_PARERR;

    if (Stat & STA_NOINIT)
        return RES_NOTRDY;

    if (!(CardType & CT_BLOCK))
        sector *= 512; /* Convert to byte address if needed */

    /* Single block read */
    if (count == 1)
    {
        #ifdef SD_DEBUG
        debugf("Single block read\r\n");
        #endif
        
        if ((send_cmd(module, CMD17, sector) == 0) && rcvr_datablock(module, buff, 512))
        {
            count = 0;
        }
    }
    
    /* Multiple block read */
    else
    {
        #ifdef SD_DEBUG
        debugf("Multiple block read\r\n");
        #endif

        /* READ_MULTIPLE_BLOCK */
        if (send_cmd(module, CMD18, sector) == 0)
        {
            do {
                if (!rcvr_datablock(module, buff, 512))
                    break;
                buff += 512;
            } while (--count);
            send_cmd(module, CMD12, 0); /* STOP_TRANSMISSION */
        }
    }

    disableSD(module);

    return count ? RES_ERROR : RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/* PF_BYTE drv : Physical drive nmuber (0)                               */
/* PF_BYTE *buff : Pointer to the data buffer to store read data         */
/* DWORD sector : Start sector number (LBA)                              */
/* PF_BYTE count : Sector count (1..255)                                 */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write(u8 module, PF_BYTE drv, const PF_BYTE *buff, DWORD sector, PF_BYTE count)
{
    if (drv || !count)
        return RES_PARERR;

    if (Stat & STA_NOINIT)
        return RES_NOTRDY;

    if (Stat & STA_PROTECT)
        return RES_WRPRT;

    if (!(CardType & CT_BLOCK))
        sector *= 512; /* Convert to byte address if needed */

    /* Single block write */
    if (count == 1)
    {
        if ((send_cmd(module, CMD24, sector) == 0) /* WRITE_BLOCK */
        && xmit_datablock(module, buff, 0xFE))
            count = 0;
    }
    
    /* Multiple block write */
    else
    {
        if (CardType & CT_SDC)
            send_cmd(module, ACMD23, count);

        /* WRITE_MULTIPLE_BLOCK */
        if (send_cmd(module, CMD25, sector) == 0)
        {
            do {
                if (!xmit_datablock(module, buff, 0xFC))
                    break;
                buff += 512;
            } while (--count);

            if (!xmit_datablock(module, 0, 0xFD)) /* STOP_TRAN token */
                count = 1;
        }
    }
    
    disableSD(module);

    return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/* PF_BYTE drv : Physical drive number (0)                               */
/* PF_BYTE ctrl : Control code                                           */
/* void *buff : Buffer to send/receive data block                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(u8 module, PF_BYTE drv, PF_BYTE ctrl, void *buff)
{
    DRESULT res;
    PF_BYTE n, csd[16], *ptr = buff;
    DWORD csize;

    if (drv)
        return RES_PARERR;
    if (Stat & STA_NOINIT)
        return RES_NOTRDY;

    res = RES_ERROR;

    switch (ctrl)
    {

        /* Flush dirty buffer if present */
        case CTRL_SYNC:
            if (select(module))
            {
                disableSD(module);
                res = RES_OK;
            }
            break;

        /* Get number of sectors on the disk (WORD) */
        case GET_SECTOR_COUNT:
            if ((send_cmd(module, CMD9, 0) == 0) && rcvr_datablock(module, csd, 16))
            {
                /* SDv2? */
                if ((csd[0] >> 6) == 1)
                {
                    csize = csd[9] + ((WORD) csd[8] << 8) + 1;
                    *(DWORD*) buff = (DWORD) csize << 10;
                }
                
                /* SDv1 or MMCv2 */
                else
                {
                    n = (csd[5] & 15) + ((csd[10] & 128) >> 7)
                            + ((csd[9] & 3) << 1) + 2;
                    csize = (csd[8] >> 6) + ((WORD) csd[7] << 2) + ((WORD) (csd[6]
                            & 3) << 10) + 1;
                    *(DWORD*) buff = (DWORD) csize << (n - 9);
                }
                res = RES_OK;
            }
            break;

        /* Get sectors on the disk (WORD) */
        case GET_SECTOR_SIZE:
            *(WORD*) buff = 512;
            res = RES_OK;
            break;

        /* Get erase block size in unit of sectors (DWORD) */
        case GET_BLOCK_SIZE:
            /* SDv2? */
            if (CardType & CT_SD2)
            {
                /* Read SD status */
                if (send_cmd(module, ACMD13, 0) == 0)
                {
                    SPI_write(module, 0xFF);
                    /* Read partial block */
                    if (rcvr_datablock(module, csd, 16))
                    {
                        for (n = 64 - 16; n; n--)
                            SPI_write(module, 0xFF); /* Purge trailing data */

                        *(DWORD*) buff = 16UL << (csd[10] >> 4);
                        res = RES_OK;
                    }
                }
            }
            
            /* SDv1 or MMCv3 */
            else
            {
                /* Read CSD */
                if ((send_cmd(module, CMD9, 0) == 0) && rcvr_datablock(module, csd, 16))
                {
                    /* SDv1 */
                    if (CardType & CT_SD1)
                    {
                        *(DWORD*) buff = (((csd[10] & 63) << 1) + ((WORD) (csd[11]
                                & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
                    }
                    /* MMCv3 */
                    else
                    {
                        *(DWORD*) buff = ((WORD) ((csd[10] & 124) >> 2) + 1)
                                * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5)
                                        + 1);
                    }
                    res = RES_OK;
                }
            }
            break;

        /* Get card type flags (1 byte) */
        case MMC_GET_TYPE:
            *ptr = CardType;
            res = RES_OK;
            break;

        /* Receive CSD as a data block (16 bytes) */
        case MMC_GET_CSD:
            if ((send_cmd(module, CMD9, 0) == 0) /* READ_CSD */
            && rcvr_datablock(module, buff, 16))
                res = RES_OK;
            break;

        /* Receive CID as a data block (16 bytes) */
        case MMC_GET_CID:
            if ((send_cmd(module, CMD10, 0) == 0) /* READ_CID */
            && rcvr_datablock(module, buff, 16))
                res = RES_OK;
            break;

        /* Receive OCR as an R3 resp (4 bytes) */
        case MMC_GET_OCR:
            if (send_cmd(module, CMD58, 0) == 0)
            { /* READ_OCR */
                for (n = 0; n < 4; n++)
                    *((PF_BYTE*) buff + n) = SPI_write(module, 0xFF);
                res = RES_OK;
            }
            break;

        /* Receive SD status as a data block (64 bytes) */
        case MMC_GET_SDSTAT:
            if (send_cmd(module, ACMD13, 0) == 0)
            { /* SD_STATUS */
                SPI_write(module, 0xFF);
                if (rcvr_datablock(module, buff, 64))
                    res = RES_OK;
            }
            break;

        default:
            res = RES_PARERR;
    }

    disableSD(module);

    return res;
}

/*-----------------------------------------------------------------------*/
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 1ms                         */

void disk_timerproc(u8 module)
{
    static WORD pv;
    WORD p;
    PF_BYTE s;
    UINT n;

    n = Timer1; /* 1000Hz decrement timer */
    if (n)
        Timer1 = --n;

    n = Timer2;
    if (n)
        Timer2 = --n;

    p = pv;
    pv = getCD(module) & getWP(module); /* Sample socket switch */

    /* Have contacts stabled? */
    if (p == pv)
    {
        s = Stat;

        if (p & getWP(module)) /* WP is H (write protected) */
            s |= STA_PROTECT;
        else
            /* WP is L (write enabled) */
            s &= ~STA_PROTECT;

        if (p & getCD(module)) /* INS = H (Socket empty) */
            s |= (STA_NODISK | STA_NOINIT);
        else
            /* INS = L (Card inserted) */
            s &= ~STA_NODISK;

        Stat = s;
    }
}

/*---------------------------------------------------------*/
/* User Provided RTC Function for FatFs module             */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support an RTC.                     */
/* This function is not required in read-only cfg.         */

/*	The current time is returned packed into a DWORD
    (32 bit) value. The bit fields are as follows:
        bits 31:25	Year from 1980 (0..127)
        bits 24:21	Month (1..12)
        bits 20:16	Day in month (1..31)
        bits 15:11	Hour (0..23)
        bits 10:05	Minute (0..59)
        bits 04:00	Second / 2 (0..29)						*/

DWORD get_fattime(void)
{
    DWORD tmr = 0;

    // Pre-processor commands added so only use RTCC if the board is known
    // to support the RTCC ** Added 07 May 2012

    // For boards known to support the RTCC library
    #if defined (PIC32_PINGUINO) || defined (PIC32_PINGUINO_OTG)

    rtccTime pTm, cTm;
    rtccDate pDt, cDt;

    RTCC_GetTimeDate(&pTm, &pDt);	// get time and date from RTC
                                            // assumes RTC has been set and is running
                                            // OK - could be expanded to check that RTC
                                            // is running and that a valid value is
                                            // being returned by the RTC
    cTm = RTCC_ConvertTime(&pTm);	// convert time from bcd to decimal format
    cDt = RTCC_ConvertDate(&pDt);	// convert date from bcd to decimal format

    /* Pack date and time into a DWORD variable */
    //	tmr = (((DWORD) pDt.year - 80)) | ((DWORD) pDt.mon) | ((DWORD) pDt.mday)
    //			| (WORD) (pTm.hour) | (WORD) (pTm.min) | (WORD) (pTm.sec);
    tmr = cDt.year + 20;
   
    // Correction according to a newer rtcc library, with diff struct members.
  
    tmr = (tmr << 4) | cDt.month;		// shifts left 4 bits and adds monthth
    tmr = (tmr << 5) | cDt.dayofmonth;		// shifts left 5 bits and adds m.day
    tmr = (tmr << 5) | cTm.hours;		// shifts left 5 bits and adds hour
    tmr = (tmr << 6) | cTm.minutes;		// shift left 6 bits and adds minutes
    tmr = (tmr << 5) | (cTm.seconds/2);	// shifts left 5 bits and adds seconds/2

    //	For other boards use a fixed date and time of 01 Jan 2012 12:00:00
    #else

     tmr = 12 + 20;
     tmr = (tmr << 4) | 1;       // shifts left 4 bits and adds month
     tmr = (tmr << 5) | 1;    	// shifts left 5 bits and adds m.day
     tmr = (tmr << 5) | 12;    	// shifts left 5 bits and adds hour
     tmr = (tmr << 6) | 0;       // shift left 6 bits and adds minutes
     tmr = (tmr << 5) | (0/2);   // shifts left 5 bits and adds seconds/2

    #endif

    return tmr;
}
