/*
  ITDB02_Graph.cpp - Arduino library support for ITDB02 LCD Board
  Copyright (C)2010-2011 Henning Karlsen. All right reserved
  
  Basic functionality of this library are based on the demo-code provided by
  ITead studio. You can find the latest version of the library at
  http://www.henningkarlsen.com/electronics

  This library has been made especially for the 2.4" TFT LCD Screen Module: 
  ITDB02-2.4 by ITead studio. This library has been designed to use 8bit mode, 
  so it will not work with the 3.2" Module.

  If you make any modifications or improvements to the code, I would appreciate
  that you share the code with me so that I might include it in the next release.
  I can be contacted through http://www.henningkarlsen.com/electronics/contact.php

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Version:   1.0  - Jul  10 2010  - initial release
			 1.01 - Aug  11 2010  - Fixed a small bug with the color green.
									Thanks to Thomas finding and fixing the bug.
			 2.0  - Aug  13 2010  - Added the possibility to use the display in
                                    Landscape mode. Also added a larger font by
									request.
			 2.1  - Sep  30 2010  - Added Arduino Mega compatibility.
                                    Fixed a bug with CENTER and RIGHT in LANDSCAPE mode.
									Fixed a bug in printNumI and printNumF when the 
									number to be printed was 0.
			 2.2  - Oct  14 2010  - Added drawBitmap() with its associated tool
			 2.3  - Nov  24 2010  - Added Arduino Mega2560 compatibility
									Added support for rotating text and bitmaps
			 2.4  - Jan  18 2011  - Fixed an error in the requirements
			 2.5  - Jan  30 2011  - Added loadBitmap()
									Optimized drawBitmap() when not using rotation
			 
			 2.5.1- Mar  04 2011  - Port to Pinguino 32X, By Marcus Fazzi
			 
			 2.6  - Mar  08 2011  - Fixed a bug in printNumF when the number to be printed
									was (-)0.something
             
             2.6  - Jun  09 2011  - Support ITDB02 16bit bus version, PORTD on PIC32 has 16 pins! ;-)

*/

#ifndef ITDB02_Graph16w_c
#define ITDB02_Graph16w_c

//TouchScreen using 3.2" wide display pre-calibrated
#define ITDB02_32w

//include <serial.c>
#include <itdb02/itdb02_graph16w.h>
#include <integer_math.c>

#include <string.h>
#include <delay.c>

#include <itdb02/SmallFont.c>
#ifndef _NO_BIG_FONT_
#include <itdb02/BigFont.c>
#endif

//TODO: Finish it!
#if defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_OTG)
void LCD_Writ_Bus(int data){
  //LCD_DATA_BUS = data;
  //LCD DB0 to DB7
  //0x4000,0x8000,0x200,0x100,0x80,0x40,0x02,0x04 (pins 8 - 15)
  //1,1,6,6,6,6,1,1,
  if(data & 0x0001){ PORTBSET = 0x4000; } else { PORTBCLR = 0x4000; }
  if(data & 0x0002){ PORTBSET = 0x8000; } else { PORTBCLR = 0x8000; }
  if(data & 0x0004){ PORTGSET = 0x200; }  else { PORTGCLR = 0x200; }
  if(data & 0x0008){ PORTGSET = 0x100; }  else { PORTGCLR = 0x100; }
  if(data & 0x0010){ PORTGSET = 0x80; }   else { PORTGCLR = 0x80; }
  if(data & 0x0020){ PORTGSET = 0x40; }   else { PORTGCLR = 0x40; }
  if(data & 0x0040){ PORTBSET = 0x02; }   else { PORTBCLR = 0x02; }
  if(data & 0x0080){ PORTBSET = 0x04; }   else { PORTBCLR = 0x04; }
  
  //LCD DB7 to DB15
  //0x04,0x08,0x01,0x20,0x40,0x80,0x100,0x800 (pins 0 - 7)
  if(data & 0x0100){ PORTDSET = 0x04; }  else { PORTDCLR = 0x04; }
  if(data & 0x0200){ PORTDSET = 0x08; }  else { PORTDCLR = 0x08; }
  if(data & 0x0400){ PORTDSET = 0x01; }  else { PORTDCLR = 0x01; }
  if(data & 0x0800){ PORTDSET = 0x20; }  else { PORTDCLR = 0x20; }
  if(data & 0x1000){ PORTDSET = 0x40; }  else { PORTDCLR = 0x40; }
  if(data & 0x2000){ PORTDSET = 0x80; }  else { PORTDCLR = 0x80; }
  if(data & 0x4000){ PORTDSET = 0x100; } else { PORTDCLR = 0x100; }
  if(data & 0x8000){ PORTDSET = 0x800; } else { PORTDCLR = 0x800; }
  
  fastDelay();
  fastWriteLow(LCD_WR);
  fastDelay();
  fastWriteHigh(LCD_WR);
  fastDelay();
  fastDelay();
	
}
#else
void LCD_Writ_Bus(int data){
  LCD_DATA_BUS = data;
  fastDelay();
  fastWriteLow(LCD_WR);
  fastDelay();
  fastWriteHigh(LCD_WR);
  fastDelay();
  fastDelay();
  /*LCD_DATA_BUS = VL;
  fastWriteLow(LCD_WR);
  fastWriteHigh(LCD_WR);*/
}
#endif

