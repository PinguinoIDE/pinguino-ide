/*	--------------------------------------------------------------------
    FILE:			stepper.c
    PROJECT:		pinguino
    PURPOSE:		Drives unipolar or bipolar stepper motors
                    using  2 wires or 4 wires
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
    LAST RELEASE:	19-12-2013
    --------------------------------------------------------------------
    When wiring multiple stepper motors to a microcontroller,
    you quickly run out of output pins, with each motor requiring 4 connections. 

    By making use of the fact that at any time two of the four motor
    coils are the inverse  of the other two, the number of
    control connections can be reduced from 4 to 2. 

    A slightly modified circuit around a Darlington transistor array or an L293 H-bridge
    connects to only 2 microcontroler pins, inverts the signals received,
    and delivers the 4 (2 plus 2 inverted ones) output signals required
    for driving a stepper motor.

    The sequence of wavestep control signals for 4 control wires is as follows:

    Step A  B  C  D
      1  1  0  0  0
      2  0  0  1  0
      3  0  1  0  0
      4  0  0  0  1

    The sequence of fullstep control signals for 4 control wires is as follows:

    Step A  B  C  D
      1  1  0  1  0
      2  0  1  1  0
      3  0  1  0  1
      4  1  0  0  1

    The sequence of halfstep control signals for 4 control wires is as follows:

    Step A  B  C  D
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

    Step A  B
      1  0  1
      2  1  1
      3  1  0
      4  0  0

    The circuits can be found at 

    http://www.arduino.cc/en/Tutorial/Stepper

    Pinguino  ULN2003      Stepper
    --------------------------------------------------------------------
    pin A     1-|o |-16   (A) Brown
    pin B     2-|  |-15   (B) Red
    pin C     3-|  |-14   (C) Yellow
    pin D     4-|  |-13   (D) Blue
    nc        5-|  |-12   nc
    nc        6-|  |-11   nc
    nc        7-|  |-10   +12V - White
    GND       8-|  |-9    +12V - White
    --------------------------------------------------------------------
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
    ------------------------------------------------------------------*/

#ifndef __STEPPER_C
#define __STEPPER_C

#define __STEPPER__

#include <system.c>                 // oscillator routines
#include <digitalw.c>               // pin definitions
//#ifdef __MICROSTEPPING__
#include <pwm.c>                    // pwm routines
//#endif
#include <interrupt.c>              // interrupts routines
//#include <trigo.c>                  // sine table * 256

int this_direction;                 // direction of rotation
int this_steps_per_rev;             // total number of steps this motor can take
int this_number_of_steps;           // total number of steps
volatile int this_current_step;     // which step the motor is on
int this_number_of_microsteps;      // how many microsteps in one full step
volatile int this_steps_left;       // how many steps left
int this_pin_count;                 // whether you're driving the motor with 2 or 4 pins
int this_jump;                      // set table pointer jump
                                    // 32 -> 1, 16 -> 2, 08 -> 4, 04 -> 8

// motor pin numbers:
int this_motor_pin_A;
int this_motor_pin_B;
int this_motor_pin_C;
int this_motor_pin_D;

// Windings status
int wave_table[4] = {
    0b1000, 0b0010, 0b0100, 0b0001
    };

int steps_table[8] = {
    0b1010, 0b0010, 0b0110, 0b0100, 0b0101, 0b0001, 0b1001, 0b1000
    };

/***********************************************************************
    Microsteps lookup table
***********************************************************************/

//#ifdef __MICROSTEPPING__            // defined if user uses Stepper_setMicrostep()

#define TABLE_DEPTH     32
#define INDEX_MASK      TABLE_DEPTH - 1

// 1 full-step of any stepper motor = 360/4 = 90 deg.
int sine_table[TABLE_DEPTH + 1] = {
    50,100,150,200,249,297,345,391,437,482,526,568,609,649,687,723,
    758,791,822,851,877,902,925,945,963,979,992,1003,1012,1018,1022,1023
    };  
