/*
 *    LedControl.cpp - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 * 
 *    Feb. 2014 - Regis Blanchot - adapted to Pinguino 
 *    Feb. 2014 - Regis Blanchot - added scroll functions 
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LEDCONTROL_C
#define LEDCONTROL_C

#include <typedef.h>
#include <const.h>
#include <macro.h>
#include <ledcontrol.h>
//#include <stdio.h>
#include <string.h>
#include <digitalw.c>       // digitalwrite
#include <digitalp.c>       // pinmode
#include <delayms.c>

//the opcodes for the MAX7221 and MAX7219
#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

void LedControl_init(u8 dataPin, u8 clkPin, u8 csPin, u8 numDevices)
{
    u8 i;
    
    LEDCONTROL_SPI_MOSI = dataPin;
    LEDCONTROL_SPI_CLK  = clkPin;
    LEDCONTROL_SPI_CS   = csPin;
    if(numDevices<=0 || numDevices>8 )
        numDevices=8;
    maxDevices=numDevices;
    pinmode(LEDCONTROL_SPI_MOSI,OUTPUT);
    pinmode(LEDCONTROL_SPI_CLK,OUTPUT);
    pinmode(LEDCONTROL_SPI_CS,OUTPUT);
    digitalwrite(LEDCONTROL_SPI_CS,HIGH);
    LEDCONTROL_SPI_MOSI=dataPin;
    for(i=0;i<64;i++) 
        status[i]=0x00;
    for(i=0;i<maxDevices;i++)
    {
        LedControl_spiTransfer(i,OP_DISPLAYTEST,0);
        //scanlimit is set to max on startup
        LedControl_setScanLimit(i,7);
        //decode is done in source
        LedControl_spiTransfer(i,OP_DECODEMODE,0);
        LedControl_clearDisplay(i);
        //we go into shutdown-mode on startup
        LedControl_shutdown(i,true);
    }
}

u8 LedControl_getDeviceCount() {
    return maxDevices;
}

void LedControl_shutdown(u8 matrix, boolean b)
{
    if(matrix<0 || matrix>=maxDevices)
        return;
    if(b)
        LedControl_spiTransfer(matrix, OP_SHUTDOWN,0);
    else
        LedControl_spiTransfer(matrix, OP_SHUTDOWN,1);
}

void LedControl_setScanLimit(u8 matrix, u8 limit)
{
    if(matrix<0 || matrix>=maxDevices)
        return;
    if(limit>=0 || limit<8)
        LedControl_spiTransfer(matrix, OP_SCANLIMIT,limit);
}

void LedControl_setIntensity(u8 matrix, u8 intensity)
{
    if(matrix<0 || matrix>=maxDevices)
        return;
    if(intensity>=0 || intensity<16)
        LedControl_spiTransfer(matrix, OP_INTENSITY,intensity);
}

void LedControl_clearDisplay(u8 matrix)
{
    u8 offset;
    u8 i;
    
    if(matrix<0 || matrix>=maxDevices)
        return;
    offset=matrix*8;
    for(i=0;i<8;i++)
    {
        status[offset+i]=0;
        LedControl_spiTransfer(matrix, i+1,status[offset+i]);
    }
}

void LedControl_clearAll()
{
    u8 i;
    
    for (i=0;i<maxDevices;i++)
        LedControl_clearDisplay(i);
}

void LedControl_setLed(u8 matrix, u8 row, u8 column, boolean state)
{
    u8 offset;
    u8 val=0x00;

    if(matrix<0 || matrix>=maxDevices)
        return;
    if(row<0 || row>7 || column<0 || column>7)
        return;
    offset=matrix*8;
    val=0b10000000 >> column;
    if(state)
        status[offset+row]=status[offset+row]|val;
    else
    {
        val=~val;
        status[offset+row]=status[offset+row]&val;
    }
    LedControl_spiTransfer(matrix, row+1,status[offset+row]);
}

void LedControl_setRow(u8 matrix, u8 row, u8 value)
{
    u8 offset;
    
    if(matrix<0 || matrix>=maxDevices)
        return;
    if(row<0 || row>7)
        return;
    offset=matrix*8;
    status[offset+row]=value;
    LedControl_spiTransfer(matrix, row+1,status[offset+row]);
}
    
void LedControl_setColumn(u8 matrix, u8 col, u8 value)
{
    u8 val;
    u8 row;

    if(matrix<0 || matrix>=maxDevices)
        return;
    if(col<0 || col>7) 
        return;
    for(row=0;row<8;row++)
    {
        //val=value >> (7-row);
        val=value >> row;
        val=val & 0x01;
        LedControl_setLed(matrix,row,col,val);
    }
}

#if defined(SETDIGIT)
void LedControl_setDigit(u8 matrix, u8 digit, u8 value, boolean dp)
{
    u8 offset;
    u8 v;

    if(matrix<0 || matrix>=maxDevices)
        return;
    if(digit<0 || digit>7 || value>15)
        return;
    offset=matrix*8;
    v=charTable[value];
    if(dp)
        v|=0b10000000;
    status[offset+digit]=v;
    LedControl_spiTransfer(matrix, digit+1,v);
    
}
#endif

#if defined(SETCHAR)
void LedControl_setChar(u8 matrix, u8 digit, char value, boolean dp)
{
    u8 offset;
    u8 index,v;

    if(matrix<0 || matrix>=maxDevices)
        return;
    if(digit<0 || digit>7)
        return;
    offset=matrix*8;
    index=(u8)value;
    if(index >127) {
        //nothing define we use the space char
        value=32;
    }
    v=charTable[index];
    if(dp)
        v|=0b10000000;
    status[offset+digit]=v;
    LedControl_spiTransfer(matrix, digit+1,v);
}
#endif

void shiftOut(u8 dataPin, u8 clockPin, u8 bitOrder, u8 val)
{
    u8 i;
    u8 bitMask;

    for (i = 0; i < 8; i++)
    {
        if (bitOrder == LSBFIRST)
            bitMask = (1 << i);
        else
            bitMask = (1 << (7 - i));

        digitalwrite(dataPin, (val & bitMask) ? HIGH : LOW);
        digitalwrite(clockPin, HIGH);
        digitalwrite(clockPin, LOW);            
    }
}

void LedControl_spiTransfer(u8 matrix, volatile u8 opcode, volatile u8 data)
{
    //Create an array with the data to shift out
    u8 offset=matrix*2;
    u8 maxbytes=maxDevices*2;
    u8 i;
    
    for(i=0;i<maxbytes;i++)
        spidata[i]=(u8)0;
    //put our device data into the array
    spidata[offset+1]=opcode;
    spidata[offset]=data;
    //enable the line 
    digitalwrite(LEDCONTROL_SPI_CS,LOW);
    //Now shift out the data 
    for(i=maxbytes;i>0;i--)
        shiftOut(LEDCONTROL_SPI_MOSI,LEDCONTROL_SPI_CLK,MSBFIRST,spidata[i-1]);
    //latch the data onto the display
    digitalwrite(LEDCONTROL_SPI_CS,HIGH);
}    

/*
u8 LedControl_getCharArrayPosition(char input)
{
     if ((input==' ')||(input=='+')) return 10;
     if (input==':') return 11;
     if (input=='-') return 12;
     if (input=='.') return 13;
     if ((input =='(')) return  14;  //replace by 'ñ'   
     if ((input >='0')&&(input <='9')) return (input-'0');
     if ((input >='A')&&(input <='Z')) return (input-'A' + 15);
     if ((input >='a')&&(input <='z')) return (input-'a' + 15);     
     return 13;
}  
*/

