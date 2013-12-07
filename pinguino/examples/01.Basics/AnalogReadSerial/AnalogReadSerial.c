/*
  AnalogReadSerial
 Reads an analog input on pin 0, prints the result to the serial monitor 
 
 This example code is in the public domain.
 */

void setup() {
  serial1init(9600);
}

void loop() {
  int sensorValue = analogRead(A0);
  serial1println(sensorValue, DEC);
}

