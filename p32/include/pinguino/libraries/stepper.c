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
    LAST RELEASE:	10-05-2014
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

#include <const.h>                      // MIPS32
#include <system.c>                     // oscillator routines
#include <digitalw.c>                   // pin definitions
#include <delay.c>                      // Delayms
#include <interrupt.c>                  // interrupts routines

#ifdef __MICROSTEPPING__
#include <pwm.c>                        // pwm routines
#endif

/*
typedef struct
{
*/
    int g_motor_pin_A;                  // motor pin numbers
    int g_motor_pin_B;
    int g_motor_pin_C;
    int g_motor_pin_D;
    int g_motor_steps_per_rev;          // total number of steps this motor can take
    int g_total_number_of_steps;        // total number of steps for one rev.
    int g_number_of_microsteps=2;       // how many microsteps in one full step
    int g_delay_us_per_step;            // delay between 2 steps
    int g_direction;                    // direction of rotation
    volatile int g_steps_left=0;        // how many steps left
    int g_4pin;                         // whether the motor is driven with 2 or 4 pins
    int g_jump;                         // set table pointer jump
                                        // 32 -> 1, 16 -> 2, 8 -> 4, 4 -> 8
/*
    void (*init);
    void (*stepMotor);
    void (*setMicrostep);
    void (*setSpeed);
    void (*step);
} Stepper_t;
*/

// Windings status
int wave_table[4] = {
    0b1000, 0b0010, 0b0100, 0b0001
    };

int steps_table[8] = {
    0b1010, 0b0010, 0b0110, 0b0100, 0b0101, 0b0001, 0b1001, 0b1000
    };

/***********************************************************************
    Microsteps lookup table
    Regis Blanchot (2013)
************************************************************************/

#ifdef __MICROSTEPPING__                // defined if user uses Stepper_setMicrostep()

#define TABLE_DEPTH     32
#define INDEX_MASK      TABLE_DEPTH - 1

// 1 full-step of any stepper motor = 360/4 = 90 deg.
int sine_table[TABLE_DEPTH] = {
      50,  100,  150,  200,  249,  297,  345,  391,
     437,  482,  526,  568,  609,  649,  687,  723,
     758,  791,  822,  851,  877,  902,  925,  945,
     963,  979,  992, 1003, 1012, 1018, 1022, 1023
    };  

#endif

/*	--------------------------------------------------------------------
    Prototypes
    ------------------------------------------------------------------*/

void Stepper_init(int, int, int, int, int);
void Stepper_stepMotor(int);
#ifdef __MICROSTEPPING__
void Stepper_setMicrostep(int);
#endif
void Stepper_setSpeed(int);
//int Stepper_step(int);
void Stepper_step(int);

/**--------------------------------------------------------------------
    Constructor for two and four-pin version
    Sets which wires should control the motor.
    -----------------------------------------------------------------**/

void Stepper_init(int number_of_steps, int motor_pin_A, int motor_pin_B, int motor_pin_C, int motor_pin_D)
{
    if ((motor_pin_C + motor_pin_D) == 0)
    {
        // Pinguino pins for the motor control connection:
        g_motor_pin_A = motor_pin_A;
        g_motor_pin_B = motor_pin_B;

        // setup the pins on the microcontroller:
        pinmode(g_motor_pin_A, OUTPUT);
        pinmode(g_motor_pin_B, OUTPUT);

        // When there are only 2 pins, set the other two to 0:
        g_motor_pin_C = 0;
        g_motor_pin_D = 0;

        // pin_count is used by the stepMotor() method:
        g_4pin = 0;
    }

    else
    {
        // Pinguino pins for the motor control connection:
        g_motor_pin_A = motor_pin_A;
        g_motor_pin_B = motor_pin_B;
        g_motor_pin_C = motor_pin_C;
        g_motor_pin_D = motor_pin_D;

        // setup the pins on the microcontroller:
        pinmode(g_motor_pin_A, OUTPUT);
        pinmode(g_motor_pin_B, OUTPUT);
        pinmode(g_motor_pin_C, OUTPUT);
        pinmode(g_motor_pin_D, OUTPUT);

        // pin_count is used by the stepMotor() method:  
        g_4pin = 1;  
    }

    g_motor_steps_per_rev = number_of_steps;     // total number of steps for this motor
    g_number_of_microsteps = 2;                  // half-steps per default
    g_total_number_of_steps = g_motor_steps_per_rev * g_number_of_microsteps;
    //g_current_step = 0;                          // which step the motor is on
    g_steps_left = 0;                            // how many steps are still to go
}

/**--------------------------------------------------------------------
    Sets microsteps per full step
    wave steps = 0
    full steps = 1
    half steps = 2
    microsteps = 4, 8, 16 or 32 per full step.
    Regis Blanchot (2013)
    -----------------------------------------------------------------**/

