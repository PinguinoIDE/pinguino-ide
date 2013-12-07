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

#include "ff.h"
#include "diskio.h"
#include <delay.c>
#include <digitalw.c>

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

#define xmit_spi(dat) 	writeSPI(dat)
#define rcvr_spi()		writeSPI(0xFF)
#define rcvr_spi_m(p)	SPI2BUF = 0xFF; while (!SPI2STATbits.SPIRBF); *(p) = (PF_BYTE)SPI2BUF;
#define xchg_spi (dat)  writeSPI(dat)
#define DLY_US(i)		Delayus(i)
#define CS_H()			digitalwrite(SDCS, HIGH)
#define CS_L()			digitalwrite(SDCS, LOW)

/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static
int wait_ready(void) {
	PF_BYTE d;
	UINT tmr;

	for (tmr = 5000; tmr; tmr--) { /* Wait for ready in timeout of 500ms */
		d = rcvr_spi();
		if (d == 0xFF)
			break;
		DLY_US(100);
	}

	return (d == 0xFF) ? 1 : 0;
}

/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/

#define deselect() disableSD();

/*-----------------------------------------------------------------------*/
/* Select the card and wait ready                                        */
/*-----------------------------------------------------------------------*/

static
int select(void) /* 1:Successful, 0:Timeout */
{
	enableSD();
	rcvr_spi(); /* Dummy clock (force DO enabled) */

	if (wait_ready())
		return 1; /* OK */
	deselect();
	return 0; /* Timeout */
}

/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/

static
int rcvr_datablock( /* 1:OK, 0:Failed */
PF_BYTE *buff, /* Data buffer to store received data */
UINT btr /* Byte count (must be multiple of 4) */
) {
	PF_BYTE d;
	UINT tmr;

	for (tmr = 1000; tmr; tmr--) { /* Wait for data packet in timeout of 100ms */
		d = rcvr_spi();
		if (d != 0xFF)
			break;
		DLY_US(100);
	}
	if (d != 0xFE)
		return 0; /* If not valid data token, return with error */

	do { /* Receive the data block into buffer */
		rcvr_spi_m(buff++);
		rcvr_spi_m(buff++);
		rcvr_spi_m(buff++);
		rcvr_spi_m(buff++);
	} while (btr -= 4);
	rcvr_spi(); /* Discard CRC */
	rcvr_spi();

	return 1; /* Return with success */
}

/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
static
int xmit_datablock( /* 1:OK, 0:Failed */
const PF_BYTE *buff, /* 512 byte data block to be transmitted */
PF_BYTE token /* Data token */
) {
	PF_BYTE resp;
	UINT bc = 512;

	if (!wait_ready())
		return 0;

	xmit_spi(token); /* Xmit a token */
	if (token != 0xFD) { /* Not StopTran token */
		do { /* Xmit the 512 byte data block to the MMC */
			xmit_spi(*buff++);
			xmit_spi(*buff++);
		} while (bc -= 2);
		xmit_spi(0xFF); /* CRC (Dummy) */
		xmit_spi(0xFF);
		resp = rcvr_spi(); /* Receive a data response */
		if ((resp & 0x1F) != 0x05) /* If not accepted, return with error */
			return 0;
	}

	return 1;
}
#endif	/* _READONLY */

/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

