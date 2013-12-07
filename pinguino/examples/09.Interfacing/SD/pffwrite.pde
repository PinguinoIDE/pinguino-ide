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
 
 Be aware to create a file with a sufficient size according your need before you try to write in thru SD.write()
  
 
 */
    /** I/O pin definitions ****************************************/
/*	circuit:
	* SD card attached to SPI bus as follows:
	** MOSI - pin 1 (RB1)
	** MISO - pin 3 (RB3)
	** CLK - pin 2  (RB2)
	** CS - pin 17   (RA5)
*/

void setup()
{
  Serial.begin(9600);
}
void loop()
{
  FRESULT FResult;
  u8 sector_buffer[512];
  u8 data_buffer_32[32]="** Hello from Pinguino Team !**\0";
  Fatfs fs;
  u16 bw;

// File to write
  u8 fileName[]="hello.txt";
  u8 folder[] = "/";
// wait for RETURN key to start
  Serial.printf("Press RETURN to start.\r\n");
  while (Serial.getkey() != '\r');

  Serial.printf("\r\nPIC is online...\r\nInitializing SD-card.\r\n");
  Serial.printf("\r\nAttempting to mount file system.\r\n");
  FResult = SD.mount(&fs);
 
    	// do a directory listing and list all files on the SD-card
  Serial.printf("\r\nDirectory and File listing\r\n");

  FResult = SD.list(folder);

  if( FResult == FR_OK )
  {
    Serial.printf("\r\nAttempting to open file %s.\r\n",fileName);
	// open file
    FResult = SD.open(fileName);
    if( FResult == FR_OK )
    {
      Serial.printf("\r\nSuccesful opening - Starting to write the file.\r\n");

        if( (FResult = SD.write(data_buffer_32, 31, &bw)) == FR_OK )
        {
  		  Serial.printf("Written bytes = %d\r\n",bw);
  		  Serial.printf("%s",data_buffer_32);
        }
        else
        {
          Serial.printf("\r\nError trying to write file; %c.\r\n",(FResult + 0x30));
          while( 1 );
        }
    }
    else
    {
      Serial.printf("\r\nError trying to open file; %c.\r\n",(FResult + 0x30));
      while( 1 );
    }
  }
  else if(FResult == FR_DISK_ERR)
  {
    Serial.printf("\r\nError while initiating SD-card; ");
    while( 1 );
  }
  else
  {
    Serial.printf("\r\nError trying to mount filesystem; %c.\r\n",(FResult + 0x30));
    while( 1 );
  }

  Serial.printf("\r\n");

	// unmount drive
  Serial.printf("\r\nAttempting to UNmount file system.");

  FResult = SD.mount(NULL); // = unmount

  if( FResult != FR_OK )
  {
    Serial.printf("\r\nError trying to UNmount filesystem; %c.\r\n",(FResult + 0x30));
    while( 1 );
  }

  Serial.printf("\r\nFile system succesfully unmounted.\r\n");

 
}
