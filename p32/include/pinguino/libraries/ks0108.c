/*
 * Copyright:         Fabian Maximilian Thiele  mailto:me@apetech.de
 * Author:            Fabian Maximilian Thiele
 * Pinguino32X Port:  Marcus Fazzi mailto:anunakin@ieee.org
 * Remarks:           this Copyright must be included
 * known Problems:    none
 * Version:           1.6.3
 * Description:       Graphic Library for KS0108- (and compatible) based LCDs
 * 
 * The port for pinguino, was based on the original version with some modifications 
 * based on version 1.1 and 2.0 of GLCD library for Arduino.
 */

#ifndef	KS0108_H
#define KS0108_H

#include <ks0108.h>

lcdCoord					Coord;
unsigned char				Inverted=0;
unsigned char				FontColor;
const unsigned char*		Font;

#ifdef SLOW_DISPLAY
void fastWriteHigh(unsigned int pin){
    LCD_CMD_SET = pin;
}
void fastWriteLow(unsigned int pin){
    LCD_CMD_CLR = pin;
}
#endif

/** 
 * Just a 450ns delay 
 * Instruction takes 12.5 nano seconds (at 80MHz).
 * This function expects 80MHz clock, default for Pinguino32X
 */
void delay450ns(void){
    //36 nops for about 450ns at 80MHz? Need test it with a good osciloscope.
	//asm("di"); //disable all interrupts
	asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); 
	asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");	asm("NOP"); asm("NOP"); 
	asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); 
	asm("NOP"); asm("NOP");	asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); 
	asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); 
	asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
	asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
	asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
	asm("NOP"); asm("NOP"); asm("NOP");
	//asm("ei"); //enable all interrupts
	//Delayus(1);
}

void ClearPage(unsigned char page, unsigned char color){
    unsigned char x;
	for(x=0; x < DISPLAY_WIDTH; x++){	
	   GotoXY(x, page * 8);
       WriteData(color);
   }	
}

void ClearScreen(unsigned char color){
 unsigned char page;
    for( page = 0; page < 8; page++){
        GotoXY(0, page * 8);
        ClearPage(page, color);
    }
}

void DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char color) {
	unsigned char length, i, y, yAlt, xTmp, yTmp;
	int m;
	//
	// vertical line
	//
	if(x1 == x2) {
		// x1|y1 must be the upper point
		if(y1 > y2) {
			yTmp = y1;
			y1 = y2;
			y2 = yTmp;
		}
		DrawVertLine(x1, y1, y2-y1, color);
	
	//
	// horizontal line
	//
	} else if(y1 == y2) {	
		// x1|y1 must be the left point
		if(x1 > x2) {
			xTmp = x1;
			x1 = x2;
			x2 = xTmp;
		}
		DrawHoriLine(x1, y1, x2-x1, color);
	
	//
	// angled line :)
	//
	} else {
		// angle >= 45�
		if((y2-y1) >= (x2-x1) || (y1-y2) >= (x2-x1)) {
			// x1 must be smaller than x2
			if(x1 > x2) {
				xTmp = x1;
				yTmp = y1;
				x1 = x2;
				y1 = y2;
				x2 = xTmp;
				y2 = yTmp;
			}
		
			length = x2-x1;		// not really the length :)
			m = ((y2-y1)*200)/length;
			yAlt = y1;
			
			for(i=0; i<=length; i++) {
				y = ((m*i)/200)+y1;
				
				if((m*i)%200 >= 100)
					y++;
				else if((m*i)%200 <= -100)
					y--;
				
				DrawLine(x1+i, yAlt, x1+i, y, color);
				
				if(length <= (y2-y1) && y1 < y2)
					yAlt = y+1;
				else if(length <= (y1-y2) && y1 > y2)
					yAlt = y-1;
				else
					yAlt = y;
			}
		
		// angle < 45�
		} else {
			// y1 must be smaller than y2
			if(y1 > y2) {
				xTmp = x1;
				yTmp = y1;
				x1 = x2;
				y1 = y2;
				x2 = xTmp;
				y2 = yTmp;
			}
			
			length = y2-y1;
			m = ((x2-x1)*200)/length;
			yAlt = x1;
			
			for(i=0; i<=length; i++) {
				y = ((m*i)/200)+x1;
				
				if((m*i)%200 >= 100)
					y++;
				else if((m*i)%200 <= -100)
					y--;
				
				DrawLine(yAlt, y1+i, y, y1+i, color);
				if(length <= (x2-x1) && x1 < x2)
					yAlt = y+1;
				else if(length <= (x1-x2) && x1 > x2)
					yAlt = y-1;
				else
					yAlt = y;
			}
		}
	}
}

