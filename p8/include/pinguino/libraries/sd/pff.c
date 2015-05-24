/*----------------------------------------------------------------------------/
/  Petit FatFs - FAT file system module  R0.02                 (C)ChaN, 2009
/-----------------------------------------------------------------------------/
/ Petit FatFs module is an open source software to implement FAT file system to
/ small embedded systems. This is a free software and is opened for education,
/ research and commercial developments under license policy of following trems.
/
/  Copyright (C) 2009, ChaN, all right reserved.
/
/ * The Petit FatFs module is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial use UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-----------------------------------------------------------------------------/
/ Jun 15,'09  R0.01a  First release. (Branched from FatFs R0.07b.)
/
/ Dec 14,'09  R0.02   Added multiple code page support.
/                     Added write funciton.
/                     Changed stream read mode interface.
/----------------------------------------------------------------------------*/
#include <sd/pff.h>		/* Petit FatFs configurations and declarations */
#include <sd/diskio.c>		/* Declarations of low level disk I/O functions */
#include <string.h>
#include <stdio.c>
#ifdef SERIAL_PRINT
  #include <serial.c>
#endif
#ifdef CDC_PRINT
  #include <__cdc.c>
#endif

/*--------------------------------------------------------------------------

   Private Functions

---------------------------------------------------------------------------*/

static FATFS *FatFs;	/* Pointer to the file system object (logical drive) */



/*-----------------------------------------------------------------------*/
/* String functions                                                      */
/*-----------------------------------------------------------------------*/

/* Fill memory */
static void mem_set (void* dst, int val, int cnt) {
	char *d = (char*)dst;
	while (cnt--) *d++ = (char)val;
}

/* Compare memory to memory */
static int mem_cmp (const void* dst, const void* src, int cnt) {
	const char *d = (const char *)dst, *s = (const char *)src;
	int r = 0;
	char A, B;
	do
	{
		A = *d++;
		B = *s++;
		r = A - B;
	} while (cnt-- && r == 0) ;
	return r;
}



/*-----------------------------------------------------------------------*/
/* FAT access - Read value of a FAT entry                                */
/*-----------------------------------------------------------------------*/

static CLUST get_fat (	/* 1:IO error, Else:Cluster status */
	CLUST clst	/* Cluster# to get the link information */
)
{
	u16 wc, bc, ofs;
	u8 buf[4];
	FATFS *fs = FatFs;

	if (clst < 2 || clst >= fs->max_clust)	/* Range check */
		return 1;

	switch (fs->fs_type) {
	case FS_FAT12 :
		bc = (u16)clst; bc += bc / 2;
		ofs = bc % 512; bc /= 512;
		if (ofs != 511) {
			if (disk_readp(buf, fs->fatbase + bc, ofs, 2)) break;
		} else {
			if (disk_readp(buf, fs->fatbase + bc, 511, 1)) break;
			if (disk_readp(buf+1, fs->fatbase + bc + 1, 0, 1)) break;
		}
		wc = LD_WORD(buf);
		return (clst & 1) ? (wc >> 4) : (wc & 0xFFF);

	case FS_FAT16 :
		if (disk_readp(buf, fs->fatbase + clst / 256, (u16)(((u16)clst % 256) * 2), 2)) break;
		return LD_WORD(buf);
#if _FS_FAT32
	case FS_FAT32 :
		if (disk_readp(buf, fs->fatbase + clst / 128, (u16)(((u16)clst % 128) * 4), 4)) break;
		return LD_DWORD(buf) & 0x0FFFFFFF;
#endif
	}

	return 1;	/* An error occured at the disk I/O layer */
}




/*-----------------------------------------------------------------------*/
/* Get sector# from cluster#                                             */
/*-----------------------------------------------------------------------*/

static u32 clust2sect (	/* !=0: Sector number, 0: Failed - invalid cluster# */
	CLUST clst		/* Cluster# to be converted */
)
{
	FATFS *fs = FatFs;

	clst -= 2;
	if (clst >= (fs->max_clust - 2)) return 0;		/* Invalid cluster# */
	return (u32)clst * fs->csize + fs->database;
}




/*-----------------------------------------------------------------------*/
/* Directory handling - Rewind directory index                           */
/*-----------------------------------------------------------------------*/