void LCD_Write_COM(int data){   
  fastWriteLow(LCD_RS);
  fastDelay();
  LCD_Writ_Bus(data);
  fastDelay();
  fastDelay();
}

void LCD_Write_DATA(int data){
  fastWriteHigh(LCD_RS);
  fastDelay();
  LCD_Writ_Bus(data);
  fastDelay();
  fastDelay();
}

void InitLCD(char orientation){
  orient=orientation;
  
//Pin Mode for Control Pins!
//OLIMEX BOARDS (Arduino Like Boards, using SHIELDs)
#if defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_OTG)
  LCD_DATA_DIR = 0x00;  //Output for All pins on DATA BUS
  LCD_DATA_BUS = 0x00;

  //LCD DB0 to DB7
  //0x4000,0x8000,0x200,0x100,0x80,0x40,0x02,0x04 (pins 8 - 15)
  //1,1,6,6,6,6,1,1,
  TRISBbits.TRISB14 = 0;
  TRISBbits.TRISB15 = 0;
  TRISGbits.TRISG9 = 0;
  TRISGbits.TRISG8 = 0;
  TRISGbits.TRISG7 = 0;
  TRISGbits.TRISG6 = 0;
  TRISBbits.TRISB1 = 0;
  TRISBbits.TRISB2 = 0;

  fastSetOutputMode(dLCD_RS);
  fastSetOutputMode(dLCD_WR);
  fastSetOutputMode(dLCD_CS);
  fastSetOutputMode(dLCD_REST);
  
  TRISDbits.TRISD4 = 1;		//RD4 is connected to D2, don't know why! :-P
  PORTDbits.RD4 = 0;
#else
  //TODO: Maybe it cause some wrong signal on SERIAL?
  LCD_DATA_DIR = 0x0000;  //Output for All pins on DATA BUS
  LCD_DATA_BUS = 0x0000;

  //Pin Mode for Control Pins!
  fastSetOutputMode(LCD_RS);
  fastSetOutputMode(LCD_WR);
  fastSetOutputMode(LCD_CS);
  fastSetOutputMode(LCD_REST);
#endif

  fastWriteHigh(LCD_REST);
  Delayms(5); 
  fastWriteLow(LCD_REST);
  Delayms(5);
  fastWriteHigh(LCD_REST);
  Delayms(5);

  fastWriteLow(LCD_CS);
  
  //************* Start Initial Sequence *********/
  //16x9 Aspect Display Size 399
  LCD_Write_COM(0x00E9);
  LCD_Write_DATA(0x0020);
  LCD_Write_COM(0x0011); //Exit Sleep
  Delayms(100);
  LCD_Write_COM(0x00D1); //VCOM Control
  LCD_Write_DATA(0x0000);
  LCD_Write_DATA(0x0071);
  LCD_Write_DATA(0x0019);
  LCD_Write_COM(0x00D0); //Power_Setting
  LCD_Write_DATA(0x0007);
  LCD_Write_DATA(0x0001);
  LCD_Write_DATA(0x0008);
  LCD_Write_COM(0x0036); //Set_address_mode
  LCD_Write_DATA(0x0048);
  LCD_Write_COM(0x003A); //Set_pixel_format
  LCD_Write_DATA(0x0005);
  LCD_Write_COM(0x00C1); //Display_Timing_Setting for Normal/Partial Mode
  LCD_Write_DATA(0x0010);
  LCD_Write_DATA(0x0010);
  LCD_Write_DATA(0x0002);
  LCD_Write_DATA(0x0002);
  LCD_Write_COM(0x00C0); //Set Default Gamma
  LCD_Write_DATA(0x0000);
  LCD_Write_DATA(0x0035);
  LCD_Write_DATA(0x0000);
  LCD_Write_DATA(0x0000);
  LCD_Write_DATA(0x0001);
  LCD_Write_DATA(0x0002);
  LCD_Write_COM(0x00C5); //Set frame rate
  LCD_Write_DATA(0x0004);//4 => 72Hz (Default)
  LCD_Write_COM(0x00D2); //power setting
  LCD_Write_DATA(0x0001);
  LCD_Write_DATA(0x0044);
  LCD_Write_COM(0x00C8); //Set Gamma
  LCD_Write_DATA(0x0004);
  LCD_Write_DATA(0x0067);
  LCD_Write_DATA(0x0035);
  LCD_Write_DATA(0x0004);
  LCD_Write_DATA(0x0008);
  LCD_Write_DATA(0x0006);
  LCD_Write_DATA(0x0024);
  LCD_Write_DATA(0x0001);
  LCD_Write_DATA(0x0037);
  LCD_Write_DATA(0x0040);
  LCD_Write_DATA(0x0003);
  LCD_Write_DATA(0x0010);
  LCD_Write_DATA(0x0008);
  LCD_Write_DATA(0x0080);
  LCD_Write_DATA(0x0000);
  LCD_Write_COM(0x002A); //Set_column_address
  LCD_Write_DATA(0x0000);
  LCD_Write_DATA(0x0000);
  LCD_Write_DATA(0x0000);
  LCD_Write_DATA(0x00eF);
  LCD_Write_COM(0x002B); //Set_page_address
  LCD_Write_DATA(0x0000);
  LCD_Write_DATA(0x0000);
  LCD_Write_DATA(0x0001);
  LCD_Write_DATA(0x008F);
  LCD_Write_COM(0x0029); //display on      
  LCD_Write_COM(0x002C); //display on
  
  fastWriteHigh(LCD_CS);

  setColor(255, 255, 255);
  setBackColor(0, 0, 0);
  fsize = FONT_SMALL;
}