#ifdef __MICROSTEPPING__
void Stepper_setMicrostep(int m)
{
    if ( (m == 0) || (m == 1) )
    {
        g_number_of_microsteps = 1;
        g_total_number_of_steps = g_motor_steps_per_rev;
        return;
    }

    else if ( m == 2 )
    {
        g_number_of_microsteps = 2;
        g_total_number_of_steps = g_motor_steps_per_rev * 2;
        return;
    }

    else // if (m > 2)
    {
        if (m > 32) m = 32;
        
        // check m is a power of 2
        while ( ( m & (m - 1) ) == 0 )
            m++; 

        PWM_init();
        
        g_jump = (TABLE_DEPTH / m);
        g_number_of_microsteps = m;
        // change number of steps
        g_total_number_of_steps = g_motor_steps_per_rev * g_number_of_microsteps;
    }
}
#endif

/**--------------------------------------------------------------------
    Sets the speed in revs per minute
    Regis Blanchot (2013)

    * delay between 2 steps (1 mn = 60 * 1000 * 1000 us)
    g_delay_us_per_step = 60 * 1000 * 1000 / ( revolutionPerMinute * g_total_number_of_steps);
    * CPU cycles in 1 us
    cycles_per_us = GetPeripheralClock() / 1000 / 1000; // 20 cy/us @ 20 MHz
    * number of CPU cycles between 2 steps
    cycles_per_step   = g_delay_us_per_step * cycles_per_us;
    cycles_per_step   = 60  * GetPeripheralClock() / ( revolutionPerMinute * g_total_number_of_steps);
    -----------------------------------------------------------------**/

void Stepper_setSpeed(int revolutionPerMinute)
{
    int steps_per_minute;
    int cycles_per_step;                // CPU cycles between 2 steps
    int prescaler = 0;                  // 0 = 1:1   default prescale value
                                        // 1 = 1:2   prescale value
                                        // 2 = 1:4   prescale value
                                        // 3 = 1:8   prescale value
                                        // 4 = 1:16  prescale value
                                        // 5 = 1:32  prescale value
                                        // 6 = 1:64  prescale value
                                        // 7 = 1:256 prescale value

    steps_per_minute = revolutionPerMinute * g_total_number_of_steps;

    // number of CPU cycles between 2 steps
    cycles_per_step = 60  * GetPeripheralClock() / steps_per_minute;
    
    // delay between 2 steps (1 mn = 60 * 1000 * 1000 us)
    g_delay_us_per_step = 60 * 1000 * 1000 / steps_per_minute;
    
    // Timer 4 prescaler
    while (cycles_per_step > 0xFFFF)
    {
        prescaler += 1;
        if (prescaler == 7)
            cycles_per_step >>= 2; // divides par 4
        else
            cycles_per_step >>= 1; // divides per 2
    }

    // Timer4 Configuration
    IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    T4CON    = prescaler << 4;          // prescaler (bit <6:4>), internal peripheral clock, 16-bit
    TMR4     = 0x00;                    // clear timer register
    PR4      = cycles_per_step;         // load period register
    IntSetVectorPriority(INT_TIMER4_VECTOR, 7, 3);
    IntClearFlag(INT_TIMER4);
    IntEnable(INT_TIMER4);

    T4CONSET = 0x8000;                  // start timer 1
}

/**--------------------------------------------------------------------
    Moves the motor of steps_to_move steps.
    If the number is negative, the motor moves in the reverse direction.
    Wait until the whole move is complete.
    -----------------------------------------------------------------**/

void Stepper_step(int steps_to_move)
{
    // wait while previous command is not finished
    while (g_steps_left > 0);
    
    if (steps_to_move == 0)
        return;

    if (steps_to_move > 0)
    {
        // clockwise rotation
        g_direction = 1;
        // steps to take
        g_steps_left = steps_to_move;
    }
    
    else // if (steps_to_move < 0)
    {
        // anticlockwise rotation
        g_direction  = -1;
        // steps to take
        g_steps_left = -steps_to_move;
    }

    // delay (us) for the whole move
    Delayus(g_delay_us_per_step * g_steps_left);
}

/**--------------------------------------------------------------------
    Moves the motor forward or backwards.
    Private function.
    Microstepping by Regis Blanchot (2013)
    -----------------------------------------------------------------**/

