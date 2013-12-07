/*
 * Project name:
     FatFs
 * Copyright:
     (c) Jonas Andersson (jonas@haksberg.net) 2009
 * Revision History:
 Thu Nov 19 2013
 	modified by Pinguino Team according to Pinguino Project
 SD Library requested 
   * Description:
     This is similar to serialtffdemo.pde replacing serial prints by CDC prints.
	 Refer to this example to know more.
 * Test configuration:
     MCU:             PIC18F26J50 & 18F47J53
     Dev.Board:       pinguino 18F26J50 & 18F47J53-A or clone
     Oscillator:      HSPLL 8 or 20 MHz  (raised with PLL to 48.000MHz)
 * NOTES:
*/
    /** I/O pin definitions ****************************************/
/*	circuit:
	* SD card attached to SPI bus as follows:
	** MOSI - pin 1 (RB1)
	** MISO - pin 3 (RB3)
	** CLK - pin 2  (RB2)
	** CS - pin 0   (RB0)
*/
#include <stdlib.h>
#define CDC_PRINT // to add if you use SD.printSector and CDC communication

SDFatfs fs; // FATFS alias
FIL Fil;			// File object
u8 Buff[65];		// File read buffer

/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/

u32 get_fattime (void)
{
	return	  ((u32)(2013 - 1980) << 25)	/* Year = 2012 */
			| ((u32)11 << 21)				/* Month = 1 */
			| ((u32)11 << 16)				/* Day_m = 1*/
			| ((u32)21 << 11)				/* Hour = 0 */
			| ((u32)0 << 5)				/* Min = 0 */
			| ((u32)0 >> 1);				/* Sec = 0 */
}

void setup()
{
}
void loop()
{
  FRESULT rc;
  DIR dirroot;				// Root directory object
  DIR dir;				/* Directory object */
  FILINFO fno;			/* File information object */
  u8 sector_buffer[512];
  u8 w_buffer_32[32]="** Hello from Pinguino Team !**\0";
  u16 br, bw;
  u8 lgcorbuff = sizeof(Buff) - 1;
// File to read
  u8 fileName[]="niceday.txt";
  u8 folder[] = "/"; // Root directory
// File to write
  u8 wfileName[]="towrite.txt";
// wait for RETURN key to start
  CDC.printf("Press RETURN to start.\r\n");
  while (CDC.getKey() != '\r');

  CDC.printf("\r\nPIC is online...\r\nInitializing SD-card.\r\n");
  CDC.printf("\r\nAttempting to mount file system.\r\n");
  rc = SD.mount(0, &fs);
  rc = SD.openDir(&dirroot, "/");
  if( rc == RES_OK )
  {
    	// do a directory listing and list all files on the SD-card
    CDC.printf("\r\nDirectory and File listing\r\n");
    for (;;) {
        rc = SD.readDir(&dirroot, &fno);		// Read a directory item
        if (rc || !fno.fname[0]) break;	// Error or end of dir
        if (fno.fattrib & AM_DIR)
            CDC.printf("   <dir>  %s\r\n", fno.fname);
        else
            CDC.printf("%8lu  %s\r\n", fno.fsize, fno.fname);
    }
// ----------------------------------------------------------------------
//                        reading test
// ----------------------------------------------------------------------

    CDC.printf("\r\nAttempting to open file %s.\r\n",fileName);
	// open file
    rc = SD.open(&Fil,fileName,1);
    if( rc == FR_OK )
    {
        CDC.printf("\r\nSuccesful opening - Starting to read the file.\r\n");

      // read file and print it until it ends
        CDC.printf("\r\nFile content :\r\n");
        do {
            rc = SD.read(&Fil, Buff, lgcorbuff, &br);	// Read a chunk of file
            if (rc || !br) break;			// Error or end of file
            else {		// Type the data
                Buff[br] = 0;
                CDC.printf("%s",Buff);
                if( br < lgcorbuff) CDC.printf("\r\n");
            }
        } while (br == lgcorbuff);
    }
    else
    {
      CDC.printf("\r\nError trying to open file; %c.\r\n",(rc + 0x30));
      while( 1 );
    }

// ----------------------------------------------------------------------
//                        writting test
// ----------------------------------------------------------------------

    CDC.printf("\r\nAttempting to open file %s.\r\n",wfileName);
	// open file
    rc = SD.open(&Fil,wfileName,2);
    if( rc == FR_OK )
    {
        CDC.printf("\r\nSuccesful opening - Starting to write the file.\r\n");

        if( (rc = SD.write(&Fil, w_buffer_32, 31, &bw)) == FR_OK )
        {
  		  CDC.printf("Written bytes = %d\r\n",bw);
  		  CDC.printf("%s",w_buffer_32);
        }
        else
        {
          CDC.printf("\r\nError trying to write file; %c.\r\n",(rc + 0x30));
          while( 1 );
        }
    }
    else
    {
        CDC.printf("\r\nError trying to open file; %c.\r\n",(rc + 0x30));
        while( 1 );
    }
// ----------------------------------------------------------------------
//                        end of reading/writting test
// ----------------------------------------------------------------------
  
  }
  else if(rc == RES_ERROR)
  {
      CDC.printf("\r\nError while initiating SD-card; ");
      while( 1 );
  }
  else
  {
      CDC.printf("\r\nError trying to mount filesystem; %c.\r\n",(rc + 0x30));
      while( 1 );
  }

  CDC.printf("\r\n");

	// unmount drive
  CDC.printf("\r\nAttempting to UNmount file system.");

  rc = SD.mount(0,NULL); // = unmount
  if( rc != FR_OK )
  {
    CDC.printf("\r\nError trying to UNmount filesystem; %c.\r\n",(rc + 0x30));
    while( 1 );
  }

  CDC.printf("\r\nFile system succesfully unmounted.\r\n");

  // read one sector from SD-card
  if( (rc = SD.readSector(sector_buffer, fs.dirbase, 0, 512)) == FR_OK )
  {
    CDC.printf("\r\nDisk_readp is ok.\r\nPrinting sector %ld.\r\n", fs.dirbase);
    SD.printSector(sector_buffer);
  }
  else
  {
    CDC.printf("\r\nError trying to use disk_readp; %c.\r\n",(rc + 0x30));
    while( 1 );
  }
 
}