void setXY(int x1, int y1, int x2, int y2){
	int tmp;

	if (orient==LANDSCAPE) {
	    tmp=x1;
	    x1=y1;
	    y1=399-tmp;
	    tmp=x2;
	    x2=y2;
	    y2=399-tmp;
	    tmp=y1;
	    y1=y2;
	    y2=tmp;
	}

//TO-DO Parei aqui, falta ver no datasheet!
    LCD_Write_COM(0x002a); //Set_column_address
    LCD_Write_DATA(x1>>8);
    LCD_Write_DATA(x1);
    LCD_Write_DATA(x2>>8);
    LCD_Write_DATA(x2);
    LCD_Write_COM(0x002b); //Set_page_address
    LCD_Write_DATA(y1>>8);
    LCD_Write_DATA(y1);
    LCD_Write_DATA(y2>>8);
    LCD_Write_DATA(y2);
    LCD_Write_COM(0x002c); //Write_memory_start
}

void drawRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
	}
	if (y1>y2)
	{
		tmp=y1;
		y1=y2;
		y2=tmp;
	}

	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1);
}

void drawRoundRect(int x1, int y1, int x2, int y2)
{
	int tmp;

	if (x1>x2)
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
	}
	if (y1>y2)
	{
		tmp=y1;
		y1=y2;
		y2=tmp;
	}
	if ((x2-x1)>4 && (y2-y1)>4)
	{
		drawPixel(x1+1,y1+1);
		drawPixel(x2-1,y1+1);
		drawPixel(x1+1,y2-1);
		drawPixel(x2-1,y2-1);
		drawHLine(x1+2, y1, x2-x1-4);
		drawHLine(x1+2, y2, x2-x1-4);
		drawVLine(x1, y1+2, y2-y1-4);
		drawVLine(x2, y1+2, y2-y1-4);
	}
}

