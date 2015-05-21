/*	--------------------------------------------------------------------
    FILE:			ili9325.c
    PROJECT:		pinguino
    PURPOSE:		Drive 2.4" TFT LCD (ILI9325 controller)
    PROGRAMER:		regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:	1 Apr. 2012
    LAST RELEASE:	7 Dec. 2013
    ----------------------------------------------------------------------------
    TODO : 
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
    ------------------------------------------------------------------*/

#ifndef __ILI9325_C
#define __ILI9325_C

#include <stdarg.h>
#include <typedef.h>
#include <macro.h>
#include <ili9325.h>
#ifndef DISPLAY_FONT
#include <fonts/font12x8.h>         // default font
#endif
#include <delayms.c>
#include <graphics.c>
#include <digitalw.c>
#include <stdio.c>

///	--------------------------------------------------------------------
/// Write functions
///	--------------------------------------------------------------------

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Write an index to IR register
        When use in i80/8-bit mode, 16-bit data is divided into
        * upper byte is sent first
        * then the lower byte
    PARAMETERS:
        index : IR register's index 
    RETURNS:
        none
    REMARKS:
        RS = 0, WR = 0, RD = 1
    ------------------------------------------------------------------*/

void ILI9325_writeRegister(u8 index)
{
    //LowCS;
    LowRS;	// Enable Register Selection Signal
    HighRD; // Disable Read Mode
    HighWR; // Disable Write Mode

    DATA = 0;
    
    LowWR;	// Enable Write
    HighWR;	// Disable Write

    DATA = index;

    LowWR;	// Enable Write
    HighWR;	// Disable Write
    //HighCS;
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Write to control registers or the internal GRAM by WDR register
        When use in i80/8-bit mode, 16-bit data is divided into
        * upper byte is sent first
        * then the lower byte
    PARAMETERS:
        data15 : data to send 
    RETURNS:
        none
    REMARKS:
        RS = 1, WR = 0, RD = 1
    ------------------------------------------------------------------*/

void ILI9325_writeData(u16 data15)
{
    word_t d;

    d.w = data15;

    //LowCS;
    HighRS;	// Disable Register Selection Signal
    HighRD; // Disable Read Mode
    HighWR; // Disable Write Mode

    DATA = d.h8;

    LowWR;	// Enable Write
    HighWR;	// Disable Write

    DATA = d.l8;

    LowWR;	// Enable Write
    HighWR;	// Disable Write
    //HighCS;
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Writes the selected LCD Register.
    PARAMETERS:
        index  = 16-bit Index Register
        data15 = 16-bit Data
    RETURNS:
        LCD Register Value.
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_write(u8 index, u16 data15)
{
    ILI9325_writeRegister(index);
    ILI9325_writeData(data15);
}

///	--------------------------------------------------------------------
/// Read functions
///	--------------------------------------------------------------------

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Reads the selected LCD Register.
        Read from the internal GRAM by RDR register.
    PARAMETERS:
    RETURNS:
        LCD Register Value.
    REMARKS:
//    Delayms(10);
    ------------------------------------------------------------------*/

u16 ILI9325_readData()
{
    word_t d;
    
    //LowCS;          // Select LCD
    HighRS;         // Disable Register Selection Signal
    HighWR;         // Disable Write Mode
    HighRD;         // Disable Read (Write Mode)

    // Data line as input
    dDATA = 0xFF;

    LowRD;          // Enable Read Mode
    HighRD;         // Disable Read Mode
    
    d.h8 = DATA;    // Read High Byte Data

    LowRD;          // Enable Read Mode
    HighRD;         // Disable Read Mode

    d.l8 = DATA;    // Read Low Byte Data

    // Data line as output
    dDATA = 0x00;

    //HighCS;         // Release LCD

    return d.w;
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Reads the selected LCD Register.
    PARAMETERS:
        index = 16-bit Index
    RETURNS:
        LCD Register Value.
    REMARKS:
    ------------------------------------------------------------------*/

u16 ILI9325_read(u8 index)
{
    ILI9325_writeRegister(index);
    return ILI9325_readData();
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Returns the 16-bit (4-hexdigit) controller code
    PARAMETERS:
    RETURNS:
        ex. 0x9325 for ILI9325 Device
        or  0x9320 for ILI9320 Device
    REMARKS:
    ------------------------------------------------------------------*/

u16 ILI9325_getDeviceID()
{
    return ILI9325_read(DriverCodeRead);
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Initialize LCD
    PARAMETERS:
    RETURNS:
    REMARKS:
        Orientation set to portrait by default
    ------------------------------------------------------------------*/

void ILI9325_init()
{
    ///---------- Default Screen Values

    ILI9325.orientation = PORTRAIT;
    ILI9325.cursor.x      = 0;
    ILI9325.cursor.y      = 0;
    ILI9325.screen.startx = 0;
    ILI9325.screen.starty = 0;
    ILI9325.screen.endx   = 239;
    ILI9325.screen.endy   = 319;
    ILI9325.screen.width  = 240;
    ILI9325.screen.height = 320;
    ILI9325.font.width    = ILI9325_getFontWidth();
    ILI9325.font.height   = ILI9325_getFontHeight();

    ///---------- IO's Output by default

    dDATA = OUTPUT;
    dCMD1 = OUTPUT;
    dCMD2 = OUTPUT;

    ///---------- Clear Command lines

    DATA = 0x00;

    ///---------- Idle

    HighCS;
    HighRD; // Disable Read Mode
    HighWR; // Disable Write Mode

    ///---------- Reset LCD Driver

    HighRST;
    Delayms(5);     // 1ms min.
    LowRST;
    Delayms(2);     // no min.
    HighRST;
    Delayms(100);   // 50ms min.

    LowCS;	// Enable LCD -> not needed if CS is connected to GND

    ///---------- Start Initial Sequence

    //---------- Start internal oscillator
    ILI9325_write(DriverCodeRead, 0x0001);
    Delayms(50);

    // SS  = 1 (shift dir. to the right)
    ILI9325_write(DriverOutputControl, 0x0100);

    // B/C = 1
    // EOR = 1
    ILI9325_write(LCDDrivingWaveControl, 0x0700); // set the line inversion

    // AM  = 0
    // I/D = 11 (H and V increment)
    // BGR = 1  (Swap the RGB data to BGR in writing into GRAM)
    ILI9325_write(EntryMode, 0x1030);		    //set GRAM write direction and BGR=1 PORTRAIT MODE
    ILI9325_write(ResizingControlRegister, 0x0000);
    ILI9325_write(DisplayControl2, 0x0202); // set the back and front porch to 4 lines
    ILI9325_write(DisplayControl3, 0x0000); // set non-display area refresh cycle ISC[3:0]
    ILI9325_write(DisplayControl4, 0x0000); // FMARK function
    ILI9325_write(RGBDisplayInterfaceControl1, 0x0000); // RGB interface setting
    ILI9325_write(FrameMarkerPosition, 0x0000); // Frame marker Position
    ILI9325_write(RGBDisplayInterfaceControl2, 0x0000); // RGB interface polarity

    ///---------- Power On sequence

    ILI9325_write(PowerControl1,   0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
    ILI9325_write(PowerControl2,   0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
    ILI9325_write(PowerControl3,   0x0000); // VREG1OUT voltage
    ILI9325_write(PowerControl4,   0x0000); // VDV[4:0] for VCOM amplitude
    //ILI9325_write(DisplayControl1, 0x0001);
    Delayms(200); // Discharge capacitor power voltage

    ILI9325_write(PowerControl1, 0x14B0); // SAP=1, BT[3:0]=110, APE=1, AP=001, DSTB=0, SLP=0, STB=0
    ILI9325_write(PowerControl2, 0x0007); // DC1[2:0]=010, DC0[2:0]=010, VC[2:0]=000
    Delayms(50);

    ILI9325_write(PowerControl3, 0x008E); // External reference voltage= Vci;
    Delayms(50);

    ILI9325_write(PowerControl4, 0x0C00); // R13=1200 when R12=009D;VDV[4:0] for VCOM amplitude
    ILI9325_write(PowerControl7, 0x0015); // R29=000C when R12=009D;VCM[5:0] for VCOMH
    //ILI9325_write(FrameRateandColorControl, 0x000D); // Set Frame Rate
    Delayms(50);

    ///---------- Adjust the Gamma Curve

    ILI9325_write(GammaControl1,  0x0000);
    ILI9325_write(GammaControl2,  0x0502);
    ILI9325_write(GammaControl3,  0x0307);
    ILI9325_write(GammaControl4,  0x0305);
    ILI9325_write(GammaControl5,  0x0004);
    ILI9325_write(GammaControl6,  0x0402);
    ILI9325_write(GammaControl7,  0x0707);
    ILI9325_write(GammaControl8,  0x0503);
    ILI9325_write(GammaControl9,  0x1505);
    ILI9325_write(GammaControl10, 0x1505);

    ///---------- Set GRAM area

    ILI9325_write(GRAMHorizontalAddressSet, 0x0000);     // GRAM horizontal Address
    ILI9325_write(GRAMVerticalAddressSet, 0x0000);       // GRAM Vertical Address
    ILI9325_write(HorizontalRAMStartAddressPosition, 0); // Horizontal GRAM Start Address
    ILI9325_write(HorizontalRAMEndAddressPosition, 239); // Horizontal GRAM End Address
    ILI9325_write(VerticalRAMStartAddressPosition, 0);   // Vertical GRAM Start Address
    ILI9325_write(VerticalRAMEndAddressPosition, 319);   // Vertical GRAM Start Address

    ILI9325_write(DriverOutputControl2, 0xA700); // Gate Scan Line
    ILI9325_write(BaseImageDisplayControl, 0x0003);//b111); // NDL, VLE, REV
    //ILI9325_write(VerticalScrollControl, 0x0000);
    
    ///---------- Partial Display Control

    ILI9325_write(PartialImage1DisplayPosition, 0x0000);
    ILI9325_write(PartialImage1RAMStartAddress, 0x0000);
    ILI9325_write(PartialImage1RAMEndAddress, 0x0000);
    ILI9325_write(PartialImage2DisplayPosition, 0x0000);
    ILI9325_write(PartialImage2RAMStartAddress, 0x0000);
    ILI9325_write(PartialImage2RAMEndAddress, 0x0000);

    ///---------- Panel Control

    ILI9325_write(PanelInterfaceControl1, 0x0010); // sync with fosc/1 - 16 clocks/line
    //ILI9325_write(PanelInterfaceControl2, 0x0600); // 0 clock overlap period
//  ILI9325_write(PanelInterfaceControl3, 0x0001);
//	ILI9325_write(PanelInterfaceControl4, 0x0110); // 16 clocks/line - min. div. ratio
//	ILI9325_write(PanelInterfaceControl5, 0x0000);
//	ILI9325_write(PanelInterfaceControl6, 0x0000);
//  Delayms(50);

    //---------- Display ON

    // D[1:0] = 11, display ON (00 = display OFF)
    // CL = 0, 262K color
    // GON & DET = 11, normal display
    // BASEE = 1, the base image is displayed
    // PTDE[1:0] = 00, only base image is displayed

    ILI9325_write(DisplayControl1, 0x0133);
    //Delayms(50);
    //I9325_writeRegister(WriteDatatoGRAM);		// write GRAM

    //HighCS;	// Disable LCD

    ///---------- Default settings

    ILI9325_setColor(White);
    ILI9325_setBackgroundColor(Blue);
    ILI9325_setPortrait();
    ILI9325_clearScreen();
}

///	--------------------------------------------------------------------
/// Color functions
///	--------------------------------------------------------------------

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Convert 24-bit to 16-bit rgb565 color.
    PARAMETERS:
        red, green and blue color components
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

u16 ILI9325_color565(u8 r, u8 g, u8 b)
{
/*
    u16 c;

    c = r >> 3;
    c <<= 6;
    c |= g >> 2;
    c <<= 5;
    c |= b >> 3;

    return c;
*/
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Save current color.
    PARAMETERS:
        rgb565 color
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_setColor(u16 color)
{
    ILI9325.color.r = (color >> 11) & 0x1f;
    ILI9325.color.g = (color >> 5)  & 0x3f;
    ILI9325.color.b = (color >> 0)  & 0x1f;
    ILI9325.color.c = color; // ILI9325_color565(r,g,b);
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Save current background color.
    PARAMETERS:
        rgb565 color
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_setBackgroundColor(u16 color)
{
    ILI9325.bcolor.r = (color >> 11) & 0x1f;
    ILI9325.bcolor.g = (color >> 5)  & 0x3f;
    ILI9325.bcolor.b = (color >> 0)  & 0x1f;
    ILI9325.bcolor.c = color; // ILI9325_color565(r,g,b);
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Test
    PARAMETERS:
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_test(void)
{
    u16 x,y;

    ILI9325_setCursor(0,0);	// set cursor home

    //LowCS;	// Enable LCD

    ILI9325_writeRegister(WriteDatatoGRAM);		// write GRAM
    for (y=ILI9325.screen.starty; y<ILI9325.screen.endy; y++)
    {
        for (x=ILI9325.screen.startx; x<ILI9325.screen.endx; x++)
        {
            if      (y>213) ILI9325_writeData(Red);
            else if (y>106) ILI9325_writeData(White);
            else            ILI9325_writeData(Blue);
        }
    }

    //HighCS;	// Disable LCD
}

///	--------------------------------------------------------------------
/// Graphic functions
///	--------------------------------------------------------------------

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Sets the cursor to the specified x,y position
    PARAMETERS:
        0 < x < 319 if 0 < y < 239
    or	0 < x < 239 if 0 < y < 319
    RETURNS:
    REMARKS:
    TODO:
        check x and y
    ------------------------------------------------------------------*/

void ILI9325_setCursor(u16 x, u16 y)
{
    if (ILI9325.orientation == LANDSCAPE)
    {
        ILI9325.cursor.x = y;			// save current position
        ILI9325.cursor.y = x;
    }
    else
    {
        ILI9325.cursor.x = x;			// save current position
        ILI9325.cursor.y = y;
    }
    
    //LowCS;	// Enable LCD
    ILI9325_write(GRAMHorizontalAddressSet, ILI9325.cursor.x);
    ILI9325_write(GRAMVerticalAddressSet, ILI9325.cursor.y);
    //HighCS;	// Disable LCD
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Draws a pixel with current color.
    PARAMETERS:
        x,y coord.
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_drawPixel(u16 x, u16 y)
{
    if ((x > ILI9325.screen.endx) || (y > ILI9325.screen.endy)) return;
    ILI9325_setCursor(x, y);
    //LowCS; // Enable LCD
    ILI9325_write(WriteDatatoGRAM, ILI9325.color.c);// write color to GRAM
    //HighCS;	// Disable LCD
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Delete a pixel with current background color.
    PARAMETERS:
        x,y coord.
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_delPixel(u16 x, u16 y)
{
    if ((x > ILI9325.screen.endx) || (y > ILI9325.screen.endy)) return;
    ILI9325_setCursor(x, y);
    //LowCS; // Enable LCD
    ILI9325_write(WriteDatatoGRAM, ILI9325.bcolor.c);// write color to GRAM
    //HighCS;	// Disable LCD
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Read pixel color.
    PARAMETERS:
        x,y coord.
    RETURNS:
        color
    REMARKS:
    ------------------------------------------------------------------*/

u16 ILI9325_readPixel(u16 x, u16 y)
{
    u16 c;
    
    if ((x > ILI9325.screen.endx) || (y > ILI9325.screen.endy)) return 0;
    ILI9325_setCursor(x, y);
    //LowCS;
    c = ILI9325_read(ReadDatatoGRAM);
    //HighCS;
    
    return c;
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Set window setting.
    PARAMETERS:
        x1: Horizontal Address Start Position
        y1: Vertical Address Start Position
        x2: Horizontal Address End Position
        y2: Vertical Address End Position
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_setWindow(u16 x1, u16 y1, u16 x2, u16 y2)
{
    ILI9325.screen.startx = x1;
    ILI9325.screen.starty = y1;
    ILI9325.screen.endx = x2;
    ILI9325.screen.endy = y2;
    ILI9325.screen.width = x2 - x1;
    ILI9325.screen.height = y2 - y1;
    
    //LowCS;	// Enable LCD

    ILI9325_write(HorizontalRAMStartAddressPosition, x1);
    ILI9325_write(HorizontalRAMEndAddressPosition, x2);
    ILI9325_write(VerticalRAMStartAddressPosition, y1);
    ILI9325_write(VerticalRAMEndAddressPosition, y2);

    //HighCS;	// Disable LCD

    ILI9325_setCursor(x1, y1);
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Clear Screen with current background color.
    PARAMETERS:
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_clearScreen()
{
    word_t c;
    u32 j=76800;                                // 240*320
    
    c.w = ILI9325.bcolor.c;						// current background color

    ILI9325_setCursor(0, 0);

    /*
    if (ILI9325.orientation == PORTRAIT)
        ILI9325_setWindow(0, 0, 239, 319);
    else // if (ILI9325.orientation == LANDSCAPE)
        ILI9325_setWindow(0, 0, 319, 239);
    */
    ILI9325_writeRegister(WriteDatatoGRAM);		// write GRAM

     //LowCS;  // Enable LCD
    HighRS;	// Disable Register Selection Signal
    HighRD; // Disable Read Mode
    HighWR; // Disable Write Mode

    while (j--)
    {
        DATA = c.h8;

        LowWR;	// Enable Write
        HighWR;	// Disable Write
        
        DATA = c.l8;

        LowWR;	// Enable Write
        HighWR;	// Disable Write
    }

    //HighCS;                                 	// Disable LCD
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Clear Window with current background color.
    PARAMETERS:
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_clearWindow(u16 x1, u16 y1, u16 x2, u16 y2)
{
    word_t c;
    u32 j=((x2-x1+1)*(y2-y1+1));                // height * width
    
    c.w = ILI9325.bcolor.c;						// current background color

    ILI9325_setCursor(x1, y1);
    ILI9325_writeRegister(WriteDatatoGRAM);		// write GRAM

    //LowCS;									    // Enable LCD
    HighRS;	// Disable Register Selection Signal
    HighRD; // Disable Read Mode
    HighWR; // Disable Write Mode

    while (j--)
    {
        DATA = c.h8;

        LowWR;	// Enable Write
        HighWR;	// Disable Write

        DATA = c.l8;

        LowWR;	// Enable Write
        HighWR;	// Disable Write
    }

    //HighCS;                                 	// Disable LCD
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Set Window's orientation
    PARAMETERS:
    RETURNS:
    REMARKS:
    *   EntryMode[7:0] = ORG - 0 - ID1 - ID0 - AM - 0 - 0 - 0
        ORG = 0, the origin address is not moved
        ORG = 1, the original address “00000h” moves according to the I/D[1:0] setting
        AM = 1, the address is updated in vertical [landscape] writing direction
        AM = 0, the address is updated in horizontal [portrait] writing direction
        ID[1:0] = address counter (AC) to automatically increase or decrease by 1 when update one pixel
            00  H- / V- 
            01  H+ / V- 
            10  H- / V+ : 
            11  H+ / V+ : portrait = left to right 
    *   Driver Output Control[10:8] = SM - 0 - SS
        When SS = 0, the shift direction of outputs is from S1 to S720
        When SS = 1, the shift direction of outputs is from S720 to S1.
    ------------------------------------------------------------------*/


void ILI9325_setPortrait()
{
    ILI9325.orientation = PORTRAIT;
    ILI9325.font.width = ILI9325_getFontWidth();
    ILI9325.font.height= ILI9325_getFontHeight();

    //LowCS;
    // set GRAM write direction to PORTRAIT MODE
    ILI9325_write(EntryMode, 0x1030); // 0x30 = 0b00110000;
    // shift dir. to the right (SM=0, SS=1)
    ILI9325_write(DriverOutputControl, 0x0100);
    //HighCS;
    
    //ILI9325_setCursor(0, 0);
    ILI9325_setWindow(0, 0, 239, 319);
    ILI9325_clearScreen();
}

void ILI9325_setLandscape()
{
    ILI9325.orientation = LANDSCAPE;
    //ILI9325.font.width = ILI9325_getFontHeight();
    //ILI9325.font.height= ILI9325_getFontWidth();

    //LowCS;
    ILI9325_write(EntryMode, 0x1028); // 0x28 = 0b00101000;
    // shift dir. to the right (SM=0, SS=0)
    ILI9325_write(DriverOutputControl, 0x0000);
    //HighCS;
    
    //ILI9325_setCursor(0, 0);
    ILI9325_setWindow(0, 0, 319, 239);
    ILI9325_clearScreen();
}

u8 ILI9325_getOrientation()
{
    return ILI9325.orientation;
}

u16 ILI9325_getScreenWidth()
{
    if (ILI9325.orientation == PORTRAIT)
        return ILI9325.screen.width;
    else // if (ILI9325.orientation == LANDSCAPE)
        return ILI9325.screen.height;
}

u16 ILI9325_getScreenHeight()
{
    if (ILI9325.orientation == PORTRAIT)
        return ILI9325.screen.height;
    else // if (ILI9325.orientation == LANDSCAPE)
        return ILI9325.screen.width;
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Scrolls the contents of the LCD screen vertically the
        specified number of pixels using a HW optimised routine
    PARAMETERS:
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_scroll(u16 pixels)
{
    u16 y = pixels;

    while (y < 0)
        y += 320;

    while (y >= 320)
        y -= 320;    

    //LowCS;									    // Enable LCD
    ILI9325_write(VerticalScrollControl, y);	
    //HighCS;                                 	// Disable LCD
}

///	--------------------------------------------------------------------
/// Print functions
///	--------------------------------------------------------------------

const u8 ILI9325_getFontWidth()
{
    return Display_getFontWidth();
}

const u8 ILI9325_getFontHeight()
{
    return Display_getFontHeight();
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        write a char at (x,y)
    PARAMETERS:
        * c ascii code of the character to print
        * x,y location on screen
    RETURNS:
    REMARKS: update to fit font > 8x8 
    ------------------------------------------------------------------*/

void ILI9325_printChar(u8 c, u16 x, u16 y)
{
    u8  i, j, by;
    u16 offset; 

    //LowCS;									    // Enable LCD
    offset = ((c-32) * ILI9325.font.height);    // 32 first characters are not included in the font
    ILI9325_write(GRAMHorizontalAddressSet, x);	// setCursor
    for (j = 0; j < ILI9325.font.height; j++)
    {
        ILI9325_write(GRAMVerticalAddressSet, y + j);
        by = font[offset++];
        for (i = 0; i < ILI9325.font.height; i++)
        {
            // Test if foreground color is set
            //if BitTest(by, ILI9325.font.height - 1)
            if (by&0x80)
                ILI9325_write(WriteDatatoGRAM, ILI9325.color.c);
            else
                ILI9325_write(WriteDatatoGRAM, ILI9325.bcolor.c);
            by <<= 1;    // Shift out bits right
        }
    }
    //HighCS;	// Disable LCD
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        write a string at curent cursor position
    PARAMETERS:
        s* pointer on a string
    RETURNS:
    REMARKS: Scrolling doesn't work
    ------------------------------------------------------------------*/

void ILI9325_printString(u8 *s)
{
    u8 c;
    
    while (c = *s++)
    {
        switch (c)
        {
            case '\r':
                if (ILI9325.cursor.y > (ILI9325.screen.endy - ILI9325.font.height))
                {
                    //ILI9325.cursor.y = 0;
                    //ILI9325_scroll(ILI9325.screen.endy - ILI9325.font.height);
                    //ILI9325_scroll(ILI9325.font.height);
                    //LowCS;									    // Enable LCD
                    ILI9325_write(VerticalScrollControl, ILI9325.font.height);	
                    //HighCS;                                 	// Disable LCD
                }
                else
                {
                    ILI9325.cursor.y += ILI9325.font.height;
                }
                break;
            case '\n':
                ILI9325.cursor.x = 0;
                break;
            default:
                ILI9325_printChar(c, ILI9325.cursor.x, ILI9325.cursor.y);
                ILI9325.cursor.x += ILI9325.font.width;
                if (ILI9325.cursor.x > ILI9325.screen.endx)
                {
                    ILI9325.cursor.x = 0;
                    ILI9325.cursor.y += ILI9325.font.height;
                }
        }
    }
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        write a formated string at curent cursor position
    PARAMETERS:
        *fmt pointer on a formated string
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_printf(const u8 *fmt, ...)
{
    u8 buffer[80];
    //char *buffer;
    u8 length;
    va_list	args;

    va_start(args, fmt);
    //length = strlen(fmt);
    //buffer = (char *) malloc(1 + length * sizeof(char));	
    length = psprintf2(buffer, fmt, args);
    ILI9325_printString(buffer);
    va_end(args);
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Enter sleep mode sequence.
    PARAMETERS:
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_enterSleep()
{
    ILI9325_write(DisplayControl1,0x0131);//setD1=0,D0=1
    Delayms(10);//Wait for 2 frames or more
    ILI9325_write(DisplayControl1,0x0130);//setD1=0,D0=0
    Delayms(10);//Wait for 2 frames or more
    ILI9325_write(DisplayControl1,0x0000);//displayoff 
    //---------- Power off sequence
    ILI9325_write(PowerControl1,0x0000);//SAP,BT[3:0],AP,DSTB,SLP,STB
    ILI9325_write(PowerControl2,0x0000);//DC1[2:0],DC0[2:0],VC[2:0]
    ILI9325_write(PowerControl3,0x0000);//VREG1OUTvoltage
    ILI9325_write(PowerControl4,0x0000);//VDV[4:0]forVCOMamplitude
    Delayms(200);//dis-chargecapacitorpowervoltage
    ILI9325_write(PowerControl1,0x0002);//SAP,BT[3:0],APE,AP,DSTB,SLP
}
 
/*	--------------------------------------------------------------------
    DESCRIPTION:
        Exit sleep mode sequence.
    PARAMETERS:
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void ILI9325_exitSleep()
{
    //---------- Power On sequence
    ILI9325_write(PowerControl1,0x0000);// SAP, BT[3:0], AP, DSTB, SLP, STB
    ILI9325_write(PowerControl2,0x0007);//DC1[2:0], DC0[2:0], VC[2:0]
    ILI9325_write(PowerControl3,0x0000);//VREG1OUT voltage
    ILI9325_write(PowerControl4,0x0000); //VDV[4:0] for VCOM amplitude
    Delayms(200);
    ILI9325_write(PowerControl1,0x1490);//SAP, BT[3:0], AP, DSTB, SLP, STB
    ILI9325_write(PowerControl2,0x0227);//DC1[2:0], DC0[2:0], VC[2:0]
    Delayms(50);
    ILI9325_write(PowerControl3,0x001C);//External reference voltage=Vci
    Delayms(50);
    ILI9325_write(PowerControl4,0x1800);//VDV[4:0] for VCOM amplitude
    ILI9325_write(PowerControl7,0x001C);//VCM[4:0] for VCOMH
    Delayms(50);
    ILI9325_write(DisplayControl1,0x0133);// 
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Graphic routines.
    PARAMETERS:
    RETURNS:
    REMARKS:
    ------------------------------------------------------------------*/

void drawPixel(u16 x, u16 y)
{
    ILI9325_drawPixel(x, y);
}
    
void ILI9325_drawCircle(u16 x, u16 y, u16 radius)
{
    drawCircle(x, y, radius);
}

void ILI9325_fillCircle(u16 x, u16 y, u16 radius)
{
    fillCircle(x, y, radius);
}

void ILI9325_drawLine(u16 x0, u16 y0, u16 x1, u16 y1)
{
    drawLine(x0, y0, x1, y1);
}

void ILI9325_drawBitmap(u16 x, u16 y, u16 w, u16 h, u16* bitmap)
{
    drawBitmap(x, y, w, h, bitmap);
}

#endif /* __ILI9325_C */
