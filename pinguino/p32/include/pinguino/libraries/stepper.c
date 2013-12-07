/*	----------------------------------------------------------------------------
	FILE:				stepper.c
	PROJECT:			pinguino
	PURPOSE:			Drives a unipolar or bipolar stepper motor using  2 wires or 4 wires
	PROGRAMER:		Original library			(0.1) by Tom Igoe.
						Two-wire modifications	(0.2) by Sebastian Gassner
						Combination version		(0.3) by Tom Igoe and David Mellis
						Bug fix for four-wire	(0.4) by Tom Igoe, bug fix from Noah Shibley  
						Adapted for pinguino				by Regis Blanchot
	FIRST RELEASE:	17-05-2011
	LAST RELEASE:	18-05-2011
	----------------------------------------------------------------------------
	When wiring multiple stepper motors to a microcontroller,
	you quickly run out of output pins, with each motor requiring 4 connections. 

	By making use of the fact that at any time two of the four motor
	coils are the inverse  of the other two, the number of
	control connections can be reduced from 4 to 2. 

	A slightly modified circuit around a Darlington transistor array or an L293 H-bridge
	connects to only 2 microcontroler pins, inverts the signals received,
	and delivers the 4 (2 plus 2 inverted ones) output signals required
	for driving a stepper motor.

	The sequence of control signals for 4 control wires is as follows:

	Step C0 C1 C2 C3
	  1  1  0  1  0
	  2  0  1  1  0
	  3  0  1  0  1
	  4  1  0  0  1

	The sequence of controls signals for 2 control wires is as follows
	(columns C1 and C2 from above):

	Step C0 C1
	  1  0  1
	  2  1  1
	  3  1  0
	  4  0  0

	The circuits can be found at 

	http://www.arduino.cc/en/Tutorial/Stepper

	ULN2003
	----------------------------------------------------------------------------
	pin1	1-|o	|-16	(A) marron
	pin2	2-|	|-15	(B) rouge
	pin3	3-|	|-14	(C) jaune
	pin4	4-|	|-13	(D) bleu
	x		5-|	|-12	x
	x		6-|	|-11	x
	x		7-|	|-10	x
	GND	8-|	|-9	+12V - fils blancs du moteur
	----------------------------------------------------------------------------
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	--------------------------------------------------------------------------*/

#ifndef __STEPPER_C
#define __STEPPER_C

#include <digitalw.c>
#include <millis.c>

int this_direction;					// Direction of rotation
int this_speed;						// Speed in RPMs
unsigned long this_step_delay;	// delay between steps, in ms, based on speed
int this_number_of_steps;			// total number of steps this motor can take
int this_pin_count;					// whether you're driving the motor with 2 or 4 pins
int this_step_number;				// which step the motor is on

// motor pin numbers:
int this_motor_pin_1;
int this_motor_pin_2;
int this_motor_pin_3;
int this_motor_pin_4;

long this_last_step_time;			// time stamp in ms of when the last step was taken

/*	----------------------------------------------------------------------------
	Prototypes
	--------------------------------------------------------------------------*/

void Stepper_init(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4);
void Stepper_setSpeed(long whatSpeed);
void Stepper_step(int steps_to_move);
void Stepper_stepMotor(int thisStep);

/*	----------------------------------------------------------------------------
	constructor for two and four-pin version
	Sets which wires should control the motor.
	--------------------------------------------------------------------------*/

void Stepper_init(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4)
{
	if (motor_pin_3 == 0 && motor_pin_4 == 0)
	{
		this_step_number = 0;							// which step the motor is on
		this_speed = 0;									// the motor speed, in revolutions per minute
		this_direction = 0;								// motor direction
		this_last_step_time = 0;						// time stamp in ms of the last step taken
		this_number_of_steps = number_of_steps;	// total number of steps for this motor

		// Arduino pins for the motor control connection:
		this_motor_pin_1 = motor_pin_1;
		this_motor_pin_2 = motor_pin_2;

		// setup the pins on the microcontroller:
		pinmode(this_motor_pin_1, OUTPUT);
		pinmode(this_motor_pin_2, OUTPUT);

		// When there are only 2 pins, set the other two to 0:
		this_motor_pin_3 = 0;
		this_motor_pin_4 = 0;

		// pin_count is used by the stepMotor() method:
		this_pin_count = 2;
	}
	else
	{
		this_step_number = 0;							// which step the motor is on
		this_speed = 0;									// the motor speed, in revolutions per minute
		this_direction = 0;								// motor direction
		this_last_step_time = 0;						// time stamp in ms of the last step taken
		this_number_of_steps = number_of_steps;	// total number of steps for this motor

		// Arduino pins for the motor control connection:
		this_motor_pin_1 = motor_pin_1;
		this_motor_pin_2 = motor_pin_2;
		this_motor_pin_3 = motor_pin_3;
		this_motor_pin_4 = motor_pin_4;

		// setup the pins on the microcontroller:
		pinmode(this_motor_pin_1, OUTPUT);
		pinmode(this_motor_pin_2, OUTPUT);
		pinmode(this_motor_pin_3, OUTPUT);
		pinmode(this_motor_pin_4, OUTPUT);

		// pin_count is used by the stepMotor() method:  
		this_pin_count = 4;  
	}
}