/*
     25,  50,  75, 100, 124, 149, 172, 196,
    219, 241, 263, 284, 305, 325, 344, 362,
    379, 396, 411, 426, 439, 452, 463, 473,
    482, 490, 497, 502, 506, 510, 511, 512
*/
//#endif

/*	--------------------------------------------------------------------
    Prototypes
    ------------------------------------------------------------------*/

void Stepper_init(int, int, int, int, int);
int  Stepper_step(int, int, int);
void Stepper_stepMotor(int);

//void Stepper_setSpeed(long);
//int Stepper_step(int);
//#ifdef __MICROSTEPPING__
//void Stepper_setMicrostep(int);
//#endif

/*	--------------------------------------------------------------------
    constructor for two and four-pin version
    Sets which wires should control the motor.
    ------------------------------------------------------------------*/

void Stepper_init(int number_of_steps, int motor_pin_A, int motor_pin_B, int motor_pin_C, int motor_pin_D)
{
    if ((motor_pin_C + motor_pin_D) == 0)
    {
        // Pinguino pins for the motor control connection:
        this_motor_pin_A = motor_pin_A;
        this_motor_pin_B = motor_pin_B;

        // setup the pins on the microcontroller:
        pinmode(this_motor_pin_A, OUTPUT);
        pinmode(this_motor_pin_B, OUTPUT);

        // When there are only 2 pins, set the other two to 0:
        this_motor_pin_C = 0;
        this_motor_pin_D = 0;

        // pin_count is used by the stepMotor() method:
        this_pin_count = 2;
    }

    else
    {
        // Pinguino pins for the motor control connection:
        this_motor_pin_A = motor_pin_A;
        this_motor_pin_B = motor_pin_B;
        this_motor_pin_C = motor_pin_C;
        this_motor_pin_D = motor_pin_D;

        // setup the pins on the microcontroller:
        pinmode(this_motor_pin_A, OUTPUT);
        pinmode(this_motor_pin_B, OUTPUT);
        pinmode(this_motor_pin_C, OUTPUT);
        pinmode(this_motor_pin_D, OUTPUT);

        // pin_count is used by the stepMotor() method:  
        this_pin_count = 4;  
    }

    this_steps_per_rev = number_of_steps;// total number of steps for this motor
    this_current_step = 0;              // which step the motor is on
    this_direction = 0;                 // motor direction
    //this_last_step_time = 0;          // time stamp in ms of the last step taken
    this_number_of_microsteps = 1;      // full steps per default
    this_jump = 1;
    this_steps_left = 0;
}

/*	--------------------------------------------------------------------
    Sets microsteps per full step
    full steps = 1
    half steps = 2
    microsteps = 4, 8, 16 or 32 per full step.
    ------------------------------------------------------------------*/
/*
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
        PWM_set_frequency(64000);
        this_number_of_steps *= this_number_of_microsteps;
        this_jump = (32 / this_number_of_microsteps);
    }
}
#endif
*/

/*	--------------------------------------------------------------------
    Sets the speed in revs per minute
    * TODO : interrupt
    ------------------------------------------------------------------*/
/*
void Stepper_setSpeed(long revolutionPerMinute)
{
    int cycles, prescaler=0;                    // number of cycles between 2 steps
    
    // delay (ms) between 2 steps
    // 1 mn = 60 sec = 60 * 1000 * 1000 us
    this_step_delay = 60000000L / (revolutionPerMinute * this_number_of_steps);

    // Set Timer overflow to this_step_delay (in ms)
    // 1 cycle = Tcy = 1 / Fcy (sec)
    // nb of cycles in this step_delay (in ms)
    //  = ( this step_delay / 1000    ) / Tcy
    //  = ( this step_delay * Fcy     ) / 1000
    //  = ( this step_delay * PBCLOCK ) / 1000    
    cycles = this_step_delay * ( GetPeripheralClock() / 1000 / 1000 );
    
    while (cycles > 0xFFFF)
    {
        cycles /= 8;
        prescaler += 1;
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
*/

/*	--------------------------------------------------------------------
    Moves the motor of steps_to_move steps.
    If the number is negative, the motor moves in the reverse direction.
    ------------------------------------------------------------------*/