void DrawRect(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned char color) {
	DrawHoriLine(x, y, width, color);				// top
	DrawHoriLine(x, y+height, width, color);		// bottom
	DrawVertLine(x, y, height, color);			    // left
	DrawVertLine(x+width, y, height, color);		// right
}

void DrawRoundRect(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned char radius, unsigned char color) {
  	int tSwitch, x1 = 0, y1 = radius;
  	tSwitch = 3 - 2 * radius;

	while (x1 <= y1) {
	    SetDot(x+radius - x1, y+radius - y1, color);
	    SetDot(x+radius - y1, y+radius - x1, color);

	    SetDot(x+width-radius + x1, y+radius - y1, color);
	    SetDot(x+width-radius + y1, y+radius - x1, color);

	    SetDot(x+width-radius + x1, y+height-radius + y1, color);
	    SetDot(x+width-radius + y1, y+height-radius + x1, color);

	    SetDot(x+radius - x1, y+height-radius + y1, color);
	    SetDot(x+radius - y1, y+height-radius + x1, color);

	    if (tSwitch < 0) {
	    	tSwitch += (4 * x1 + 6);
	    } else {
	    	tSwitch += (4 * (x1 - y1) + 10);
	    	y1--;
	    }
	    x1++;
	}

	DrawHoriLine(x+radius, y, width-(2*radius), color);			// top
	DrawHoriLine(x+radius, y+height, width-(2*radius), color);	// bottom
	DrawVertLine(x, y+radius, height-(2*radius), color);		// left
	DrawVertLine(x+width, y+radius, height-(2*radius), color);	// right
}

/**
 * Hardware-Functions
 */
void FillRect(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned char color) {
	unsigned char mask, pageOffset, h, i, dat;
	height++;

	pageOffset = y%8;
	y -= pageOffset;
	mask = 0xFF;
	if(height < 8-pageOffset) {
		mask >>= (8-height);
		h = height;
	} else {
		h = 8-pageOffset;
	}
	mask <<= pageOffset;

	GotoXY(x, y);
	for(i=0; i<=width; i++) {
		dat = ReadData();

		if(color == BLACK) {
			dat |= mask;
		} else {
			dat &= ~mask;
		}

		WriteData(dat);
	}

	while(h+8 <= height) {
		h += 8;
		y += 8;
		GotoXY(x, y);

		for(i=0; i<=width; i++) {
			WriteData(color);
		}
	}

	if(h < height) {
		mask = ~(0xFF << (height-h));
		GotoXY(x, y+8);

		for(i=0; i<=width; i++) {
			dat = ReadData();

			if(color == BLACK) {
				dat |= mask;
			} else {
				dat &= ~mask;
			}

			WriteData(dat);
		}
	}
}

void InvertRect(unsigned char x, unsigned char y, unsigned char width, unsigned char height) {
	unsigned char mask, pageOffset, h, i, dat, tmpData;
	height++;

	pageOffset = y%8;
	y -= pageOffset;
	mask = 0xFF;
	if(height < 8-pageOffset) {
		mask >>= (8-height);
		h = height;
	} else {
		h = 8-pageOffset;
	}
	mask <<= pageOffset;

	GotoXY(x, y);
	for(i=0; i<=width; i++) {
		dat = ReadData();
		tmpData = ~dat;
		dat = (tmpData & mask) | (dat & ~mask);
		WriteData(dat);
	}

	while(h+8 <= height) {
		h += 8;
		y += 8;
		GotoXY(x, y);

		for(i=0; i<=width; i++) {
			dat = ReadData();
			WriteData(~dat);
		}
	}

	if(h < height) {
		mask = ~(0xFF << (height-h));
		GotoXY(x, y+8);

		for(i=0; i<=width; i++) {
			dat = ReadData();
			tmpData = ~dat;
			dat = (tmpData & mask) | (dat & ~mask);
			WriteData(dat);
		}
	}
}

