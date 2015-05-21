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

			 2.6.1 -Fev  29 2012  - Added support to OLIMEX Boards.
									There is a problem with D2 pin (PIC32 pin52 ?). using A6.

*/

#ifndef ITDB02_Graph_c
#define ITDB02_Graph_c

//TouchScreen using 2.4" wide display pre-calibrated
#define ITDB02_24

#include <itdb02/itdb02_graph.h>

//Can't use sin/cos with that library, don't know why! :-(
#include <integer_math.c> 
//include <math.h>

#include <string.h>
#include <delay.c>

#include <itdb02/SmallFont.c>

#ifndef _NO_BIG_FONT_
#include <itdb02/BigFont.c>
#endif

//OLIMEX BOARDS (Arduino Like Boards, using SHIELDs)
//You must remove capacitor C20 if you have a OLIMEX Pinguino32 rev. A, B or C
#if defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_OTG)

void LCD_Writ_Bus(int VH,int VL){
  //LCD_DATA_BUS = VH;
  //0x04,0x08,0x01,0x20,0x40,0x80,0x100,0x800 (pins 0 - 7)
  if(VH & 0x01){ PORTDSET = 0x04; }  else { PORTDCLR = 0x04; }
  if(VH & 0x02){ PORTDSET = 0x08; }  else { PORTDCLR = 0x08; }
  if(VH & 0x04){ PORTDSET = 0x01; }  else { PORTDCLR = 0x01; }
  if(VH & 0x08){ PORTDSET = 0x20; }  else { PORTDCLR = 0x20; }
  if(VH & 0x10){ PORTDSET = 0x40; }  else { PORTDCLR = 0x40; }
  if(VH & 0x20){ PORTDSET = 0x80; }  else { PORTDCLR = 0x80; }
  if(VH & 0x40){ PORTDSET = 0x100; } else { PORTDCLR = 0x100; }
  if(VH & 0x80){ PORTDSET = 0x800; } else { PORTDCLR = 0x800; }
  fastDelay();
  fastWriteLow(LCD_WR);
  fastDelay();
  fastWriteHigh(LCD_WR);
  fastDelay();
  //LCD_DATA_BUS = VL;
  //0x04,0x08,0x01,0x20,0x40,0x80,0x100,0x800 (pins 0 - 7)
  if(VL & 0x01){ PORTDSET = 0x04; }  else { PORTDCLR = 0x04; }
  if(VL & 0x02){ PORTDSET = 0x08; }  else { PORTDCLR = 0x08; }
  if(VL & 0x04){ PORTDSET = 0x01; }  else { PORTDCLR = 0x01; }
  if(VL & 0x08){ PORTDSET = 0x20; }  else { PORTDCLR = 0x20; }
  if(VL & 0x10){ PORTDSET = 0x40; }  else { PORTDCLR = 0x40; }
  if(VL & 0x20){ PORTDSET = 0x80; }  else { PORTDCLR = 0x80; }
  if(VL & 0x40){ PORTDSET = 0x100; } else { PORTDCLR = 0x100; }
  if(VL & 0x80){ PORTDSET = 0x800; } else { PORTDCLR = 0x800; }
  fastDelay();
  fastWriteLow(LCD_WR);
  fastDelay();
  fastWriteHigh(LCD_WR);
  fastDelay();
  fastDelay();
}

#else

void LCD_Writ_Bus(int VH,int VL){
  LCD_DATA_BUS = VH;
  fastWriteLow(LCD_WR);
  fastWriteHigh(LCD_WR);
  LCD_DATA_BUS = VL;
  fastWriteLow(LCD_WR);
  fastWriteHigh(LCD_WR);
}

#endif

void LCD_Write_COM(int VH,int VL) {   
  fastWriteLow(LCD_RS);
  LCD_Writ_Bus(VH,VL);
}

void LCD_Write_DATA(int VH,int VL){
  fastWriteHigh(LCD_RS);
  LCD_Writ_Bus(VH,VL);
}