void fillRect(int x1, int y1, int x2, int y2)
{
	int tmp, i;

	if (x1>x2)
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
	}
	if (y1>y2)
	{
		tmp=y1;
		y1=y2;
		y2=tmp;
	}

	if (orient==PORTRAIT)
	{
		for (i=0; i<((y2-y1)/2)+1; i++)
		{
			drawHLine(x1, y1+i, x2-x1);
			drawHLine(x1, y2-i, x2-x1);
		}
	}
	else
	{
		for (i=0; i<((x2-x1)/2)+1; i++)
		{
			drawVLine(x1+i, y1, y2-y1);
			drawVLine(x2-i, y1, y2-y1);
		}
	}
}

void fillRoundRect(int x1, int y1, int x2, int y2){
	int tmp, i;

	if (x1>x2)
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
	}
	if (y1>y2)
	{
		tmp=y1;
		y1=y2;
		y2=tmp;
	}

	if ((x2-x1)>4 && (y2-y1)>4)
	{
		for (i=0; i<=((y2-y1)/2)+1; i++)
		{
			switch(i)
			{
			case 0:
				drawHLine(x1+2, y1+i, x2-x1-4);
				drawHLine(x1+2, y2-i, x2-x1-4);
				break;
			case 1:
				drawHLine(x1+1, y1+i, x2-x1-2);
				drawHLine(x1+1, y2-i, x2-x1-2);
				break;
			default:
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
	}
}

void drawCircle(int x, int y, int radius){
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x1 = 0;
  int y1 = radius;
 
  drawPixel(x, y + radius);
  drawPixel(x, y - radius);
  drawPixel(x + radius, y);
  drawPixel(x - radius, y);
 
  while(x1 < y1)
  {
    if(f >= 0) 
    {
      y1--;
      ddF_y += 2;
      f += ddF_y;
    }
    x1++;
    ddF_x += 2;
    f += ddF_x;    
    drawPixel(x + x1, y + y1);
    drawPixel(x - x1, y + y1);
    drawPixel(x + x1, y - y1);
    drawPixel(x - x1, y - y1);
    drawPixel(x + y1, y + x1);
    drawPixel(x - y1, y + x1);
    drawPixel(x + y1, y - x1);
    drawPixel(x - y1, y - x1);
  }
}

void fillCircle(int x, int y, int radius){
	int x1, y1;
	for(y1=-radius; y1<=radius; y1++) 
		for(x1=-radius; x1<=radius; x1++) 
			if(x1*x1+y1*y1 <= radius*radius) 
				drawPixel(x+x1, y+y1); 
}

void clrScr(){
	unsigned long i;
	
	fastWriteLow(LCD_CS);
	if (orient==PORTRAIT)
		setXY(0,0,239,399);
	else
		setXY(0,0,399,239);
    
    fastWriteHigh(LCD_RS);
	for (i=0; i<96000; i++){
		LCD_Writ_Bus(0);
	}
	fastWriteHigh(LCD_CS);
}

void fillScr(char r, char g, char b){
	long i;
	
	fastWriteLow(LCD_CS); 
	if (orient==PORTRAIT)
		setXY(0,0,239,399);
	else
		setXY(0,0,399,239);
        
    fastWriteHigh(LCD_RS);    
	for (i=0; i<96000; i++)	{
		//setPixel(r, g, b);
        LCD_Writ_Bus(((r & 0xF8)<<8) + ((g & 0xFC)<<3) + ((b & 0xF8)>>3));
	}
	fastWriteHigh(LCD_CS); 
}

void setColor(unsigned char r, unsigned char g, unsigned char b){
	fcolorr=r;
	fcolorg=g;
	fcolorb=b;
}

void setBackColor(unsigned char r, unsigned char g, unsigned char b){
	bcolorr=r;
	bcolorg=g;
	bcolorb=b;
}

void setPixel(int r,int g,int b){
	LCD_Write_DATA(((r & 0xF8)<<8) + ((g & 0xFC)<<3) + ((b & 0xF8)>>3));
}

void drawPixel(int x, int y)
{
	fastWriteLow(LCD_CS);  
	setXY(x, y, x, y);
	//setPixel(fcolorr, fcolorg, fcolorb);
	LCD_Write_DATA(((fcolorr & 0xF8)<<8) + ((fcolorg & 0xFC)<<3) + ((fcolorb & 0xF8)>>3));
	if (orient==PORTRAIT)
	    setXY(0,0,239,399);
	else
	    setXY(0,0,399,239);
	fastWriteHigh(LCD_CS);  
}

void drawLine(int x1, int y1, int x2, int y2)
{
	int tmp, i;
	double delta, tx, ty;
	double m, b, dx, dy;

    if (((x2-x1)<0)){
		tmp=x1;
		x1=x2;
		x2=tmp;
		tmp=y1;
		y1=y2;
		y2=tmp;
	}
    if (((y2-y1)<0)){
		tmp=x1;
		x1=x2;
		x2=tmp;
		tmp=y1;
		y1=y2;
		y2=tmp;
	}

	if (y1==y2){
		if (x1>x2){
			tmp=x1;
			x1=x2;
			x2=tmp;
		}
		drawHLine(x1, y1, x2-x1);
	}
	else if (x1==x2){
		if (y1>y2){
			tmp=y1;
			y1=y2;
			y2=tmp;
		}
		drawVLine(x1, y1, y2-y1);
	}
	else if (abs(x2-x1)>abs(y2-y1)){
        
        fastWriteLow(LCD_CS);
        
		delta=(double)(y2-y1)/(double)(x2-x1);
		ty=(double)y1;
        
		if (x1>x2){
            for (i=x1; i>=x2; i--){
                setXY(i, (int)(ty+0.5), i, (int)(ty+0.5));
                LCD_Write_DATA(((fcolorr & 0xF8)<<8) + ((fcolorg & 0xFC)<<3) + ((fcolorb & 0xF8)>>3));
                ty=ty-delta;
            }
        }
        else{
            for (i=x1; i<=x2; i++){
                setXY(i, (int)(ty+0.5), i, (int)(ty+0.5));
                LCD_Write_DATA(((fcolorr & 0xF8)<<8) + ((fcolorg & 0xFC)<<3) + ((fcolorb & 0xF8)>>3));
        		ty=ty+delta;
            }
        }
        fastWriteHigh(LCD_CS);
	}
	else{
        fastWriteLow(LCD_CS);
        
		delta=(double)(x2-x1)/(double)(y2-y1);
		tx=(double)x1;
        if (y1>y2) {
            for (i=y2+1; i>y1; i--){
                setXY((int)(tx+0.5), i, (int)(tx+0.5), i);
                LCD_Write_DATA(((fcolorr & 0xF8)<<8) + ((fcolorg & 0xFC)<<3) + ((fcolorb & 0xF8)>>3));
                tx=tx+delta;
            }
        }
        else{
            for (i=y1; i<y2+1; i++){
                setXY((int)(tx+0.5), i, (int)(tx+0.5), i);
                LCD_Write_DATA(((fcolorr & 0xF8)<<8) + ((fcolorg & 0xFC)<<3) + ((fcolorb & 0xF8)>>3));
                tx=tx+delta;
            }
        }
        fastWriteHigh(LCD_CS);
	}

	if (orient==PORTRAIT)
		setXY(0,0,239,399);
	else
		setXY(0,0,399,239);
}

void drawHLine(int x, int y, int l)
{
	int i;
	
	fastWriteLow(LCD_CS);  
	setXY(x, y, x+l, y);
	for (i=0; i<l+1; i++){
		fastSetPixel(fcolorr, fcolorg, fcolorb);
	}
	fastWriteHigh(LCD_CS);  
}

void drawVLine(int x, int y, int l)
{
	int i;
	fastWriteLow(LCD_CS);
	setXY(x, y, x, y+l);
	for (i=0; i<l; i++) {
		fastSetPixel(fcolorr, fcolorg, fcolorb);
	}
	fastWriteHigh(LCD_CS);  
}

void printChar(char c, int x, int y){
	unsigned char i,j,ch;
	unsigned int temp; 

	fastWriteLow(LCD_CS);   
  
	if (fsize==FONT_SMALL)
	{
		if (orient==PORTRAIT)
		{
			setXY(x,y,x+7,y+11);
		  
			temp=((c-32)*12);
			for(j=0;j<12;j++)
			{
				ch=(SmallFont[temp]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<(7-i)))!=0)   
					{
						fastSetPixel(fcolorr, fcolorg, fcolorb);
					} 
					else
					{
						fastSetPixel(bcolorr, bcolorg, bcolorb);
					}   
				}
				temp++;
			}
		}
		else
		{
		  
			temp=((c-32)*12);
			for(j=0;j<12;j++)
			{
				setXY(x,y+j,x+7,y+j);
				ch=(SmallFont[temp]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<i))!=0)   
					{
						fastSetPixel(fcolorr, fcolorg, fcolorb);
					} 
					else
					{
						fastSetPixel(bcolorr, bcolorg, bcolorb);
					}   
				}
				temp++;
			}
		}
	}
