/*
 * Project name:
     Petit FatFs
 * Copyright:
     (c) Jonas Andersson (jonas@haksberg.net) 2009
 * Revision History:
 1st July 2012
 19 Nov. 2013
 	modified by Pinguino Team according to Pinguino Project
 SD Library requested 
   * Description:File

 * Test configuration:
     MCU:             PIC18F26J50 & 18F47J53
     Dev.Board:       pinguino 18F26J50 & 18F47J53-A or clone
     Oscillator:      HSPLL 8 or 20 MHz  (raised with PLL to 48.000MHz)
 * NOTES:
 This application allows to read/dump files from SD cards to RS232 terminal or to write
 data or messages on existing files on your SD cards.
 It requests to run a serial terminal (9600 bauds).
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
#define SPIINIT
#define SERIAL_PRINT // to add if you use MMC.printSector or MMC.list and serial communication

// Following get_line function allows to enter commands from a serial terminal
// available commands to end with Enter key :
// fi<enter> to init the exchange with your SD card (FAT16 formatted)
// fl /<enter> to list the root directory (designated by /) and known the existing files
// fo <filename><enter> to open a file among the preceding list
// fd<enter> to dump the entire file previously opened on the terminal
// fe<enter> to position the file pointer before writing (0 address only in this 
//            application - to change later
// fw <len> <value><enter> to write the text contained in value. Be careful the number of characters must be equals to <len>
// fu<enter> to unmount the SD card

// see other applications to record data periodically and read these data 
//         (raw data mode without opening a file)

u8 Line[128];  /* Serial Console input buffer */
static void get_line (u8 *buff, u8 len)
{
	u8 c, i;

	i = 0;
	for (;;) {
		c = Serial.getKey();
		if (c == '\r') break;
		if ((c == '\b') && i) i--;
		if ((c >= ' ') && (i < len - 1))
				buff[i++] = c;
	}
	buff[i] = 0;
	Serial.printf("\n");
}

void setup()
{
  Serial.begin(9600);
}
void loop()
{
  u8 *ptr;
  FRESULT FResult;
  u8 sector_buffer[512];
  u8 data_buffer_32[33];
  u8 alen[4];
  u8 na;
  int len;
  Fatfs fs;
  DIR dir;
  u16 br, bw;

// entering commands
  Serial.printf("\r\n>");
  get_line(Line, sizeof(Line));
  ptr = Line;
  delay(100);
  switch (*ptr++)
  {
    case 'f' :
      switch (*ptr++)
      {
        case 'i' :	/* fi - Mount the volume */

          Serial.printf("\r\Init. SD-card.\r\n");
          Serial.printf("\r\nAttempting to mount fs.\r\n");
          FResult = MMC.mount(&fs);
          if( FResult == FR_OK )
            Serial.printf("\r\nSuccesful mounting.\r\n");
          else if(FResult == FR_DISK_ERR)
          {
            Serial.printf("\r\nInit error; ");
            break;
          }
          else
          {
            Serial.printf("\r\nFs mounting error; %c.\r\n",(FResult + 0x30));
            break;
          }
          break;

        case 'l' :	// fl [<path>] - Directory listing.
          while (*ptr == ' ') ptr++;
// do a directory listing and list all files on the SD-card
          Serial.printf("\r\nDir & File list in %s\r\n",ptr);

          FResult = MMC.list(ptr);
          break;

        case 'o' : // fo <filename>
          while (*ptr == ' ') ptr++;

          Serial.printf("\r\nAttempting to open file %s.\r\n",ptr);
// open file
          FResult = MMC.open(ptr);
          if( FResult == FR_OK )
            Serial.printf("\r\nSuccesful opening \r\n");
          else
            Serial.printf("\r\nOpen error; %c.\r\n",(FResult + 0x30));
          break;
  
        case 'd' :	// fd - Read the file 128 bytes and dump it.

// read file and print it until it ends
          do
          {
          // Read repeatedly 32 bytes from the file
            if( (FResult = MMC.read(data_buffer_32, 32, &br)) == FR_OK )
            {
          // printf() needs a C-string (NULL terminated)
              data_buffer_32[br] = 0;
              Serial.printf("%s",data_buffer_32);
              if( br < 32) Serial.printf("\r\n");
            }
          else
          {
            Serial.printf("\r\nRead error; %c.\r\n",(FResult + 0x30));
            break;
          }
          } while( br == 32 );	// if the pf_Read reads less then 32 bytes the file has ended
          break;
        case 'e' : // fe <value>- Move file pointer
          while (*ptr == ' ') ptr++;
          na=0;
          while (*ptr != ' ') {alen[na++]= *ptr; ptr++;}
          alen[na]= 0; // length to write (ASCII value)
          len = atoi (alen); // conversion to int
          if( (FResult = MMC.lseek((u32)len)) == FR_OK)
            Serial.printf("ptr at %ld\r\n",(u32)len);
          else
            Serial.printf("Lseek error; %c\r\n",(FResult + 0x30));
          break;
        case 'w' :	// fw <len> <value>- Write the file
          while (*ptr == ' ') ptr++;
          na=0;
          while (*ptr != ' ') {alen[na++]= *ptr; ptr++;}
          alen[na]= 0; // length to write (ASCII value)
          len = atoi (alen); // conversion to int
          while (*ptr == ' ') ptr++;
          if( (FResult = MMC.write(ptr, len, &bw)) == FR_OK )
          {
            Serial.printf("Written bytes = %d\r\n",bw);
            Serial.printf("%s\r\n",ptr);
            MMC.write(0, 0, &bw);//to finalize neatly the file
      //the last cluster/sector is filled with NULL as much as necessary
          }
          else
            Serial.printf("\r\nWrite error; %c.\r\n",(FResult + 0x30));
          break;

        case 'u' :	// fu - unmount drive

        // unmount drive
          Serial.printf("\r\nAttempting to UNmount fs.");

          FResult = MMC.mount(NULL); // = unmount
          if( FResult != FR_OK )
            Serial.printf("\r\nUNmount error; %c.\r\n",(FResult + 0x30));
          else
            Serial.printf("\r\nSuccesful unmount.\r\n");
          break;

    }
  }
}
