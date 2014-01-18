/*	----------------------------------------------------------------------------
   FILE:       DataLoggerOTG.pde
   PROJECT:    Pinguino
   PURPOSE:    Log pseudo temperature measurements to a file on SD card
   PROGRAMER:		Mark Harper
   BOARD:      PIC32-PINGUINO-OTG	(OLIMEX) with 32.768 kHz clock crystal
   FIRST RELEASE:	17 Jan 2012
   LAST RELEASE:  18 May 2012 M Harper
   IDE RELEASE:   TBA
   ----------------------------------------------------------------------------
   Based on previous Pinguino IDE examples by Regis Blanchot
   Dump.pde				(trunk	r208) - for reading a file
   Datalogger.pde		(trunk	r208) - for data assembly into string
   modified to use new SD library based on FatFs R0.09 & 
   developed/modified for use on Pinguino by Alfred Broda
   ----------------------------------------------------------------------------
   18 May 2012 Example updated to reflect changes in SD Library, identify 
               settings for individual Pinguino Boards and identify areas 
               dependent on the built-in RTCC also changed CDC.println() 
               with multiple parameters to CDC.printf() and to use 
               Pinguino Sprintf() rather than sprintf().

   Circuit:
   * SD card is attached to SPI2 bus as follows:
      SPI   microSD Card   PIC32
      MOSI  3 = CMD/DI     SDO2
      MISO  7 = DAT0/D0    SDI2
      SCK   5 = CLK/SCLK   SCK2
      SS    ------------   ----     default SS not used - 
                                    separate pin used for CS (card select)
      (CS)  2 = CD/DAT3/CS set by SD.mount( )

                           example set for Pic32 Pinguino OTG
                           if using with another board change value
                           in SD.mount() function calls by changing
                           the define SDCS_PIN # line 
    ----------------------------------------------------------------------------
    Tested with 2G SD card and 8G SDHD card
    ----------------------------------------------------------------------------
    The first time the user presses [RETURN] the program adds a new line of data
    to the datafile and then pauses.  The next time the user presses [RETURN]
    the program lists out all the data in the file so that the user can see the
    new data added to the end of the file.
	
    The process then repeats, adding further lines of data and then displaying 
    the updated data file.
	
    The program uses pseudo (dummy) temperature data - but is easily modifiable 
    for use with say a onewire temperature sensor.  The example includes 
    remarked out lines showing the assembly of onewire temperature data 
    (variable t, data type TEMPERATURE) into the string for output to the SD 
    card.
	
    output:
       (GNU/linux) sudo minicom -o -D /dev/ttyACM0
       (Windows)   run a suitable terminal emulator AFTER the program has 
                   been uploaded to the Pinguino board and the board has
                   left the bootloader mode.
                   you may not see the first prompt but press <return>
                   
	      press <return> to start
		---------------------------------------------------------------------------*/

#define BUFFER_SIZE 32  // Size of read buffer - arbitrary length,
                        // try changing it to improve efficiency 
#define SDCS_PIN 8      // The pin for the SD Card select line

   u16      i, lines_total, bytes_to_write, bytes_written, bytes_read;
   u32      bytes_total;
   u8       filename[]	= "datalog1.csv";	// should include full path to file
// char     filename[]	= "LEVEL_0/datalog.csv";

   FIL      fil;							// File object
   FRESULT  res,	rc;
   DWORD    file_size;
   u8       read_buffer[BUFFER_SIZE];

// Arrays and variables only used with RTCC

