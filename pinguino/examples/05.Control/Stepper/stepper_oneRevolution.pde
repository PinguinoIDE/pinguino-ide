
/* 
	Stepper Motor Control - one revolution

	This program drives a unipolar or bipolar stepper motor. 
	The motor is attached to digital pins 0 - 3 of the Pinguino.

	The motor should revolve one revolution in one direction, then
	one revolution in the other direction.  

	Created 11 Mar. 2007
	Modified 30 Nov. 2009 by Tom Igoe
	Adapted 18 May 2011	by Régis Blanchot
*/

// change this to fit the number of steps per revolution for your motor
const int stepsPerRevolution = 48;

void setup()
{
	// initialize the stepper library on pins 0 through 3:
	Stepper.init(stepsPerRevolution,0,1,2,3);            
	// set the speed at 60 rpm:
	Stepper.setSpeed(60);
	// initialize the serial port:
	Serial.begin(9600);
}

void loop()
{
	// step one revolution  in one direction:
	Serial.println("clockwise");
	Stepper.step(stepsPerRevolution);
	delay(500);

	// step one revolution in the other direction:
	Serial.println("counterclockwise");
	Stepper.step(-stepsPerRevolution);
	delay(500); 
}
