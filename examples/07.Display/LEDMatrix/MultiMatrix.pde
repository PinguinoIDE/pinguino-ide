#define MATRIX 3 // number of matrix

void setup()
{
    u8 m;
    
    delay(1000);
    /*
    We need a LedControl to work with.
    pin 0 is connected to the DataIn  
    pin 1 is connected to the CLK 
    pin 2 is connected to LOAD 
    */
    LedControl.init(0, 1, 2, MATRIX);

    for (m=0; m<MATRIX; m++)
    {
        /*
        The MAX72XX is in power-saving mode on startup,
        we have to do a wakeup call
        */
        LedControl.shutdown(m, false);
        /* Set the brightness to a medium value (0~15 possible values) */
        LedControl.setIntensity(m, 8);
        /* and clear the display */
        LedControl.clearDisplay(m);
    }
}

void loop()
{ 
    /* scroll the string 1 pixel to the left */
    LedControl.scroll("   3x Led Matrix Scroll Demo - www.pinguino.cc   ");
}
