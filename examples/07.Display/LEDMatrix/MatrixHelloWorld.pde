void setup()
{
    delay(1000);
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
    /* Set the brightness to a medium value (0~15 possible values) */
    LedControl.setIntensity(0, 8);
    /* and clear the display */
    LedControl.clearDisplay(0);
}

void loop()
{ 
    /* display the string char by char */
    //LedControl.writeString(" Hello World! ");
    //delay(1000);
    /* scroll the string 1 pixel to the left */
    LedControl.scroll(" www.pinguino.cc ");
}