static FRESULT dir_rewind (
	DIR *dj			/* Pointer to directory object */
)
{
	CLUST clst;
	FATFS *fs = FatFs;

	dj->index = 0;
	clst = dj->sclust;
	if (clst == 1 || clst >= fs->max_clust)	/* Check start cluster range */
		return FR_DISK_ERR;
#if _FS_FAT32
	if (!clst && fs->fs_type == FS_FAT32)	/* Replace cluster# 0 with root cluster# if in FAT32 */
		clst = fs->dirbase;
#endif
	dj->clust = clst;						/* Current cluster */
	dj->sect = clst ? clust2sect(clst) : fs->dirbase;	/* Current sector */

	return FR_OK;	/* Seek succeeded */
}




/*-----------------------------------------------------------------------*/
/* Directory handling - Move directory index next                        */
/*-----------------------------------------------------------------------*/

static FRESULT dir_next (	/* FR_OK:Succeeded, FR_NO_FILE:End of table */
	DIR *dj			/* Pointer to directory object */
)
{
	CLUST clst;
	u16 i;
	FATFS *fs = FatFs;

	i = dj->index + 1;
	if (!i || !dj->sect)	/* Report EOT when index has reached 65535 */
		return FR_NO_FILE;

	if (!(i & (16-1))) {	/* Sector changed? */
		dj->sect++;			/* Next sector */

		if (dj->clust == 0) {	/* Static table */
			if (i >= fs->n_rootdir)	/* Report EOT when end of table */
				return FR_NO_FILE;
		}
		else {					/* Dynamic table */
			if (((i / 16) & (fs->csize-1)) == 0) {	/* Cluster changed? */
				clst = get_fat(dj->clust);		/* Get next cluster */
				if (clst <= 1) return FR_DISK_ERR;
				if (clst >= fs->max_clust)		/* When it reached end of dynamic table */
					return FR_NO_FILE;			/* Report EOT */
				dj->clust = clst;				/* Initialize data for new cluster */
				dj->sect = clust2sect(clst);
			}
		}
	}

	dj->index = i;

	return FR_OK;
}




/*-----------------------------------------------------------------------*/
/* Directory handling - Find an object in the directory                  */
/*-----------------------------------------------------------------------*/

static
FRESULT dir_find (
	DIR *dj			/* Pointer to the directory object linked to the file name */
)
{
	FRESULT res;
	u8 c, *dir;


	res = dir_rewind(dj);			/* Rewind directory object */
   if (res != FR_OK) return res;

	dir = FatFs->buf;
	do {
		res = disk_readp(dir, dj->sect, (u16)((dj->index % 16) * 32), 32)	/* Read an entry */
			? FR_DISK_ERR : FR_OK;
		if (res != FR_OK) break;
		c = dir[DIR_Name];	/* First character */
		if (c == 0) {
		   res = FR_NO_FILE;
		   break;
      }	/* Reached to end of table */
	//	if (!(dir[DIR_Attr] & AM_VOL) && !mem_cmp(dir, dj->fn, 11)) /* Is it a valid entry? */
	//		break;
		if (!(dir[DIR_Attr] & AM_VOL) )
			if( !memcmp(dir, dj->fn, 11) ) /* Is it a valid entry? */
				break;

		res = dir_next(dj);							/* Next entry */
	} while (res == FR_OK);

	return res;
}




/*-----------------------------------------------------------------------*/
/* Read an object from the directory                                     */
/* *dj : Pointer to the directory object to store read object name       */
/*-----------------------------------------------------------------------*/
#if _USE_DIR
static FRESULT dir_read(DIR *dj)
{
    FRESULT res;
    u8 adir,c,*dir;
    
	res = FR_NO_FILE;
	dir = FatFs->buf;
	while (dj->sect) {
		res = disk_readp(dir, dj->sect, (u16)((dj->index % 16) * 32), 32)	/* Read an entry */
			? FR_DISK_ERR : FR_OK;
		if (res != FR_OK) break;
		c = dir[DIR_Name];
		if (c == 0) { res = FR_NO_FILE; break; }	/* Reached to end of table */
		adir = dir[DIR_Attr] & AM_MASK;
		if (c != 0xE5 && c != '.' && !(adir & AM_VOL))	/* Is it a valid entry? */
			break;
		res = dir_next(dj);				/* Next entry */
		if (res != FR_OK) break;
	}

	if (res != FR_OK) dj->sect = 0;

	return res;
}
#endif



