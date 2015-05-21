/*	----------------------------------------------------------------------------
	interrupt library demo
	----------------------------------------------------------------------------
	author:		Régis Blanchot
	first release:	19/12/2010
	last update:	06/02/2015
	pinguino ide:	11
--------------------------------------------------------------------------*/

void blink() { toggle(USERLED); }

void setup()
{
    pinMode(USERLED,OUTPUT);

    // Use Timer1 to toggle USERLED every 1000 ms
    OnTimer1(blink, INT_MILLISEC, 1000);
}

void loop()
{
    // your code here
}
