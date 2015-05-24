/*

  Smoothing

  Reads repeatedly from an analog input, calculating a running average
  and printing it to the computer.  Keeps ten readings in an array and 
  continually averages them.
  
  The circuit:
    * Analog sensor (potentiometer will do) attached to analog input 0

  Created 22 April 2007
  By David A. Mellis  <dam@mellis.org>

  http://www.arduino.cc/en/Tutorial/Smoothing
  
  This example code is in the public domain.


*/


// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
#define numReadings 10

int readings[numReadings];      // the readings from the analog input
int i = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int inputPin = 0;

void setup()
{
  int thisReading;
  // initialize serial communication with computer:
  Serial.begin(9600);                   
  // initialize all the readings to 0: 
  for (thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;          
}

void loop() {
  // subtract the last reading:
  total= total - readings[i];         
  // read from the sensor:  
  readings[i] = analogRead(inputPin); 
  // add the reading to the total:
  total= total + readings[i];       
  // advance to the next position in the array:  
  i = i + 1;                    

  // if we're at the end of the array...
  if (i >= numReadings)              
    // ...wrap around to the beginning: 
    i = 0;                           

  // calculate the average:
  average = total / numReadings;         
  // send it to the computer (as ASCII digits) 
  Serial.printf("%d\r\n", average);               
}


