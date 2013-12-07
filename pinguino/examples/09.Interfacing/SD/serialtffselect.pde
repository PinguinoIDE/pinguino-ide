/*----------------------------------------------------- 
Author:    André Gentric
Date: Thu Nov 19 2013
Description:
Application to demonstrate the capabilities of (Tiny) FatFs lib, an extension of Petit Fatfs.
Extended doc at http://elm-chan.org/fsw/ff/00index_e.html
Look at serialtffdemo to know the MCUs, boards, MCU-card connections to apply this example. 
We are only interested by SD cards with low capacity (< 4GB) and FAT12/16.
Nevertheless this lib is able to manage FAT32.
This demo is done in an interactive way thru a hyperterminal.
Our SD card only contains one drive and one FAT.
The user can send synthetic orders to create files or directories, etc ...
fi to initialize the FAT file system and open the root directory (/)
fo 4 <filename> to create a file, filename as mysdfile.txt (length <= 8)
fo 3 <filename> to open, then write/read the file. We cannot work on 2 files at once.
fw <vlength> <string> to write a string with length = vlength
fy to save the file previously written
fk to close the last opened file. This saves the previous writting too.
fe <position> to point (counting bytes) the file content before writing/reading,
 mandatory order, if not done the position depends on the last writing/reading 
fa to position the opened file pointer at the end, ready to append the file.
fd to read the whole file
fv <filename> to remove/delete a file. The file must not be already opened.
fn <old filename> <new one> file name can be as subDir/filename - to rename a file
   if new subDir is different from old subDir (or file is initially in root Dir)
   the file is moved to the new Dir.
fl / or l to read and list the root Dir. or the last opened Dir.
fm <dirname> dirname can be subd1 or subd1/subd2 to make/create a subdirectory
fp <dirname> to open a Dir. as subd1 or subd1/subd2
ff to destroy the current FAT and create a new empty one. To use carefully !

sg <vlength> to get a string into an opened file
sc <char> to input
ss <string> to input
sp <format> (%  omitted) <numeric value or char or string> according <format>
in the 4 above cases the result depends on the file pointer position.
-----------------------------------------------------*/
#include <stdlib.h>
SDFatfs fs; // FATFS alias
extern static u16 CardType;

FIL Fil;			// File object
u8 Buff[65];		// File read buffer

u8 Line[128];  // Serial Console input buffer
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

void die (		// Stop with dying message
	FRESULT rc	// FatFs return value
)
{
	Serial.printf("Failed with rc=%u.\r\n", rc);
// error codes :
// 0 OK
// 1 not ready
// 2 nonexistent file 
// 3 nonexistent path
// 4 invalid name
// 6 refused
// 7 existent
// 8 red/write error
// 10 not enable
// 11 no file system
// 12 invalid object (DIR, FILE, ...)
// 13 FS creation aborted
}

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


void setup() {
    Serial.begin(9600);
    }

