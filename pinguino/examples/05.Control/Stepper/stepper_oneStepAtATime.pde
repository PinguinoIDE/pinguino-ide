/* 
	Stepper Motor Control - one step at a time

	This program drives a unipolar or bipolar stepper motor. 
	The motor is attached to digital pins 0 - 3 of the Pinguino.

	The motor will step one step at a time, very slowly.  You can use this to
	test that you've got the four wires of your stepper wired to the correct
	pins. If wired correctly, all steps should be in the same direction.

	Use this also to count the number of steps per revolution of your motor,
	if you don't know it.  Then plug that number into the oneRevolution
	example to see if you got it right.

	Created 30 Nov. 2009
	by Tom Igoe
	Adapted 18 May 2011
	by Régis Blanchot
*/

const int stepsPerRevolution = 48;  // change this to fit the number of steps per revolution for your motor
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
	// step one step:
	Stepper.step(1);
	Serial.printf("steps: %d\r\n", stepCount);
	/*
	Serial.print("steps:" );
	Serial.println(stepCount);
	*/
	stepCount++;
	delay(500);
}
