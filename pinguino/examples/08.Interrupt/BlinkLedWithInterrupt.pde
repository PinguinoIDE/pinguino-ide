/*	----------------------------------------------------------------------------
	blink built-in led with help from interrupt library
	----------------------------------------------------------------------------
	author:			Régis Blanchot
	first release:  19/12/2010
	last update:	15/01/2013
	pinguino ide:   > 9.5
 	boards:         8-bit ONLY
	--------------------------------------------------------------------------*/

void myBlink0() { toggle(USERLED); }
//void myBlink1() { toggle(1); }
//void myBlink2() { toggle(2); }
//void myBlink3() { toggle(3); }
//void myBlink4() { toggle(4); }
//void myBlink5() { toggle(5); }
//void myBlink6() { toggle(6); }
//void myBlink8() { toggle(7); }

void setup()
{
    u8 i;
    
    for (i=0; i<8; i++)
        pinMode(i, OUTPUT);

    OnTimer0(myBlink0, INT_MILLISEC, 1000);  // Use Timer0 to toggle the USERLED every 1000 ms
    //OnTimer1(myBlink1, INT_MILLISEC, 200);  // Use Timer1 to toggle pin 1 every 200 ms
    //OnTimer2(myBlink2, INT_MILLISEC, 300);  // Use Timer2 to toggle pin 2 every 300 ms
    //OnTimer3(myBlink3, INT_MILLISEC, 400);  // Use Timer3 to toggle pin 3 every 400 ms
    // x6j50 and xxj53 only
    //OnTimer4(myBlink4, INT_MILLISEC, 500);  // Use Timer4 to toggle pin 4 every 500 ms
    // xxj53 only
    //OnTimer5(myBlink5, INT_MILLISEC, 600);  // Use Timer5 to toggle pin 5 every 600 ms
    //OnTimer6(myBlink6, INT_MILLISEC, 700);  // Use Timer6 to toggle pin 6 every 700 ms
    //OnTimer8(myBlink8, INT_MILLISEC, 800);  // Use Timer8 to toggle pin 7 every 800 ms
}

void loop()
{
    // Whatever you want here
}