#ifndef _NO_BIG_FONT_
	else
	{
		if (orient==PORTRAIT)
		{
			setXY(x,y,x+15,y+15);
		  
			temp=((c-32)*32);
			for(j=0;j<32;j++)
			{
				ch=(BigFont[temp]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<(7-i)))!=0)   
					{
						fastSetPixel(fcolorr, fcolorg, fcolorb);
					} 
					else
					{
						fastSetPixel(bcolorr, bcolorg, bcolorb);
					}   
				}
				temp++;
			}
		}
		else
		{
			temp=((c-32)*32);
			for(j=0;j<32;j+=2)
			{
				setXY(x,y+(j/2),x+15,y+(j/2));
				ch=(BigFont[temp+1]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<i))!=0)   
					{
						fastSetPixel(fcolorr, fcolorg, fcolorb);
					} 
					else
					{
						fastSetPixel(bcolorr, bcolorg, bcolorb);
					}   
				}
				ch=(BigFont[temp]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<i))!=0)   
					{
						fastSetPixel(fcolorr, fcolorg, fcolorb);
					} 
					else
					{
						fastSetPixel(bcolorr, bcolorg, bcolorb);
					}   
				}
				temp+=2;
			}
		}
	}
#endif
	fastWriteHigh(LCD_CS);
}

