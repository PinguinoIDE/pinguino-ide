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

#include <typedef.h>

// standard C libraries used
#include <ctype.h>              // toupper...
#include <string.h>             // memcpy...
//#include <malloc.h>           // malloc, free?
#include <spi.h>                // in order to use default SPI port
#include <spi.c>                // in order to use default SPI port
#include <sd/fileio.h>          // file I/O routines
#include <sd/sdmmc.c>           // sd/mmc card interface
#include <sd/ff.c>              // Fat Filesystem
#include <sd/diskio.c>          // card access functions

#ifdef SD_DEBUG
    //#include <__cdc.c>       // USB CDC functions
    #include <serial1.c>     // USB CDC functions
#endif

/*	----------------------------------------------------------------------------
 mount
 initializes a MEDIA structure for FILEIO access
 will mount only the first partition on the disk/card
 --------------------------------------------------------------------------*/
static FATFS _Fat;

char mount(u8 module, u8 cs)
{
    int flag, i;
    FRESULT r;
    PF_BYTE res;

    #if defined(SD_DEBUG) && defined(__SERIAL__)
    serial1init(9600);
    #endif
    
    #ifdef SD_DEBUG
    debugf("\r\n\r\n\r\n");
    #endif

    // 1. init the I/Os
    SD[module].cs = cs;
    pinmode(SD[module].cs, OUTPUT);     // make Card select an output pin
    //disableSD(module);                  // initially keep the SD card disabled
    digitalwrite(SD[module].cs, HIGH);  // Deselected = CS high

    // 2. init the spi module
    #ifdef SD_DEBUG
    debugf("Initializing SPI...\r\n");
    #endif
    SPI_setMode(module, SPI_MASTER8);   // Master mode (0x20)
    SPI_setDataMode(module, SPI_MODE1); // CKE=1 (0x100), CKP=0
    //minimum baud rate possible = FPB/1024
    SPI_setClockDivider(module, SPI_PBCLOCK_DIV1024);
    //SPI_setClock(module, 250000);       // slow (safe) clock speed
    SPI_begin(module);

    // 3. check if the card is in the slot
    #ifndef __ST7735__
    #ifdef SD_DEBUG
    debugf("Looking for a SD card ...\r\n");
    #endif
    if (!getCD(module))
    {
        FError = FE_NOT_PRESENT;
        #ifdef SD_DEBUG
        debugf("Failed!\r\n");
        #endif
        return False;
    }
    #endif
    
    // 4. initialize the card
    // The steps to switch the SD card into SPI mode should therefore be as follows:

    #ifdef SD_DEBUG
    debugf("Initializing SD card...\r\n");
    #endif
    // - Send at least 74 clock pulses to the card with CS and Data Outlines set to logic “1.”
    disableSD(module);
    for (i=0; i<80; i++)
        SPI_read(module);
    // - Set CD line low.
    enableSD(module);
    // - Send 6-byte CMD0 command “40 00 00 00 00 95” to put the card in SPI mode.
    //r = send_cmd(module, CMD0, 0);
    //disableSD(module);
    //#ifdef SD_DEBUG
    //if (r != 1)
    //    debugf("Command rejected");
    //#endif
    // - Check R1 response to make sure there are no error bits set.
    // - Send command CMD1 repeatedly until the “in-idle-state” bit in R1 response is set to “0,”
    // - and there are no error bits set. The card is now ready for read/write operations.

    // 5. memory allocation
    // We're skipping the old step 5 because there's no need for malloc
    // This takes 6k off the code size if malloc is not used elsewhere.
    // Instead, just point it to our _Fat var.
    // The FATFS struct takes only 560 bytes of mem.
    Fat = &_Fat;

    // 4. mount media
    #ifdef SD_DEBUG
    debugf("Mounting FAT filesystem...\r\n");
    #endif
    r = f_mount(0, Fat);
    if (r != FR_OK)
    {
        FError = r;
        #ifdef SD_DEBUG
        debugf("Failed (code %d)\r\n", r);
        #endif
        //free(Fat);
        return False;
    }

    // 5. check FAT
    #ifdef SD_DEBUG
    debugf("Checking FAT filesystem...\r\n");
    #endif
    const TCHAR *pth = "/";
    r = chk_mounted(module, &pth, &Fat, 0);
    if (r != FR_OK)
    {
        FError = r;
        #ifdef SD_DEBUG
        put_rc(r);
        #endif
        unmount(module);
        return False;
    }

    return True;
} // mount

/*	----------------------------------------------------------------------------
 unmount    initializes a MEDIA structure for FILEIO access
 --------------------------------------------------------------------------*/

void unmount(u8 module)
{
    f_mount(0, NULL);
    //free(Fat);
    SPI_close(module);
} // unmount

#ifdef SD_DEBUG
static void put_rc(FRESULT rc)
{
    FRESULT i;
    const char *str =
        "OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
        "INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
        "INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
        "LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";

    for (i = 0; i != rc && *str; i++)
        while (*str++);

    debugf("Failed (FR_%s)\r\n", str);
}
#endif

/*	----------------------------------------------------------------------------
 present   test if a SD card is present
 --------------------------------------------------------------------------*/

char SD_present(u8 module, u8 pin)
{
    if (mount(module, pin))
    {
        unmount(module);
        return True;
    }
    else
    {
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
unsigned listDir(u8 module, const char *path)
{
    //TODO: remove all CDC references
    long p1;
    PF_BYTE res, b;
    UINT s1, s2;
    DIR dir; /* Directory object */

    res = f_opendir(module, &dir, "/");
    #ifdef SD_DEBUG
    debugf("f_opendir?\r\n ");
    put_rc(res);
    #endif
    p1 = s1 = s2 = 0;
    debugf("\nf_readdir('%s'): ", path);

    for (;;)
    {
        res = f_readdir(module, &dir, &Finfo);
        #ifdef SD_DEBUG
        put_rc(res);
        #endif
        if ((res != FR_OK) || !Finfo.fname[0])
            break;

        if (Finfo.fattrib & AM_DIR)
        {
            s2++;
        }
        else
        {
            s1++;
            p1 += Finfo.fsize;
        }

/* what about other outputs ?
        debugf("%c%c%c%c%c ",
                (Finfo.fattrib & AM_DIR) ? 'D' : '-',
                (Finfo.fattrib & AM_RDO) ? 'R' : '-',
                (Finfo.fattrib & AM_HID) ? 'H' : '-',
                (Finfo.fattrib & AM_SYS) ? 'S' : '-',
                (Finfo.fattrib & AM_ARC) ? 'A' : '-');
        debugf("%u/%02u/%02u %02u:%02u ",
                (Finfo.fdate >> 9) + 1980,
                (Finfo.fdate >> 5) & 15, Finfo.fdate & 31, (Finfo.ftime >> 11),
                (Finfo.ftime >> 5) & 63);
        debugf(" %9u ", Finfo.fsize);
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

char isDirectory(FILINFO file)
{
    if (file.fattrib & AM_DIR)
        return True;
    else
        return False;
}

char isReadOnly(FILINFO file)
{
    if (file.fattrib & AM_RDO)
        return True;
    else
        return False;
}

char isHidden(FILINFO file)
{
    if (file.fattrib & AM_HID)
        return True;
    else
        return False;
}

char isSystem(FILINFO file)
{
    if (file.fattrib & AM_SYS)
        return True;
    else
        return False;
}

char isArchive(FILINFO file)
{
    if (file.fattrib & AM_ARC)
        return True;
    else
        return False;
}

#endif /* __FILEIO_C__ */