void InitLCD(char orientation){
  orient=orientation;
  
  LCD_DATA_DIR = 0x00;  //Output for All pins on DATA BUS
  LCD_DATA_BUS = 0x0000;

//Pin Mode for Control Pins!
//OLIMEX BOARDS (Arduino Like Boards, using SHIELDs)
#if defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_OTG)
  fastSetOutputMode(dLCD_RS);
  fastSetOutputMode(dLCD_WR);
  fastSetOutputMode(dLCD_CS);
  fastSetOutputMode(dLCD_REST);
  
  TRISDbits.TRISD4 = 1;		//RD4 is connected to D2, don't know why! :-P
  PORTDbits.RD4 = 0;
#else
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
  //************* Start Initial Sequence **********//
  LCD_Write_COM(0x00,0xE5); 
  LCD_Write_DATA(0x78,0xF0); // set SRAM internal timing

  LCD_Write_COM(0x00,0x01); 
  LCD_Write_DATA(0x01,0x00); // set SS and SM bit

  LCD_Write_COM(0x00,0x02); 
  LCD_Write_DATA(0x07,0x00); // set 1 line inversion

  LCD_Write_COM(0x00,0x03); 
  LCD_Write_DATA(0x10,0x30); // set GRAM write direction and BGR=1.

  LCD_Write_COM(0x00,0x04); 
  LCD_Write_DATA(0x00,0x00); // Resize register

  LCD_Write_COM(0x00,0x08); 
  LCD_Write_DATA(0x02,0x07); // set the back porch and front porch

  LCD_Write_COM(0x00,0x09); 
  LCD_Write_DATA(0x00,0x00); // set non-display area refresh cycle ISC[3:0]

  LCD_Write_COM(0x00,0x0A); 
  LCD_Write_DATA(0x00,0x00); // FMARK function

  LCD_Write_COM(0x00,0x0C); 
  LCD_Write_DATA(0x00,0x00); // RGB interface setting

  LCD_Write_COM(0x00,0x0D); 
  LCD_Write_DATA(0x00,0x00); // Frame marker Position

  LCD_Write_COM(0x00,0x0F); 
  LCD_Write_DATA(0x00,0x00); // RGB interface polarity
  //*************Power On sequence ****************//
  LCD_Write_COM(0x00,0x10); 
  LCD_Write_DATA(0x00,0x00); // SAP, BT[3:0], AP, DSTB, SLP, STB
  LCD_Write_COM(0x00,0x11); 
  LCD_Write_DATA(0x00,0x07); // DC1[2:0], DC0[2:0], VC[2:0]
  LCD_Write_COM(0x00,0x12); 
  LCD_Write_DATA(0x00,0x00); // VREG1OUT voltage
  LCD_Write_COM(0x00,0x13); 
  LCD_Write_DATA(0x00,0x00); // VDV[4:0] for VCOM amplitude
  LCD_Write_COM(0x00,0x07); 
  LCD_Write_DATA(0x00,0x01);
  Delayms(50); // Dis-charge capacitor power voltage
  LCD_Write_COM(0x00,0x10); 
  LCD_Write_DATA(0x10,0x90); // 1490//SAP, BT[3:0], AP, DSTB, SLP, STB
  LCD_Write_COM(0x00,0x11); 
  LCD_Write_DATA(0x02,0x27); // DC1[2:0], DC0[2:0], VC[2:0]
  Delayms(50); // Delay 50ms
  LCD_Write_COM(0x00,0x12); 
  LCD_Write_DATA(0x00,0x1F); //001C// Internal reference voltage= Vci;
  Delayms(50); // Delay 50ms
  LCD_Write_COM(0x00,0x13); 
  LCD_Write_DATA(0x15,0x00); //0x1000//1400   Set VDV[4:0] for VCOM amplitude  1A00
  LCD_Write_COM(0x00,0x29); 
  LCD_Write_DATA(0x00,0x27); //0x0012 //001a  Set VCM[5:0] for VCOMH  //0x0025  0034
  LCD_Write_COM(0x00,0x2B); 
  LCD_Write_DATA(0x00,0x0D); // Set Frame Rate   000C
  Delayms(50); // Delay 50ms
  LCD_Write_COM(0x00,0x20); 
  LCD_Write_DATA(0x00,0x00); // GRAM horizontal Address
  LCD_Write_COM(0x00,0x21); 
  LCD_Write_DATA(0x00,0x00); // GRAM Vertical Address
  // ----------- Adjust the Gamma Curve ----------//
  LCD_Write_COM(0x00,0x30); 
  LCD_Write_DATA(0x00,0x00);
  LCD_Write_COM(0x00,0x31); 
  LCD_Write_DATA(0x07,0x07);
  LCD_Write_COM(0x00,0x32); 
  LCD_Write_DATA(0x03,0x07);
  LCD_Write_COM(0x00,0x35); 
  LCD_Write_DATA(0x02,0x00);
  LCD_Write_COM(0x00,0x36); 
  LCD_Write_DATA(0x00,0x08);//0207
  LCD_Write_COM(0x00,0x37); 
  LCD_Write_DATA(0x00,0x04);//0306
  LCD_Write_COM(0x00,0x38); 
  LCD_Write_DATA(0x00,0x00);//0102
  LCD_Write_COM(0x00,0x39); 
  LCD_Write_DATA(0x07,0x07);//0707
  LCD_Write_COM(0x00,0x3C); 
  LCD_Write_DATA(0x00,0x02);//0702
  LCD_Write_COM(0x00,0x3D); 
  LCD_Write_DATA(0x1D,0x04);//1604

    //------------------ Set GRAM area ---------------//
  LCD_Write_COM(0x00,0x50); 
  LCD_Write_DATA(0x00,0x00); // Horizontal GRAM Start Address
  LCD_Write_COM(0x00,0x51); 
  LCD_Write_DATA(0x00,0xEF); // Horizontal GRAM End Address
  LCD_Write_COM(0x00,0x52); 
  LCD_Write_DATA(0x00,0x00); // Vertical GRAM Start Address
  LCD_Write_COM(0x00,0x53); 
  LCD_Write_DATA(0x01,0x3F); // Vertical GRAM Start Address
  LCD_Write_COM(0x00,0x60); 
  LCD_Write_DATA(0xA7,0x00); // Gate Scan Line
  LCD_Write_COM(0x00,0x61); 
  LCD_Write_DATA(0x00,0x01); // NDL,VLE, REV
  LCD_Write_COM(0x00,0x6A); 
  LCD_Write_DATA(0x00,0x00); // set scrolling line
  //-------------- Partial Display Control ---------//
  LCD_Write_COM(0x00,0x80); 
  LCD_Write_DATA(0x00,0x00);
  LCD_Write_COM(0x00,0x81); 
  LCD_Write_DATA(0x00,0x00);
  LCD_Write_COM(0x00,0x82); 
  LCD_Write_DATA(0x00,0x00);
  LCD_Write_COM(0x00,0x83); 
  LCD_Write_DATA(0x00,0x00);
  LCD_Write_COM(0x00,0x84); 
  LCD_Write_DATA(0x00,0x00);
  LCD_Write_COM(0x00,0x85); 
  LCD_Write_DATA(0x00,0x00);
  //-------------- Panel Control -------------------//
  LCD_Write_COM(0x00,0x90); 
  LCD_Write_DATA(0x00,0x10);
  LCD_Write_COM(0x00,0x92); 
  LCD_Write_DATA(0x06,0x00);
  LCD_Write_COM(0x00,0x07); 
  LCD_Write_DATA(0x01,0x33); 
  fastWriteHigh(LCD_CS);

  setColor(255, 255, 255);
  setBackColor(0, 0, 0);
  fsize = FONT_SMALL;
}

