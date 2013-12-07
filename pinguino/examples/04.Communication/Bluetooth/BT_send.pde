/*
    JY-MCU              Pinguino
    - RX                TX (RC6)
    - TX                RX (RC7)
    - VCC 3.6-6V        5V
    - GND               GND
*/

u16 i=0;

void setup()
{
  Serial.begin(9600);        // start serial communication at 9600bps
}

void loop()
{
  Serial.printf("Pinguino26j50 - i=%d\r\n", i);
  delay(1000);                // wait 1s for next sending
  i = i + 1;
}