// Scroll Message
// d'après : http://breizhmakers.over-blog.com/article-un-peu-d-animation-ou-le-scrolling-a-base-de-max7219-105669349.html
#if defined(SCROLL)
void LedControl_scroll(const char * displayString)
{
    u8 nextchar;   // next char to display
    u8 r;          // current row
    u8 m;           // current matrix
    u8 ascii;     // current ASCII code
    u8 curchar;    // current displayed char
    u8 offset;     // nombre de bits à prendre dans l'octet suivant
    u8 row[8];     //
    u8 scrollmax = 8 * max(maxDevices, strlen(displayString));
    
    // octet de départ
    curchar = scroll / 8;

    // nombre de bits à prendre dans l'octet suivant
    offset = scroll % 8;

    for (m=0; m<maxDevices; m++)
    {
        // for every line of the matrix
        for(r=0; r<8; r++)
        {
            row[r]=0;

            // matrix = current char shifted by offset

            ascii = displayString[curchar]-32;
            row[r] |= ( font[ascii][r] >> offset );

            // si on n'est pas sur le premier bit d'un octet il faut prendre les bits qui restent à
            // afficher dans l'octet suivant

            if (offset > 0) 
            {
                ascii = displayString[curchar+1]-32;
                if (ascii >= 0x20 && ascii <= 0x7F)
                {
                    nextchar = font[ascii][r];
                    row[r] |=  nextchar << (8-offset);
                }
            }
        }

        // Display the new matrix
        for (r=0; r<8; r++)
            LedControl_setColumn(m, 7-r, row[r]);     

        curchar++;
    }

    // and wait a bit
    Delayms(50);

    // Do we cover the whole scroll area ?
    scroll = (scroll+1) % scrollmax;
}
#endif

#if defined(WRITESTRING)
void LedControl_writeString(const char * displayString)
{
    u8 matrix=0;
    char c;
    
    while (c = *displayString++)
    {
        LedControl_displayChar(matrix, c);
        Delayms(300);
        //LedControl_clearAll();
        //LedControl_clearDisplay(matrix);
        matrix++;
        if (matrix >= maxDevices)
            matrix=0;
    }
    Delayms(300);
}
#endif

#if defined(WRITESTRING) || defined(DISPLAYCHAR)
void LedControl_displayChar(u8 matrix, u8 charIndex)
{
    u8 i;
    
    if (charIndex >= 0x20 && charIndex <= 0x7F)
    {
        for (i=0; i<8;i++)
            LedControl_setColumn(matrix, 7-i, font[charIndex-32][i]);
    }
}
#endif

#endif /* LEDCONTROL_C */