void setXY(int x1, int y1, int x2, int y2){
	int tmp;

	if (orient==LANDSCAPE)
	{
		tmp=x1;
		x1=y1;
		y1=319-tmp;
		tmp=x2;
		x2=y2;
		y2=319-tmp;
		tmp=y1;
		y1=y2;
		y2=tmp;
	}

	LCD_Write_COM(0x00,0x20);
	LCD_Write_DATA(x1>>8,x1);    
	LCD_Write_COM(0x00,0x21);
	LCD_Write_DATA(y1>>8,y1);
	   
	LCD_Write_COM(0x00,0x50);
	LCD_Write_DATA(x1>>8,x1);   
	LCD_Write_COM(0x00,0x52);
	LCD_Write_DATA(y1>>8,y1);   
	LCD_Write_COM(0x00,0x51);
	LCD_Write_DATA(x2>>8,x2);  
	LCD_Write_COM(0x00,0x53);
	LCD_Write_DATA(y2>>8,y2);   
	LCD_Write_COM(0x00,0x22);           				 
}

void drawRect(int x1, int y1, int x2, int y2){
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
		for (i=0; i<((y2-y1)/2)+1; i++)
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
		setXY(0,0,239,319);
	else
		setXY(0,0,319,239);
	for (i=0; i<76800; i++)
	{
		setPixel(0,0,0);
	}
	fastWriteHigh(LCD_CS); 
}

