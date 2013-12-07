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
   Application to demonstrate the capabilities of (Tiny) FatFs lib, an extension of Petit Fatfs.
   Extended doc at http://elm-chan.org/fsw/ff/00index_e.html
   Functions only used here :
   SD.mount(...)
   SD.openDir(...)
   SD.readDir(...)
   SD.open(...)
   SD.read(...)
   SD.write(...)
   SD.readSector(...)
   SD.printSector(...)
   see serialtffselect.pde for more functions.   

   * Test configuration:
     MCU:             PIC18F26J50 & 18F47J53
     Dev.Board:       pinguino 18F26J50 & 18F47J53-A or clone
     Oscillator:      HSPLL 8 or 20 MHz  (raised with PLL to 48.000MHz)
 
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
#define SERIAL_PRINT // to add if you use CDC.printSector or CDC.list and serial communication
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
  Serial.begin(9600);
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
  Serial.printf("Press RETURN to start.\r\n");
  while (Serial.getKey() != '\r');

  Serial.printf("\r\nPIC is online...\r\nInitializing SD-card.\r\n");
  Serial.printf("\r\nAttempting to mount file system.\r\n");
  rc = SD.mount(0, &fs);
  rc = SD.openDir(&dirroot, "/");
  if( rc == RES_OK )
  {
    	// do a directory listing and list all files on the SD-card
    Serial.printf("\r\nDirectory and File listing\r\n");
    for (;;) {
        rc = SD.readDir(&dirroot, &fno);		// Read a directory item
        if (rc || !fno.fname[0]) break;	// Error or end of dir
        if (fno.fattrib & AM_DIR)
            Serial.printf("   <dir>  %s\r\n", fno.fname);
        else
            Serial.printf("%8lu  %s\r\n", fno.fsize, fno.fname);
    }
// ----------------------------------------------------------------------
//                        reading test
// ----------------------------------------------------------------------

    Serial.printf("\r\nAttempting to open file %s.\r\n",fileName);
	// open file
    rc = SD.open(&Fil,fileName,1);
    if( rc == FR_OK )
    {
        Serial.printf("\r\nSuccesful opening - Starting to read the file.\r\n");

      // read file and print it until it ends
        Serial.printf("\r\nFile content :\r\n");
        do {
            rc = SD.read(&Fil, Buff, lgcorbuff, &br);	// Read a chunk of file
            if (rc || !br) break;			// Error or end of file
            else {		// Type the data
                Buff[br] = 0;
                Serial.printf("%s",Buff);
                if( br < lgcorbuff) Serial.printf("\r\n");
            }
        } while (br == lgcorbuff);
    }
    else
    {
        Serial.printf("\r\nError trying to open file; %c.\r\n",(rc + 0x30));
        while( 1 );
    }

// ----------------------------------------------------------------------
//                        writting test
// ----------------------------------------------------------------------

    Serial.printf("\r\nAttempting to open file %s.\r\n",wfileName);
	// open file
    rc = SD.open(&Fil,wfileName,2);
    if( rc == FR_OK )
    {
        Serial.printf("\r\nSuccesful opening - Starting to write the file.\r\n");

        if( (rc = SD.write(&Fil, w_buffer_32, 31, &bw)) == FR_OK )
        {
  		  Serial.printf("Written bytes = %d\r\n",bw);
  		  Serial.printf("%s",w_buffer_32);
        }
        else
        {
          Serial.printf("\r\nError trying to write file; %c.\r\n",(rc + 0x30));
          while( 1 );
        }
    }
    else
    {
        Serial.printf("\r\nError trying to open file; %c.\r\n",(rc + 0x30));
        while( 1 );
    }
// ----------------------------------------------------------------------
//                        end of reading/writting test
// ----------------------------------------------------------------------
  
  }
  else if(rc == RES_ERROR)
  {
      Serial.printf("\r\nError while initiating SD-card; ");
      while( 1 );
  }
  else
  {
      Serial.printf("\r\nError trying to mount filesystem; %c.\r\n",(rc + 0x30));
      while( 1 );
  }

  Serial.printf("\r\n");

	// unmount drive
  Serial.printf("\r\nAttempting to UNmount file system.");

  rc = SD.mount(0,NULL); // = unmount
  if( rc != FR_OK )
  {
    Serial.printf("\r\nError trying to UNmount filesystem; %c.\r\n",(rc + 0x30));
    while( 1 );
  }

  Serial.printf("\r\nFile system succesfully unmounted.\r\n");

  // read one sector from SD-card
  if( (rc = SD.readSector(sector_buffer, fs.dirbase, 0, 512)) == FR_OK )
  {
    Serial.printf("\r\nDisk_readp is ok.\r\nPrinting sector %ld.\r\n", fs.dirbase);
    SD.printSector(sector_buffer);
  }
  else
  {
    Serial.printf("\r\nError trying to use disk_readp; %c.\r\n",(rc + 0x30));
    while( 1 );
  }
 
}
