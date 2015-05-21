/**
        Author: 	    Régis Blanchot (Apr. 2014)
        Library :     Thomas Missonier (sourcezax@users.sourceforge.net).
        Tested on:	Pinguino 32MX250 and Pinguino 47J53A
        Output:	    Nokia 5110 84x48 LCD Display  with PCD8544 Controller
**/

/// Load one or more fonts and active them with PCD8544.setFont()

#include <fonts/font6x8.h>
//#include <fonts/font8x8.h>          // wrong direction
//#include <fonts/font10x14.h>        // ???
//#include <fonts/font12x8.h>         // wrong direction
//#include <fonts/font16x8.h>         // wrong direction
//#include <fonts/font16x16.h>        // ???

/* Pin configuration */
#define PCD8544_INTERFACE     (PCD8544_SPIHW)
//define PCD8544_INTERFACE       (PCD8544_SPISW | PCD8544_PORTB)

//define PCD8544_INTERFACE       (PCD8544_SPISW)
#define NOKIA_RST               4  // LCD RST 
//define NOKIA_SCE               6  // Pinguino SS
#define NOKIA_DC                6  // LCD Dat/Com
//define NOKIA_SDIN              4  // Pinguino SDO
//define NOKIA_SCLK              3  // Pinguino SCLK 
//define NOKIA_VCC               2  // 3.3V 
//define NOKIA_LIGHT             1  // GND or 3.3V depends on models                                      
//define NOKIA_GND               0  // GND 

u16 alpha=0;                // rotation angle
u8 x, y;
u8 xo, yo;

void setup()
{
    /// Screen init

    // if PCD8544_INTERFACE == PCD8544_SPISW or PCD8544_INTERFACE == PCD8544_SPIHW
    /*
    digitalWrite(NOKIA_VCC,   HIGH); // LCD NOKIA_VCC to 3.3V
    digitalWrite(NOKIA_LIGHT, LOW);  // LCD BackNOKIA_LIGHT On
    digitalWrite(NOKIA_GND,   LOW);  // LCD NOKIA_GND to NOKIA_GND

    pinMode(NOKIA_VCC,   OUTPUT);
    pinMode(NOKIA_LIGHT, OUTPUT);
    pinMode(NOKIA_GND,   OUTPUT);
    */
    
    // if PCD8544_INTERFACE == PCD8544_SPISW and PCD8544_PORTB
    /*
    PCD8544.init();
    */
    
    // if PCD8544_INTERFACE == PCD8544_SPISW
    /*
    PCD8544.init(NOKIA_SCLK, NOKIA_SDIN, NOKIA_DC, NOKIA_SCE, NOKIA_RST);
    */
    
    // if PCD8544_INTERFACE == PCD8544_SPIHW
    PCD8544.init(NOKIA_DC, NOKIA_RST);
 
    PCD8544.setContrast(40); // 0 to 127
    //PCD8544.refresh(); // show Pinguino splashscreen
    //delay(1000);
    
    xo = PCD8544.screen.width  / 2;
    yo = PCD8544.screen.height / 2;
}

void loop()
{
    x = xo + 24.0f * cosr(alpha);
    y = yo + 24.0f * sinr(alpha);
    
    // display
    PCD8544.clearScreen();
    PCD8544.drawLine(xo, yo, x, y);
    PCD8544.refresh();
    
    // increments angle
    alpha = (alpha + 1) % 360;
}
