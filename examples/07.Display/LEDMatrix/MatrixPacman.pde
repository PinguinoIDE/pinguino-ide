const u8 pacman[2][8] = {
{
    0b00111100,
    0b01111110,
    0b11111000,
    0b11100000,
    0b11100000,
    0b11111000,
    0b01111110,
    0b00111100,
},
{
    0b00111100,
    0b01111110,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100,
}
};

void displayPacman(u8 num)
{
    u8 i;
    
    for (i=0; i<8;i++)
        LedControl.setColumn(0, i, pacman[num][i]);
    delay(200);
    for (i=0; i<8;i++)
        LedControl.setColumn(0, i, 0);
}

void setup()
{
    /*
    We need a LedControl to work with.
    pin 0 is connected to the DataIn 
    pin 1 is connected to the CLK 
    pin 2 is connected to LOAD 
    1 : We have only a single MAX72XX (adress = 0).
    */
    LedControl.init(0, 1, 2, 1);
    /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
    */
    LedControl.shutdown(0, false);
    /* Set the brightness to a medium value */
    LedControl.setIntensity(0, 8);
    /* and clear the display */
    LedControl.clearDisplay(0);
}

void loop()
{ 
    displayPacman(0);
    displayPacman(1);
}
