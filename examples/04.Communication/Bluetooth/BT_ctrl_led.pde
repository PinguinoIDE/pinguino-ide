/*
    JY-MCU              Pinguino
    - RX                TX (RC6)
    - TX                RX (RC7)
    - VCC 3.6-6V        5V
    - GND               GND
*/

char val; // variable to receive data from the serial port

void setup()
{
  pinMode(USERLED, OUTPUT);  // pin where on-board LED is connected as OUTPUT
  Serial.begin(9600);        // start serial communication at 9600bps
}

void loop()
{
  if ( Serial.available() )       // if data is available to read
    val = Serial.read();         // read it and store it in 'val'

  if ( val == 'h' )               // if 'h' was received
    digitalWrite(USERLED, HIGH);  // turn ON the LED

  if ( val == 'l' )               // if 'l' was received
    digitalWrite(USERLED, LOW);   // turn OFF the led

  delay(100);                    // wait 100ms for next reading
}