void loop() {

/*-----------------------------------------------------------------------*/
/* Program Main                                                          */
/*-----------------------------------------------------------------------*/

  FRESULT rc;				// Result code
  DIR dirroot;				// Root directory object
  DIR dir;				   // Directory object
  DIR* dirx;
  FILINFO fno;			 // File information object
  word bw, br, i;
  u8 *ptr;//
  u8 fnamenul = '\0';
  u8 bufname[13];
  u8 bufform[8];
  u8 bufdec[10];
  u8 alen[4];
  u8 na, rwtype;
  int len;
  dword val;
  u8 lgcorbuff = sizeof(Buff) - 1;

  bufform[0]='%';
  
	// entering commands
  Serial.printf("\r\n>");
  get_line(Line, sizeof(Line));
  ptr = Line;
  Serial.printf("Your command >%s\r\n",Line);
  Delayms(100);
  if(*ptr == 'f')
  {   ptr++;
      if(*ptr == 'f') // to start a new fat file system. This detroys the current FAT 
      {
          rc=SD.mkfs(0,16384U);
          if (rc) die(rc);
          else Serial.printf("\r\nsuccessful format\r\n");
      }
      else if(*ptr == 'i')
      {
      	// fi - Mount the volume - Open the root directory
          rc=SD.mount(0, &fs);		/* Register volume work area (never fails) */
          bufname[0]=fnamenul;
          rc = SD.openDir(&dirroot, "/");
          if (rc) die(rc);
          else Serial.printf("\r\nSD card initialized, type found: %04x\r\n",CardType);

      }
      else if (*ptr == 'm') 	// fm <DirName> - Directory creation
      {
          ptr++;
          while (*ptr == ' ') ptr++;
          rc = SD.mkDir(ptr);
          if (rc) die(rc);
          else Serial.printf("\r\n dir %s created.", ptr);
      }

      else if (*ptr == 'p') 	// fp <DirName> open Dir other than root
      {
          ptr++;
          while (*ptr == ' ') ptr++;
          rc = SD.openDir(&dir,ptr);
          if (rc) die(rc);
          else Serial.printf("\r\n dir %s opened.", ptr);
      }
 
      else if (*ptr == 'l') 	// fl [/ or l]
//                                / -> root directory listing
//                                l -> last opened directory listing
      {
          ptr++;
          while (*ptr == ' ') ptr++;
          if(*ptr == '/') dirx = &dirroot;
          else dirx = &dir; // last opened dir
// list all files or subdirectories in the selected directory
          Serial.printf("\r\nDirectory listing...(size & name)\r\n");
          for (;;) {
              rc = SD.readDir(dirx, &fno);		// Read a directory item
              if (rc || !fno.fname[0]) break;	// Error or end of dir
              if (fno.fattrib & AM_DIR)
                  Serial.printf("   <dir>  %s\r\n", fno.fname);
              else
                  Serial.printf("%8lu  %s\r\n", fno.fsize, fno.fname);
          }
          if (rc) die(rc);
      }

      else if (*ptr == 'v') // fv <filename> - remove the file
      {
          ptr++;
          while (*ptr == ' ') ptr++;
          rc = SD.remove(ptr);
          if (rc) die(rc);
          else Serial.printf("\r\n%s removed.", ptr);
      }

      else if (*ptr == 'o') // fo  mode <filename>
       // mode = 1 to read, 2 to write, 3 to read/write an existing file
       // mode = 4 to create a file, 7 additional read/write functions
      {
          ptr++;
          while (*ptr == ' ') ptr++;
          na=0;
          while (*ptr != ' ') {alen[na++]= *ptr; ptr++;}
          alen[na]= 0; // length to write (ASCII value)
          rwtype = atoi (alen); // conversion to int -> mode
          // type read  1 write 2 create 4
          if(rwtype <= 16) { 
            ptr++;
            while (*ptr == ' ') ptr++;
            i=0;
            do
            {  bufname[i]=*ptr ; i++; ptr++;}
            while(*ptr != '\0');
            bufname[i]=*ptr;// to create a string

// open file
            
            rc = SD.open(&Fil, bufname, rwtype);
            if (rc) die(rc);
            else { 
              Serial.printf("\r\nFile opened : %s in mode %d\r\n",bufname,rwtype);
            }
          }
          else Serial.printf("\r\nMode error %d",rwtype);
      }

      else if (*ptr == 'y') // fy synchronization
      {
          rc = SD.save(&Fil);
          if (rc) die(rc);
          else Serial.printf("\r\nFile %s synchronized.\r\n", bufname);
      }

      else if (*ptr == 'a')
      {
          if(bufname[0] != fnamenul) {
              rc = SD.seek(&Fil,Fil.fsize);
              if (rc) die(rc);
              else Serial.printf("\r\nFile ready to be appended at # %ld.\r\n",Fil.fsize);
          }
          else Serial.printf("\r\nNo file opened\r\n");
      }

      else if (*ptr == 'd')// fd - Read the file 128 bytes and dump it.
      {
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
          if (rc) die(rc);
        }

      else if (*ptr == 'e')// fe <value>- Move file pointer
      {
          ptr++;
          while (*ptr == ' ') ptr++;
          na=0;
          while (*ptr != 0) {
              if(*ptr > 0x2F && *ptr < 0x3A) alen[na++]= *ptr;
              ptr++;
          }
          alen[na]= 0; // length to write (ASCII value)
          len = atoi (alen); // conversion to int
          rc = SD.seek(&Fil,(dword)len);
          if (rc) die(rc);
          else Serial.printf("\r\nLseeked # %ld\r\n",(dword)len);
      }
          
      else if (*ptr == 'w') // fw <len> <value>- Write the file
      {
          ptr++;
          while (*ptr == ' ') ptr++;
          na=0;
          while (*ptr != ' ') {alen[na++]= *ptr; ptr++;}
          alen[na]= 0; // length to write (ASCII value)
          len = atoi (alen); // conversion to int
          while (*ptr == ' ') ptr++;

          rc = SD.write(&Fil, ptr, len, &bw);
          if (rc) die(rc);
          else {
              Serial.printf("\r\n%u bytes written.\r\n", bw);
              Serial.printf("%s\r\n",ptr);
          }
      }

      else if (*ptr == 'k') // close
      {
          rc = SD.close(&Fil);
          if (rc) die(rc);
          else Serial.printf("File %s closed\r\n", bufname);
      }

      else if (*ptr == 'n') //fn <old name> <new name> rename
      {
          ptr++;
          while (*ptr == ' ') ptr++;
          i=0;
          do
          {  bufname[i]=*ptr ; i++; ptr++;} //old name
          while(*ptr != ' ');
          bufname[i]=0;// to create a string
          ptr++; // pointer to new name
          rc = SD.rename(bufname, ptr);
          if (rc) die(rc);
          else Serial.printf("File %s renamed %s\r\n", bufname, ptr);
      }

      else if (*ptr == 'u')	// fu - Mount the volume
      {
          rc=SD.mount(0, NULL);		// Register volume work area (never fails)
          if (rc) die(rc);
          else Serial.printf("\r\nFatfs unmounted");
      }
      else Serial.printf("\r\nInvalid command 2 %c",*ptr);
      }
      else if (*ptr == 's') // use of string functions
      {   ptr++;
         // get a string
          if(*ptr == 'g')
          {
            ptr++;
            while (*ptr == ' ') ptr++;
            na=0;
            while (*ptr != ' ') {alen[na++]= *ptr; ptr++;}
            alen[na]= 0; // length to write (ASCII value)
            len = atoi (alen); // conversion to int
            SD.gets(Buff, len, &Fil);
            Serial.printf("\r\nlg=%d %s",len,Buff);
          }
         // put a string
          else if (*ptr == 's') // ss <yourString>
          {
            ptr++;
            while (*ptr == ' ') ptr++;
            SD.puts(ptr,&Fil);
          }
         // put a character
          else if (*ptr == 'c') // sc <yourChar>
          {
            ptr++;
            while (*ptr == ' ') ptr++;
            SD.putc((int)*ptr,&Fil);
          }
         //put a formatted (printf) string
          else if (*ptr == 'p') // sp <your format without %> <data alpha or num>
         // examples sp s myString (without "  ")
         //          sp c myChar (without '  ')
         //          sp d 123 or sp 6d -321 (-> "  -321") or sp ld 123456
         //          sp 02u 5 (-> 05) or sp 04X 510 (->01FE)
         
          {
            ptr++;
            while (*ptr == ' ') ptr++;
            na = 1;
            while (*ptr != ' ') {bufform[na++]= *ptr; ptr++;}
            ptr++;
            bufform[na] = 0;
            if (bufform[1] !='c' && bufform[1] !='s') {
              i=0;
              do
              {  bufdec[i]=*ptr ; i++; ptr++;}
              while(*ptr != '\0');
              bufdec[i]=*ptr ; //to terminate the string
              val = (dword)atoi (bufdec); // conversion to long

            }
            if (bufform[1]=='c') SD.printf(&Fil,bufform,*ptr);
            else if (bufform[1]=='s') SD.printf(&Fil,bufform,ptr);
            else SD.printf(&Fil,bufform,val);
            Serial.printf("\r\n %s %s", bufform, ptr);
          } //end "sp"

          else Serial.printf("\r\nInvalid command 3 %c",*ptr);
      } // end "f" or "s"
      else Serial.printf("\r\nInvalid command 1 %c",*ptr);

}



