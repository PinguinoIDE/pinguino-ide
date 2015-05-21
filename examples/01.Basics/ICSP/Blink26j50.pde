/*	----------------------------------------------------------------------------
    Blink a LED connected to RC2 on a Pinguino 26j50
    ----------------------------------------------------------------------------
    author:	Régis Blanchot
    ----------------------------------------------------------------------------
    READ THIS CAREFULLY :
    This is intended only for people who don't want to use USB bootloader
    but an ICSP programmer instead.
    --------------------------------------------------------------------------*/

void setup()
{
    TRISCbits.TRISC2 = OUTPUT;
}

void loop()
{
    LATCbits.LATC2 ^= 1;
    delay(50);
}
