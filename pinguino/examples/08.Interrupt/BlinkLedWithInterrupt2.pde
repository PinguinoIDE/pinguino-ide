/*	----------------------------------------------------------------------------
	blink built-in led with help from interrupt library
	----------------------------------------------------------------------------
	author:			    Régis Blanchot
	first release:  19/12/2010
	last update:	  25/11/2013
	pinguino ide:   > x.4 rev 950
 	boards:         8-bit ONLY
	--------------------------------------------------------------------------*/

u16 volatile flag = 0;
u16 volatile cycles;

void setup()
{
    pinMode(USERLED, OUTPUT);
    
    // Set Timer0 overflow to 1ms
    // 1 ms = 1 / 1 000 sec
    // 1 cycle = Tcy = 1 / Fcy = 4 / Fosc (Fcy = Fosc / 4)
    // nb of cycles in 1 ms = ( 1 / 1000 ) / Tcy
    //                      = Fcy / 1000
    //                      = System.getPeripheralFrequency() / 1000    
    cycles = 0xFFFF - ( System.getPeripheralFrequency() / 1000 );
    TMR0H = highByte(cycles);
    TMR0L =  lowByte(cycles);
    T0CON = T0_ON | T0_16BIT | T0_SOURCE_INT | T0_PS_OFF;

    Int.clearFlag(INT_TMR0);
    Int.enable(INT_TMR0);
    Int.start();
}

void loop()
{
    // Whatever you want here
}

void interrupt()
{
    if (Int.isFlagSet(INT_TMR0) == 1)
    {
        flag++;
        TMR0H = highByte(cycles);
        TMR0L =  lowByte(cycles);
        Int.clearFlag(INT_TMR0);
        if (flag >= 1000) // 1000 ms
        {
            flag = 0;
            toggle(USERLED);
        }        
    }
}
