/*
 * Project name:
     FatFs
 * Copyright:
     (c) Jonas Andersson (jonas@haksberg.net) 2009
 * Revision History:
 1st July 2012
 	modified by Pinguino Team according to Pinguino Project
 SD Library requested 
   * Description:

 * Test configuration:
     MCU:             PIC18F26J50
     Dev.Board:       pinguino 18f26j50 or clone
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
#define CDC_PRINT // to add if you use SD.printSector or SD.list and CDC communication

void setup()
{
  Delayms(2000);
  CDC.printf("Press RETURN to start.\r\n");
}
void loop()
{
  FRESULT FResult;
  u8 sector_buffer[512];
  u8 data_buffer_32[32];
  Fatfs fs;
  u16 br;

// File to read
  u8 fileName[]="niceday.txt";
  u8 folder[] = "/"; // Root directory
// wait for RETURN key to start
  while (CDC.getKey() != '\r')Delayms(100);

  CDC.printf("\r\nPIC is online...\r\nInitializing SD-card.\r\n");
  Delayms(100);
  CDC.printf("\r\nAttempting to mount file system.\r\n");
  Delayms(100);
  FResult = SD.mount(&fs);
  if( FResult == FR_OK )
  {
    CDC.printf("\r\nAttempting to open file %s.\r\n",fileName);
    Delayms(100);
	// open file
    FResult = SD.open(fileName);
    if( FResult == FR_OK )
    {
      CDC.printf("\r\nSuccesful opening - Starting to read the file.\r\n");
      // read file and print it until it ends
       do
      {
      // Read 31 bytes from the file
       if( (FResult = SD.read(data_buffer_32, 31, &br)) == FR_OK )
        {
           Delayms(100);
       // printf() needs a C-string (NULL terminated)
          data_buffer_32[br] = 0;
          CDC.printf("%s",data_buffer_32);
          Delayms(100);
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

	// do a directory listing and list all files on the SD-card
  CDC.printf("\r\nDirectory and File listing\r\n");

  FResult = SD.list(folder);

	// unmount drive
  CDC.printf("\r\nAttempting to UNmount file system.");

  FResult = SD.mount(NULL); // = unmount

  if( FResult != FR_OK )
  {
    CDC.printf("\r\nError trying to UNmount filesystem; %c.\r\n",(FResult + 0x30));
    while( 1 );
  }

  CDC.printf("\r\nFile system succesfully unmounted.\r\n");
	// read one sector from SD-card
  if( (FResult = SD.readSector(sector_buffer, fs.dirbase, 0, 512)) == FR_OK )
  {
    CDC.printf("\r\nDisk_readp is ok.\r\nPrinting sector %ld.\r\n", fs.dirbase);
    SD.printSector(sector_buffer);
  }
  else
  {
    CDC.printf("\r\nError trying to use disk_readp; %c.\r\n",(FResult + 0x30));
    while( 1 );
  }
 
}
