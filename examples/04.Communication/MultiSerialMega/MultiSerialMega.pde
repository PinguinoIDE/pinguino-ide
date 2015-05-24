/*
  Mega multple serial test
 
 Receives from the main serial port, sends to the others. 
 Receives from serial port 1, sends to the main serial (Serial 0).
 
 This example works only on 32-bit Pinguino
 
 The circuit: 
 * Any serial device attached to Serial port 1
 * Serial monitor open on Serial port 0:
 
 created 30 Dec. 2008
 by Tom Igoe
 
 This example code is in the public domain.
 
 */


void setup() {
  // initialize both serial ports:
  Serial1.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  // read from port 2, send to port 1:
  if (Serial2.available()) {
    int inByte = Serial2.read();
    Serial1.print(inByte, BYTE); 
  }
}
