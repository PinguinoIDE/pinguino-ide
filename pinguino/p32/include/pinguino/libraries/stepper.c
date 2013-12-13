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

#define TABLE_DEPTH     16
#define INDEX_MASK      TABLE_DEPTH - 1

#include <system.c>                 // oscillator routines
#include <digitalw.c>               // pin definitions
#ifdef __MICROSTEPPING__
#include <pwm.c>                    // pwm routines
#endif
#include <interrupt.c>              // interrupts routines
//#include <trigo.c>                  // sine table * 256

int this_direction;                 // direction of rotation
int this_speed;                     // speed in RPMs
int this_step_delay;                // delay (ms) between steps, based on speed
int this_number_of_steps;           // total number of steps this motor can take
int this_current_step_number;       // which step the motor is on
int this_number_of_microsteps;      // how many microsteps in one full step
int this_current_microstep_number;  // which step the motor is on
int this_steps_left;                // how many steps left
int this_microsteps_left;           // how many microsteps left
int this_pin_count;                 // whether you're driving the motor with 2 or 4 pins

// motor pin numbers:
int this_motor_pin_1;
int this_motor_pin_2;
int this_motor_pin_3;
int this_motor_pin_4;

/***********************************************************************
    Microsteps looup table
***********************************************************************/

#ifdef __MICROSTEPPING__            // defined if user uses Stepper_setMicrostep()

// 1 full-step of any stepper motor = 360/4 = 90 deg.
// 
int sine_table[TABLE_DEPTH] = {
    100, 199, 297, 391, 482, 568, 649, 724,
    791, 851, 903, 946, 979, 1004, 1019, 1023
 };  

#endif

/*	----------------------------------------------------------------------------
    Prototypes
    --------------------------------------------------------------------------*/

void Stepper_init(int, int, int, int, int);
void Stepper_setSpeed(long);
void Stepper_step(int);
void Stepper_stepMotor(int);
#ifdef __MICROSTEPPING__
void Stepper_setMicrostep(int);
#endif

/*	--------------------------------------------------------------------
    constructor for two and four-pin version
    Sets which wires should control the motor.
    ------------------------------------------------------------------*/

