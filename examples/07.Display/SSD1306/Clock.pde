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

s16 a=-90;
u8 x, y;
u8 xo=128/2;
u8 yo=64/2;
u8 old=0;

u8 Day[7][5]		= {"Sat","Sun","Mon","Tue","Wed","Thu","Fri"};
u8 Month[13][5]	= {"  ","Jan","Feb","Mar","Apr","Mei","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

void setup()
{
    u32 Tm  = 0x00090000;   // 09hr, 00 min, 00 sec
    u32 Dt  = 0x14031406;   // Friday (day 6 of the week), 14 March 2014
    u16 drift = 200;        // add 200 pulse every minute to adjust time

    //On Pingino 32MX250 USERLED is on pin 13
    //which is also used by the PMP Data bus 
    //pinMode(USERLED, OUTPUT);
    SSD1306.init(3, 4);
    SSD1306.clearScreen();
    SSD1306.setFont(font6x8);
    // Init real time calendar
    RTCC.init(Tm, Dt, drift);
}

void loop()
{
    rtccTime cT;
    rtccDate cD;

    RTCC.getTime(&cT);
    RTCC.getDate(&cD);

    if (cT.seconds != old)
    {
        // actual spin position
        x = xo + 64 * cosr(a - 90);
        y = yo + 32 * sinr(a - 90);
        SSD1306.clearScreen();
        // draw spin
        SSD1306.drawLine(xo, yo, x, y);
        // display date
        SSD1306.setCursor(3, 3);
        SSD1306.printf("%3s %02d %3s. %04d\r\n", Day[cD.dayofweek], cD.dayofmonth, Month[cD.month], cD.year+2000);
        // display time
        SSD1306.setCursor(6, 4);
        SSD1306.printf("%02d:%02d:%02d\r\n", cT.hours, cT.minutes, cT.seconds);
        // update screen
        SSD1306.refresh();
        //toggle(USERLED);
        // update next spin position
        a = (a + 6) % 360;
        // store last second value
        old = cT.seconds;
    }
    
}
