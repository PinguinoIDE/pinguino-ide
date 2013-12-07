/*	--------------------------------------------------------------------
    Clock switching    
    --------------------------------------------------------------------
    author:			    Régis Blanchot
    first release:	25/01/2013
    last update:	  25/01/2013
    pinguino ide:   x.4
    boards:			    8-bit ONLY
    ------------------------------------------------------------------*/

const u8 osc[]={_48MHZ_, _8MHZ_, _4MHZ_, _2MHZ_, _1MHZ_, _500KHZ_, _250KHZ_, _31KHZ_};

void myDelay()
{
    u16 i;
    while (i++);
}

void setup()
{
    pinMode(USERLED, OUTPUT);
}

void loop()
{
    u8 i, j;
    
    for (j=0; j<sizeof(osc); j++)
    {
        //CDC.printf("Changing Clock Speed ... \r\n");
        // switch clock to internal oscillator
        System.setIntOsc(osc[j]);
        
        // toggle build-in led 20 times (10 times On, 10 times Off)
        for (i=0; i<20; i++)
        {
            toggle(USERLED);
            myDelay();              // delay (depends on CPU clock)
        }
    }
}
