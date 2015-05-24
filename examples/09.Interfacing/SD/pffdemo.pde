/*
 * Project name:
     Petit FatFs
 * Copyright:
     (c) Jonas Andersson (jonas@haksberg.net) 2009
 * Revision History:
 1st July 2012
 19 Nov. 2013 (this replaces pffread.pde and pffwrite.pde proposed on July 2012)
 	modified by Pinguino Team according to Pinguino Project
 SD Library requested 
   * Description:

 * Test configuration:
     MCU:             PIC18F26J50 + 18F47J53
     Dev.Board:       pinguinos 18F26J50 & 18F47J53-A or clone
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
#define SPIINIT
#define CDC_PRINT // to add if you use MMC.printSector or MMC.list and CDC communication

void setup()
{
  Delayms(2000);
  CDC.printf("Press RETURN to start.\r\n");
}
void loop()
{
  FRESULT FResult;
  DIR dir;				/* Directory object */
  FILINFO fno;			/* File information object */
  u8 sector_buffer[512];
  u8 data_buffer_32[32];
  u8 w_buffer_32[32]="** Hello from Pinguino Team !**\0";
  Fatfs fs;
  u16 br, bw;

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
  FResult = MMC.mount(&fs);
  if( FResult == FR_OK )
  {
    	// do a directory listing and list all files on the SD-card
    CDC.printf("\r\nDirectory and File listing\r\n");

    FResult = MMC.list(folder);

// ----------------------------------------------------------------------
//                        reading test
// ----------------------------------------------------------------------

    CDC.printf("\r\nAttempting to open file %s.\r\n",fileName);
	// open file
    FResult = MMC.open(fileName);
    if( FResult == FR_OK )
    {
      CDC.printf("\r\nSuccesful opening - Starting to read the file.\r\n");

      // read file and print it until it ends
      do
      {
      // Read 31 bytes from the file
        if( (FResult = MMC.read(data_buffer_32, 31, &br)) == FR_OK )
        {
        // printf() needs a C-string (NULL terminated)
          data_buffer_32[br] = 0;
          CDC.printf("%s",data_buffer_32);
        }
        else
        {
          CDC.printf("\r\nError trying to read file; %c.\r\n",(FResult + 0x30));
          while( 1 );
        }
      } while( br == 31 );	// if the pf_Read reads less then 31 bytes the file has ended

    }
    else
    {
      CDC.printf("\r\nError trying to open file; %c.\r\n",(FResult + 0x30));
      while( 1 );
    }

// ----------------------------------------------------------------------
//                        writting test
// ----------------------------------------------------------------------

    CDC.printf("\r\nAttempting to open file %s.\r\n",wfileName);
	// open file
    FResult = MMC.open(wfileName);
    if( FResult == FR_OK )
    {
      CDC.printf("\r\nSuccesful opening - Starting to write the file.\r\n");

        if( (FResult = MMC.write(w_buffer_32, 31, &bw)) == FR_OK )
        {
  		  CDC.printf("Written bytes = %d\r\n",bw);
  		  CDC.printf("%s",w_buffer_32);
        }
        else
        {
          CDC.printf("\r\nError trying to write file; %c.\r\n",(FResult + 0x30));
          while( 1 );
        }
    }
    else
    {
      CDC.printf("\r\nError trying to open file; %c.\r\n",(FResult + 0x30));
      while( 1 );
    }
// ----------------------------------------------------------------------
//                        end of reading/writting test
// ----------------------------------------------------------------------
  
  }
  else if(FResult == FR_DISK_ERR)
  {
    CDC.printf("\r\nError while initiating SD-card; ");
    while( 1 );
  }
  else
  {
    CDC.printf("\r\nError trying to mount filesystem; %c.\r\n",(FResult + 0x30));
    while( 1 );
  }

  CDC.printf("\r\n");

	// unmount drive
  CDC.printf("\r\nAttempting to UNmount file system.");

  FResult = MMC.mount(NULL); // = unmount
  if( FResult != FR_OK )
  {
    CDC.printf("\r\nError trying to UNmount filesystem; %c.\r\n",(FResult + 0x30));
    while( 1 );
  }

  CDC.printf("\r\nFile system succesfully unmounted.\r\n");

  // read one sector from SD-card
  if( (FResult = MMC.readSector(sector_buffer, fs.dirbase, 0, 512)) == FR_OK )
  {
    CDC.printf("\r\nDisk_readp is ok.\r\nPrinting sector %ld.\r\n", fs.dirbase);
    MMC.printSector(sector_buffer);
  }
  else
  {
    CDC.printf("\r\nError trying to use disk_readp; %c.\r\n",(FResult + 0x30));
    while( 1 );
  }
 
}
