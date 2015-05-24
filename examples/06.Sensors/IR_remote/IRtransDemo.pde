/*
 * IRtransDemo - demonstrates the IR remote library
 * An IR led must be connected to a valid PWM pin through a 100 Ohm resistor.
 * Send Power Off command when push button is pressed
 * Target : Philips TV
 * Protocol : RC5
 */

#define SEND_PIN 11    // must be a valid PWM pin

#define RC6_POWER_OFF 12

void setup()
{
    pinMode(USERLED, OUTPUT);
    digitalWrite(USERLED, ON);    
    IRremote.enabeIROut(SEND_PIN);     // Defined the Output pin
}

void loop()
{
    digitalWrite(USERLED, ON);    
    delay(100);
    IRremote.sendRC6(RC6_POWER_OFF, 14);
    digitalWrite(USERLED, OFF);    
    delay(900);
}