/*-----------------------------------------------------------------------*/
/* Pick a segment and create the object name in directory form           */
/*-----------------------------------------------------------------------*/

#ifdef _EXCVT
	static const u8 cvt[] = _EXCVT;
#endif

static FRESULT create_name (
	DIR *dj,			/* Pointer to the directory object */
	const char **path	/* Pointer to pointer to the segment in the path string */
)
{
	u8 c, d, ni, si, i, *sfn;
	const char *p;
	u8 tt[15];

	/* Create file name in directory form */
	sfn = dj->fn;
	mem_set(sfn, ' ', 11);
	si = i = 0; ni = 8;
	p = *path;

//	mem_set(tt, 0, 15);
//	memcpypgm2ram(tt, p, 15);

	for (;;) {
		c = p[si++];
		//c = *(*path + si); si++;
		//c = tt[si++];
		if (c <= ' ' || c == '/') break;	/* Break on end of segment */
		if (c == '.' || i >= ni) {
			if (ni != 8 || c != '.') break;
			i = 8; ni = 11;
			continue;
		}
#ifdef _EXCVT
		if (c >= 0x80)					/* To upper extended char (SBCS) */
			c = cvt[c - 0x80];
#endif
		if (IsDBCS1(c) && i >= ni - 1) {	/* DBC 1st byte? */
			d = p[si++];					/* Get 2nd byte */
			sfn[i++] = c;
			sfn[i++] = d;
		} else {						/* Single byte code */
			if (IsLower(c)) c -= 0x20;	/* toupper */
			sfn[i++] = c;
		}
	}
	*path = &p[si];						/* Rerurn pointer to the next segment */

	sfn[11] = (c <= ' ') ? 1 : 0;		/* Set last segment flag if end of path */

	return FR_OK;
}




/*-----------------------------------------------------------------------*/
/* Get file information from directory entry                             */
/*-----------------------------------------------------------------------*/
#if _USE_DIR
static void get_fileinfo (		/* No return code */
	DIR *dj,			/* Pointer to the directory object */
	FILINFO *fno	 	/* Pointer to store the file information */
)
{
	u8 i, c, *dir;
	char *p;


	p = fno->fname;
	if (dj->sect) {
		dir = FatFs->buf;
		for (i = 0; i < 8; i++) {	/* Copy file name body */
			c = dir[i];
			if (c == ' ') break;
			if (c == 0x05) c = 0xE5;
			*p++ = c;
		}
		if (dir[8] != ' ') {		/* Copy file name extension */
			*p++ = '.';
			for (i = 8; i < 11; i++) {
				c = dir[i];
				if (c == ' ') break;
				*p++ = c;
			}
		}
		fno->fattrib = dir[DIR_Attr];				/* Attribute */
		fno->fsize = LD_DWORD(dir+DIR_FileSize);	/* Size */
		fno->fdate = LD_WORD(dir+DIR_WrtDate);		/* Date */
		fno->ftime = LD_WORD(dir+DIR_WrtTime);		/* Time */
	}
	*p = 0;
}
#endif /* _USE_DIR */



/*-----------------------------------------------------------------------*/
/* Follow a file path                                                    */
/*-----------------------------------------------------------------------*/

static FRESULT follow_path (	/* FR_OK(0): successful, !=0: error code */
	DIR *dj,			/* Directory object to return last directory and found object */
	const char *path	/* Full-path string to find a file or directory */
)
{
	FRESULT res;
	u8 *dir;
	u8 t[15];

//	memcpypgm2ram(t, path, 15);

	while (*path == ' ') path++;		/* Skip leading spaces */
	if (*path == '/') path++;			/* Strip heading separator */
	dj->sclust = 0;						/* Set start directory (always root dir) */

//	memcpypgm2ram(t, path, 15);

	if ((u8)*path <= ' ') {			/* Null path means the root directory */
		res = dir_rewind(dj);
		FatFs->buf[0] = 0;

	} else {							/* Follow path */
		for (;;) {
			res = create_name(dj, &path);	/* Get a segment */
			if (res != FR_OK) break;
         res = dir_find(dj);
			if (res != FR_OK) {				/* Could not find the object */
				if (res == FR_NO_FILE && !*(dj->fn+11))
					res = FR_NO_PATH;
				break;
			}
			if (*(dj->fn+11)) break;		/* Last segment match. Function completed. */
			dir = FatFs->buf;				/* There is next segment. Follow the sub directory */
			if (!(dir[DIR_Attr] & AM_DIR)) { /* Cannot follow because it is a file */
				res = FR_NO_PATH; break;
			}
			dj->sclust =
#if _FS_FAT32
				((u32)LD_WORD(dir+DIR_FstClusHI) << 16) |
#endif
				LD_WORD(dir+DIR_FstClusLO);
		}
	}

	return res;
}