void SetInverted(unsigned char invert) {
	if(Inverted != invert) {
		InvertRect(0,0,DISPLAY_WIDTH-1,DISPLAY_HEIGHT-1);
		Inverted = invert;
	}
}

void SetDot(unsigned char x, unsigned char y, unsigned char color) {
	unsigned char dat;

	GotoXY(x, y-y%8);					// read data from display memory
	dat = ReadData();

	if(color == BLACK) {
		dat |= 0x01 << (y%8);			// set dot
	} else {
		dat &= ~(0x01 << (y%8));		// clear dot
	}

	WriteData(dat);					    // write data back to display
}

/**
 * Font Functions
 */
void SelectFont(const char* font, unsigned char color) {
	Font = font;	
	FontColor = color;
}

/** Print a integer number on LCD */
void PrintNumber(long n){
   unsigned char buf[10];  // prints up to 10 digits  
   unsigned char i=0;
   if(n==0)
	   PutChar('0');
   else{
	 if(n < 0){
        PutChar('-');
		n = -n;
	 }
     while(n>0 && i <= 10){
	   buf[i++] = n % 10;  // n % base
	   n /= 10;   // n/= base
	 }
	 for(; i >0; i--)
		 PutChar((char) (buf[i-1] < 10 ? '0' + buf[i-1] : 'A' + buf[i-1] - 10));	  
   }
}

