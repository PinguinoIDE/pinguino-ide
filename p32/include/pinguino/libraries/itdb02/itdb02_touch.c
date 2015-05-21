/*
  ITDB02_Touch.cpp - Arduino library support for ITDB02 Touch function
  Copyright (C)2010 Henning Karlsen. All right reserved
  
  Basic functionality of this library are based on the demo-code provided by
  ITead studio. You can find the latest version of the library at
  http://www.henningkarlsen.com/electronics

  This library has been made for the 2.4" TFT LCD Screen Module: ITDB02-2.4, 
  the 3.2" TFT LCD Screen Module: ITDB02-3.2 and the 3.2" Wide-screen module 
  ITDB02-3.2WC by ITead studio.

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

  Version:   1.0  - Sep  13 2010  - initial release
			 1.01 - Sep  18 2010  - Added example: ITDB02_Touch_ButtonTest
			 1.1  - Oct   7 2010  - Fixed incompatibility with ITDB02-3.2
			 1.2  - Oct  12 2010  - Added support for ITDB02-3.2WC
			 1.2.1- Mar  11 2011  - Port to Pinguino32X (PIC32) by Marcus Fazzi
             1.2.2- Aug  29 2011  - Support for ITDB02 3.2" widescreen

*/

#ifndef ITDB02_Touch_c
#define ITDB02_Touch_c

//Use ITDB02_Touch_Calibration.pde to setup this values, for best results.
//ITDB02 3.2" Display 320x240
#ifdef ITDB02_32
#define PixSizeX	-16 //-15.58
#define PixOffsX	206
#define PixSizeY	11 //10.95
#define PixOffsY	380

//ITDB02 3.2" widescreen Display 400x240
#elif defined(ITDB02_32w)
#define PixSizeX	-14 //-14.41
#define PixOffsX	323
#define PixSizeY	-9
#define PixOffsY	155

//ITDB02 2.4" Display 320x240
#else
#define PixSizeX	15 //14.44
#define PixOffsX	220
#define PixSizeY	11 //11.07
#define PixOffsY	358
#endif

#include <itdb02/itdb02_touch.h>
#include <digitalw.c>
//myTouch.setTouch(56,57,58,59,60); //clk, cs, din, dout, irq
//		   A6,G0,G1,F1,F0
void touch_setTouch(unsigned char tclk, unsigned char tcs, unsigned char din, unsigned char dout, unsigned char irq){
    T_CLK = tclk;
    T_CS  = tcs;
    T_DIN = din;
    T_DOUT= dout;
    T_IRQ = irq;
}

void touch_initTouch(unsigned char orient_tation){
    orient_t = orient_tation;

    pinmode(T_CLK,  OUTPUT);
    pinmode(T_CS,   OUTPUT);
    pinmode(T_DIN,  OUTPUT);
    pinmode(T_DOUT, INPUT);
    pinmode(T_IRQ,  INPUT);

    digitalwrite(T_CS,  HIGH);
    digitalwrite(T_CLK, HIGH);
    digitalwrite(T_DIN, HIGH);
    digitalwrite(T_CLK, HIGH);
}

void touch_writeData(unsigned char data){
    unsigned char temp;
    unsigned char nop;
    unsigned char count;

    temp=data;
    digitalwrite(T_CLK,LOW);
    
    for(count=0; count<8; count++) {
	if(temp & 0x80)
		digitalwrite(T_DIN, HIGH);
	else
		digitalwrite(T_DIN, LOW);
	
	temp = temp << 1; 
	digitalwrite(T_CLK, LOW);	
	nop++;
	digitalwrite(T_CLK, HIGH);	
	nop++;
    }
}

unsigned int touch_readData(){
    unsigned char nop;
    unsigned int data = 0;
    unsigned char count;
    
    for(count=0; count<12; count++) {
	data <<= 1;
	digitalwrite(T_CLK, HIGH);	          
	nop++;
	digitalwrite(T_CLK, LOW);	
	nop++;
	if (digitalread(T_DOUT))
	    data++;
    }
    return(data);
}

void touch_read() {
    unsigned long tx=0;
    unsigned long ty=0;
    int i;
    
    digitalwrite(T_CS,LOW);
                  
    for (i=0; i<prec; i++){
	touch_writeData(0x90);        
	digitalwrite(T_CLK,HIGH);
	digitalwrite(T_CLK,LOW);	
	ty+=touch_readData();
	touch_writeData(0xD0);      
	digitalwrite(T_CLK,HIGH);	
	digitalwrite(T_CLK,LOW);
	tx+=touch_readData();
    }

    digitalwrite(T_CS,HIGH);
    
    TP_X=tx/prec;
    TP_Y=ty/prec;
}

unsigned char touch_dataAvailable(){
  unsigned char avail;
  avail = !digitalread(T_IRQ);
  return avail;
}

int touch_getX(){
    int value;

    if (orient_t == PORTRAIT) {
	    if (PixSizeX>=0) {
		value = (240-(TP_X-PixOffsX)/PixSizeX);
	    }
	    else {
		value = ((TP_X-PixOffsX)/-(PixSizeX));
	    }
    }
    else {
	if (PixSizeY<0)
	    value = (400-(TP_Y-PixOffsY)/-PixSizeY);
	else
	    value = ((TP_Y-PixOffsY)/PixSizeY);
    }

    if (value < 0)
	value = 0;
	
    return value;
}

int touch_getY(){
    int value;

    if (orient_t == PORTRAIT){
	    if (PixSizeY<0)
		    value = ((TP_Y-PixOffsY)/-PixSizeY);
	    else
		    value = (320-(TP_Y-PixOffsY)/PixSizeY);
    }
    else{
	if (PixSizeX>=0){
	    value = (240-(TP_X-PixOffsX)/PixSizeX);
	}
	else{
	    value = ((TP_X-PixOffsX)/-(PixSizeX));
	}
    }

    if (value < 0)
	    value = 0;
    return value;  
}

void touch_setPrecision(unsigned char precision){
    switch (precision){
	case PREC_LOW:
	    prec=1;
	    break;
	case PREC_MEDIUM:
	    prec=10;
	    break;
	case PREC_HI:
	    prec=25;
	    break;
	case PREC_EXTREME:
	    prec=100;
	    break;
	default:
	    prec=10;
	    break;
    }
}

#endif