void rotateChar(char c, int x, int y, int pos, int deg)
{
	char i,j,ch;
	unsigned int temp;
	int newx,newy;
	//double radian;
	//radian=deg*0.0175;  

	fastWriteLow(LCD_CS);   
	if (fsize==FONT_SMALL)
	{
		temp=((c-32)*12); 
		for(j=0;j<12;j++) 
		{
			ch=(SmallFont[temp]); 
			for(i=0;i<8;i++)
			{   
				newx=x+(((i+(pos*8))*cosi(deg))-((j)*sini(deg)));
				newy=y+(((j)*cosi(deg))+((i+(pos*8))*sini(deg)));

				setXY(newx,newy,newx+1,newy+1);
				
				if((ch&(1<<(7-i)))!=0)   
				{
					fastSetPixel(fcolorr, fcolorg, fcolorb);
				} 
				else  
				{
					fastSetPixel(bcolorr, bcolorg, bcolorb);
				}   
			}
			temp++;
		}
	}
#ifndef _NO_BIG_FONT_
	else
	{
		temp=((c-32)*32); 
		for(j=0;j<16;j++) 
		{
			ch=(BigFont[temp]); 
			for(i=0;i<8;i++)
			{   
				newx=x+(((i+(pos*16))*cosi(deg))-((j)*sini(deg)));
				newy=y+(((j)*cosi(deg))+((i+(pos*16))*sini(deg)));

				setXY(newx,newy,newx+1,newy+1);
				
				if((ch&(1<<(7-i)))!=0)   
				{
					fastSetPixel(fcolorr, fcolorg, fcolorb);
				} 
				else  
				{
					fastSetPixel(bcolorr, bcolorg, bcolorb);
				}   
			}
			temp++;
			ch=(BigFont[temp]); 
			for(i=8;i<16;i++)
			{   
				newx=x+(((i+(pos*16))*cosi(deg))-((j)*sini(deg)));
				newy=y+(((j)*cosi(deg))+((i+(pos*16))*sini(deg)));

				setXY(newx,newy,newx+1,newy+1);
				
				if((ch&(1<<(15-i)))!=0)   
				{
					fastSetPixel(fcolorr, fcolorg, fcolorb);
				} 
				else  
				{
					fastSetPixel(bcolorr, bcolorg, bcolorb);
				}   
			}
			temp++;
		}
	}
#endif
	fastWriteHigh(LCD_CS);
}

