/**
        Author: 	Régis Blanchot (Mar. 2014)
        Tested on:	Pinguino 47J53 & Pinguino 32MX250
        Output:	262K-color graphic TFT-LCD with ST7735 controller

        2 modes available :
        - Hardware SPI
            . default mode
            . SPI operations are handled by the CPU
            . pins have to be the CPU SPI pins
        - Software SPI
            . activated with #define SPISW
            . SPI operations are handled by the ST7735 library
            . pins can be any digital pin
        
        Wiring :
        
        ST7735    PINGUINO
        ---------------------------------------
        LED       VSS (backlight on)
        SCK       SCK
        SDA       SDO
        A0 (DC)   can be connected to any digital pin
        RESET     VSS
        CS        can be connected to any digital pin
        GND       GND
        VSS       VSS (+5V or +3.3V)
**/

//#define SPISW

/**
    Load one or more fonts and active them with ST7735.setFont()
**/

#include <fonts/font6x8.h>
//#include <fonts/font8x8.h>          // wrong direction
//#include <fonts/font10x14.h>        // ???
//#include <fonts/font12x8.h>         // wrong direction
//#include <fonts/font16x8.h>         // wrong direction
//#include <fonts/font16x16.h>        // ???

s16 a=-90;
u8 x, y;
u8 xo, yo;
u8 old=0;

u8 Day[7][5]    = {"Sat","Sun","Mon","Tue","Wed","Thu","Fri"};
u8 Month[13][5] = {"  ","Jan","Feb","Mar","Apr","Mei","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

void setup()
{
    u32 Tm  = 0x00090000;   // 09hr, 00 min, 00 sec
    u32 Dt  = 0x14031406;   // Friday (day 6 of the week), 14 March 2014
    u16 drift = 200;        // add 200 pulse every minute to adjust time

    pinMode(USERLED, OUTPUT);
    
    // if SPISW is defined
    // ST7735_init(cs, dc, sda, sck);

    ST7735.init(0, 2); // CS and DC
    ST7735.setFont(font6x8);
    ST7735.setBackgroundColor(ST7735_BLACK);
    ST7735.setColor(ST7735_WHITE);
    ST7735.clearScreen();

    // Init real time calendar
    RTCC.init(Tm, Dt, drift);

    xo = ST7735.screen.width  / 2;
    yo = ST7735.screen.height / 2;
}

void loop()
{
    rtccTime cT;
    rtccDate cD;

    // 8-bit syntax
    //RTCC.getTime(&cT);
    //RTCC.getDate(&cD);
    
    // 32-bit syntax
    cT = RTCC.getTime();
    cD = RTCC.getDate();

    if (cT.seconds != old)
    {
        // actual spin position
        x = xo + 64.0f * cosr(a - 90);
        y = yo + 32.0f * sinr(a - 90);
        ST7735.clearScreen();
        // draw spin
        ST7735.drawLine(xo, yo, x, y);
        // display date
        ST7735.setCursor(3, 3);
        ST7735.printf("%3s %02d %3s. %04d\r\n", Day[cD.dayofweek], cD.dayofmonth, Month[cD.month], cD.year+2000);
        // display time
        ST7735.setCursor(6, 4);
        ST7735.printf("%02d:%02d:%02d\r\n", cT.hours, cT.minutes, cT.seconds);

        toggle(USERLED);
        
        // update next spin position
        a = (a + 6) % 360;
        // store last second value
        old = cT.seconds;
    }
    
}
