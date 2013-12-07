/*
 ** fileio.h
 **
 ** FAT16 support
 **
 ** 01/16/03	v1.0 LDJ PIC18
 ** 08/17/06 v2.0 LDJ PIC24 and SDMMC cards porting
[30-03-12][hgmvanbeek@gmail.com][Some cards have no card detect and no write protect]
07 May 2012	As part of providing support for PIC32 Pinguino Micro and
				potentially other cards removed #if !defined (PIC32_Pinguino) etc
				and #endif under "globals" so that SDCS is declared in the same
				manner for all cards and then set via the SD.mount() function.
 */

#ifndef __FILEIO_H__
#define __FILEIO_H__

#include "sdmmc.h"
#include "ff.h"

// FILEIO ERROR CODES
#define FE_IDE_ERROR        1   // IDE command execution error
#define FE_NOT_PRESENT      2   // CARD not present
#define FE_PARTITION_TYPE   3   // WRONG partition type
#define FE_INVALID_MBR      4   // MBR sector invalid signtr
#define FE_INVALID_BR       5   // Boot Record invalid signtr
#define FE_MEDIA_NOT_MNTD   6   // Media not mounted
#define FE_FILE_NOT_FOUND   7   // File not found,open for read
#define FE_INVALID_FILE     8   // File not open
#define FE_FAT_EOF          9   // attempt to read beyond EOF
#define FE_EOF             10   // Reached the end of file
#define FE_INVALID_CLUSTER 11   // Invalid cluster > maxcls
#define FE_DIR_FULL        12   // All root dir entry are taken
#define FE_MEDIA_FULL      13   // All clusters taken
#define FE_FILE_OVERWRITE  14   // A file with same name exist
#define FE_CANNOT_INIT     15   // Cannot init the CARD
#define FE_CANNOT_READ_MBR 16   // Cannot read the MBR
#define FE_MALLOC_FAILED   17   // Could not allocate memory
#define FE_INVALID_MODE    18   // Mode was not r.w.
#define FE_FIND_ERROR      19   // Failure during FILE search

typedef struct {
	char filename[9];
	char ext[4];
	char attrib;
	char reserved;
	char time;
	int ctime;
	int cdate;
	int latime;
	int eaindex;
	int ltime;
	int ldate;
	int cluster;
	long size;
} DIRTABLE;

// globals
char SDCS;

char FError; // error mail box
FATFS *Fat; // mounting info for storage device

DWORD AccSize; /* Work register for fs command */
PF_WORD AccFiles, AccDirs;
FILINFO Finfo;

// file attributes
#define ATT_RO      1           // attribute read only
#define ATT_HIDE    2           // attribute hidden
#define ATT_SYS     4           //  "       system file
#define ATT_VOL     8           //  "       volume label
#define ATT_DIR     0x10        //  "       sub-directory
#define ATT_ARC     0x20        //  "       (to) archive
#define ATT_LFN     0x0f        // mask for Long File Name
#define FOUND       2           // directory entry match
#define NOT_FOUND   1           // directory entry not found

#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)

// macros to extract words and longs from a byte array
// watch out, a processor trap will be generated if the address
// is not word aligned
#define ReadW( a, f) *(unsigned short*)(a+f)
#define ReadL( a, f) *(unsigned short*)(a+f) + \
(( *(unsigned short*)(a+f+2))<<16)

// this is a "safe" versions of ReadW
// to be used on odd address fields
#define ReadOddW( a, f) (*(a+f) + ( *(a+f+1) << 8))

// prototypes
char mount(unsigned char);
void unmount(void);

char isReadOnly(FILINFO file);
char isHidden(FILINFO file);
char isSystem(FILINFO file);
char isArchive(FILINFO file);

//unsigned listTYPE(char *listname, long *listsize, int max, const char *ext);
unsigned listTYPE(DIRTABLE *list, int max, const char *ext);
//unsigned listTYPE(char *list, int max, const char *ext);

unsigned listDir(const char *path);

#endif /* __FILEIO_H__ */