int Stepper_step(int steps_to_move, int microsteps, int revolutionPerMinute)
{
    int delay_us_per_step;              // delay between 2 steps
    int delay_ms_per_move;              // delay for the whole move
    int cycles_per_step;                // number of cycles between 2 steps
    int prescaler = 0;                  // 0 = 1:1 default prescale value
                                        // 1 = 1:8 prescale value
                                        // 2 = 1:64 prescale value
                                        // 3 = 1:256 prescale value

    // wait previous command to be finished
    while (this_steps_left > 0);

    switch ( microsteps )
    {
        case 0:
            // wave steps todo
            this_number_of_microsteps = 1;
            break;

        case 1:
            this_number_of_microsteps = 1;
            break;
    
        default:
            if ( (microsteps % 2) == 1 ) // odd ?
                this_number_of_microsteps = microsteps + 1; 
            else
                this_number_of_microsteps = microsteps;

            if (this_number_of_microsteps > 32)
                this_number_of_microsteps = 32;

            if (this_number_of_microsteps < 2)
                this_number_of_microsteps = 2;
            break;
    }
    
    // determine direction based on whether steps_to_move is + or -:
    if (steps_to_move > 0) this_direction = 1;
    if (steps_to_move < 0) this_direction = 0;

    // change number of steps
    this_number_of_steps = this_steps_per_rev * this_number_of_microsteps;
    steps_to_move = this_number_of_steps;

    // how many steps to take ?
    this_steps_left = steps_to_move > 0 ? steps_to_move : -steps_to_move;

    // microsteps ?
    if (this_number_of_microsteps > 2)
    {
        PWM_init(); // Timer3
        PWM_set_frequency(100000); // 40KHz
        this_jump = (TABLE_DEPTH / this_number_of_microsteps);
    }
    
    // delay between 2 steps (1 mn = 60 * 1000 * 1000 us)
    delay_us_per_step = 60 * 1000 * 1000 / revolutionPerMinute / this_number_of_steps;
    delay_ms_per_move = delay_us_per_step * steps_to_move / 1000;
    cycles_per_step   = delay_us_per_step * ( GetPeripheralClock() / 1000 / 1000 );
    
    while (cycles_per_step > 0xFFFF)
    {
        prescaler += 1;
        if (prescaler > 2)
        {
            prescaler = 3;
            cycles_per_step /= 4;
        }
        else
        {
            cycles_per_step /= 8;
        }
    }

    // Timer1 Configuration
    IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    //INTCONSET = 0x1000;
    T1CON    = prescaler << 4;          // prescaler, internal peripheral clock
    TMR1     = 0x00;                    // clear timer register
    PR1      = cycles_per_step;         // load period register
    IntSetVectorPriority(INT_TIMER1_VECTOR, 7, 3);
    //IPC1SET  = 7;                       // select interrupt priority and sub-priority
    IntClearFlag(INT_TIMER1);
    //IFS0CLR  = 1 << 4; //INT_TIMER1_VECTOR;  // clear interrupt flag
    IntEnable(INT_TIMER1);
    //IEC0SET  = 1 << 4; //INT_TIMER1_VECTOR;  // enable timer 1 interrupt
    T1CONSET = 0x8000;                  // start timer 1

    //  return time (ms) to revolve the whole steps to move
    return ( delay_ms_per_move );
}

/*	--------------------------------------------------------------------
    Moves the motor forward or backwards.
    ------------------------------------------------------------------*/

