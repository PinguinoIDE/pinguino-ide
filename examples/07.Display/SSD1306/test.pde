/**
        Author: 	Régis Blanchot (Mar. 2014)
        Tested on:	Pinguino 47J53A & Pinguino 32MX250
        Output:	Oled 0.96" with SSD1306 Controller

        2 size available : SSD1306_128X64 or SSD1306_128X32
        
        Wiring :
        
        if SSD1306_6800
            if SSD1306_PMP
                OLED CS#     connected to GND
                OLED RES#   connected to any GPIO (D3)
                OLED D/C#   connected to Pinguino PMA1 (D4)
                OLED W/R#  connected to Pinguino PMRD/PMWR (D13)
                OLED E/RD# connected to GND
                OLED D[7:0]  connected to Pinguino PMD[7:0] (D[31:24])
            else
                OLED CS#     connected to GND
                OLED RES#   connected to any GPIO (D0)
                OLED D/C#   connected to any GPIO (D1)
                OLED W/R#  connected to any GPIO (D2)
                OLED E/RD# connected to GND
                OLED D[7:0]  connected to Pinguino D[31:24]
        if SSD1306_8080 
            if SSD1306_PMP
                OLED CS#     connected to GND
                OLED RES#   connected to any GPIO (D3)
                OLED D/C#   connected to Pinguino PMA1 (D4)
                OLED W/R#  connected to Pinguino PMWR (D14)
                OLED E/RD# connected to GND
                OLED D[7:0]  connected to Pinguino PMD[7:0]
            else
                OLED CS#     connected to GND
                OLED RES#   connected to any GPIO (D0)
                OLED D/C#   connected to any GPIO (D1)
                OLED W/R#  connected to any GPIO (D2)
                OLED E/RD# connected to GND
                OLED D[7:0]  connected to Pinguino D[31:24]
        if SSD1306_I2C
        if SSD1306_SPI3
        if SSD1306_SPI4
**/

#define DISPLAY (SSD1306_PMP | SSD1306_6800 | SSD1306_128X64)
//#define DISPLAY (SSD1306_8080 | SSD1306_128X64)
//#define DISPLAY (SSD1306_I2C  | SSD1306_128X64)
//#define DISPLAY (SSD1306_SPI3 | SSD1306_128X64)
//#define DISPLAY (SSD1306_SPI4 | SSD1306_128X64)

/**
    Load one or more fonts and active them with SSD1306.setFont()
**/

#include <fonts/font6x8.h>
//#include <fonts/font8x8.h>          // wrong direction
//#include <fonts/font10x14.h>        // ???
//#include <fonts/font12x8.h>         // wrong direction
//#include <fonts/font16x8.h>         // wrong direction
//#include <fonts/font16x16.h>        // ???

u16 alpha=0;                // rotation angle
u16 fps=0, maxfps=0;      // frame per second and stats
u16 x, y, xo, yo;

void setup()
{
    SSD1306.init(3, 4);  	// init. LCD
    SSD1306.clearScreen();
    SSD1306.setFont(font6x8);
    
    xo = SSD1306.screen.width  / 2;
    yo = SSD1306.screen.height / 2;
}

void loop()
{
    u16 frame=0;
    //u32 timeEnd = millis() + 1000; // 1000 ms = 1 sec
    
    //while (millis() < timeEnd)
    //{
        x = xo + 32.0f * cosr(alpha);
        y = yo + 32.0f * sinr(alpha);
        
        SSD1306.clearScreen();
        
        //SSD1306.printf("%u fps (max. %u)", fps, maxfps);
        SSD1306.printNumber(x, DEC);
        SSD1306.print("/");
        SSD1306.printNumber(y, DEC);
        //SSD1306.print("\r\n");
        /* or
        SSD1306.printNumber(fps, DEC);
        SSD1306.print(" fps (max. ");
        SSD1306.printNumber(maxfps, DEC);
        SSD1306.print(")"); */

        //SSD1306.drawLine(xo, yo, x, y);
        
        SSD1306.refresh();
        
        // increments angle
        alpha = (alpha + 1) % 360;

        // one frame done !
        frame++;
    //}
    
    fps = frame;
    if (fps > maxfps) maxfps = fps;
}
