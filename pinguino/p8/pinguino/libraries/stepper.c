/*	----------------------------------------------------------------------------
    FILE:			stepper.c
    PROJECT:		pinguino
    PURPOSE:		Drives a unipolar or bipolar stepper motor using  2 wires or 4 wires
    PROGRAMER:		Original library        (0.1) by Tom Igoe.
                    Two-wire modifications  (0.2) by Sebastian Gassner
                    Combination version     (0.3) by Tom Igoe and David Mellis
                    Bug fix for four-wire   (0.4) by Tom Igoe, bug fix from Noah Shibley  
                    Adapted for pinguino    (-.-) by Regis Blanchot (2011)
                    Half Step for four-wire (0.5) by John Zandbergen 20091104
                    Adapted for pinguino    (-.-) by Regis Blanchot (2013)
                    Interrupt               (0.6) by Regis Blanchot (2013)
                    Microstepping           (0.7) by Regis Blanchot (2013)

    FIRST RELEASE:	17-05-2011
    LAST RELEASE:	28-11-2013
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

    The sequence of fullstep control signals for 4 control wires is as follows:

    Step C0 C1 C2 C3
      1  1  0  1  0
      2  0  1  1  0
      3  0  1  0  1
      4  1  0  0  1

    The sequence of halfstep control signals for 4 control wires is as follows:

    Step C0 C1 C2 C3
     1   1  0  1  0
     2   0  0  1  0
     3   0  1  1  0
     4   0  1  0  0
     5   0  1  0  1
     6   0  0  0  1
     7   1  0  0  1
     8   1  0  0  0

    The sequence of controls signals for 2 control wires is as follows :
    (columns C1 and C2 from above):

    Step C0 C1
      1  0  1
      2  1  1
      3  1  0
      4  0  0

    The circuits can be found at 

    http://www.arduino.cc/en/Tutorial/Stepper

    Pinguino  ULN2003      Stepper
    ----------------------------------------------------------------------------
    pin1      1-|o |-16   (A) Brown
    pin2      2-|  |-15   (B) Red
    pin3      3-|  |-14   (C) Yellow
    pin4      4-|  |-13   (D) Blue
    nc        5-|  |-12   nc
    nc        6-|  |-11   nc
    nc        7-|  |-10   +12V - White
    GND       8-|  |-9    +12V - White
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

#define __STEPPER__

#include <digitalw.c>
//#include <millis.c>
#include <pin.h>
#include <oscillator.c>

int this_direction;                 // direction of rotation
int this_speed;                     // speed in RPMs
int this_step_delay;                // delay (ms) between steps, based on speed
int this_cycles;                    // cycles between steps, based on delay
int this_number_of_steps;           // total number of steps this motor can take
int this_step_number;               // which step the motor is on
int this_number_of_microsteps;      // how many microsteps in one full step
int this_microstep_number;          // which step the motor is on
int this_steps_left;                // how many steps left
int this_microsteps_left;           // how many microsteps left
int this_pin_count;                 // whether you're driving the motor with 2 or 4 pins

// motor pin numbers:
int this_motor_pin_1;
int this_motor_pin_2;
int this_motor_pin_3;
int this_motor_pin_4;
int this_stepper_pins[4];
    
//long this_last_step_time;           // time stamp in ms of when the last step was taken

/***********************************************************************
    Table for the microsteps.
    Even numbered values are loaded to CCPRxL and
    Odd numbered values are ORed with CCPxCON(CCPxCON<5:4>)
    to load complete 10 bits of PWMx duty cycle
***********************************************************************/

#ifdef __MICROSTEPPING__

int sine_table[] = {
    0x0,0x00,0x20,0x30,0x34,0x20,0x42,0x10,0x50,0x0,0x5C,0x10,0x68,0x20,
    0x74,0x30,0x80,0x30,0x8C,0x30,0x98,0x30,0xA4,0x20,0xB0,0x10,0xC3,0x10,
    0xD6,0x20,0xEA,0x20,0xFF,0x30
    };