void Stepper_stepMotor(int thisStep)
{
    int index;
    int current_microstep;
    int t, inc, dec;

    // wave-step
    if (this_number_of_microsteps == 0)
    {
        // step the motor to step number 0, 1, 2, or 3:
        t = wave_table [ (thisStep % 4) ];

        // Winding A
        digitalwrite( this_motor_pin_A, t >> 0 & 1); // bit 0
        digitalwrite( this_motor_pin_B, t >> 1 & 1); // bit 1

        // Winding B
        if (this_pin_count == 4)
        {
            digitalwrite( this_motor_pin_C, t >> 2 & 1); // bit 2
            digitalwrite( this_motor_pin_D, t >> 3 & 1); // bit 3
        }
    }

    // full-step
    else if (this_number_of_microsteps == 1)
    {
        // step the motor to step number 0, 1, 2, or 3:
        t = steps_table [ (thisStep % 4) * 2 ];

        // Winding A
        digitalwrite( this_motor_pin_A, t >> 0 & 1); // bit 0
        digitalwrite( this_motor_pin_B, t >> 1 & 1); // bit 1

        // Winding B
        if (this_pin_count == 4)
        {
            digitalwrite( this_motor_pin_C, t >> 2 & 1); // bit 2
            digitalwrite( this_motor_pin_D, t >> 3 & 1); // bit 3
        }
    }

    // half-step
    else if (this_number_of_microsteps == 2)
    {
        // step the motor to step number 0, 1, 2, 3, 4, 5, 6 or 7
        t = steps_table [ thisStep % 8 ];

        // Winding A
        digitalwrite( this_motor_pin_A, t >> 0 & 1); // bit 0
        digitalwrite( this_motor_pin_B, t >> 1 & 1); // bit 1

        // Winding B
        if (this_pin_count == 4)
        {
            digitalwrite( this_motor_pin_C, t >> 2 & 1); // bit 2
            digitalwrite( this_motor_pin_D, t >> 3 & 1); // bit 3
        }
    }
    
    // microstepping (4, 8, 16 or 32 microsteps / step)
    //#ifdef __MICROSTEPPING__
    else if (this_number_of_microsteps > 2)
    {
        /**
         * In practice, the current in one winding is kept
         * constant over half of the complete step and current
         * in the other winding is varied as a function of
         * sinθ to maximize the motor torque.
        **/

        // from 0 to total number of microsteps - 1
        current_microstep = ( thisStep % this_number_of_microsteps );// + 1;
        
        // table index is function of total microsteps number
        // this_jump = (TABLE_DEPTH / this_number_of_microsteps);
        index = current_microstep * this_jump;

        // from 0 to 90 degrees (from 0 to 1023)
        // increase current in next active coil
        inc = sine_table[ index & INDEX_MASK ]; 

        // from 90 to 0 degrees (from 1023 to 0)
        // decrease current in active coil
        dec = sine_table[ INDEX_MASK - (index & INDEX_MASK) ];

        switch ( (thisStep / this_number_of_microsteps) % 4 )
        {
            case 0: // from A to C
                analogwrite( this_motor_pin_A, (u16) dec );
                analogwrite( this_motor_pin_B, (u16) 0 );
                analogwrite( this_motor_pin_C, (u16) inc );
                analogwrite( this_motor_pin_D, (u16) 0 );
                break;

            case 1: // from C to B
                analogwrite( this_motor_pin_A, (u16) 0 );
                analogwrite( this_motor_pin_B, (u16) inc );
                analogwrite( this_motor_pin_C, (u16) dec );
                analogwrite( this_motor_pin_D, (u16) 0 );
                break;

            case 2: // from B to D
                analogwrite( this_motor_pin_A, (u16) 0 );
                analogwrite( this_motor_pin_B, (u16) dec );
                analogwrite( this_motor_pin_C, (u16) 0 );
                analogwrite( this_motor_pin_D, (u16) inc );
                break;

            case 3: // from D to A
                analogwrite( this_motor_pin_A, (u16) inc );
                analogwrite( this_motor_pin_B, (u16) 0 );
                analogwrite( this_motor_pin_C, (u16) 0 );
                analogwrite( this_motor_pin_D, (u16) dec );
                break;
        }
    }
    //#endif
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
                this_current_step += 1;
                if (this_current_step == this_number_of_steps)
                    this_current_step = 0;
            } 

            // decrement the step number,
            else
            { 
                if (this_current_step == 0)
                    this_current_step = this_number_of_steps;
                this_current_step -= 1;
            }
            
            // decrement the steps left:
            this_steps_left -= 1;

            // step the motor
            Stepper_stepMotor(this_current_step);
        }

        IntClearFlag(INT_TIMER1);
        //IFS0CLR = 1 << 4;
    }
}

#endif
