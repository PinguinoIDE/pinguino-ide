/*	----------------------------------------------------------------------------
   FILE:       Dir.pde
   PROJECT:    Pinguino
   PURPOSE:    List files in top level directory on SD card
   PROGRAMER:  2011 Regis Blanchot    <rblanchot@gmail.com>
               2012 Alfred Broda      <alfredbroda@gmail.com>
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
    SPI    microSD Card    PIC32
    MOSI   3 = CMD/DI      SDO2
    MISO   7 = DAT0/D0     SDI2
    SCK    5 = CLK/SCLK    SCK2
    SS     ------------    ----    default SS not used - 
                                   separate pin used for CS (card select)
    (CS)   2 = CD/DAT3/CS	 set by SD.mount( )
                            see values for particular boards below
                            example set for Pic32 Pinguino OTG
                            if using with another board change value
                            in SD.mount() function call

Note: In addition to the yellow LED flashing as indicated by the comments below,
    for the PIC32 Pinguino, the PIC32 Pinguino OTG and the PIC32 Pinguino Micro
    the green LED is, by default, linked to SPI SCK/SCK2 line.  If the 
    LED1_E/L1_E jumper is still closed (the default) the green LED may be seen 
    to flash breifly every time program attempts to access the SD card.
    
    output:
       (GNU/linux) sudo minicom -o -D /dev/ttyACM0
       (Windows)   run a suitable terminal emulator AFTER the program has 
                   been uploaded to the Pinguino board and the board has
                   left the bootloader mode.
                   you may not see the first prompt but press <return>
                   
	      press <return> to start
	---------------------------------------------------------------------------*/

u8 i;

void setup()
{
    pinmode(YELLOWLED, OUTPUT);
}

void loop()
{
   // wait for RETURN key to start
   CDC.println("Press RETURN to start.");
   while (CDC.getKey() != '\r');
/*	if you want to run the program without seeing the directory list on a
   terminal then replace the above while line with */
//	delay(5000);
/*	and the loop will repeat approximately every 6-7 seconds
   the correct functioning of the program is also be indicated by the
   flashing of the yellow LED, 
      one long flash = directory successfully read
      5 short flashes = failed to mount SD card
   To stop either the board should be switched off or the SD card should 
   be removed immediately after yellow LED goes out or when the message
   "Press RETURN to start." appears on the terminal.
 */
    
   CDC.println(" ");
	
/* card select (CS) is on pin 8	for PIC32 Pinguino & PIC32 Pinguino OTG
   card select (CS) is on pin 40 for PIC32 Pinguino Micro
   put required value in SD.mount(#)                              */
   
   if (SD.mount(8)) {  // i.e. PIC32 Pinguino & PIC32 Pinguino OTG
//	if (SD.mount(40)) {  // i.e. PIC32 Pinguino & PIC32 Pinguino OTG

      CDC.println("SD card contents");
      u16 files, dirs;
      SD_Dir dir; /* Directory object */
      SD_FileInfo Finfo; /* File info */
      SD_Error res;

      // open directory
      res = SD.openDir(&dir, "/"); // To list files and folders in the top directory
//    res = SD.openDir(&dir, "/Folder01"); // To list files and folders in Folder01
      files = dirs = 0;
      CDC.println("Attr  Time               Size      Name");
      for (;;) {
         if ((SD.readDir(&dir, &Finfo) != 0) || !Finfo.fname[0]) {
            // error or end of files
            break;
         }

         if (SD.isDir(Finfo)) {
            dirs++;
         } else {
            files++;
         }
         CDC.printf("%c%c%c%c%c ", SD.isDir(Finfo) ? 'D' : '-', SD.isRO(
            Finfo) ? 'R' : '-', SD.isHidden(Finfo) ? 'H' : '-',
            SD.isSys(Finfo) ? 'S' : '-', SD.isArch(Finfo) ? 'A' : '-');
         CDC.printf("%u/%02u/%02u %02u:%02u ", (Finfo.fdate >> 9) + 1980,
            (Finfo.fdate >> 5) & 15, Finfo.fdate & 31, (Finfo.ftime >> 11),
            (Finfo.ftime >> 5) & 63);
         CDC.printf(" %9u ", Finfo.fsize);
		       delay(1); // Slow output slightly for Windows so file name displayed
         CDC.printf(" %-12s\n\r", Finfo.fname);
      }
      CDC.printf("Found %d file(s), %d directories\n\r", files, dirs);
      unmount();
      digitalWrite(YELLOWLED,HIGH);  // Long Flash of Yellow LED
      delay(1000);                   // if mount successful and
      digitalWrite(YELLOWLED,LOW);    // directory output
   } else {
      CDC.println("Mount failed!");
      for (i=0;i<5;i++){
         digitalWrite(YELLOWLED,HIGH);  // 5 short flashes of Yellow LED
         delay(200);                   // if mount not successful
         digitalWrite(YELLOWLED,LOW);
         delay(200);
      }
   }
}