#endif

/*	----------------------------------------------------------------------------
    Prototypes
    --------------------------------------------------------------------------*/

void Stepper_init(int, int, int, int, int);
void Stepper_setSpeed(long);
void Stepper_step(int);
void Stepper_stepMotor(int);
void Stepper_setMicrostep(int);

/*	--------------------------------------------------------------------
    constructor for two and four-pin version
    Sets which wires should control the motor.
    ------------------------------------------------------------------*/

void Stepper_init(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4)
{
    //millis_init();
    
    if (motor_pin_3 == 0 && motor_pin_4 == 0)
    {
        this_step_number = 0;                   // which step the motor is on
        this_speed = 0;                         // the motor speed, in revolutions per minute
        this_direction = 0;                     // motor direction
        //this_last_step_time = 0;                // time stamp in ms of the last step taken
        this_number_of_steps = number_of_steps;	// total number of steps for this motor
        this_number_of_microsteps = 1;                    // full steps
        
        // Pinguino pins for the motor control connection:
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
        this_step_number = 0;                   // which step the motor is on
        this_speed = 0;                         // the motor speed, in revolutions per minute
        this_direction = 0;                     // motor direction
        //this_last_step_time = 0;                // time stamp in ms of the last step taken
        this_number_of_steps = number_of_steps;	// total number of steps for this motor
        this_number_of_microsteps = 1;                    // full steps

        // Pinguino pins for the motor control connection:
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

    this_stepper_pins[0] = this_motor_pin_1;
    this_stepper_pins[1] = this_motor_pin_2;
    this_stepper_pins[2] = this_motor_pin_3;
    this_stepper_pins[3] = this_motor_pin_4;
}

/*	--------------------------------------------------------------------
    Sets microsteps per full step
    full steps = 1
    half steps = 2
    microsteps = 4, 8, 16 or 32 per full step.
    ------------------------------------------------------------------*/

void Stepper_setMicrostep(int microsteps)
{
    int i;
    
    this_number_of_microsteps = microsteps;
    
    if (microsteps > 2)
    {
        #if defined(__18f26j53) || defined(__18f46j53) || \
            defined(__18f27j53) || defined(__18f47j53)

        CCPTMRS1 = 0;                           // assign Timer2 to all CCP pins
        CCPTMRS2 = 0;
      
        #endif

        PR2 = 0xFF;                             // set PWM period to the max. to get 10-bit res.
        T2CON = 0b00000100;                     // Timer2 on, prescaler is 1

        for (i=0; i<4; i++)
        {
            // Configure the CCPx module for PWM operation
            switch (this_stepper_pins[i])
            {
                case CCP4:  CCP4CON   = 0b00001100; break;
                case CCP5:  CCP5CON   = 0b00001100; break;
                case CCP6:  CCP6CON   = 0b00001100; break;
                case CCP7:  CCP7CON   = 0b00001100; break;
                case CCP8:  CCP8CON   = 0b00001100; break;
                case CCP9:  CCP9CON   = 0b00001100; break;
                case CCP10: CCP10CON  = 0b00001100; break;
            }
        }
    }
}

/*	--------------------------------------------------------------------
    Sets the speed in revs per minute
    * TODO : interrupt
    ------------------------------------------------------------------*/

void Stepper_setSpeed(long whatSpeed)
{
    // delay (ms) between 2 steps
    // 1 mn = 60 sec = 60 * 1000 ms
    this_step_delay = 60000L / this_number_of_steps / whatSpeed / this_number_of_microsteps;

    // Set Timer overflow to this_step_delay (ms)
    // 1 cycle = Tcy = 1 / Fcy = 4 / Fosc (Fcy = Fosc / 4)
    // nb of cycles in this step_delay
    //  = ( this step_delay / 1000 ) / Tcy
    //  = this step_delay * Fcy / 1000
    //  = this step_delay * System.getPeripheralFrequency() / 1000    
    this_cycles = this_step_delay * ( System_getPeripheralFrequency() / 1000 );

    // Timer3 config.
    INTCONbits.GIEH = 0;    // enable global HP interrupts
    INTCONbits.GIEL = 0;    // enable global LP interrupts
    TMR3H = high8(0xFFFF - this_cycles);
    TMR3L =  low8(0xFFFF - this_cycles);
    T3CON = 0b00000011;     // 16-bit, prescaller 1:1, Timer ON;
    IPR2bits.TMR3IP = 1;    // interrupt has high priority
    PIR2bits.TMR3IF = 0;    // reset interrupt flag
    PIE2bits.TMR3IE = 1;    // enable interrupt 
    INTCONbits.GIEH = 1;    // enable global HP interrupts
    INTCONbits.GIEL = 1;    // enable global LP interrupts
}

/*	--------------------------------------------------------------------
    Moves the motor steps_to_move steps.  If the number is negative, 
    the motor moves in the reverse direction.
    ------------------------------------------------------------------*/

void Stepper_step(int steps_to_move)
{
    steps_to_move = steps_to_move * this_number_of_microsteps;
    // how many steps to take
    this_steps_left = steps_to_move > 0 ? steps_to_move : -steps_to_move;

    // determine direction based on whether steps_to_mode is + or -:
    if (steps_to_move > 0) {this_direction = 1;}
    if (steps_to_move < 0) {this_direction = 0;}
}

/*	--------------------------------------------------------------------
    Moves the motor forward or backwards.
    ------------------------------------------------------------------*/

void Stepper_stepMotor(int thisStep)
{
    int p, lsb, msb;
    
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
        switch (this_number_of_microsteps)
        {

            case 1: // full-step

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
                break;
                
            case 2: // half-step

                switch (thisStep)
                {
                    case 0:    //1010
                        digitalwrite(this_motor_pin_1, HIGH);
                        digitalwrite(this_motor_pin_2, LOW);
                        digitalwrite(this_motor_pin_3, HIGH);
                        digitalwrite(this_motor_pin_4, LOW);
                        break;
                    case 1:    //0010
                        digitalwrite(this_motor_pin_1, LOW);
                        digitalwrite(this_motor_pin_2, LOW);
                        digitalwrite(this_motor_pin_3, HIGH);
                        digitalwrite(this_motor_pin_4, LOW);
                        break;
                    case 2:    //0110
                        digitalwrite(this_motor_pin_1, LOW);
                        digitalwrite(this_motor_pin_2, HIGH);
                        digitalwrite(this_motor_pin_3, HIGH);
                        digitalwrite(this_motor_pin_4, LOW);
                        break;
                    case 3:    //0100
                        digitalwrite(this_motor_pin_1, LOW);
                        digitalwrite(this_motor_pin_2, HIGH);
                        digitalwrite(this_motor_pin_3, LOW);
                        digitalwrite(this_motor_pin_4, LOW);
                        break;
                    case 4:    //0101
                        digitalwrite(this_motor_pin_1, LOW);
                        digitalwrite(this_motor_pin_2, HIGH);
                        digitalwrite(this_motor_pin_3, LOW);
                        digitalwrite(this_motor_pin_4, HIGH);
                        break;
                    case 5:    //0001
                        digitalwrite(this_motor_pin_1, LOW);
                        digitalwrite(this_motor_pin_2, LOW);
                        digitalwrite(this_motor_pin_3, LOW);
                        digitalwrite(this_motor_pin_4, HIGH);
                        break;
                    case 6:    //1001
                        digitalwrite(this_motor_pin_1, HIGH);
                        digitalwrite(this_motor_pin_2, LOW);
                        digitalwrite(this_motor_pin_3, LOW);
                        digitalwrite(this_motor_pin_4, HIGH);
                        break;
                    case 7:    //1000
                        digitalwrite(this_motor_pin_1, HIGH);
                        digitalwrite(this_motor_pin_2, LOW);
                        digitalwrite(this_motor_pin_3, LOW);
                        digitalwrite(this_motor_pin_4, LOW);
                        break;
                }
                break;

            default:// microstepping (4, 8, 16 or 32 microsteps / step)

                for (p=0; p<4; p++)
                {
                    switch (p)
                    {
                        case 0:
                            lsb = (sine_table[thisStep] >> 2) & 0xFF;
                            msb = (sine_table[thisStep+1] & 0x300) >> 4;
                            break;
                        case 1:
                            lsb = (sine_table[thisStep] >> 2) & 0xFF;
                            msb = (-sine_table[thisStep+1] & 0x300) >> 4;
                            break;
                        case 2:
                            lsb = (-sine_table[thisStep] >> 2) & 0xFF;
                            msb = (-sine_table[thisStep+1] & 0x300) >> 4;
                            break;
                        case 3:
                            lsb = (-sine_table[thisStep] >> 2) & 0xFF;
                            msb = (sine_table[thisStep+1] & 0x300) >> 4;
                            break;
                    }

                    // Configure the CCPx module for PWM operation
                    switch (this_stepper_pins[p])
                    {
                        case CCP4:
                            CCPR4L   = lsb;
                            CCP4CON |= msb;
                            break;
                        case CCP5:
                            CCPR5L   = lsb;
                            CCP5CON |= msb;
                            break;
                        case CCP6:
                            CCPR6L   = lsb;
                            CCP6CON |= msb;
                            break;
                        case CCP7:
                            CCPR7L   = lsb;
                            CCP7CON |= msb;
                            break;
                        case CCP8:
                            CCPR8L   = lsb;
                            CCP8CON |= msb;
                            break;
                        case CCP9:
                            CCPR9L   = lsb;
                            CCP9CON |= msb;
                            break;
                        case CCP10:
                            CCPR10L   = lsb;
                            CCP10CON |= msb;
                            break;
                    }
                }
                break;
        }
    }
}