void Stepper_stepMotor(int current_step)
{
    int t;

    #ifdef __MICROSTEPPING__
    int current_microstep;
    int index, sine, cosine;
    #endif

    // wave-step
    if (g_number_of_microsteps == 0)
    {
        // 0, 1, 2, or 3 if current_step > 0
        // 0, 3, 2, or 1 if current_step < 0
        t = wave_table [ current_step % 4 ];

        // Winding A
        digitalwrite( g_motor_pin_A, t >> 0 & 1); // bit 0
        digitalwrite( g_motor_pin_B, t >> 1 & 1); // bit 1

        // Winding B
        if (g_4pin)
        {
            digitalwrite( g_motor_pin_C, t >> 2 & 1); // bit 2
            digitalwrite( g_motor_pin_D, t >> 3 & 1); // bit 3
        }
    }

    // full-step
    else if (g_number_of_microsteps == 1)
    {
        // 0, 1, 2, or 3 if current_step > 0
        // 0, 3, 2, or 1 if current_step < 0
        t = steps_table [ 2 * (current_step % 4) ];

        // Winding A
        digitalwrite( g_motor_pin_A, t >> 0 & 1); // bit 0
        digitalwrite( g_motor_pin_B, t >> 1 & 1); // bit 1

        // Winding B
        if (g_4pin)
        {
            digitalwrite( g_motor_pin_C, t >> 2 & 1); // bit 2
            digitalwrite( g_motor_pin_D, t >> 3 & 1); // bit 3
        }
    }

    // half-step
    else if (g_number_of_microsteps == 2)
    {
        // 0, 1, 2, 3, 4, 5, 6 or 7 if current_step > 0
        // 0, 7, 6, 5, 4, 3, 2 or 1 if current_step < 0
        t = steps_table [ current_step % 8 ];

        // Winding A
        digitalwrite( g_motor_pin_A, t >> 0 & 1); // bit 0
        digitalwrite( g_motor_pin_B, t >> 1 & 1); // bit 1

        // Winding B
        if (g_4pin)
        {
            digitalwrite( g_motor_pin_C, t >> 2 & 1); // bit 2
            digitalwrite( g_motor_pin_D, t >> 3 & 1); // bit 3
        }
    }
    
    // microstepping (4, 8, 16 or 32 microsteps / step)
    #ifdef __MICROSTEPPING__
    else if (g_number_of_microsteps > 2)
    {
        // get the current microstep (from 0 to total number of microsteps - 1)
        current_microstep = ( current_step % g_number_of_microsteps );
        
        // table index is function of total microsteps number
        // g_jump = (TABLE_DEPTH / g_number_of_microsteps);
        index = ( current_microstep * g_jump ) & INDEX_MASK;

        // sine from 0 to 90 degrees (from 0 to 1023)
        // increase current in next active coil
        sine = sine_table[ index ]; 

        // cosine from 0 to 90 degrees is the same as
        // sine from 90 to 180 degrees is the same as
        // sine from 90 to 0 degrees (from 1023 to 0)
        // decrease current in active coil
        cosine = sine_table[ INDEX_MASK - index ];

        /**
         * In practice, the current in one winding is kept
         * constant over half of the complete step and current
         * in the other winding is varied as a function of
         * sinθ to maximize the motor torque.
         * 
         *              A
         *              |
         *          D---+---C
         *              |
         *              B
        **/

        // determines the rotor position
        switch ( (current_step / g_number_of_microsteps) % 4 )
        {
            case 0: // from A to C
                analogwrite( g_motor_pin_A, cosine );
                analogwrite( g_motor_pin_B, 0 );
                analogwrite( g_motor_pin_C, sine );
                analogwrite( g_motor_pin_D, 0 );
                break;

            case 1: // from C to B
                analogwrite( g_motor_pin_A, 0 );
                analogwrite( g_motor_pin_B, sine );
                analogwrite( g_motor_pin_C, cosine );
                analogwrite( g_motor_pin_D, 0 );
                break;

            case 2: // from B to D
                analogwrite( g_motor_pin_A, 0 );
                analogwrite( g_motor_pin_B, cosine );
                analogwrite( g_motor_pin_C, 0 );
                analogwrite( g_motor_pin_D, sine );
                break;

            case 3: // from D to A
                analogwrite( g_motor_pin_A, sine );
                analogwrite( g_motor_pin_B, 0 );
                analogwrite( g_motor_pin_C, 0 );
                analogwrite( g_motor_pin_D, cosine );
                break;
        }

    }
    #endif
}

/**---------------------------------------------------------------------
    Interrupt routine
    Regis Blanchot (2013)
    --------------------------------------------------------------------
    millis  uses Timer1
    servos  uses Timer2
    pwm     uses Timer3
    stepper uses Timer4
    dcf77   uses Timer1
    ------------------------------------------------------------------**/
#if 0
// Put the ISR_wrapper in the good place
void MIPS32 ISR_wrapper_vector_16(void) __attribute__ ((section (".vector_16")));

// servo_interrupt is declared as an interrupt routine
void MIPS32 stepper_interrupt(void) __attribute__ ((interrupt));

// ISR_wrapper will call the servo_interrupt()
void MIPS32 ISR_wrapper_vector_16(void)
{
    stepper_interrupt();
}
#endif

//void servo_interrupt()
//void MIPS32 stepper_interrupt(void)
void Timer4Interrupt(void)
{
    static volatile int current_step=0;
    
    // is this a TMR4 interrupt ?
    if (IntGetFlag(INT_TIMER4))
    {
        if (g_steps_left > 0)
        {
            // increment or decrement the step number
            current_step += g_direction;

            // keep the current step in the right range and step the motor
            Stepper_stepMotor(current_step % g_total_number_of_steps);

            // decrement the steps left:
            g_steps_left -= 1;
        }
        
        // enable interrupt again
        IntClearFlag(INT_TIMER4);
    }
}

#endif
