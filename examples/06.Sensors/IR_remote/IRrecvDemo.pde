/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

int RECV_PIN = 0;   // can be any digital pin

decode_results results;

void setup()
{
    lcd.pins(8, 9, 4, 5, 6, 7, 0, 0, 0, 0);
    lcd.begin(20, 4);
    IRremote.enableIRIn(RECV_PIN); // Start the receiver
    IRremote.blink(true);
}

void loop()
{
    if (IRremote.decode(&results))
    {
        lcd.printNumber(results.value, HEX);
        IRremote.resume(); // Receive the next value
    }
}