/*-----------------------------------------------------------------------*/
/* Check a sector if it is an FAT boot record                            */
/*-----------------------------------------------------------------------*/

static u8 check_fs (	/* 0:The FAT boot record, 1:Valid boot record but not an FAT, 2:Not a boot record, 3:Error */
	u8 *buf,	/* Working buffer */
	u32 sect	/* Sector# (lba) to check if it is an FAT boot record or not */
)
{
	if (disk_readp(buf, sect, 510, 2))		/* Read the boot sector */
	{
		return 3;
	}
	if (LD_WORD(buf) != 0xAA55)				/* Check record signature */
		return 2;

	if (!disk_readp(buf, sect, BS_FilSysType, 2) && LD_WORD(buf) == 0x4146)	/* Check FAT12/16 */
		return 0;
#if _FS_FAT32
	if (!disk_readp(buf, sect, BS_FilSysType32, 2) && LD_WORD(buf) == 0x4146)	/* Check FAT32 */
		return 0;
#endif
	return 1;
}




/*--------------------------------------------------------------------------

   Public Functions

--------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------*/
/* Mount/Unmount a Locical Drive                                         */
/*-----------------------------------------------------------------------*/

FRESULT pf_mount (
	FATFS *fs		/* Pointer to new file system object (NULL: Unmount) */
)
{
	u8 fmt, buf[36];
	u32 bsect, fsize, tsect, mclst;
	DRESULT Dresult;
	Dresult = disk_initialize();
	if(Dresult != RES_OK)
		return FR_DISK_ERR;
	FatFs = 0;
	if (!fs) return FR_OK;				/* Unregister fs object */

	if (disk_initialize() & STA_NOINIT){	/* Check if the drive is ready or not */
		return FR_NOT_READY;
	}
	/* Search FAT partition on the drive */
	bsect = 0;
	fmt = check_fs(buf, bsect);			/* Check sector 0 as an SFD format */
	if (fmt == 1) {						/* Not an FAT boot record, it may be FDISK format */
		/* Check a partition listed in top of the partition table */
		if (disk_readp(buf, bsect, MBR_Table, 16)) {	/* 1st partition entry */
			fmt = 3;
		} else {
			if (buf[4]) {					/* Is the partition existing? */
				bsect = LD_DWORD(&buf[8]);	/* Partition offset in LBA */
				fmt = check_fs(buf, bsect);	/* Check the partition */
			}
		}
	}
	if (fmt == 3) return FR_DISK_ERR;
	if (fmt) return FR_NO_FILESYSTEM;	/* No valid FAT patition is found */
	/* Initialize the file system object */
	if (disk_readp(buf, bsect, 13, sizeof(buf))) return FR_DISK_ERR;

	fsize = LD_WORD(buf+BPB_FATSz16-13);				/* Number of sectors per FAT */
	if (!fsize) fsize = LD_DWORD(buf+BPB_FATSz32-13);

	fsize *= buf[BPB_NumFATs-13];						/* Number of sectors in FAT area */
	fs->fatbase = bsect + LD_WORD(buf+BPB_RsvdSecCnt-13); /* FAT start sector (lba) */
	fs->csize = buf[BPB_SecPerClus-13];					/* Number of sectors per cluster */
	fs->n_rootdir = LD_WORD(buf+BPB_RootEntCnt-13);		/* Nmuber of root directory entries */
	tsect = LD_WORD(buf+BPB_TotSec16-13);				/* Number of sectors on the file system */
	if (!tsect) tsect = LD_DWORD(buf+BPB_TotSec32-13);
	mclst = (tsect						/* Last cluster# + 1 */
		- LD_WORD(buf+BPB_RsvdSecCnt-13) - fsize - fs->n_rootdir / 16
		) / fs->csize + 2;
	fs->max_clust = (CLUST)mclst;

	fmt = FS_FAT12;							/* Determine the FAT sub type */
	if (mclst >= 0xFF7) fmt = FS_FAT16;		/* Number of clusters >= 0xFF5 */
	if (mclst >= 0xFFF7)					/* Number of clusters >= 0xFFF5 */
#if _FS_FAT32
		fmt = FS_FAT32;
#else
		return FR_NO_FILESYSTEM;
#endif

	fs->fs_type = fmt;		/* FAT sub-type */
#if _FS_FAT32
	if (fmt == FS_FAT32)
		fs->dirbase = LD_DWORD(buf+(BPB_RootClus-13));	/* Root directory start cluster */
	else
#endif
		fs->dirbase = fs->fatbase + fsize;				/* Root directory start sector (lba) */
	fs->database = fs->fatbase + fsize + fs->n_rootdir / 16;	/* Data start sector (lba) */

	fs->flag = 0;
	FatFs = fs;

	return FR_OK;
}