void fillScr(char r, char g, char b){
	long i;
	
	fastWriteLow(LCD_CS); 
	if (orient==PORTRAIT)
		setXY(0,0,239,319);
	else
		setXY(0,0,319,239);
	for (i=0; i<76800; i++)
	{
		setPixel (r, g, b);
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
	LCD_Write_DATA(((r&248)|g>>5),((g&28)<<3|b>>3));
}

void drawPixel(int x, int y){
	fastWriteLow(LCD_CS);  
	setXY(x, y, x, y);
	setPixel(fcolorr, fcolorg, fcolorb);
	if (orient==PORTRAIT)
		setXY(0,0,239,319);
	else
		setXY(0,0,319,239);
	fastWriteHigh(LCD_CS);  
}

void drawLine(int x1, int y1, int x2, int y2){
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
		if (x1>x2)
		{
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
		delta=(double)(y2-y1)/(x2-x1);
		ty=(double)y1;
		if (x1>x2) {
            for (i=x1; i>=x2; i--){
                drawPixel(i,(int)(ty+0.5));
        		ty=ty-delta;
            }
        }
        else{
            for (i=x1; i<=x2; i++){
                drawPixel(i,(int)(ty+0.5));
        		ty=ty+delta;
            }
        }
	}
	else{
		delta=(double)(x2-x1)/(y2-y1);
		tx=(double)x1;
        if (y1>y2) {
            for (i=y2+1; i>y1; i--){
                drawPixel((int)(tx+0.5), i);
    		    tx=tx+delta;
        		}
        }
        else {
        	for (i=y1; i<y2+1; i++){
                drawPixel((int)(tx+0.5), i);
    		    tx=tx+delta;
            }
        }
	}

	if (orient==PORTRAIT){
		setXY(0,0,239,319);
    }
	else{
		setXY(0,0,319,239);
    }
}

void drawHLine(int x, int y, int l){
	int i;
	
	fastWriteLow(LCD_CS);  
	setXY(x, y, x+l, y);
	for (i=0; i<l+1; i++)
	{
		setPixel(fcolorr, fcolorg, fcolorb);
	}
	fastWriteHigh(LCD_CS);  
}

void drawVLine(int x, int y, int l){
	int i;
	fastWriteLow(LCD_CS);  
	setXY(x, y, x, y+l);
	for (i=0; i<l; i++)
	{
		setPixel(fcolorr, fcolorg, fcolorb);
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
						setPixel(fcolorr, fcolorg, fcolorb);
					} 
					else
					{
						setPixel(bcolorr, bcolorg, bcolorb);
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
						setPixel(fcolorr, fcolorg, fcolorb);
					} 
					else
					{
						setPixel(bcolorr, bcolorg, bcolorb);
					}   
				}
				temp++;
			}
		}
	}
#ifndef _NO_BIG_FONT_
	else {
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
						setPixel(fcolorr, fcolorg, fcolorb);
					} 
					else
					{
						setPixel(bcolorr, bcolorg, bcolorb);
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
						setPixel(fcolorr, fcolorg, fcolorb);
					} 
					else
					{
						setPixel(bcolorr, bcolorg, bcolorb);
					}   
				}
				ch=(BigFont[temp]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<i))!=0)   
					{
						setPixel(fcolorr, fcolorg, fcolorb);
					} 
					else
					{
						setPixel(bcolorr, bcolorg, bcolorb);
					}   
				}
				temp+=2;
			}
		}
	}
#endif
	fastWriteHigh(LCD_CS);
}

void rotateChar(char c, int x, int y, int pos, int deg){
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
					setPixel(fcolorr, fcolorg, fcolorb);
				} 
				else  
				{
					setPixel(bcolorr, bcolorg, bcolorb);
				}   
			}
			temp++;
		}
	}
#ifndef _NO_BIG_FONT_
	else{
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
					setPixel(fcolorr, fcolorg, fcolorb);
				} 
				else  
				{
					setPixel(bcolorr, bcolorg, bcolorb);
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
					setPixel(fcolorr, fcolorg, fcolorb);
				} 
				else  
				{
					setPixel(bcolorr, bcolorg, bcolorb);
				}   
			}
			temp++;
		}
	}
