/* we always wait a bit between updates of the display */
#define DELAYTIME 250

void setup()
{
    /*
    Now we need a LedControl to work with.
    ***** These pin numbers will probably not work with your hardware *****
    pin 0 is connected to the DataIn 
    pin 1 is connected to the CLK 
    pin 2 is connected to LOAD 
    1 : We have only a single MAX72XX.
    */
    LedControl.init(0,1,2,1);

    /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
    */
    LedControl.shutdown(0,false);
    /* Set the brightness to a medium values */
    LedControl.setIntensity(0,8);
    /* and clear the display */
    LedControl.clearDisplay(0);
}


/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void writePinguinoOn7Segment()
{
    LedControl.setChar(0,0,'p',false);
    delay(DELAYTIME);
    LedControl.setChar(0,0,'i',false);
    delay(DELAYTIME);
    LedControl.setChar(0,0,'n',false);
    delay(DELAYTIME);
    LedControl.setChar(0,0,'g',false);
    delay(DELAYTIME);
    LedControl.setChar(0,0,'u',false);
    delay(DELAYTIME);
    LedControl.setChar(0,0,'i',false);
    delay(DELAYTIME);
    LedControl.setChar(0,0,'n',false);
    delay(DELAYTIME);
    LedControl.setChar(0,0,'o',false);
    delay(DELAYTIME);
    LedControl.clearDisplay(0);
    delay(DELAYTIME);
} 

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void scrollDigits()
{
    int i;
    for(i=0;i<13;i++)
    {
        LedControl.setDigit(0,3,i,false);
        LedControl.setDigit(0,2,i+1,false);
        LedControl.setDigit(0,1,i+2,false);
        LedControl.setDigit(0,0,i+3,false);
        delay(DELAYTIME);
    }
    LedControl.clearDisplay(0);
    delay(DELAYTIME);
}

void loop()
{ 
    writePinguinoOn7Segment();
    scrollDigits();
}