/** Print a float number to LCD */
#ifdef USEFLOAT
void PrintFloat(float number, unsigned char digits) { 
  unsigned char i, toPrint;
  unsigned long int_part;
  float rounding, remainder;
  
  // Handle negative numbers
  if (number < 0.0) {
     PutChar('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"  
  rounding = 0.5;
  for (i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it  
  int_part = (unsigned long)number;
  remainder = number - (float)int_part;
  PrintNumber(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    PutChar('.'); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    toPrint = (unsigned int)remainder; //Integer part without use of math.h lib, I think better! (Fazzi)
    PrintNumber(toPrint);
    remainder -= toPrint; 
  }
}
#endif

int PutChar(char c) {
	unsigned char width = 0;
	unsigned char height = *(Font+FONT_HEIGHT);
	unsigned char bytes = (height+7)/8;
	
	unsigned char firstChar = *(Font+FONT_FIRST_CHAR);
	unsigned char charCount = *(Font+FONT_CHAR_COUNT);
	
	unsigned int index = 0;
	unsigned char x = Coord.x;
    unsigned char y = Coord.y;
	
    unsigned char i, j;
    
	if(c < firstChar || c >= (firstChar+charCount)) {
		return 1;
	}
	c-= firstChar;

	if( *(Font+FONT_LENGTH) == 0 && *(Font+FONT_LENGTH+1) == 0) {
    // zero length is flag indicating fixed width font (array does not contain width data entries)
	   width = *(Font+FONT_FIXED_WIDTH); 
	   index = c*bytes*width+FONT_WIDTH_TABLE;
	}
	else{
	// variable width font, read width data, to get the index
	   for(i=0; i<c; i++) {  
		 index += *(Font+FONT_WIDTH_TABLE+i);
	   }
	   index = index*bytes+charCount+FONT_WIDTH_TABLE;
	   width = *(Font+FONT_WIDTH_TABLE+c);
    }

	// last but not least, draw the character
	for(i=0; i<bytes; i++) {
		unsigned char page = i*width;
		for(j=0; j<width; j++) {
			unsigned char dat = *(Font+index+page+j);
		
			if(height > 8 && height < (i+1)*8) {
				dat >>= (i+1)*8-height;
			}
			
			if(FontColor == BLACK) {
				WriteData(dat);
			} else {
				WriteData(~dat);
			}
		}
		// 1px gap between chars
		if(FontColor == BLACK) {
			WriteData(0x00);
		} else {
			WriteData(0xFF);
		}
		GotoXY(x, Coord.y+8);
	}
	GotoXY(x+width+1, y);

	return 0;
}

void Puts(char* str) {
	int x = Coord.x;
	while(*str != 0) {
		if(*str == '\n') {
			GotoXY(x, Coord.y+*(Font+FONT_HEIGHT));
		} else {
			PutChar(*str);
		}
		str++;
	}
}

unsigned char CharWidth(char c) {
	unsigned char width = 0;
	unsigned char firstChar = *(Font+FONT_FIRST_CHAR);
	unsigned char charCount = *(Font+FONT_CHAR_COUNT);

	// read width data
	if(c >= firstChar && c < (firstChar+charCount)) {
		c -= firstChar;
		width = *(Font+FONT_WIDTH_TABLE+c)+1;
	}

	return width;
}

unsigned int StringWidth(char* str) {
	unsigned int width = 0;

	while(*str != 0) {
		width += CharWidth(*str++);
	}

	return width;
}


void CursorTo( unsigned char x, unsigned char y){    // 0 based coordinates for fixed width fonts (i.e. systemFont5x7)
   GotoXY( x * (*(Font+FONT_FIXED_WIDTH)+1),
	       y * (*(Font+FONT_HEIGHT)+1)
		 ) ; 
}

void GotoXY(unsigned char x, unsigned char y) {
  unsigned char chip, cmd;
	
  if( (x > DISPLAY_WIDTH-1) || (y > DISPLAY_HEIGHT-1) )	// exit if coordinates are not legal
    return;
  Coord.x = x;								// save new coordinates
  Coord.y = y;
	
  if(y/8 != Coord.page) {
  	Coord.page = y/8;
	cmd = LCD_SET_PAGE | Coord.page;		// set y address on all chips	
	for(chip=0; chip < DISPLAY_WIDTH/CHIP_WIDTH; chip++){
	   WriteCommand(cmd, chip);	
	}
  }
  chip = Coord.x/CHIP_WIDTH;
  x = x % CHIP_WIDTH;
  cmd = LCD_SET_ADD | x;
  WriteCommand(cmd, chip);					// set x address on active chip	
}

/** Start GLCD... */
void Init(unsigned char invert) {
    /*RSTOUT = 0;
    RST = 1;*/
	Coord.x = 0;
	Coord.y = 0;
	Coord.page = 0;

	Inverted = invert;

	LCD_CMD_DIR = 0x00;							// command port is output
	WriteCommand(LCD_ON, CHIP1);				// power on
	WriteCommand(LCD_ON, CHIP2);

	WriteCommand(LCD_DISP_START, CHIP1);		// display start line = 0
	WriteCommand(LCD_DISP_START, CHIP2);
	ClearScreen(invert ? BLACK : WHITE);	    // display clear
	GotoXY(0,0);
}

void Enable(void) {
    fastWriteHigh(EN);					// EN high level width: min. 450ns
	delay450ns();
	fastWriteLow(EN);
//for(volatile unsigned char i=0; i<8; i++);  // big delay loop in Fabian's code, was 5.7us, replaced by call to 450ns delay function
	delay450ns();	
}

unsigned char DoReadData(unsigned char first) {
	unsigned char dat;
	//unsigned char i;

	LCD_DATA_OUT = 0x00;
	LCD_DATA_DIR = 0xFF;				// data port is input

	if(Coord.x < CHIP_WIDTH) {
		fastWriteLow(CSEL2);			// deselect chip 2
		fastWriteHigh(CSEL1);			// select chip 1
	} else if(Coord.x >= CHIP_WIDTH) {
		fastWriteLow(CSEL1);			// deselect chip 1
	    fastWriteHigh(CSEL2);			// select chip 2
	}
	if(Coord.x == CHIP_WIDTH && first) {	// chip2 X-address = 0
		WriteCommand(LCD_SET_ADD, CHIP2); 	// wuff wuff
	}

	fastWriteHigh(D_I);					// D/I = 1
	fastWriteHigh(R_W);					// R/W = 1

	fastWriteHigh(EN); 					// EN high level width: min. 450ns

    delay450ns();

	dat = LCD_DATA_IN;					// read Data

	fastWriteLow(EN); 
//	for(volatile unsigned char i=0; i<8; i++);  // big delay loop in Fabian's code, was 5.7us, replaced by 450ns delay below
	delay450ns();
    
	LCD_DATA_DIR = 0x00;                 //Output mode?

	GotoXY(Coord.x, Coord.y);

	if(Inverted)
		dat = ~dat;
	return dat;
}

unsigned char ReadData(void) {
	DoReadData(1);							// dummy read
	return DoReadData(0);					// "real" read
}

/** Write a command to chip */
void WriteCommand(unsigned char cmd, unsigned char chip) {
	if(chip == CHIP1) {
		fastWriteLow(CSEL2);			// deselect chip 2
		asm("nop");
		fastWriteHigh(CSEL1);			// select chip 1
	} else if(chip == CHIP2) {
		fastWriteLow(CSEL1);			// deselect chip 1
	    asm("nop");
	    fastWriteHigh(CSEL2);			// select chip 2
	}

	fastWriteLow(D_I);					// D/I = 0
	fastWriteLow(R_W);					// R/W = 0
	LCD_DATA_DIR = 0x00;				// data port is output
	LCD_DATA_OUT = cmd;					// write command
	Enable();							// enable
	LCD_DATA_OUT = 0x00;
}

/** Write data to chip */
void WriteData(unsigned char dat) {
	unsigned char displayData, yOffset, cmdPort;

#ifdef DEBUG
	unsigned int i;
	for(i=0; i<5000; i++);
#endif

	if(Coord.x >= 128)
		return;

	if(Coord.x < 64) {
		fastWriteLow(CSEL2);			// deselect chip 2
		asm("nop");
		fastWriteHigh(CSEL1);			// select chip 1
	} else if(Coord.x >= 64) {
		fastWriteLow(CSEL1);			// deselect chip 1
		asm("nop");
	    fastWriteHigh(CSEL2);			// select chip 2
	}
	if(Coord.x == 64)					// chip2 X-address = 0
		WriteCommand(LCD_SET_ADD, CHIP2);

	fastWriteHigh(D_I);					// D/I = 1
	fastWriteLow(R_W);  				// R/W = 0
	LCD_DATA_DIR = 0x00;				// data port is output


	yOffset = Coord.y%8;
	if(yOffset != 0) {
		// first page
		cmdPort = LCD_CMD_PORT;						// save command port
		displayData = ReadData();

		LCD_CMD_PORT = cmdPort;						// restore command port
		LCD_DATA_DIR = 0x00;						// data port is output

		displayData |= dat << yOffset;
		if(Inverted)
			displayData = ~displayData;
		LCD_DATA_OUT = displayData;				// write data
		Enable();								// enable

		// second page
		GotoXY(Coord.x, Coord.y+8);

		displayData = ReadData();

		LCD_CMD_PORT = cmdPort;						// restore command port
		LCD_DATA_DIR = 0x00;						// data port is output

		displayData |= dat >> (8-yOffset);
		if(Inverted)
			displayData = ~displayData;
		LCD_DATA_OUT = displayData;				// write data
		Enable();								// enable

		GotoXY(Coord.x+1, Coord.y-8);
	} else {
		if(Inverted)
			dat = ~dat;
		LCD_DATA_OUT = dat;						// write data
		Enable();								// enable
		Coord.x++;
	}
	LCD_DATA_OUT = 0x00;
}

/** Loads a Bitmap to GLCD */
void DrawBitmap(const unsigned char * bitmap, unsigned char x, unsigned char y, unsigned char color){
unsigned char width, height;
unsigned char i, j;

  width = *(bitmap++); 
  height = *(bitmap++);
  for(j = 0; j < height / 8; j++) {
     GotoXY(x, y + (j*8) );
	 for(i = 0; i < width; i++) {
		 unsigned char displayData = *(bitmap++);
	   	 if(color == BLACK)
			WriteData(displayData);
		 else
		    WriteData(~displayData);
	 }
  }
}
#endif