#endif
	fastWriteHigh(LCD_CS);
}

void myGLCD_print(char *st, int x, int y, int deg){
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
			x=320-(stl*8);
		if (x==CENTER)
			x=(320-(stl*8))/2;
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
			x=320-(stl*16);
		if (x==CENTER)
			x=(320-(stl*16))/2;
		}
	}

	for (i=0; i<stl; i++)
		if (deg==0)
			printChar(*st++, x + (i*(fsize*8)), y);
		else
			rotateChar(*st++, x, y, i, deg);
}

void printNumI(long num, int x, int y){
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

  myGLCD_print(st,x,y,0);
}

void printNumF(double num, char dec, int x, int y){
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
	  
	  while (inum>0)
	  {
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

  myGLCD_print(st,x,y,0);
}

void fontSize(char size)
{
#ifdef _NO_BIG_FONT_
	fsize = FONT_SMALL;
#else
	fsize = size;
#endif
}

void drawBitmap(int x, int y, int sx, int sy, unsigned int* data, int scale){
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
				LCD_Write_DATA(col>>8,col & 0xff);
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
					LCD_Write_DATA(col>>8,col & 0xff);
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
							LCD_Write_DATA(col>>8,col & 0xff);
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
							LCD_Write_DATA(col>>8,col & 0xff);
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
	char r, g, b;
	//double radian;
	//radian=deg*0.0175;  

	if (deg==0)
		drawBitmap(x, y, sx, sy, data, 1);
	else
	{
		for (ty=0; ty<sy; ty++)
			for (tx=0; tx<sx; tx++)
			{
				col=(unsigned int)(data[(ty*sx)+tx]);
				r=(col & 0xF800)>>8;
				g=(((col & 0x7e0)>>5)<<2);
				b=(col & 0x1F)<<3;

				setColor(r,g,b);

				newx=x+rox+(((tx-rox)*cosi(deg))-((ty-roy)*sini(deg)));
				newy=y+roy+(((ty-roy)*cosi(deg))+((tx-rox)*sini(deg)));

				drawPixel(newx, newy);
			}
	}
}

#ifdef _ENABLE_tinyFAT_INTEGRATION_
unsigned int loadBitmap(int x, int y, int sx, int sy, char *filename){
	int res;
	int cx, cy, cp;
	unsigned int temp, result;
	char r,g,b;
	int i;

	res=openFile(filename, FILEMODE_BINARY);
	if (res==NO_ERROR)
	{
		fastWriteLow(LCD_CS);  
		cx=0;
		cy=0;
		result=512;
		if (orient==PORTRAIT)
		{
			setXY(x, y, x+sx-1, y+sy-1);
		}
		while (result==512)
		{
			result=readBinary();
			switch(result)
			{
				case ERROR_WRONG_FILEMODE:
					return ERROR_WRONG_FILEMODE;
					break;
				case ERROR_NO_FILE_OPEN:
					return ERROR_NO_FILE_OPEN;
					break;
				default:
					if (orient==PORTRAIT)
					{
						for (i=0; i<result; i+=2)
							LCD_Write_DATA(buffer[i],buffer[i+1]);
					}
					else
					{
						cp=0;
						while (cp<result)
						{
							if (((result-cp)/2)<(sx-cx))
							{
								setXY(x+cx, y+cy, x+cx+((result-cp)/2)-1, y+cy);
								for (i=(result-cp)-2; i>=0; i-=2)
									LCD_Write_DATA(buffer[cp+i],buffer[cp+i+1]);
								cx+=((result-cp)/2);
								cp=result;
							}
							else
							{
								setXY(x+cx, y+cy, x+sx-1, y+cy);
								for (i=sx-cx-1; i>=0; i--)
									LCD_Write_DATA(buffer[cp+(i*2)],buffer[cp+(i*2)+1]);
								cp+=(sx-cx)*2;
								cx=0;
								cy++;
							}
						}
					}
					break;
			}              
		}
		closeFile();
		if (orient==PORTRAIT)
			setXY(0,0,239,319);
		else
			setXY(0,0,319,239);
		fastWriteHigh(LCD_CS);  
		return 0;
	}
	else
	{
		return res;
	}
}
#endif
#endif
