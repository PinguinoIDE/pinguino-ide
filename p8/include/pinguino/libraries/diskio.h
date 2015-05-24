/*-----------------------------------------------------------------------
/  PFF - Low level disk interface modlue include file    (C)ChaN, 2009
/-----------------------------------------------------------------------*/

#ifndef _DISKIO
#define _DISKIO
#define _USE_IOCTL	1	/* 1: Enable disk_ioctl function */
#include <typedef.h>
/* Status of Disk Functions */
typedef u8	DSTATUS;


/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Function succeeded */
	RES_ERROR,		/* 1: Disk error */
	RES_WRPRT,		/* 2: Write Protected */
	RES_NOTRDY,		/* 2: Not ready */
	RES_PARERR		/* 3: Invalid parameter */
} DRESULT;


/*---------------------------------------*/
/* Prototypes for disk control functions */

DSTATUS disk_initialize (void);
DRESULT disk_read (u8*, u32, u8);
DRESULT disk_write (const u8*, u32, u8);
DRESULT disk_readp (u8*, u32, u16, u16);
DRESULT disk_writep (const u8*, u32);
DRESULT disk_ioctl (u8, void *);

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */


/* Card type flags (CardType) */
#define CT_MMC				0x01	/* MMC ver 3 */
#define CT_SD1				0x02	/* SD ver 1 */
#define CT_SD2				0x04	/* SD ver 2 */
#define CT_SDC				(CT_SD1|CT_SD2)	/* SD */
#define CT_BLOCK			0x08	/* Block addressing */

/* Command code for disk_ioctrl fucntion */
/* Generic command (used by FatFs) */
#define CTRL_SYNC			0	/* Flush disk cache (for write functions) */
#define GET_SECTOR_COUNT	1	/* Get media size (for only f_mkfs()) */
#define GET_SECTOR_SIZE		2	/* Get sector size (for multiple sector size (_MAX_SS >= 1024)) */
#define GET_BLOCK_SIZE		3	/* Get erase block size (for only f_mkfs()) */
#define CTRL_ERASE_SECTOR	4	/* Force erased a block of sectors (for only _USE_ERASE) */


/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE		10	/* Get card type */
#define MMC_GET_CSD			11	/* Get CSD */
#define MMC_GET_CID			12	/* Get CID */
#define MMC_GET_OCR			13	/* Get OCR */
#define MMC_GET_SDSTAT		14	/* Get SD status */

#endif
