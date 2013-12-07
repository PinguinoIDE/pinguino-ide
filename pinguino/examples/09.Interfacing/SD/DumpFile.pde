/*	----------------------------------------------------------------------------
   FILE:       Dumpfile.pde
   PROJECT:    Pinguino
   PURPOSE:    Read a file on SD card
   PROGRAMER:  Regis Blanchot <rblanchot@gmail.com>
   BOARDS:     PIC32 Pinguino OTG
               (PIC32 Pinguino, PIC32 Pingino Micro)
   FIRST RELEASE:	17 Jan 2012
   LAST RELEASE:  18 May 2012 M Harper
   IDE RELEASE:   TBA
   ----------------------------------------------------------------------------
   18 May 2012 -  Example updated to reflect changes in SD Library, identify
                  settings for individual Pinguino Boards and show change
                  to allow for running without CDC output also
                  changed CDC.println() with parameters to CDC.printf()

   Circuit:
   * SD card is attached to SPI2 bus as follows:
      SPI   microSD Card   PIC32
      MOSI  3 = CMD/DI     SDO2
      MISO  7 = DAT0/D0    SDI2
      SCK   5 = CLK/SCLK   SCK2
      SS    ------------   ----     default SS not used - 
                                    separate pin used for CS (card select)
      (CS)  2 = CD/DAT3/CS set by SD.mount( )

                           see values for particular boards below
                           example set for Pic32 Pinguino OTG
                           if using with another board change value
                           in SD.mount() function call

    To run successfully the file "lorem.txt" must be in the top level directory 
    on the SD card.
    
    output:
       (GNU/linux) sudo minicom -o -D /dev/ttyACM0
       (Windows)   run a suitable terminal emulator AFTER the program has 
                   been uploaded to the Pinguino board and the board has
                   left the bootloader mode.
                   you may not see the first prompt but press <return>
                   
	      press <return> to start

Note: In addition to the yellow LED flashing as indicated by the comments below,
    for the PIC32 Pinguino, the PIC32 Pinguino OTG and the PIC32 Pinguino Micro
    the green LED is, by default, linked to SPI SCK/SCK2 line.  If the 
    LED1_E/L1_E jumper is still closed (the default) the green LED may be seen 
    to flash breifly every time program attempts to access the SD card.
   	---------------------------------------------------------------------------*/

#define BUFFER_SIZE 32  // blocks of arbitrary length,
                        // try to change it to improve efficiency 

void setup()
{
   pinmode(YELLOWLED, OUTPUT);  // so that yellow LED can be used to indicate
                                // loop completion.
}

void loop()
{
   u8 filename[] = "lorem.txt";
   SD_File file;
   u8 data[BUFFER_SIZE];
   u16 i, bytes_total, lines_total, bytes_read;
   SD_Error error;
	
   // wait for RETURN key to start
   CDC.println("Press RETURN to start.");
   while (CDC.getKey() != '\r');
/* if you want to run the program without seeing the directory list on a
   terminal then replace the above while line with */
// delay(5000);
/* and the loop will repeat approximately every 6-7 seconds
   the correct functioning of the program is also be indicated by the
   flashing of the yellow LED, 
      one long flash = directory successfully read
      3 short flashes = failed to find file on SD card
      5 short flashes = failed to mount SD card

   To stop either the board should be switched off or the SD card should 
   be removed immediately after yellow LED goes out or when the message
   "Press RETURN to start." appears on the terminal.
   */
   CDC.println("Trying to mount FAT filesystem");
	
// mount FAT filesystem
/* card select (CS) is on pin 8	for PIC32 Pinguino & PIC32 Pinguino OTG
   card select (CS) is on pin 40 for PIC32 Pinguino Micro
   put required value in SD.mount(#) 
   */
   if (SD.mount(8)) {  // i.e. PIC32 Pinguino & PIC32 Pinguino OTG
// if (SD.mount(40)) {  // i.e. PIC32 Pinguino Micro
      // open file
      error = SD.open(&file, filename, FA_OPEN_EXISTING | FA_READ);
      if (!error){
         /*	Display data file on terminal including new data added above */
         CDC.printf("\n\r*** File \"%s\" contents ***\n\r", filename);
         bytes_total = 0; 								// zero byte counter 
         lines_total = 0; 								// zero line counter
         do {
            error = SD.read(&file, data, BUFFER_SIZE, &bytes_read);
            // Read a chunk of file
            bytes_total = bytes_total + bytes_read;						// update byte counter
            for(i = 0; i < bytes_read; i++){
               CDC.printf("%c", data[i]); 
               delay(1);// delay to stop Windows buffer overflow
               if (data[i] == '\n'){
                  CDC.printf("\r"); 
                  delay(1);    // delay to stop Windows buffer overflow
                  lines_total++;						// increment line counter
               }
            }
         } while(bytes_read == BUFFER_SIZE);			// while buffer is full 
         CDC.printf("\n%d lines, %d bytes read.\n\r", lines_total, bytes_total);
         SD.close(&file);
         SD.unmount();
         digitalWrite(YELLOWLED,HIGH);  // Long Flash of Yellow LED
         delay(1000);                   // if mount successful and
         digitalWrite(YELLOWLED,LOW);    // directory output
      }
      else {
         CDC.println("file not found!");
         SD.unmount();
         for (i=0;i<3;i++){
            digitalWrite(YELLOWLED,HIGH); // 3 short flashes of Yellow LED
            delay(200);                   // if file not found
            digitalWrite(YELLOWLED,LOW);
            delay(200);
         }
      }
   }
   else {
      CDC.println("mount failed!");
      for (i=0;i<5;i++){
         digitalWrite(YELLOWLED,HIGH); // 5 short flashes of Yellow LED
         delay(200);                   // if mount not successful
         digitalWrite(YELLOWLED,LOW);
         delay(200);
      }
   }
}
