/*	----------------------------------------------------------------------------
    FILE:			graphics.c
    PROJECT:		pinguino
    PURPOSE:		graphic routines
    PROGRAMER:		Henning Karlsen http://www.henningkarlsen.com
                    Marcus Fazzi
                    Regis Blanchot
    FIRST RELEASE:	10 Jul. 2010
    LAST RELEASE:	03 Apr. 2012
    ----------------------------------------------------------------------------
    CHANGELOG : 

    Jul  10 2010  - initial release
    Aug  11 2010  - Fixed a small bug with the color green.
                    Thanks to Thomas finding and fixing the bug.
    Aug  13 2010  - Added the possibility to use the display in
                    Landscape mode. Also added a larger font by
                    request.
    Sep  30 2010  - Added Arduino Mega compatibility.
                    Fixed a bug with CENTER and RIGHT in LANDSCAPE mode.
                    Fixed a bug in printNumI and printNumF when the 
                    number to be printed was 0.
    Oct  14 2010  - Added drawBitmap() with its associated tool
    Nov  24 2010  - Added Arduino Mega2560 compatibility
                    Added support for rotating text and bitmaps
    Jan  18 2011  - Fixed an error in the requirements
    Jan  30 2011  - Added loadBitmap()
                    Optimized drawBitmap() when not using rotation
    Mar  04 2011  - Port to Pinguino 32X, By Marcus Fazzi
    Mar  08 2011  - Fixed a bug in printNumF when the number to be printed
                    was (-)0.something
    Fev  29 2012  - Added support to OLIMEX Boards.
                    There is a problem with D2 pin (PIC32 pin52 ?). using A6.
    ----------------------------------------------------------------------------
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    --------------------------------------------------------------------------*/

#ifndef __GRAPHICS_C
#define __GRAPHICS_C

#include <typedef.h>    // u8, u16, ..
#include <macro.h>      // swap
#include <mathlib.c>    // abs
//#define _abs_(a) (((a)> 0) ? (a) : -(a))

// Specific to each display
extern void drawPixel(u16, u16);
extern void drawVLine(u16, u16, u16);
extern void drawHLine(u16, u16, u16);
//extern void setColor(u16 c);

/*	--------------------------------------------------------------------
    Prototypes
    ------------------------------------------------------------------*/

void drawCircle(u16 x, u16 y, u16 radius)
{
    s16 f = 1 - radius;
    s16 ddF_x = 1;
    s16 ddF_y = -2 * radius;
    s16 x1 = 0;
    s16 y1 = radius;

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

void fillCircle(u16 x, u16 y, u16 radius)
{
    s16 x1, y1;
    for(y1=-radius; y1<=radius; y1++) 
        for(x1=-radius; x1<=radius; x1++) 
            if(x1*x1+y1*y1 <= radius*radius) 
                drawPixel(x+x1, y+y1); 
}

void drawLine(u16 x0, u16 y0, u16 x1, u16 y1)
{
    s16 steep, t;
    s16 deltax, deltay, error;
    s16 x, y;
    s16 ystep;
    
    // simple clipping
    /*
    if (( x0 < 0) || (x0 > HRES)) return;
    if (( x1 < 0) || (x1 > HRES)) return;
    if (( y0 < 0) || (y0 > VRES)) return;
    if (( y1 < 0) || (y1 > VRES)) return;
    */
    
    steep = abs(y1 - y0) > abs(x1 - x0);

    if (steep) 
    {
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1) 
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    deltax = x1 - x0;
    deltay = abs(y1 - y0);

    error = 0;
    y = y0;

    if (y0 < y1) ystep = 1; else ystep = -1;

    for (x = x0; x < x1; x++)
    {
        if (steep)
            drawPixel(y,x);
        else
            drawPixel(x,y);
            
        error += deltay;

        if ( (error<<1) >= deltax)
        {
            y += ystep;
            error -= deltax;
        }
    }
}

void drawRect(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 tmp;

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

void drawRoundRect(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 tmp;

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

void fillRect(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 tmp, i;

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
    for (i=0; i<((x2-x1)/2)+1; i++)
    {
        drawVLine(x1+i, y1, y2-y1);
        drawVLine(x2-i, y1, y2-y1);
    }
}

void fillRoundRect(u16 x1, u16 y1, u16 x2, u16 y2){
    u16 tmp, i;

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

/*
void rotateChar(char c, u16 x, u16 y, u16 pos, u16 deg){
    char i,j,ch;
    unsigned u16 temp;
    u16 newx,newy;
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
*/

/*
void drawBitmap(u16 xo, u16 yo, u16 w, u16 h, const u16* bitmap)
{
    u16 i, j;

    for (j=0; j<h; j++)
    {
        for (i=0; i<w; i++)
        {
            //setColor(bitmap[j * w + i + 2]);
            drawPixel(xo+i, yo+j);
            //ILI9325_write(WriteDatatoGRAM, bitmap[ ( j * w ) + i + 2]);
            //ILI9325_write(GRAMHorizontalAddressSet, xo + i);
            //ILI9325_write(GRAMVerticalAddressSet,   yo + j);
        }
    }
}
*/

/*
void drawBitmapR(u16 x, u16 y, u16 sx, u16 sy, unsigned u16* data, u16 deg, u16 rox, u16 roy)
{
    unsigned u16 col;
    u16 tx, ty, newx, newy;
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
                col=(unsigned u16)(data[(ty*sx)+tx]);
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

unsigned u16 loadBitmap(u16 x, u16 y, u16 sx, u16 sy, char *filename)
{
    u16 res;
    u16 cx, cy, cp;
    unsigned u16 temp, result;
    char r,g,b;
    u16 i;

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
*/

#endif /* __GRAPHIC_C */
