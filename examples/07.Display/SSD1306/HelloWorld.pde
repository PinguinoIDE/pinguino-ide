/**
        Author: 	Régis Blanchot (Mar. 2014)
        Tested on:	Pinguino 47J53A & Pinguino 32MX250
        Output:	Oled 0.96" with SSD1306 Controller

        2 size available : SSD1306_128X64 or SSD1306_128X32
        
        Wiring :
        
        if SSD1306_6800
            if SSD1306_PMP
                OLED CS#     connected to GND
                OLED RES#   connected to any GPIO
                OLED D/C#   connected to Pinguino PMA[0:15]
                OLED W/R#  connected to Pinguino PMRD/PMWR
                OLED E/RD# connected to GND
                OLED D[7:0]  connected to Pinguino PMD[7:0]
            else
                OLED CS#     connected to GND
                OLED RES#   connected to any GPIO
                OLED D/C#   connected to any GPIO
                OLED W/R#  connected to any GPIO
                OLED E/RD# connected to GND
                OLED D[7:0]  connected to any GPIO
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

//#define DISPLAY (SSD1306_PMP | SSD1306_6800 | SSD1306_128X64)
#define DISPLAY (SSD1306_6800 | SSD1306_128X64)
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

void setup()
{
    //NB : On Pingino 32MX250 USERLED is on pin 13 which is also used by the PMP Data bus
    // so comment the next line if you use 32MX250 and PMP mode
    pinMode(USERLED, OUTPUT);
    
    // if 6800- or 8080-interface and PMP is used
    //SSD1306.init(1, PMA3); // RST on D1, DC on PMA3 (D2 on a 47J53A)
    
    // if i2c interface is used
    //SSD1306.init(0x3C); // i2c address of the display
    
    // if 6800- or 8080-interface (but not PMP) is used
    //void SSD1306.init(u8 rst, u16 dc, u8 d0, u8 d1, u8 d2, u8 d3, u8 d4, u8 d5, u8 d6, u8 d7)
    SSD1306.init(8, 9, 0, 1, 2, 3, 4, 5, 6, 7);

    SSD1306.clearScreen();
    SSD1306.setFont(font6x8);
}   

void loop()
{
    SSD1306.println("Hello World!");
    SSD1306.refresh();
    toggle(USERLED);
    delay(1000);
}