u8	Day[7][4]		= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
u8	Month[13][4]	= {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

rtccTime 	cT, curTime;
rtccDate		cD, curDate;

void setup()
{
/* Sets Real Time Clock and Calendar
   For PIC32 Pinguino and PIC32 Pinguino OTG boards the RTCC needs to be set as the
   RTCC is used for the time and date that a file is written to the SD card, even
   if the time and date is not to be included in the data written to the file.   */

// format of data in Tm = 0xHHMMSS00 */
   const	u32   Tm       = 0x08300000; 	// Time for RTCC clock setting

// format of data in Dt = 0xYYMMDD0W 
   const	u32   Dt       = 0x12051704;  // Date and day for RTCC clock setting
                                       //	W = day of week, Sunday = 0 Saturday = 6
                                       // see array Day[][] above
   const	u16   drift    = 180;	         // Calibration for RTCC clock setting
                                       //	may need to change value for accurate time
                                       //	keeping by a particular board.

   RTCC.open(Tm, Dt, drift);           // Sets date, time and calibration & starts RTC
}

void loop()
{
   CDC.println("\n\r*** Press RETURN to add data to data file.");	
   while (CDC.getKey() != '\r');						// wait for RETURN key to start

/*	----------------------------------------------------------------------------
   Insert temperature measurement code here if required 
   If the 18b20.c library functions are used then a variable t of data type 
   TEMPERATURE will need to be declared. 

   see //examples/6.Sensors/DS18B20/temp18b20.pde 
      for code for a single onewire temperature measurement.
	---------------------------------------------------------------------------*/

/*	Get time and date if board has full RTCC capability*/
   RTCC.getTimeDate(&cT, &cD);         // get time and date
   curTime = RTCC.convertTime(&cT);    // convert time from bcd to decimal format
   curDate = RTCC.convertDate(&cD);    // convert date from bcd to decimal format

/*	Assemble data in buffer with data items separated by comma, i.e. CSV data. 

   If the board has full RTCC capability then the data logged is :-
      Reference:	 e.g. T0
      Day:			 e.g. Mon
      Date:			 e.g. 16-Jan-2012
      Time:			 e.g. 13:15:12
      Temperature: e.g. 12.34	
   i.e. "T#,ddd,dd-mmm-yyyy,hh:mm:ss,##.##" 

   If the datafile is opened with a spreadsheet which is set to recognise CSV data, and 
   possibly also to detect special numbers, each data item should appear in a separate 
   column, although some data items may be automatically converted e.g. the date may be 
   displayed as, say, 16/01/12 if the spreadsheets default date formate is dd/mm/yy. */
	
   u8  buffer[40]  = "";    // arbitary value of 40 for length of character array
                              // needs to be large enough for maximum line length
   u8  temp[13]    = "";    // temporary variable used for int to char conversions
                              // needs to be large enough for maximum string length
                              // generated by sprintf() statements below
   Sprintf(temp, "T%d,", 0);  // converts y to a string T#,
   strcat(buffer, temp);      // adds T#, to buffer
   Sprintf(temp, "%3s,", Day[curDate.wday]);		// converts wday to a string ddd,
   strcat(buffer, temp);      // adds ddd, to buffer
   Sprintf(temp, "%02d-%3s-%04d,", curDate.mday, Month[curDate.mon], curDate.year+2000);		
                              // converts date to a string dd-mmm-yyyy,
   strcat(buffer, temp);      // adds dd-mmm-yyyy, to buffer
   Sprintf(temp, "%02d:%02d:%02d,", curTime.hour, curTime.min, curTime.sec);		
                              // converts hours, mins & secs to a string HH:MM:SS,
   strcat(buffer, temp);      // adds HH:MM:SS, to buffer

/*	Code for including a dummy temperature and new line character*/
   Sprintf(temp, "%d.%02d\n", 12, 45);

/*	----------------------------------------------------------------------------
	Alternative code for converting TEMPERATURE t measured using 18b20.c library 
	to string. As noted above a variable t of data type TEMPERATURE needs to be 
	declared to make use of this alternative code.
	---------------------------------------------------------------------------*
   if (t.sign) {
      strcat(buffer, "-");       // add -ve sign if negative
   }
   Sprintf(temp, "%d.%02d\n", t.integer, t.fraction);			
                                 // converts temperature t to a 
                                 // string integer.fraction 
/*	---------------------------------------------------------------------------*/
   strcat(buffer, temp);         // adds temperature to buffer

/* At this point buffer contains the comma separated data terminated by a 
   new line (line feed) character (and of course a NULL)                      */

   bytes_to_write = strlen(buffer);         //	number of Bytes To be Written

   SD.mount(SDCS_PIN);           //	Allocate file structure
	
/* Open existing file or new file if file does not already exist */
   res = SD.open(&fil, filename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
   if (!res){
      file_size = fil.fsize;     // Get existing file size
      SD.seek(&fil,file_size);   // Set R/W pointer to end of file

/*	Write data to file */
      rc = SD.write(&fil, buffer, bytes_to_write, &bytes_written);		// write new data to file
      SD.flush(&fil);									   // flush any data in cache to file
                                 // to avoid data loss from loss of power etc
                                 // only required if not closing file
      SD.close(&fil);            //	Included for safety to ensure file properly closed
      SD.unmount();              //	in case user removes card instead of proceeding.
																
/*	If all bytes written display data written on terminal */
      if (bytes_to_write == bytes_written){									// check that all bytes written to file
         CDC.printf("*** New data added ***\r\n%s", buffer);	// prints out buffer
      }
      else {
         CDC.printf("\n\r*** Not all data written ***\n\r", filename);
      }

/*	Wait for user input to allow examination of data written */
      CDC.println("\n\r*** Press RETURN to show amended data file.");	
      while (CDC.getKey() != '\r');						// wait for RETURN key to continue

/*	Re-open existing data file - effectively resets R/W pointer to start of file */
      SD.mount(SDCS_PIN); 							//	Allocate file structure
      res = SD.open(&fil, filename, FA_OPEN_EXISTING | FA_READ | FA_WRITE);

      if (!res){

/*	Display data file on terminal including new data added above */
         CDC.printf("\n\r*** File %s contents ***\n\r", filename);
         bytes_total = 0;         // zero byte counter 
         lines_total = 0;         // zero line counter
         do {
            rc = SD.read(&fil, read_buffer, BUFFER_SIZE, &bytes_read);	
                                  // Read a chunk of file
            bytes_total = bytes_total + bytes_read;						// update byte counter
            for(i = 0; i < bytes_read; i++){
               CDC.printf("%c", read_buffer[i]);
               delay(1); // delay to prevent buffer over in Windows
               if (read_buffer[i] == '\n'){
                  CDC.printf("\r"); 
                  lines_total++;  // increment line counter
               }
            }
         } while(bytes_read == BUFFER_SIZE);			// while buffer is full 
         SD.close(&fil);   //	Included for safety to ensure file properly closed
         SD.unmount();     //	in case user removes card instead of proceeding.
         CDC.println("*** End of file ***  ");
         CDC.printf("*** %d lines, %d bytes read.\n\r", lines_total, bytes_total);
      }
      else {
        CDC.printf("\n\r*** File %s", filename);
        CDC.println(" not found or card not in reader (on read) ***");
      }
   }
   else {
      CDC.printf("\n\r*** File %s", filename);
      CDC.println(" not found or card not in reader (on write) ***");
   }
}