void myGLCD16w_print(char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (fsize==FONT_SMALL)
	{
		if (orient==PORTRAIT)
		{
		if (x==RIGHT)
			x=240-(stl*8);
		if (x==CENTER)
			x=(240-(stl*8))/2;
		}
		else
		{
		if (x==RIGHT)
			x=400-(stl*8);
		if (x==CENTER)
			x=(400-(stl*8))/2;
		}
	}
	else
	{
		if (orient==PORTRAIT)
		{
		if (x==RIGHT)
			x=240-(stl*16);
		if (x==CENTER)
			x=(240-(stl*16))/2;
		}
		else
		{
		if (x==RIGHT)
			x=400-(stl*16);
		if (x==CENTER)
			x=(400-(stl*16))/2;
		}
	}

	for (i=0; i<stl; i++)
		if (deg==0)
			printChar(*st++, x + (i*(fsize*8)), y);
		else
			rotateChar(*st++, x, y, i, deg);
}

void printNumI(long num, int x, int y)
{
  char buf[25];
  char st[27];
  unsigned char neg=0;
  int i,c=0;
  
  if (num==0)
  {
	  st[0]=48;
	  st[1]=0;
  }
  else
  {
	  if (num<0)
	  {
		neg=1;
		num=-num;
	  }
	  
	  while (num>0)
	  {
		buf[c]=48+(num % 10);
		c++;
		num=(num-(num % 10))/10;
	  }
	  buf[c]=0;
	  
	  if (neg)
	  {
		st[0]=45;
	  }
	  
	  for (i=0; i<c; i++)
	  {
		st[i+neg]=buf[c-i-1];
	  }
	  st[c+neg]=0;
  }

  myGLCD16w_print(st,x,y,0);
}