/*  --------------------------------------------------------------------
    stepper_interrupt
    --------------------------------------------------------------------
    millis  uses Timer0
    servos  uses Timer1
    pwm     uses Timer2
    stepper uses Timer3
    ------------------------------------------------------------------*/

void stepper_interrupt()
{
    if (PIR2bits.TMR3IF)
    {
        if (this_microsteps_left > 0)
        {
            // increment the microstep number,
            if (this_direction == 1)
            {
                this_microstep_number++;
                if (this_microstep_number == this_number_of_microsteps)
                    this_microstep_number = 0;
            } 

            // decrement the step number,
            else
            { 
                if (this_microstep_number == 0)
                    this_microstep_number = this_number_of_microsteps;
                this_microstep_number--;
            }

            // decrement the microsteps left:
            this_microsteps_left--;
        }
        
        else if (this_steps_left > 0)
        {
            // increment the step number,
            if (this_direction == 1)
            {
                this_step_number++;
                if (this_step_number == this_number_of_steps)
                    this_step_number = 0;
            } 

            // decrement the step number,
            else
            { 
                if (this_step_number == 0)
                    this_step_number = this_number_of_steps;
                this_step_number--;
                
                if (this_microstep_number == 0)
                    this_microstep_number = this_number_of_microsteps;
                this_microstep_number--;
            }
            
            // decrement the steps left:
            this_steps_left--;

            switch (this_number_of_microsteps)
            {
                case 1: // full-step
                    // step the motor to step number 0, 1, 2, or 3:
                    Stepper_stepMotor(this_step_number % 4);
                    break;

                case 2: // half-step
                    // step the motor to step number 0, 1, 2, 3, 4, 5, 6 or 7
                    Stepper_stepMotor(this_step_number % 8);
                    break;
                    
                default:// microstepping (4, 8, 16 or 32 microsteps / step)
                // set table pointer jump
                    Stepper_stepMotor(this_microstep_number + 32 / this_number_of_microsteps);
                    break;
            }
        }

        // load Timer again
        TMR3H = high8(0xFFFF - this_cycles);
        TMR3L =  low8(0xFFFF - this_cycles);
        PIR2bits.TMR3IF = 0;

    }
}


#endif