void Stepper_init(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4)
{
    if ((motor_pin_3 + motor_pin_4) == 0)
    {
        this_current_step_number = 0;           // which step the motor is on
        this_speed = 0;                         // the motor speed, in revolutions per minute
        this_direction = 0;                     // motor direction
        //this_last_step_time = 0;              // time stamp in ms of the last step taken
        this_number_of_steps = number_of_steps; // total number of steps for this motor
        this_number_of_microsteps = 1;          // full steps
        
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
        this_current_step_number = 0;           // which step the motor is on
        this_speed = 0;                         // the motor speed, in revolutions per minute
        this_direction = 0;                     // motor direction
        //this_last_step_time = 0;              // time stamp in ms of the last step taken
        this_number_of_steps = number_of_steps;	// total number of steps for this motor
        this_number_of_microsteps = 1;          // full steps

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
    this_steps_left = 0;
}

/*	--------------------------------------------------------------------
    Sets microsteps per full step
    full steps = 1
    half steps = 2
    microsteps = 4, 8, 16 or 32 per full step.
    ------------------------------------------------------------------*/

#ifdef __MICROSTEPPING__
void Stepper_setMicrostep(int microsteps)
{
    int i, a;

    if ( (microsteps % 2) == 1 ) // odd ?
        this_number_of_microsteps = microsteps + 1; 
    else
        this_number_of_microsteps = microsteps;

    if (this_number_of_microsteps > 32)
        this_number_of_microsteps = 32;

    if (this_number_of_microsteps < 2)
        this_number_of_microsteps = 2;

    if (this_number_of_microsteps > 2)
    {
        PWM_init(); // Timer3
        this_number_of_steps *= this_number_of_microsteps;
    }
}
#endif

/*	--------------------------------------------------------------------
    Sets the speed in revs per minute
    * TODO : interrupt
    ------------------------------------------------------------------*/

void Stepper_setSpeed(long whatSpeed)
{
    int cycles, prescaler=0;                    // number of cycles between 2 steps
    
    // delay (ms) between 2 steps
    // 1 mn = 60 sec = 60 * 1000 * 1000 us
    this_step_delay = 60000000L / (whatSpeed * this_number_of_steps);

    // Set Timer overflow to this_step_delay (in ms)
    // 1 cycle = Tcy = 1 / Fcy (sec)
    // nb of cycles in this step_delay (in ms)
    //  = ( this step_delay / 1000    ) / Tcy
    //  = ( this step_delay * Fcy     ) / 1000
    //  = ( this step_delay * PBCLOCK ) / 1000    
    cycles = this_step_delay * ( GetPeripheralClock() / 1000 / 1000 );
    
    while (cycles > 0xFFFF)
    {
        cycles = cycles / 8;
        prescaler = prescaler + 1;
    }

    // Timer1 Configuration
    IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    //INTCONSET = 0x1000;
    T1CON    = prescaler << 4;          // prescaler, internal peripheral clock
    TMR1     = 0x00;                    // clear timer register
    PR1      = cycles;                  // load period register
    IntSetVectorPriority(INT_TIMER1_VECTOR, 7, 3);
    //IPC1SET  = 7;                       // select interrupt priority and sub-priority
    IntClearFlag(INT_TIMER1);
    //IFS0CLR  = 1 << 4; //INT_TIMER1_VECTOR;  // clear interrupt flag
    IntEnable(INT_TIMER1);
    //IEC0SET  = 1 << 4; //INT_TIMER1_VECTOR;  // enable timer 1 interrupt
    T1CONSET = 0x8000;                  // start timer 1
}

/*	--------------------------------------------------------------------
    Moves the motor steps_to_move steps.  If the number is negative, 
    the motor moves in the reverse direction.
    ------------------------------------------------------------------*/

void Stepper_step(int steps_to_move)
{
    if (this_steps_left == 0)
    {
        steps_to_move = steps_to_move * this_number_of_steps;
        // how many steps to take
        this_steps_left = steps_to_move > 0 ? steps_to_move : -steps_to_move;

        // determine direction based on whether steps_to_move is + or -:
        if (steps_to_move > 0) {this_direction = 1;}
        if (steps_to_move < 0) {this_direction = 0;}
    }
}

/*	--------------------------------------------------------------------
    Moves the motor forward or backwards.
    ------------------------------------------------------------------*/

void Stepper_stepMotor(int thisStep)
{
    int m;
    
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

            #ifdef __MICROSTEPPING__
            default:// microstepping (4, 8, 16 or 32 microsteps / step)
            
                    /**
                     * In practice, the current in one winding is kept
                     * constant over half of the complete step and current
                     * in the other winding is varied as a function of
                     * sinθ to maximize the motor torque.
                    **/

                    
                    // Winding A
                    analogwrite( this_motor_pin_1, 1023 );
                    analogwrite( this_motor_pin_2, sine_table[thisStep & INDEX_MASK] );
                
                    // Winding B
                    analogwrite( this_motor_pin_3, 1023 );
                    analogwrite( this_motor_pin_4, sine_table[ (thisStep + TABLE_DEPTH / 2) & INDEX_MASK] );

                break;
            #endif

        }
    }
}

/*  --------------------------------------------------------------------
    stepper_interrupt
    --------------------------------------------------------------------
    millis  uses Timer0
    servos  uses Timer1
    pwm     uses Timer3
    stepper uses Timer2
    ------------------------------------------------------------------*/

// Put the ISR_wrapper at the good place (Timer1 = Vector 4)
void ISR_wrapper_vector_4(void) __attribute__ ((section (".vector_4")));
void stepper_interrupt(void)    __attribute__ ((interrupt));
void ISR_wrapper_vector_4(void) { stepper_interrupt(); }

void stepper_interrupt(void)
{
    // is this an TMR1 interrupt ?
    //if ( IFS0bits.T1IF )
    if (IntGetFlag(INT_TIMER1))
    {
        if (this_steps_left > 0)
        {
            // increment the step number,
            if (this_direction == 1)
            {
                this_current_step_number++;
                if (this_current_step_number == this_number_of_steps)
                    this_current_step_number = 0;
            } 

            // decrement the step number,
            else
            { 
                if (this_current_step_number == 0)
                    this_current_step_number = this_number_of_steps;
                this_current_step_number--;
                
                if (this_current_microstep_number == 0)
                    this_current_microstep_number = this_number_of_microsteps;
                this_current_microstep_number--;
            }
            
            // decrement the steps left:
            this_steps_left--;

            switch (this_number_of_microsteps)
            {
                case 1: // full-step
                    // step the motor to step number 0, 1, 2, or 3:
                    Stepper_stepMotor(this_current_step_number % 4);
                    break;

                case 2: // half-step
                    // step the motor to step number 0, 1, 2, 3, 4, 5, 6 or 7
                    Stepper_stepMotor(this_current_step_number % 8);
                    break;
                    
                default:// microstepping (4, 8, 16 or 32 microsteps / step)
                    Stepper_stepMotor(this_current_step_number);
                    break;
            }
        }

        IntClearFlag(INT_TIMER1);
        //IFS0CLR = 1 << 4;

    }
}

#endif
