/*
$Id:$

ST7565 LCD library!

Copyright (C) 2010 Limor Fried, Adafruit Industries

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
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

some of this code was written by <cstone@pobox.com> originally; it is in the public domain.
*/

#ifndef __ST7565_H
#define __ST7565_H

#include <typedef.h>

#define BLACK 1
#define WHITE 0

#define LCDWIDTH 128
#define LCDHEIGHT 64

#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF

#define CMD_SET_DISP_START_LINE  0x40
#define CMD_SET_PAGE  0xB0

#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_SET_ADC_NORMAL  0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5
#define CMD_SET_BIAS_9 0xA2 
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW  0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8
#define CMD_SET_POWER_CONTROL  0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST  0x81
#define  CMD_SET_VOLUME_SECOND  0
#define CMD_SET_STATIC_OFF  0xAC
#define  CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define  CMD_SET_BOOSTER_5  1
#define  CMD_SET_BOOSTER_6  3
#define CMD_NOP  0xE3
#define CMD_TEST  0xF0

void ST7565_command(u8 c);
void ST7565_data(u8 c);
void ST7565_init(u8 SID, u8 SCLK, u8 A0, u8 RST, u8 CS);
void ST7565_setBrightness(u8 val);
void ST7565_clearDisplay(void);
void ST7565_clear();
void ST7565_refresh();

static void updateBoundingBox(u8 xmin, u8 ymin, u8 xmax, u8 ymax);
//u8 read_st7565_buffer(u8 pos);

void ST7565_setPixel(u8 x, u8 y, u8 color); 
u8 ST7565_getPixel(u8 x, u8 y);
void ST7565_fillCircle(u8 x0, u8 y0, u8 r, u8 color);
void ST7565_drawCircle(u8 x0, u8 y0, u8 r, u8 color);
void ST7565_drawRect(u8 x, u8 y, u8 w, u8 h, u8 color);
void ST7565_fillRect(u8 x, u8 y, u8 w, u8 h, u8 color);
void ST7565_drawLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 color);
void ST7565_drawChar(u8 x, u8 line, unsigned char c);
void ST7565_drawString(u8 x, u8 line, unsigned char *c);
void ST7565_drawString_P(u8 x, u8 line, const unsigned char *c);

void ST7565_drawBitmap(u8 x, u8 y, const u8 *bitmap, u8 w, u8 h, u8 color);
void ST7565_shiftOut(u8 dataPin, u8 clockPin, u8 bitOrder, u8 val);

void ST7565_my_setPixel(u8 x, u8 y, u8 color);

#endif /* __ST7565_H */
