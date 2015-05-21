/*--------------------------------------------------------------------------
	blink built-in led with help from interrupt library
----------------------------------------------------------------------------
	author:         Régis Blanchot
	first release:  19/12/2010
	last update:    15/01/2015
	pinguino ide:   > 9.5
 	boards:         ALL
--------------------------------------------------------------------------*/

u32 i;

void myBlink()
{
    toggle(USERLED);
}

void setup()
{
    Serial.begin(9600);
    pinMode(USERLED, OUTPUT);

    /* 8-bit Pinguino Only */
    //OnTimer0(myBlink, INT_MILLISEC, 500);  // Use Timer0 to toggle the USERLED every 500 ms

    /* All Pinguino */
    //OnTimer1(myBlink, INT_MILLISEC, 500);    // Use Timer1 to toggle the USERLED every 500 ms
    OnTimer2(myBlink, INT_MILLISEC, 500); // Use Timer2 to toggle the USERLED every 500 ms
    //OnTimer3(myBlink, INT_MILLISEC, 500);         // Use Timer3 to toggle the USERLED every 500 ms
    
    /* 32-bit Pinguino, x6j50 and xxj53 only */
    //OnTimer4(myBlink, INT_MILLISEC, 500);  // Use Timer4 to toggle the USERLED every 500 ms
    
    /* 32-bit Pinguino and xxj53 only */
    //OnTimer5(myBlink, INT_MILLISEC, 500);  // Use Timer5 to toggle the USERLED every 500 ms
    
    /* Pinguino xxj53 only */
    //OnTimer6(myBlink, INT_MILLISEC, 500);  // Use Timer6 to toggle the USERLED every 500 ms
    //OnTimer8(myBlink, INT_MILLISEC, 500);  // Use Timer8 to toggle the USERLED every 500 ms
}

void loop()
{
    // Whatever you want here
    Serial.printf("i=%d\r\n", i++);
    delay(1000);
}
