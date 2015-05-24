/* 
	Stepper Motor Control - speed control

	This program drives a unipolar or bipolar stepper motor. 
	The motor is attached to digital pins 0 - 3 of the Arduino.
	A potentiometer is connected to analog input 0.

	The motor will rotate in a clockwise direction. The higher the potentiometer value,
	the faster the motor speed. Because setSpeed() sets the delay between steps, 
	you may notice the motor is less responsive to changes in the sensor value at
	low speeds.

	Created 30 Nov. 2009
	Modified 28 Oct 2010
	by Tom Igoe
	Adapted 18 May 2011
	by Régis Blanchot
*/

const int stepsPerRevolution = 48;	// change this to fit the number of steps per revolution for your motor
int stepCount = 0;						// number of steps the motor has taken

void setup()
{
	// initialize the serial port:
	Serial.begin(9600);
	// initialize the stepper library on pins 8 through 11:
	Stepper.init(stepsPerRevolution, 0,1,2,3);            
}

void loop()
{
	// read the sensor value:
	int sensorReading = analogRead(A0);
	// map it to a range from 0 to 100:
	int motorSpeed = map(sensorReading, 0, 1023, 0, 100);
	// set the motor speed:
	if (motorSpeed > 0)
	{
		Stepper.setSpeed(motorSpeed);
		// step 1/100 of a revolution:
		Stepper.step(stepsPerRevolution/100);
	} 
}
