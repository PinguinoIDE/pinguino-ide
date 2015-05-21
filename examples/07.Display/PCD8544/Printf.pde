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

u8 i=0;

void setup()
{
    pinMode(USERLED,     OUTPUT);

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
 
    // Change the contrast around to adapt the display for the best viewing!
    PCD8544.setContrast(40); // 0 to 127
    //PCD8544.setTextColor(BLACK);
    PCD8544.setFont(font6x8);

    // Clear screen buffer
    PCD8544.clearScreen();
}

void loop()
{
    //PCD8544.clearScreen();
    PCD8544.printf("i=%03d\r\n", i++);
    PCD8544.refresh();
    toggle(USERLED);
    delay(500);
}
