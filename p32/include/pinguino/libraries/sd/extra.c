/*
 * extra.c
 *
 *  Created on: Jan 5, 2012
 *      Author: Alfred Broda <alfredbroda@gmail.com>
 */

#include "extra.h"

/* char* path : Pointer to the path name working buffer */

FRESULT scan_files(char* path)
{
	DIR dirs;
	FRESULT res;
	int i;
	char *fn;

	if ((res = f_opendir(&dirs, path)) == FR_OK) {
		i = strlen(path);
        //CDCprintln("scan path: %s", path);
		while (((res = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0])
        {
			if (_FS_RPATH && Finfo.fname[0] == '.')
				continue;

            #if _USE_LFN
			fn = *Finfo.lfname ? Finfo.lfname : Finfo.fname;
            #else
			fn = Finfo.fname;
            #endif

			if (Finfo.fattrib & AM_DIR)
            {
				AccDirs++;
                //CDCprintln("next path: %s", fn);

				/* FIXME: doubles code size!
				char * s = malloc(snprintf(NULL, 0, "%s/%s", path, fn) + 1);
				sprintf(s, "%s/%s", path, fn); */
				res = scan_files(s);

				/* FIXME: seg fault
				path[i] = '/';
				CDCprintln("#1: %s", path);
				mem_cpy(&path[i + 1], fn);
				CDCprintln("#2: %s", path);
				res = scan_files(path);
				path[i] = 0;
				CDCprintln("#3: %s", path);  */

				if (res != FR_OK)
					break;
			}
            
            else
            {
                #if 0
				CDCprintln("%s/%s\n", path, fn);
                #endif
				AccFiles++;
				AccSize += Finfo.fsize;
			}
		}
	}

	return res;
}

void sd_status(void)
{
	long p2;
	PF_BYTE res, b;
	const PF_BYTE ft[] = { 0, 12, 16, 32 };

	CDCprintln("\nf_getfree(): ");
	res = f_getfree("/", (DWORD*) &p2, &Fat);

	if (!res)
    {
		CDCprintln(" FAT type = FAT%u", ft[Fat->fs_type & 3]);
		CDCprintln(" Bytes/Cluster = %u", Fat->csize * 512UL);
		CDCprintln(" Number of FATs = %u", Fat->n_fats);
		CDCprintln(" Root DIR entries = %u", Fat->n_rootdir);
		CDCprintln(" Sectors/FAT = %u", Fat->fsize);
		CDCprintln(" Number of clusters = %u", Fat->n_fatent - 2);
		CDCprintln(" FAT start (lba) = %u", Fat->fatbase);
		CDCprintln(" DIR start (lba,cluster) = %u", Fat->dirbase);
		CDCprintln(" Data start (lba) = %u", Fat->database);

    #ifdef SD_DEBUG
	}
    else
    {
		put_rc(res);
    #endif
	}

	AccSize = AccFiles = AccDirs = 0;
	CDCprintln("\nscan_files(): ");
	res = scan_files("/");

	if (!res) {
		CDCprintln(" %u files, %u bytes.", AccFiles, AccSize);
		CDCprintln(" %u folders.", AccDirs);
		CDCprintln(" %u KB total disk space.", (Fat->n_fatent - 2)
				* (Fat->csize / 2));
		CDCprintln(" %u KB available.\r\n", p2 * (Fat->csize / 2));
    #ifdef SD_DEBUG
	} else {
		CDCprintln("Failed!");
		put_rc(res);
    #endif
	}

	if ((res = disk_ioctl(0, MMC_GET_TYPE, &b)) == RES_OK) {
		CDCprintln("MMC/SDC type: %u", b);
    #ifdef SD_DEBUG
	} else {
		CDCprintf("Could not determine card type. ");
		put_rc(res);
    #endif
	}

	if ((res = disk_ioctl(0, GET_SECTOR_COUNT, &p2)) == RES_OK) {
		CDCprintln("Drive size: %u sectors", p2);
    #ifdef SD_DEBUG
	} else {
		CDCprintf("Could not determine drive size. ");
		put_rc(res);
    #endif
	}

	if ((res = disk_ioctl(0, GET_BLOCK_SIZE, &p2)) == RES_OK) {
		CDCprintln("Erase block: %u sectors", p2);
    #ifdef SD_DEBUG
	} else {
		CDCprintf("Could not determine Erase block size. ");
		put_rc(res);
    #endif
	}

}