void printNumF(double num, char dec, int x, int y)
{
  char buf[25];
  char st[27];
  unsigned char neg=0;
  int i,c=0;
  int c2;
  unsigned long inum;
  
  if (num==0)
  {
	  st[0]=48;
	  st[1]=46;
	  for (i=0; i<dec; i++)
		  st[2+i]=48;
	  st[2+dec]=0;
  }
  else
  {
	  if (num<0)
	  {
		neg=1;
		num=-num;
	  }
	  
	  if (dec<1)
		dec=1;
	  if (dec>5)
		dec=5;
	  
	  inum=(long)(num*powi(10,dec));
	  
	  while (inum>0) {
		buf[c]=48+(inum % 10);
		c++;
		inum=(inum-(inum % 10))/10;
	  }
	  if ((num<1) && (num>0))
	  {
		  buf[c]=48;
		  c++;
	  }
	  buf[c]=0;
	  
	  if (neg)
	  {
		st[0]=45;
	  }
	  
	  c2=neg;
	  for (i=0; i<c; i++)
	  {
		st[c2]=buf[c-i-1];
		c2++;
		if ((c-(c2-neg))==dec)
		{
		  st[c2]=46;
		  c2++;
		}
	  }
	  st[c2]=0;
  }

  myGLCD16w_print(st,x,y,0);
}

void fontSize(char size)
{
#ifdef _NO_BIG_FONT_
	fsize = FONT_SMALL;
#else
	fsize = size;
#endif
}

void drawBitmap(int x, int y, int sx, int sy, unsigned int* data, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;

	if (scale==1)
	{
		if (orient==PORTRAIT)
		{
			fastWriteLow(LCD_CS);  
			setXY(x, y, x+sx-1, y+sy-1);
			for (tc=0; tc<(sx*sy); tc++)
			{
				col=(unsigned int)(data[tc]);
				LCD_Write_DATA(col);
			}
			fastWriteHigh(LCD_CS);  
		}
		else
		{
			fastWriteLow(LCD_CS);  
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+ty, x+sx-1, y+ty);
				for (tx=sx; tx>=0; tx--)
				{
					col=(unsigned int)(data[(ty*sx)+tx]);
					LCD_Write_DATA(col);
				}
			}
			fastWriteHigh(LCD_CS);  
		}
	}
	else
	{
		if (orient==PORTRAIT)
		{
			fastWriteLow(LCD_CS);  
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
				for (tsy=0; tsy<scale; tsy++)
					for (tx=0; tx<sx; tx++)
					{
						col=(unsigned int)(data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							LCD_Write_DATA(col);
					}
			}
			fastWriteHigh(LCD_CS);  
		}
		else
		{
			fastWriteLow(LCD_CS);  
			for (ty=0; ty<sy; ty++)
			{
				for (tsy=0; tsy<scale; tsy++)
				{
					setXY(x, y+(ty*scale)+tsy, x+((sx*scale)-1), y+(ty*scale)+tsy);
					for (tx=sx; tx>=0; tx--)
					{
						col=(unsigned int)(data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							LCD_Write_DATA(col);
					}
				}
			}
			fastWriteHigh(LCD_CS);  
		}
	}
}

void drawBitmapR(int x, int y, int sx, int sy, unsigned int* data, int deg, int rox, int roy){
    unsigned int col;
    int tx, ty, newx, newy;
    //char r, g, b;
    //float vrad = 0;        
    //vrad=deg*0.0174533;
    
    //vals = (int)(100000*sini(deg));
    
    //SerialPrintf(COM, "x: sini(%d)= %d\r\n", deg, vals);
    
    if (deg==0)
	    drawBitmap(x, y, sx, sy, data, 1);
    else {
	for (ty=0; ty<sy; ty++){
	    for (tx=0; tx<sx; tx++){
		col=(unsigned int)(data[(ty*sx)+tx]);
		newx=x+rox+((tx-rox)*cosi(deg)-(ty-roy)*sini(deg));
		newy=y+roy+((ty-roy)*cosi(deg)+(tx-rox)*sini(deg));
		setXY(newx, newy, newx, newy);
		fastWriteData(col);
	    }
	}
    }
}

#endif
