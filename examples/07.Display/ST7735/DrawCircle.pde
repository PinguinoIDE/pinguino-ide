/**
        Author: 	Régis Blanchot (Mar. 2014)
        Tested on:	Pinguino 47J53 & Pinguino 32MX250
        Output:	262K-color graphic TFT-LCD with ST7735 controller

        2 modes available :
        - Hardware SPI
            . default mode
            . SPI operations are handled by the CPU
            . pins have to be the CPU SPI pins
            . PINGUINO 32 have up to 4 SPI module (SPI1 to SPI4)
            . PINGUINO 8  have only one SPI module (SPI1)
        - Software SPI
            . SPISW
            . SPI operations are handled by the SPI library
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

#define SPIMODULE SPI2

void setup()
{
    // init. the random number generator    
    int seed;
    seed = millis();
    randomSeed(seed);

    ST7735.init(SPIMODULE, 6, 5, 0, 0); // CS and DC
    ST7735.setBackgroundColor(SPIMODULE, ST7735_BLACK);
    ST7735.clearScreen(SPIMODULE);
}   

void loop()
{
    u16 x = random(0, 159);     // coordinate x E [0,159]
    u16 y = random(0, 127);     // coordinate y E [0, 127]
    u16 r = random(0, 63);      // radius r E [0,63]
    //u16 c  = random(0, 0xFFFF); // color c E [0, 65535]
    u16 c = millis() % 0xFFFF;
    u8 f   = random(0, 100);    // form c E [0, 1]
    
    // display
    ST7735.setColor(SPIMODULE, c);
    if (f>49)
        ST7735.fillCircle(SPIMODULE, x, y, r);
    else
        ST7735.drawCircle(SPIMODULE, x, y, r);
    
    // delay
    delay(150);
}