/*-----------------------------------------------------------------------*/
/* Open or Create a File                                                 */
/*-----------------------------------------------------------------------*/

FRESULT pf_open (
	const char *path	/* Pointer to the file name */
)
{
	FRESULT res;
	DIR dj;
	u8 sp[12], dir[32];
	FATFS *fs = FatFs;

	if (!fs)						/* Check file system */
		return FR_NOT_ENABLED;

	fs->flag = 0;
	fs->buf = dir;
	dj.fn = sp;
	res = follow_path(&dj, path);	/* Follow the file path */
	if (res != FR_OK) return res;	/* Follow failed */
	if (!dir[0] || (dir[DIR_Attr] & AM_DIR))	/* It is a directory */
		return FR_NO_FILE;

	fs->org_clust =						/* File start cluster */
#if _FS_FAT32
		((u32)LD_WORD(dir+DIR_FstClusHI) << 16) |
#endif
		LD_WORD(dir+DIR_FstClusLO);
	fs->fsize = LD_DWORD(dir+DIR_FileSize);	/* File size */
	fs->fptr = 0;						/* File pointer */
	fs->flag = FA_OPENED;

	return FR_OK;
}




/*-----------------------------------------------------------------------*/
/* Read File                                                             */
/*-----------------------------------------------------------------------*/
#if _USE_READ

FRESULT pf_read (
	void* buff,		/* Pointer to the read buffer (NULL:Forward data to the stream)*/
	u16 btr,		/* Number of bytes to read */
	u16* br		/* Pointer to number of bytes read */
)
{
	DRESULT dr;
	CLUST clst;
	u32 sect, remain;
	u8 *rbuff = buff;
	u16 rcnt;
	FATFS *fs = FatFs;

	*br = 0;
	if (!fs) return FR_NOT_ENABLED;		/* Check file system */
	if (!(fs->flag & FA_OPENED))		/* Check if opened */
		return FR_NOT_OPENED;

	remain = fs->fsize - fs->fptr;
	if (btr > remain) btr = (u16)remain;			/* Truncate btr by remaining bytes */

	while (btr)	{									/* Repeat until all data transferred */
		if ((fs->fptr % 512) == 0) {				/* On the sector boundary? */
			if ((fs->fptr / 512 % fs->csize) == 0) {	/* On the cluster boundary? */
				clst = (fs->fptr == 0) ?			/* On the top of the file? */
					fs->org_clust : get_fat(fs->curr_clust);
				if (clst <= 1) goto fr_abort;
				fs->curr_clust = clst;				/* Update current cluster */
				fs->csect = 0;						/* Reset sector offset in the cluster */
			}
			sect = clust2sect(fs->curr_clust);		/* Get current sector */
			if (!sect) goto fr_abort;
			fs->dsect = sect + fs->csect++;
		}
		rcnt = 512 - ((u16)fs->fptr % 512);		/* Get partial sector data from sector buffer */
		if (rcnt > btr) rcnt = btr;
		dr = disk_readp(!buff ? 0 : rbuff, fs->dsect, (u16)(fs->fptr % 512), rcnt);
		if (dr) goto fr_abort;
		fs->fptr += rcnt; rbuff += rcnt;			/* Update pointers and counters */
		btr -= rcnt; *br += rcnt;
	}

	return FR_OK;

fr_abort:
	fs->flag = 0;
	return FR_DISK_ERR;
}
#endif



