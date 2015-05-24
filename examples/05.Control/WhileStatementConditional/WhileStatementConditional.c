/*
  Conditionals - while statement
 
 This example demonstrates the use of  while() statements.
 
 While the pushbutton is pressed, the sketch runs the calibration routine.
 The  sensor readings during the while loop define the minimum and maximum 
 of expected values from the photo resistor.
 
 This is a variation on the calibrate example.
 
 The circuit:
 * photo resistor connected from +5V to analog in pin 0
 * 10K resistor connected from ground to analog in pin 0
 * LED connected from digital pin 9 to ground through 220 ohm resistor
 * pushbutton attached from pin 2 to +5V
 * 10K resistor attached from pin 2 to ground
 
 created 17 Jan 2009
 modified 4 Sep 2010
 by Tom Igoe
 
 This example code is in the public domain.

 http://arduino.cc/en/Tutorial/WhileLoop
 
 */


//
const int sensorPin = A2;       // pin that the sensor is attached to
const int ledPin = 9;           // pin that the LED is attached to
const int indicatorLedPin = 13; // pin that the built-in LED is attached to
const int buttonPin = 2;        // pin that the button is attached to


//
int sensorMin = 1023;  // minimum sensor value
int sensorMax = 0;     // maximum sensor value
int sensorValue = 0;         // the sensor value

void calibrate() {
//
  digitalWrite(indicatorLedPin, HIGH);
//
  sensorValue = analogRead(sensorPin);

//
  if (sensorValue > sensorMax) {
    sensorMax = sensorValue;
  }

//
  if (sensorValue < sensorMin) {
    sensorMin = sensorValue;
  }
}

void setup() {
//
  pinMode(indicatorLedPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
//
  while (digitalRead(buttonPin) == HIGH) {
    calibrate(); 
  }
//
  digitalWrite(indicatorLedPin, LOW);  

//
  sensorValue = analogRead(sensorPin);

//
  sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);

//
  sensorValue = constrain(sensorValue, 0, 255);

//
  analogWrite(ledPin, sensorValue);
}

