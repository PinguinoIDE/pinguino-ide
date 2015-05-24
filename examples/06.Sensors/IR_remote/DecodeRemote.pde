/*
 * DecodeRemote: demonstrates decoding IR codes from a remote control
 * An IR detector/demodulator must be connected to the input IRSENSOR.
 * Version 0.1 March, 2014
 * Author : Regis Blanchot
 */

#define IRSENSOR 0 // pin 0, can be any digital pin

char brand[][10] = {"UNKNOWN", "NEC", "SONY", "RC5", "RC6", "DISH", "SHARP", "PANASONIC", "JVC", "SANYO", "MITSUBISHI"};

decode_results results; // Results returned from the decoder

void setup()
{
    IRremote.enableIRIn(IRSENSOR);  // Start the receiver
    IRremote.blink(true);           // Built-in led will blink at reception
}

void loop()
{
    if (IRremote.decode(&results)==DECODED)
    {
        if (results.decode_type>10)
            results.decode_type = 0;
        
        CDC.print("Decoded ");
        CDC.printNumber(results.value, DEC);
        CDC.print(" (type ");
        CDC.print(brand[results.decode_type]);
        CDC.print(")\r\n");

        /// or
        
        //CDC.printf("Decoded %ld (type %s)\r\n", results.value, brand[results.decode_type]);

        IRremote.resume(); // Ready to receive the next value
    }
}