/*-----------------------------------------------------------------------*/
/* Write File                                                            */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE

FRESULT pf_write (
	const void* buff,	/* Pointer to the data to be written */
	u16 btw,			/* Number of bytes to write (0:Finalize the current write operation) */
	u16* bw			/* Pointer to number of bytes written */
)
{
	CLUST clst;
	u32 sect, remain;
	const u8 *p = buff;
	u16 wcnt;
	FATFS *fs = FatFs;

	*bw = 0;
	if (!fs) return FR_NOT_ENABLED;		/* Check file system */
	if (!(fs->flag & FA_OPENED))		/* Check if opened */
		return FR_NOT_OPENED;

	if (!btw) {		/* Finalize request */
		if ((fs->flag & FA__WIP) && disk_writep(0, 0)) goto fw_abort;
		fs->flag &= ~FA__WIP;
		return FR_OK;
	} else {		/* Write data request */
		if (!(fs->flag & FA__WIP))		/* Round down fptr to the sector boundary */
			fs->fptr &= 0xFFFFFE00;
	}
	remain = fs->fsize - fs->fptr;
	if (btw > remain) btw = (u16)remain;			/* Truncate btw by remaining bytes */
	while (btw)	{									/* Repeat until all data transferred */
		if (((u16)fs->fptr % 512) == 0) {				/* On the sector boundary? */
			if ((fs->fptr / 512 % fs->csize) == 0) {	/* On the cluster boundary? */
				clst = (fs->fptr == 0) ?			/* On the top of the file? */
					fs->org_clust : get_fat(fs->curr_clust);
				if (clst <= 1) goto fw_abort;
				fs->curr_clust = clst;				/* Update current cluster */
				fs->csect = 0;						/* Reset sector offset in the cluster */
			}
			sect = clust2sect(fs->curr_clust);		/* Get current sector */
			if (!sect) goto fw_abort;
			fs->dsect = sect + fs->csect++;
			if (disk_writep(0, fs->dsect)) goto fw_abort;	/* Initiate a sector write operation */
			fs->flag |= FA__WIP;
		}
		wcnt = 512 - ((u16)fs->fptr % 512);		/* Number of bytes to write to the sector */
		if (wcnt > btw) wcnt = btw;
		if (disk_writep(p, wcnt)) goto fw_abort;	/* Send data to the sector */
		fs->fptr += wcnt; p += wcnt;				/* Update pointers and counters */
		btw -= wcnt; *bw += wcnt;
		if (((u16)fs->fptr % 512) == 0) {
			if (disk_writep(0, 0)) goto fw_abort;	/* Finalize the currtent secter write operation */
			fs->flag &= ~FA__WIP;
		}
	}

	return FR_OK;

fw_abort:
	fs->flag = 0;
	return FR_DISK_ERR;
}
#endif



/*-----------------------------------------------------------------------*/
/* Seek File R/W Pointer                                                 */
/*-----------------------------------------------------------------------*/
#if _USE_LSEEK

FRESULT pf_lseek (
	u32 ofs		/* File pointer from top of file */
)
{
	CLUST clst;
	u32 bcs, sect, ifptr;
	FATFS *fs = FatFs;

	if (!fs) return FR_NOT_ENABLED;		/* Check file system */
	if (!(fs->flag & FA_OPENED))		/* Check if opened */
			return FR_NOT_OPENED;

	if (ofs > fs->fsize) ofs = fs->fsize;	/* Clip offset with the file size */
	ifptr = fs->fptr;
	fs->fptr = 0;
	if (ofs > 0) {
		bcs = (u32)fs->csize * 512;	/* Cluster size (byte) */
		if (ifptr > 0 &&
			(ofs - 1) / bcs >= (ifptr - 1) / bcs) {	/* When seek to same or following cluster, */
			fs->fptr = (ifptr - 1) & ~(bcs - 1);	/* start from the current cluster */
			ofs -= fs->fptr;
			clst = fs->curr_clust;
		} else {							/* When seek to back cluster, */
			clst = fs->org_clust;			/* start from the first cluster */
			fs->curr_clust = clst;
		}
		while (ofs > bcs) {				/* Cluster following loop */
			clst = get_fat(clst);		/* Follow cluster chain */
			if (clst <= 1 || clst >= fs->max_clust) goto fe_abort;
			fs->curr_clust = clst;
			fs->fptr += bcs;
			ofs -= bcs;
		}
		fs->fptr += ofs;
		sect = clust2sect(clst);		/* Current sector */
		if (!sect) goto fe_abort;
		fs->csect = (u8)(ofs / 512);	/* Sector offset in the cluster */
		if (ofs % 512)
			fs->dsect = sect + fs->csect++;
	}

	return FR_OK;

fe_abort:
	fs->flag = 0;
	return FR_DISK_ERR;
}
#endif



