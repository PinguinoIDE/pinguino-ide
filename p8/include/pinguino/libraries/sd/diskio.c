/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2009      */
/*-----------------------------------------------------------------------*/
#ifndef _DISKIOC
#define _DISKIOC

#include <sd/diskio.h>
#include <spi.h>
#include <spi.c>
#include <delayms.c>
#include <serial.c>
/* Definitions for MMC/SDC command */
#define CMD0	(0x40+0)	/* GO_IDLE_STATE */
#define CMD1	(0x40+1)	/* SEND_OP_COND (MMC) */
#define ACMD41	(0xC0+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(0x40+8)	/* SEND_IF_COND */
#define CMD9	(0x40+9)	/* SEND_CSD */
#define CMD10	(0x40+10)	/* SEND_CID */
#define CMD12	(0x40+12)	/* STOP_TRANSMISSION */
#define ACMD13	(0xC0+13)	/* SD_STATUS (SDC) */
#define CMD16	(0x40+16)	/* SET_BLOCKLEN */
#define CMD17	(0x40+17)	/* READ_SINGLE_BLOCK */
#define CMD18	(0x40+18)	/* READ_MULTIPLE_BLOCK */
#define CMD23	(0x40+23)	/* SET_BLOCK_COUNT (MMC) */
#define ACMD23	(0xC0+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(0x40+24)	/* WRITE_BLOCK */
#define CMD25	(0x40+25)	/* WRITE_MULTIPLE_BLOCK */
#define CMD55	(0x40+55)	/* APP_CMD */
#define CMD58	(0x40+58)	/* READ_OCR */

/* Port Controls  (Platform dependent) */
#define CS_LOW()  SD_CS = 0	/* MMC CS = L */
#define CS_HIGH() SD_CS = 1	/* MMC CS = H */

/* Port Controls  (Platform dependent) */
#define SELECT()	SD_CS = 0	/* MMC CS = Low */
#define DESELECT()	SD_CS = 1	/* MMC CS = High */

#define	FCLK_SLOW()	(CONFIG = 0x22)	/* Set slow clock (100k-400k) */
#define	FCLK_FAST()	(CONFIG = 0x21)	/* Set fast clock (depends on the CSD) */

#define NCR_TIMEOUT     (u8)20        //Byte times before command response is expected (must be at least 8)
#define WRITE_TIMEOUT   (u32)0xA0000  //SPI byte times to wait before timing out when the media is performing a write operation (should be at least 250ms for SD cards).

/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/
void SPI_init(u8 sync_mode, u8 bus_mode, u8 smp_phase);
u8 SPI_write(u8 datax);
u8 SPI_read();

static volatile
DSTATUS Stat = STA_NOINIT;	/* Disk status */

static u16 CardType;

/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static u8 wait_ready (void)
{
	u8 res;
	u16 tmr = 500;	/* Wait for ready in timeout of 500ms */

	do
		res = SPI_read();
	while ((res != 0xFF) && (tmr--));

	return res;
}

/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/

static
void deselect (void)
{
	CS_HIGH();
	SPI_write(0xFF);		/* Dummy clock (force DO hi-z for multiple slave SPI) */
}

/*-----------------------------------------------------------------------*/
/* Select the card and wait ready                                        */
/*-----------------------------------------------------------------------*/

static
int select (void)	/* 1:Successful, 0:Timeout */
{
	CS_LOW();
	SPI_write(0xFF);		/* Dummy clock (force DO enabled) */

	if (wait_ready()) return 1;	/* OK */
	deselect();
	return 0;	/* Timeout */
}

/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/

static void release_spi (void)
{
	DESELECT();
	SPI_read();
}
static void power_off (void)
{
	SELECT();				/* Wait for card ready */
	wait_ready();
	release_spi();

	ENABLE = 0;				/* Disable SPI2 */
	Stat |= STA_NOINIT;	/* Force uninitialized */
	return;
}

/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