static PF_BYTE send_cmd(PF_BYTE cmd, /* Command byte */
DWORD arg /* Argument */
) {
	PF_BYTE n, res;

	if (cmd & 0x80) { /* ACMD<n> is the command sequense of CMD55-CMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1)
			return res;
	}

	/* Select the card and wait for ready */
	deselect();
	if (!select())
		return 0xFF;

	/* Send command packet */
	xmit_spi(0x40 | cmd); /* Start + Command index */
	xmit_spi((PF_BYTE)(arg >> 24)); /* Argument[31..24] */
	xmit_spi((PF_BYTE)(arg >> 16)); /* Argument[23..16] */
	xmit_spi((PF_BYTE)(arg >> 8)); /* Argument[15..8] */
	xmit_spi((PF_BYTE)arg); /* Argument[7..0] */
	n = 0x01; /* Dummy CRC + Stop */
	if (cmd == CMD0)
		n = 0x95; /* Valid CRC for CMD0(0) */
	if (cmd == CMD8)
		n = 0x87; /* Valid CRC for CMD8(0x1AA) */
	xmit_spi(n);

	/* Receive command response */
	if (cmd == CMD12)
		rcvr_spi(); /* Skip a stuff byte when stop reading */
	n = 10; /* Wait for a valid response in timeout of 10 attempts */
	do
		res = rcvr_spi();
	while ((res & 0x80) && --n);

	return res; /* Return with the response value */
}

/*--------------------------------------------------------------------------

 Public Functions

 ---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(PF_BYTE drv /* Physical drive number (0) */
) {
	PF_BYTE n, ty, cmd, buf[4];
	UINT tmr;
	DSTATUS s = 0;

	if (drv)
		return STA_NOINIT; /* Supports only single drive */
	if (Stat & STA_NODISK)
		return Stat; /* No card in the socket */

	deselect(); /* Force socket power on */
	//FCLK_SLOW();
	for (n = 10; n; n--)
		rcvr_spi(); /* 80 dummy clocks */

	CS_H();
	for (n = 10; n; n--)
		rcvr_spi(); /* 80 dummy clocks */

	ty = 0;
	if (send_cmd(CMD0, 0) == 1) { /* Enter Idle state */
		if (send_cmd(CMD8, 0x1AA) == 1) { /* SDv2? */
			for (n = 0; n < 4; n++)
				buf[n] = rcvr_spi(); /* Get trailing return value of R7 resp */
			if (buf[2] == 0x01 && buf[3] == 0xAA) { /* The card can work at vdd range of 2.7-3.6V */
				for (tmr = 1000; tmr; tmr--) { /* Wait for leaving idle state (ACMD41 with HCS bit) */
					if (send_cmd(ACMD41, 1UL << 30) == 0)
						break;
					DLY_US(1000);
				}
				if (tmr && send_cmd(CMD58, 0) == 0) { /* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++)
						buf[n] = rcvr_spi();
					ty = (buf[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2; /* SDv2 */
				}
			}
		} else { /* SDv1 or MMCv3 */
			if (send_cmd(ACMD41, 0) <= 1) {
				ty = CT_SD1;
				cmd = ACMD41; /* SDv1 */
			} else {
				ty = CT_MMC;
				cmd = CMD1; /* MMCv3 */
			}
			for (tmr = 1000; tmr; tmr--) { /* Wait for leaving idle state */
				if (send_cmd(ACMD41, 0) == 0)
					break;
				DLY_US(1000);
			}
			if (!tmr || send_cmd(CMD16, 512) != 0) /* Set R/W block length to 512 */
				ty = 0;
		}
	}
	CardType = ty;
	if (ty) {/* Initialization succeded */
		s &= ~STA_NOINIT;
		// 6. increase speed
		SPI2CON = 0; // disable the SPI2 module
		SPI2BRG = 0; // maximum possible baud rate = Fpb/2
		SPI2CON = 0x8120; // re-enable the SPI2 module
	} else {
		/* Initialization failed */
		s |= STA_NOINIT;
	}
	Stat = s;

	deselect();

	return Stat;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(PF_BYTE drv /* Physical drive number (0) */
) {
	if (drv)
		return STA_NOINIT; /* Supports only single drive */
	return Stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(PF_BYTE drv, /* Physical drive nmuber (0) */
PF_BYTE *buff, /* Pointer to the data buffer to store read data */
DWORD sector, /* Start sector number (LBA) */
PF_BYTE count /* Sector count (1..255) */
) {
	if (drv || !count)
		return RES_PARERR;
	if (Stat & STA_NOINIT)
		return RES_NOTRDY;

	if (!(CardType & CT_BLOCK))
		sector *= 512; /* Convert to byte address if needed */

	if (count == 1) { /* Single block read */
		if ((send_cmd(CMD17, sector) == 0) /* READ_SINGLE_BLOCK */
		&& rcvr_datablock(buff, 512))
			count = 0;
	} else { /* Multiple block read */
		if (send_cmd(CMD18, sector) == 0) { /* READ_MULTIPLE_BLOCK */
			do {
				if (!rcvr_datablock(buff, 512))
					break;
				buff += 512;
			} while (--count);
			send_cmd(CMD12, 0); /* STOP_TRANSMISSION */
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write(PF_BYTE drv, /* Physical drive nmuber (0) */
const PF_BYTE *buff, /* Pointer to the data to be written */
DWORD sector, /* Start sector number (LBA) */
PF_BYTE count /* Sector count (1..255) */
) {
	if (drv || !count)
		return RES_PARERR;
	if (Stat & STA_NOINIT)
		return RES_NOTRDY;
	if (Stat & STA_PROTECT)
		return RES_WRPRT;

	if (!(CardType & CT_BLOCK))
		sector *= 512; /* Convert to byte address if needed */

	if (count == 1) { /* Single block write */
		if ((send_cmd(CMD24, sector) == 0) /* WRITE_BLOCK */
		&& xmit_datablock(buff, 0xFE))
			count = 0;
	} else { /* Multiple block write */
		if (CardType & CT_SDC)
			send_cmd(ACMD23, count);
		if (send_cmd(CMD25, sector) == 0) { /* WRITE_MULTIPLE_BLOCK */
			do {
				if (!xmit_datablock(buff, 0xFC))
					break;
				buff += 512;
			} while (--count);
			if (!xmit_datablock(0, 0xFD)) /* STOP_TRAN token */
				count = 1;
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(PF_BYTE drv, /* Physical drive number (0) */
PF_BYTE ctrl, /* Control code */
void *buff /* Buffer to send/receive data block */
) {
	DRESULT res;
	PF_BYTE n, csd[16], *ptr = buff;
	DWORD csize;

	if (drv)
		return RES_PARERR;
	if (Stat & STA_NOINIT)
		return RES_NOTRDY;

	res = RES_ERROR;
	switch (ctrl) {
	case CTRL_SYNC: /* Flush dirty buffer if present */
		if (select()) {
			deselect();
			res = RES_OK;
		}
		break;

	case GET_SECTOR_COUNT: /* Get number of sectors on the disk (WORD) */
		if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
			if ((csd[0] >> 6) == 1) { /* SDv2? */
				csize = csd[9] + ((WORD) csd[8] << 8) + 1;
				*(DWORD*) buff = (DWORD) csize << 10;
			} else { /* SDv1 or MMCv2 */
				n = (csd[5] & 15) + ((csd[10] & 128) >> 7)
						+ ((csd[9] & 3) << 1) + 2;
				csize = (csd[8] >> 6) + ((WORD) csd[7] << 2) + ((WORD) (csd[6]
						& 3) << 10) + 1;
				*(DWORD*) buff = (DWORD) csize << (n - 9);
			}
			res = RES_OK;
		}
		break;

	case GET_SECTOR_SIZE: /* Get sectors on the disk (WORD) */
		*(WORD*) buff = 512;
		res = RES_OK;
		break;

	case GET_BLOCK_SIZE: /* Get erase block size in unit of sectors (DWORD) */
		if (CardType & CT_SD2) { /* SDv2? */
			if (send_cmd(ACMD13, 0) == 0) { /* Read SD status */
				rcvr_spi();
				if (rcvr_datablock(csd, 16)) { /* Read partial block */
					for (n = 64 - 16; n; n--)
						rcvr_spi(); /* Purge trailing data */
					*(DWORD*) buff = 16UL << (csd[10] >> 4);
					res = RES_OK;
				}
			}
		} else { /* SDv1 or MMCv3 */
			if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) { /* Read CSD */
				if (CardType & CT_SD1) { /* SDv1 */
					*(DWORD*) buff = (((csd[10] & 63) << 1) + ((WORD) (csd[11]
							& 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
				} else { /* MMCv3 */
					*(DWORD*) buff = ((WORD) ((csd[10] & 124) >> 2) + 1)
							* (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5)
									+ 1);
				}
				res = RES_OK;
			}
		}
		break;

	case MMC_GET_TYPE: /* Get card type flags (1 byte) */
		*ptr = CardType;
		res = RES_OK;
		break;

	case MMC_GET_CSD: /* Receive CSD as a data block (16 bytes) */
		if ((send_cmd(CMD9, 0) == 0) /* READ_CSD */
		&& rcvr_datablock(buff, 16))
			res = RES_OK;
		break;

	case MMC_GET_CID: /* Receive CID as a data block (16 bytes) */
		if ((send_cmd(CMD10, 0) == 0) /* READ_CID */
		&& rcvr_datablock(buff, 16))
			res = RES_OK;
		break;

	case MMC_GET_OCR: /* Receive OCR as an R3 resp (4 bytes) */
		if (send_cmd(CMD58, 0) == 0) { /* READ_OCR */
			for (n = 0; n < 4; n++)
				*((PF_BYTE*) buff + n) = rcvr_spi();
			res = RES_OK;
		}
		break;

	case MMC_GET_SDSTAT: /* Receive SD status as a data block (64 bytes) */
		if (send_cmd(ACMD13, 0) == 0) { /* SD_STATUS */
			rcvr_spi();
			if (rcvr_datablock(buff, 64))
				res = RES_OK;
		}
		break;

	default:
		res = RES_PARERR;
	}

	deselect();

	return res;
}

/*-----------------------------------------------------------------------*/
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 1ms                         */

void disk_timerproc(void) {
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
	pv = getCD() & getWP(); /* Sample socket switch */

	if (p == pv) { /* Have contacts stabled? */
		s = Stat;

		if (p & getWP()) /* WP is H (write protected) */
			s |= STA_PROTECT;
		else
			/* WP is L (write enabled) */
			s &= ~STA_PROTECT;

		if (p & getCD()) /* INS = H (Socket empty) */
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

DWORD get_fattime(void) {
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

static
void put_rc(FRESULT rc) {
	const char
			*str =
					"OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
						"INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
						"INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
						"LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
	FRESULT i;

	for (i = 0; i != rc && *str; i++) {
		while (*str++)
			;
	}
	CDCprintln("rc=%u FR_%s", (UINT) rc, str);
}