/*-----------------------------------------------------------------------*/
/* Create a Directroy Object                                             */
/*-----------------------------------------------------------------------*/
#if _USE_DIR

FRESULT pf_opendir (
	DIR *dj,			/* Pointer to directory object to create */
	const char *path	/* Pointer to the directory path */
)
{
	FRESULT res;
	u8 sp[12], dir[32];
	FATFS *fs = FatFs;


	if (!fs) {				/* Check file system */
		res = FR_NOT_ENABLED;
	} else {
		fs->buf = dir;
		dj->fn = sp;
		res = follow_path(dj, path);			/* Follow the path to the directory */
		if (res == FR_OK) {						/* Follow completed */
			if (dir[0]) {						/* It is not the root dir */
				if (dir[DIR_Attr] & AM_DIR) {	/* The object is a directory */
					dj->sclust =
#if _FS_FAT32
					((u32)LD_WORD(dir+DIR_FstClusHI) << 16) |
#endif
					LD_WORD(dir+DIR_FstClusLO);
				} else {						/* The object is not a directory */
					res = FR_NO_PATH;
				}
			}
			if (res == FR_OK)
				res = dir_rewind(dj);			/* Rewind dir */
		}
		if (res == FR_NO_FILE) res = FR_NO_PATH;
	}

	return res;
}




/*-----------------------------------------------------------------------*/
/* Read Directory Entry in Sequense                                      */
/*-----------------------------------------------------------------------*/

FRESULT pf_readdir (
	DIR *dj,			/* Pointer to the open directory object */
	FILINFO *fno		/* Pointer to file information to return */
)
{
	FRESULT res;
	u8 sp[12], dir[32];
	FATFS *fs = FatFs;


	if (!fs) {				/* Check file system */
		res = FR_NOT_ENABLED;
	} else {
		fs->buf = dir;
		dj->fn = sp;
		if (!fno) {
			res = dir_rewind(dj);
		} else {
			res = dir_read(dj);
			if (res == FR_NO_FILE) {
				dj->sect = 0;
				res = FR_OK;
			}
			if (res == FR_OK) {				/* A valid entry is found */
				get_fileinfo(dj, fno);		/* Get the object information */
				res = dir_next(dj);			/* Increment index for next */
				if (res == FR_NO_FILE) {
					dj->sect = 0;
					res = FR_OK;
				}
			}
		}
	}

	return res;
}

#endif /* _USE_DIR */

/********************************************************************
 * Function:        FRESULT scan_files (char* path)
 *
 * PreCondition:    None
 *
 * Input:           makes a directory listing of all files on the SD-card
 *
 * Output:          directory listin over USART
 *
 * Side Effects:    None
 *
 * Overview:        http://elm-chan.org/fsw/ff/pf/readdir.html
 *
 * Note:            None
 *******************************************************************/

FRESULT scan_files (char* path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    char bufpath[12];
    res = pf_opendir(&dir, path);
    if (res == FR_OK) {
        for (;;) {
            res = pf_readdir(&dir, &fno);
           if (res != FR_OK || fno.fname[0] == 0) break;
            if (fno.fattrib & AM_DIR) {
                psprintf(bufpath, "/%s", fno.fname);
                res = scan_files(bufpath);
                if (res != FR_OK) break;
            } else {
                #ifdef SERIAL_PRINT
				Serial_printf("%s/", path);
                Serial_printf("%s\r\n", fno.fname);
				#endif
                #ifdef CDC_PRINT
                CDCprintf("%s/", path);
                CDCprintf("%s\r\n", fno.fname);
				#endif
            }
        }
    }

    return res;
}


