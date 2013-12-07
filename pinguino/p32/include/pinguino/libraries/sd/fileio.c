/*	----------------------------------------------------------------------------
	FILE:			fileio.c
	PROJECT:		pinguino32
	PURPOSE:		SD Card file system functions
	AUTHORS:		Alfred Broda <alfredbroda@gmail.com>
					Mark Harper <markfh@f2s.com>
					Regis Blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	23 dec. 2011
	LAST RELEASE:	06 jan. 2012
	----------------------------------------------------------------------------
	based on original code by Regis Blanchot and FatFS example for PIC24
	----------------------------------------------------------------------------
 	[30-03-12][hgmvanbeek@gmail.com][Some cards have no card detect and no write protect]
	07 May 2012	As part of providing support for PIC32 Pinguino Micro and
					potentially other cards removed #if defined (PIC32_Pinguino) etc
					and #endif in function mount() so that SDCS is set via mount 
					for all cards.
*/

#ifndef __FILEIO_C__
#define __FILEIO_C__

// standard C libraries used
#include <ctype.h>      		// toupper...
#include <string.h>     		// memcpy...
//#include <malloc.h>				// malloc, free?

#include <sd/fileio.h>  	   	// file I/O routines
#include <sd/sdmmc.c>   	   	// sd/mmc card interface
#include "ff.c"					// Fat Filesystem
#include "diskio.c"				// card access functions

//#define SD_DEBUG

#ifdef SD_DEBUG
    #include <__cdc.c>          // USB CDC functions
#endif

/*	----------------------------------------------------------------------------
 mount
 initializes a MEDIA structure for FILEIO access
 will mount only the first partition on the disk/card
 --------------------------------------------------------------------------*/

static FATFS _Fat;

char mount(unsigned char pin) {
	int flag, i;
	FRESULT r;

	SDCS = pin;

	// 0. init the I/Os
#ifdef SD_DEBUG
	CDCprintln("Initializing I/Os... ");
#endif
	initSD();

#ifdef SD_DEBUG
	CDCprintf("Looking for SD slot... ");
#endif
	// 1. check if the card is in the slot
	if (!getCD()) {
		FError = FE_NOT_PRESENT;
#ifdef SD_DEBUG
		CDCprintln("Failed!");
#endif
		return False;
	}
#ifdef SD_DEBUG
	CDCprintln("OK");
#endif

	// 2. initialize the card
#ifdef SD_DEBUG
	CDCprintf("Initializing SD card... ");
#endif
	initMedia();

	// We're skipping the old step 3 because there's no need for malloc
	// This takes 6k off the code size if malloc is not used elsewhere.
	// Instead, just point it to our _Fat var.
	// The FATFS struct takes only 560 bytes of mem.
	Fat = &_Fat;

	// Mount media
#ifdef SD_DEBUG
	CDCprintf("Mounting FAT filesystem... ");
#endif
	r = f_mount(0, Fat);
	if (r != FR_OK) {
		FError = r;
#ifdef SD_DEBUG
		CDCprintln("Failed!");
#endif
		//free(Fat);
		return False;
	}
#ifdef SD_DEBUG
	CDCprintln("OK");
#endif

#ifdef SD_DEBUG
	CDCprintf("Checking FAT filesystem... ");
#endif
	const TCHAR * pth = "/";
	r = chk_mounted(&pth, &Fat, 0);
	if (r != FR_OK) {
		FError = r;
#ifdef SD_DEBUG
		CDCprintln("Failed!");
		put_rc(r);
#endif
		unmount();
		return False;
	}
#ifdef SD_DEBUG
	CDCprintln("OK");
#endif

	return True;
} // mount

/*	----------------------------------------------------------------------------
 unmount    initializes a MEDIA structure for FILEIO access
 --------------------------------------------------------------------------*/

void unmount(void) {
	f_mount(0, NULL);
	//free(Fat);
	SPI2CONCLR = 0x8000; // SPI2 OFF
} // unmount

/*	----------------------------------------------------------------------------
 present   test if a SD card is present
 --------------------------------------------------------------------------*/

char SD_present(unsigned char pin) {
	if (mount(pin)) {
		unmount();
		return True;
	} else {
		return False;
	}
}

/*	----------------------------------------------------------------------------
 Scans the current disk and compiles a list of files with a given extension
 list     array of file names max * 8
 max      number of entries
 ext      file extension we are searching for
 return   number of files found
 --------------------------------------------------------------------------*/

//unsigned listTYPE(char *listname, long *listsize, int max, const char *ext )
unsigned listTYPE(DIRTABLE *list, int max, const char *ext)
//unsigned listTYPE(char *list, int max, const char *ext )
{
	//TODO: implement

	return 0;
} // listTYPE

/* Prints the directory contents */
unsigned listDir(const char *path) {
	//TODO: remove all CDC references
	long p1;
	PF_BYTE res, b;
	UINT s1, s2;
	DIR dir; /* Directory object */

	res = f_opendir(&dir, "/");
#ifdef SD_DEBUG
	CDCprintf("f_opendir? ");
	put_rc(res);
#endif
	p1 = s1 = s2 = 0;
	CDCprintln("\nf_readdir('%s'): ", path);
	for (;;) {
		res = f_readdir(&dir, &Finfo);
#ifdef SD_DEBUG
		put_rc(res);
#endif
		if ((res != FR_OK) || !Finfo.fname[0]) {
			break;
		}

		if (Finfo.fattrib & AM_DIR) {
			s2++;
		} else {
			s1++;
			p1 += Finfo.fsize;
		}
/* what about other outputs ?
		CDCprintf("%c%c%c%c%c ",
                (Finfo.fattrib & AM_DIR) ? 'D' : '-',
				(Finfo.fattrib & AM_RDO) ? 'R' : '-',
				(Finfo.fattrib & AM_HID) ? 'H' : '-',
				(Finfo.fattrib & AM_SYS) ? 'S' : '-',
				(Finfo.fattrib & AM_ARC) ? 'A' : '-');
		CDCprintf("%u/%02u/%02u %02u:%02u ",
                (Finfo.fdate >> 9) + 1980,
				(Finfo.fdate >> 5) & 15, Finfo.fdate & 31, (Finfo.ftime >> 11),
				(Finfo.ftime >> 5) & 63);
		CDCprintf(" %9u ", Finfo.fsize);
		CDCprintln(" %-12s %s", Finfo.fname,
#if _USE_LFN
				Lfname);
#else
				"");
#endif
*/
	}

	return s1;
} // listDir

char isDirectory(FILINFO file) {
	if (file.fattrib & AM_DIR) {
		return True;
	} else {
		return False;
	}
}

char isReadOnly(FILINFO file) {
	if (file.fattrib & AM_RDO) {
		return True;
	} else {
		return False;
	}
}

char isHidden(FILINFO file) {
	if (file.fattrib & AM_HID) {
		return True;
	} else {
		return False;
	}
}

char isSystem(FILINFO file) {
	if (file.fattrib & AM_SYS) {
		return True;
	} else {
		return False;
	}
}

char isArchive(FILINFO file) {
	if (file.fattrib & AM_ARC) {
		return True;
	} else {
		return False;
	}
}
#endif /* __FILEIO_C__ */