/*	----------------------------------------------------------------------------
	Sets the speed in revs per minute
	--------------------------------------------------------------------------*/

void Stepper_setSpeed(long whatSpeed)
{
  this_step_delay = 60L * 1000L / this_number_of_steps / whatSpeed;
}

/*	----------------------------------------------------------------------------
	Moves the motor steps_to_move steps.  If the number is negative, 
	the motor moves in the reverse direction.
	--------------------------------------------------------------------------*/

void Stepper_step(int steps_to_move)
{  
	int steps_left = abs(steps_to_move);  // how many steps to take

	// determine direction based on whether steps_to_mode is + or -:
	if (steps_to_move > 0) {this_direction = 1;}
	if (steps_to_move < 0) {this_direction = 0;}

	// decrement the number of steps, moving one step each time:
	while(steps_left > 0)
	{
		// move only if the appropriate delay has passed:
		if (millis() - this_last_step_time >= this_step_delay)
		{
			// get the timeStamp of when you stepped:
			this_last_step_time = millis();
			// increment or decrement the step number,
			// depending on direction:
			if (this_direction == 1)
			{
				this_step_number++;
				if (this_step_number == this_number_of_steps)
				{
					this_step_number = 0;
				}
			} 
			else
			{ 
				if (this_step_number == 0)
				{
					this_step_number = this_number_of_steps;
				}
				this_step_number--;
			}
			// decrement the steps left:
			steps_left--;
			// step the motor to step number 0, 1, 2, or 3:
			Stepper_stepMotor(this_step_number % 4);
		}
	}
}

/*	----------------------------------------------------------------------------
	Moves the motor forward or backwards.
	--------------------------------------------------------------------------*/

void Stepper_stepMotor(int thisStep)
{
	if (this_pin_count == 2)
	{
		switch (thisStep)
		{
			case 0: /* 01 */
				digitalwrite(this_motor_pin_1, LOW);
				digitalwrite(this_motor_pin_2, HIGH);
				break;
			case 1: /* 11 */
				digitalwrite(this_motor_pin_1, HIGH);
				digitalwrite(this_motor_pin_2, HIGH);
				break;
			case 2: /* 10 */
				digitalwrite(this_motor_pin_1, HIGH);
				digitalwrite(this_motor_pin_2, LOW);
				break;
			case 3: /* 00 */
				digitalwrite(this_motor_pin_1, LOW);
				digitalwrite(this_motor_pin_2, LOW);
				break;
		} 
	}
	if (this_pin_count == 4)
	{
		switch (thisStep)
		{
			case 0:    // 1010
				digitalwrite(this_motor_pin_1, HIGH);
				digitalwrite(this_motor_pin_2, LOW);
				digitalwrite(this_motor_pin_3, HIGH);
				digitalwrite(this_motor_pin_4, LOW);
				break;
			case 1:    // 0110
				digitalwrite(this_motor_pin_1, LOW);
				digitalwrite(this_motor_pin_2, HIGH);
				digitalwrite(this_motor_pin_3, HIGH);
				digitalwrite(this_motor_pin_4, LOW);
				break;
			case 2:    //0101
				digitalwrite(this_motor_pin_1, LOW);
				digitalwrite(this_motor_pin_2, HIGH);
				digitalwrite(this_motor_pin_3, LOW);
				digitalwrite(this_motor_pin_4, HIGH);
				break;
			case 3:    //1001
				digitalwrite(this_motor_pin_1, HIGH);
				digitalwrite(this_motor_pin_2, LOW);
				digitalwrite(this_motor_pin_3, LOW);
				digitalwrite(this_motor_pin_4, HIGH);
				break;
		} 
	}
}

#endif
