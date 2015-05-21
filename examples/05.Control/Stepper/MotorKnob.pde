/*
	MotorKnob

	A stepper motor follows the turns of a potentiometer
	(or other sensor) on analog input 0.

	http://www.arduino.cc/en/Reference/Stepper
	This example code is in the public domain.

	Adapted 18 May 2011
	by Régis Blanchot
*/

// change this to the number of steps on your motor
#define STEPS 48

// the previous reading from the analog input
int previous = 0;

void setup()
{
	// create an instance of the stepper class, specifying
	// the number of steps of the motor and the pins it's
	// attached to
	Stepper.init(STEPS, 0, 1, 2, 3);
	// set the speed of the motor to 30 RPMs
	Stepper.setSpeed(30);
}

void loop()
{
	// get the sensor value
	int val = analogRead(A0);

	// move a number of steps equal to the change in the
	// sensor reading
	Stepper.step(val - previous);

	// remember the previous value of the sensor
	previous = val;
}
 