static u8 send_cmd (
	u8 cmd,		/* Command byte */
	u32 arg		/* Argument */
)
{
	u8 n, res;
	u32 longtimeout;

	if (cmd & 0x80) {	/* ACMD<n> is the command sequense of CMD55-CMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Send command packet */
	SELECT();
	SPI_write(cmd);						/* Start + Command index */
	SPI_write((u8)(arg >> 24));		/* Argument[31..24] */
	SPI_write((u8)(arg >> 16));		/* Argument[23..16] */
	SPI_write((u8)(arg >> 8));			/* Argument[15..8] */
	SPI_write((u8)arg);				/* Argument[7..0] */
	n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;			/* Valid CRC for CMD0(0) */
	if (cmd == CMD8) n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
	if (cmd == CMD12) n = 0xC3;
	SPI_write(n);

	/* Receive command response */
	if (cmd == CMD12) SPI_read();		/* Skip a stuff byte when stop reading */
	n = NCR_TIMEOUT;								/* Wait for a valid response in timeout of 10 attempts */
	do
		res = SPI_read();
	while ((res == 0xFF) && (--n) );

	if(cmd==CMD12)
	{
		longtimeout = WRITE_TIMEOUT;
		do
			res = SPI_read();
		while ((res == 0x00) && (--longtimeout));
		res = 0x00;
	}

	SPI_write(0xFF);
	if((cmd != CMD9)&&(cmd != CMD10)&&(cmd != CMD17)&&(cmd != CMD18)&&(cmd != CMD24)&&(cmd != CMD25))
		DESELECT();

	return res;			/* Return with the response value */
}
 
/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(void)
{
	u8 n, cmd, ty, ocr[4], rep, timeout;
	u16 tmr;
	if (Stat & STA_NODISK) return Stat;	/* No card in the socket */

	SPI_init(2,0,0);							/* Force socket power on */

	for (n = 10; n; n--) SPI_write(0xFF);	/* Dummy clocks */
	ty = 0;
	timeout=100;
// Trying to enter Idle state
	do {
		DESELECT();
		SPI_write(0xFF);
		SELECT();
		rep = send_cmd(CMD0,0);
	} while ((rep != 1) && (--timeout) );
    if(timeout == 0)
    {
		DESELECT();
		SPI_write(0xFF);
		SELECT();
		rep = send_cmd(CMD12,0);
		rep = send_cmd(CMD0,0);
		if (rep != 1) 	return STA_NOINIT;
	}

	rep = send_cmd(CMD8, 0x1AA);

	if ( rep == 1) {	/* SDHC */
		for (n = 0; n < 4; n++) ocr[n] = SPI_read();		/* Get trailing return value of R7 resp */

		if (ocr[2] == 0x01 && ocr[3] == 0xAA) {				/* The card can work at vdd range of 2.7-3.6V */
			for (tmr = 25000; tmr && send_cmd(ACMD41, 1UL << 30); tmr--) ;	/* Wait for leaving idle state (ACMD41 with HCS bit) */

			if (tmr && send_cmd(CMD58, 0) == 0) {		/* Check CCS bit in the OCR */
				for (n = 0; n < 4; n++) ocr[n] = SPI_read();
				ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* SDv2 */
			}
		}

	} else {							/* SDSC or MMC */

		if (send_cmd(ACMD41, 0) <= 1) 	{
			ty = CT_SD1; cmd = ACMD41;	/* SDv1 */
		} else {
			ty = CT_MMC; cmd = CMD1;	/* MMCv3 */
		}

		for (tmr = 25000; tmr && send_cmd(cmd, 0); tmr--) ;	/* Wait for leaving idle state */

		if (!tmr || send_cmd(CMD16, 512) != 0)			/* Set R/W block length to 512 */
			ty = 0;

	}

	CardType = ty;
	release_spi();

	if (ty) {			/* Initialization succeded */
		Stat &= ~STA_NOINIT;	/* Clear STA_NOINIT */
    	FCLK_FAST();
	} else {			/* Initialization failed */
		power_off();
	}
	return Stat;
}
#ifdef _USE_TFF
/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/

static
int rcvr_datablock (	/* 1:OK, 0:Failed */
	byte *buff,			/* Data buffer to store received data */
	word btr			/* Byte count (must be multiple of 4) */
)
{
	byte token, i;
	word tmr = 1000, cnt = btr;

	do {							/* Wait for data packet in timeout of 100ms */
		for(i=0; i<100; i++);
		token = SPI_read();
	} while ((token == 0xFF) && tmr--);

	if(token != 0xFE) return 1;		/* If not valid data token, retutn with error */

	if(tmr) {
		do
			*buff++ = SPI_read();
		while (--cnt);
			/* Skip CRC */
		SPI_read();SPI_read();
	}
	return 0;						/* Return with success */
}

static
int xmit_datablock (	/* 1:OK, 0:Failed */
	const byte *buff,	/* 512 byte data block to be transmitted */
	byte token			/* Data token */
)
{
	byte resp;
	word bc = 512;

	if (!wait_ready()) return 1;

	SPI_write(token);		/* Xmit a token */
	if (token != 0xFD) {	/* Not StopTran token */
		do { /* Xmit the 512 byte data block to the MMC */
			SPI_write(*buff++);
			SPI_write(*buff++);
		} while (bc -= 2);

		SPI_write(0xFF);
		SPI_write(0xFF);
		resp = SPI_read();		/* xchg_spi(0xFF)Receive a data response */
		if ((resp & 0x1F) == 0x05){	/* If not accepted, return with error */
			for (bc = 5000; SPI_read() != 0xFF && bc; bc--) ;	/* Wait ready */
			if(bc) return 0;
		}
	}
	return 1;
}

/*-----------------------------------------------------------------------*/
/* Read Sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	u8 *buff,		/* Pointer to the read buffer (NULL:Read bytes are forwarded to the stream) */
	u32 lba,		/* Sector number (LBA) */
	u8 count		/* Sector count (1..255) */
)
{
	DRESULT res;
	u8 rc;
	u16 cnt = 512;
	if (Stat & STA_NOINIT) return RES_NOTRDY;
	if (!(CardType & CT_BLOCK)) lba *= 512;		/* Convert to byte address if needed */

	res = RES_ERROR;
	if (count == 1) {
		SPI_read();SPI_read();select();SPI_read();SPI_read();
		rc = send_cmd(CMD17, lba);

		if((rc==0) && !rcvr_datablock(buff, cnt))
			res = RES_OK;
	}
	else {				/* Multiple block read */
		if (send_cmd(CMD18, lba) == 0) {	/* READ_MULTIPLE_BLOCK */
			do {
				if (rcvr_datablock(buff, 512)) break;
				buff += 512;
			} while (--count);
			if (count == 0) res= RES_OK;
			send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}

	release_spi();

	return res;
}

/*-----------------------------------------------------------------------*/
/* Write sector                                                  */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	const u8 *buff,	/* Pointer to the bytes to be written (NULL:Initiate/Finalize sector write) */
	u32 sa,			/* Number of bytes to send, Sector number (LBA) or zero */
	u8 count				/* Sector count (1..255) */
)
{
	DRESULT res;
	static u16 wc;
	if (Stat & STA_NOINIT) return RES_NOTRDY;
	if (Stat & STA_PROTECT) return RES_WRPRT;

	if (!(CardType & CT_BLOCK)) sa *= 512;	/* Convert to byte address if needed */
	res = RES_ERROR;
	if (count == 1) {		/* Single block write */
		if ( (send_cmd(CMD24, sa) == 0)	/* WRITE_BLOCK */
			&& !xmit_datablock(buff,0xFE))
			res = RES_OK;
	}
	else {				/* Multiple block write */
		if (CardType & CT_SDC) send_cmd(ACMD23, count);
		if (send_cmd(CMD25, sa) == 0) {	/* WRITE_MULTIPLE_BLOCK */
			do {
				if (xmit_datablock(buff, 0xFC)) break;
				buff += 512;
			} while (--count);
			if (!xmit_datablock(0, 0xFD) && (count==0))	/* STOP_TRAN token */
				res = RES_OK;
		}
	}
    deselect();
	return res;
}
/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	byte cmd,		/* Control code */
	void *buff		/* Buffer to send/receive data block */
)
{
	DRESULT res;
	byte n, csd[16], *ptr = buff;
	dword csz;

	if (Stat & STA_NOINIT) return RES_NOTRDY;

	res = RES_ERROR;
	switch (cmd) {
	case CTRL_SYNC :	/* Flush write-back cache, Wait for end of internal process */
		if (select()) res = RES_OK;
		break;

	case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (word) */
		if ((send_cmd(CMD9, 0) == 0) && !rcvr_datablock(csd, 16)) {
			if ((csd[0] >> 6) == 1) {	/* SDv2? */
				csz = csd[9] + ((word)csd[8] << 8) + ((dword)(csd[7] & 63) << 16) + 1;
				*(dword*)buff = csz << 10;
			} else {					/* SDv1 or MMCv3 */
				n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
				csz = (csd[8] >> 6) + ((word)csd[7] << 2) + ((word)(csd[6] & 3) << 10) + 1;
				*(dword*)buff = csz << (n - 9);
			}
			res = RES_OK;
		}
		break;

	case GET_BLOCK_SIZE :	/* Get erase block size in unit of sectors (dword) */
		if (CardType & CT_SD2) {	/* SDv2? */
			if (send_cmd(ACMD13, 0) == 0) {		/* Read SD status */
				SPI_write(0xFF);
				if (!rcvr_datablock(csd, 16)) {				/* Read partial block */
					for (n = 64 - 16; n; n--) SPI_write(0xFF);	/* Purge trailing data */
					*(dword*)buff = 16UL << (csd[10] >> 4);
					res = RES_OK;
				}
			}
		} else {					/* SDv1 or MMCv3 */
			if ((send_cmd(CMD9, 0) == 0) && !rcvr_datablock(csd, 16)) {	/* Read CSD */
				if (CardType & CT_SD1) {	/* SDv1 */
					*(dword*)buff = (((csd[10] & 63) << 1) + ((word)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
				} else {					/* MMCv3 */
					*(dword*)buff = ((word)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
				}
				res = RES_OK;
			}
		}
		break;

	case MMC_GET_TYPE :		/* Get card type flags (1 byte) */
		*ptr = CardType;
		res = RES_OK;
		break;

	case MMC_GET_CSD :	/* Receive CSD as a data block (16 bytes) */
		if ((send_cmd(CMD9, 0) == 0)	/* READ_CSD */
			&& !rcvr_datablock(buff, 16))
			res = RES_OK;
		break;

	case MMC_GET_CID :	/* Receive CID as a data block (16 bytes) */
		if ((send_cmd(CMD10, 0) == 0)	/* READ_CID */
			&& !rcvr_datablock(buff, 16))
			res = RES_OK;
		break;

	case MMC_GET_OCR :	/* Receive OCR as an R3 resp (4 bytes) */
		if (send_cmd(CMD58, 0) == 0) {	/* READ_OCR */
			for (n = 0; n < 4; n++)
				*((byte*)buff+n) = SPI_read();
			res = RES_OK;
		}
		break;

	case MMC_GET_SDSTAT :	/* Receive SD statsu as a data block (64 bytes) */
		if ((CardType & CT_SD2) && send_cmd(ACMD13, 0) == 0) {	/* SD_STATUS */
			SPI_write(0xFF);
			if (!rcvr_datablock(buff, 64))
				res = RES_OK;
		}
		break;

	default:
		res = RES_PARERR;
	}

	deselect();

	return res;
}
#endif
#endif //_USE_TFF
#ifdef _USE_PFF
/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
	u8 *buff,		/* Pointer to the read buffer (NULL:Read bytes are forwarded to the stream) */
	u32 lba,		/* Sector number (LBA) */
	u16 ofs,		/* Byte offset to read from (0..511) */
	u16 cnt		/* Number of bytes to read (ofs + cnt mus be <= 512) */
)
{
	DRESULT res;
	u8 rc, tmr = 200;
	u16 bc;

	if (!(CardType & CT_BLOCK)) lba *= 512;		/* Convert to byte address if needed */

	res = RES_ERROR;
	if (send_cmd(CMD17, lba) == 0) {		/* READ_SINGLE_BLOCK */
		do							/* Wait for data packet in timeout of 200ms */
			rc = SPI_read();
		while (rc == 0xFF && (tmr--));
	}

	if(tmr) {
		if (rc == 0xFE) {				/* A data packet arrived */
			bc = 514 - ofs - cnt;


			/* Skip leading bytes */
			if (ofs) {
				do SPI_read(); while (--ofs);
			}

			/* Receive a part of the sector */
				do
					*buff++ = SPI_read();
				while (--cnt);

			/* Skip trailing bytes and CRC */
			do SPI_read(); while (--bc);

			res = cnt ? 1 : RES_OK;
		}
	}

	release_spi();

	return res;
}

/*-----------------------------------------------------------------------*/
/* Write partial sector                                                  */
/*-----------------------------------------------------------------------*/

DRESULT disk_writep (
	const u8 *buff,	/* Pointer to the bytes to be written (NULL:Initiate/Finalize sector write) */
	u32 sa			/* Number of bytes to send, Sector number (LBA) or zero */
)
{
	DRESULT res;
	u16 bc;
	static u16 wc;


	res = RES_ERROR;

	if (buff) {		/* Send data bytes */
		bc = (u16)sa;
		while (bc && wc) {		/* Send data bytes to the card */
			SPI_write(*buff++);
			wc--; bc--;
		}
		res = RES_OK;
	} else {
		if (sa) {	/* Initiate sector write process */
			if (!(CardType & CT_BLOCK)) sa *= 512;	/* Convert to byte address if needed */
			if (send_cmd(CMD24, sa) == 0) {			/* WRITE_SINGLE_BLOCK */
				SPI_write(0xFF); SPI_write(0xFE);		/* Data block header */
				wc = 512;							/* Set byte counter */
				res = RES_OK;
			}
		} else {	/* Finalize sector write process */
			bc = wc + 2;
			while (bc--) SPI_write(0);	/* Fill left bytes and CRC with zeros */
			if ((SPI_read() & 0x1F) == 0x05) {	/* Receive data resp and wait for end of write process in timeout of 300ms */
				for (bc = 65000; SPI_read() != 0xFF && bc; bc--) ;	/* Wait ready */
				if (bc) res = RES_OK;
			}
			release_spi();
		}
	}

	return res;
}
#endif
/********************************************************************
 * Function:        void PrintSectorData( BYTE* data )
 *
 * PreCondition:    None
 *
 * Input:           Pointer to a 512 byte buffer
 *
 * Output:          Humen readable data
 *
 * Side Effects:    None
 *
 * Overview:        Data is outputed in groups of 16 bytes per row
 *
 * Note:            None
 *******************************************************************/
/*
void disk_printp( u8* datx )
{
	u16 k, px;

	for(k = 0; k < 512; k++)
	{
		serial_printf("%2X ",datx[k]);

		if( ((k + 1) % 16) == 0)
		{
			serial_printf("  ");

			for(px = (k - 15); px <= k; px++)
			{
				if( ((datx[px] > 33) && (datx[px] < 126)) || (datx[px] == 0x20) )
				{
					serial_printf("%c ",datx[px]);
				}
				else
				{
					serial_printf(".");
				}
			}

			serial_printf("\n\r");
		}
	}

	return;
}
*/
#endif
