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

#include <pic18fregs.h>             // PIC registers
#include <typedef.h>                // u8, u16, u32
#include <const.h>                  // INPUT, OUTPUT, USERLED
#include <macro.h>                  // BitRead, high8, low8
#include <digitalp.c>               // pinmode
#include <digitalw.c>               // digitalwrite
#include <oscillator.c>             // System_getPeripheralFrequency
#include <delayms.c>                // Delayms

#ifdef __DEBUG__
    #include <serial.c>             // UART
    #define  SERIALPRINTF           // enable printf function
    volatile u8 flag = 0;
#endif

#ifdef __MICROSTEPPING__
    #if !defined(__18f27j53) && !defined(__18f47j53)
        #error "Microstepping is only available on PIC with at least 4 PWM."
    #else
        #include <analog.c>         // analogwrite
        //#include <pwmclose.c>       // PWM_close
    #endif
#endif

//#include <interrupt.c>              // interrupts routines

#define MAXPRESCALER     8          // max. prescaler
#define MINPRESCALER     1          // min. prescaler
#define MAXCYCLES        0x8000     // max. CPU cycles between 2 steps
#define MINCYCLES        0xFFFF     // max. CPU cycles between 2 steps

u8  this_direction;                 // direction of rotation [0 or 1]
u16 this_steps_per_rev;             // total number of steps this motor can take [0, 800]
u8  this_number_of_microsteps=2;    // how many microsteps in one full step [0, 32]
u16 this_number_of_steps;           // total number of steps for one rev. [0, 32*800=25600]
                                    // Note : this_number_of_steps = this_steps_per_rev * this_number_of_microsteps;
volatile u16 this_current_step;     // which step the motor is on [0, 32*800=25600]
                                    // Note : this_current_step = this_number_of_steps;
volatile u32 this_steps_left=0;     // how many steps left [0, u32 max]
                                    // Note : this_steps_left = steps_to_move > 0 ? steps_to_move : -steps_to_move;
u8  this_pin_count;                 // whether you're driving the motor with [2 or 4] pins
u8  this_jump;                      // set table pointer jump
                                    // Note : 32 -> 1, 16 -> 2, 08 -> 4, 04 -> 8
u32 this_delay_us_per_step;         // delay us for one step

volatile u8 _TMRH_;                 // Timer
volatile u8 _TMRL_;                 // Timer

// motor pin numbers:
u8 this_motor_pin_A;
u8 this_motor_pin_B;
u8 this_motor_pin_C;
u8 this_motor_pin_D;

/***********************************************************************
    Microsteps lookup table
***********************************************************************/

#ifdef __MICROSTEPPING__            // defined if user uses Stepper_setMicrostep()

#define TABLE_DEPTH     32
#define INDEX_MASK      TABLE_DEPTH - 1

// Note : 1 full-step of any stepper motor = 360/4 = 90 deg.
const u16 sine_table[TABLE_DEPTH + 1] =
{
    50,100,150,200,249,297,345,391,
    437,482,526,568,609,649,687,723,
    758,791,822,851,877,902,925,945,
    963,979,992,1003,1012,1018,1022,1023
/*
     25,  50,  75, 100, 124, 149, 172, 196,
    219, 241, 263, 284, 305, 325, 344, 362,
    379, 396, 411, 426, 439, 452, 463, 473,
    482, 490, 497, 502, 506, 510, 511, 512
*/
};  

#endif

// Windings status
const u8 steps_table[8] =
{
    0b1000, 0b1010,
    0b0010, 0b0110,
    0b0100, 0b0101,
    0b0001, 0b1001
};

/*	--------------------------------------------------------------------
    Prototypes
    ------------------------------------------------------------------*/

void Stepper_init(u16, u8, u8, u8, u8);
#ifdef __MICROSTEPPING__
void Stepper_setMicrostep(u8);
#endif
u16 Stepper_getMaxSpeed();
u16 Stepper_getMinSpeed();
void Stepper_setSpeed(u16);
//void Stepper_step(int, int, int);
void Stepper_step(s32);

static void Stepper_oneStep(u8);
static void Stepper_stepMotor(u16);

/**--------------------------------------------------------------------
    Constructor for two and four-pin version
    Sets which wires should control the motor.
    -----------------------------------------------------------------**/

void Stepper_init(u16 number_of_steps, u8 motor_pin_A, u8 motor_pin_B, u8 motor_pin_C, u8 motor_pin_D)
{
    #if defined(__DEBUG__)
        #if defined(__18f26j50) || defined(__18f46j50) || \
            defined(__18f26j53) || defined(__18f46j53) || \
            defined(__18f27j53) || defined(__18f47j53)
        TRISCbits.TRISC2 = OUTPUT;
        #else
        TRISAbits.TRISA4 = OUTPUT;
        #endif
        Serial_begin(115200);
    #endif
    
    if ((motor_pin_C + motor_pin_D) == 0)
    {
        // Pinguino pins for the motor control connection
        this_motor_pin_A = motor_pin_A;
        this_motor_pin_B = motor_pin_B;

        // setup the pins on the microcontroller
        pinmode(this_motor_pin_A, OUTPUT);
        pinmode(this_motor_pin_B, OUTPUT);

        // When there are only 2 pins, set the other two to 0
        this_motor_pin_C = 0;
        this_motor_pin_D = 0;

        // pin_count is used by stepMotor()
        this_pin_count = 2;
    }

    else
    {
        // Pinguino pins for the motor control connection
        this_motor_pin_A = motor_pin_A;
        this_motor_pin_B = motor_pin_B;
        this_motor_pin_C = motor_pin_C;
        this_motor_pin_D = motor_pin_D;

        // setup the pins on the microcontroller
        pinmode(this_motor_pin_A, OUTPUT);
        pinmode(this_motor_pin_B, OUTPUT);
        pinmode(this_motor_pin_C, OUTPUT);
        pinmode(this_motor_pin_D, OUTPUT);

        // pin_count is used by stepMotor()
        this_pin_count = 4;  
    }

    this_steps_per_rev = number_of_steps;// total number of steps for this motor
    this_current_step = 0;              // which step the motor is on
    this_direction = 0;                 // motor direction
    //this_last_step_time = 0;          // time stamp in ms of the last step taken
    this_number_of_microsteps = 1;      // full-steps per default
    this_jump = 1;
    this_steps_left = 0;
    // change number of steps
    this_number_of_steps = this_steps_per_rev * this_number_of_microsteps;
}

/**--------------------------------------------------------------------
    Sets microsteps per full step
    full steps = 1
    half steps = 2
    microsteps = 4, 8, 16 or 32 per full step.
    -----------------------------------------------------------------**/

#ifdef __MICROSTEPPING__
void Stepper_setMicrostep(u8 microsteps)
{
    switch ( microsteps )
    {
        case 0:
            // wave steps
            this_number_of_microsteps = 1;
            break;

        case 1:
            // full-steps
            this_number_of_microsteps = 1;
            break;
    
        default:
            if (microsteps % 2) // odd number ?
                this_number_of_microsteps = microsteps + 1; 
            else
                this_number_of_microsteps = microsteps;

            if (this_number_of_microsteps > 32)
                this_number_of_microsteps = 32;

            //if (this_number_of_microsteps < 2)
            //    this_number_of_microsteps = 2;
            break;
    }

    // microsteps ?
    if (this_number_of_microsteps > 2)
    {
        //PWM_init(); // Timer3
        //PWM_set_frequency(64000);
        analogwrite_init();
        //PWM_setFrequency(100000); // 100KHz
        this_jump = (TABLE_DEPTH / this_number_of_microsteps);
    }
/*    
    else
    {
        PWM_close(this_motor_pin_A);
        PWM_close(this_motor_pin_B);
        PWM_close(this_motor_pin_C);
        PWM_close(this_motor_pin_D);
    }    
*/
    // change number of steps
    this_number_of_steps = this_steps_per_rev * this_number_of_microsteps;
}
#endif

/** --------------------------------------------------------------------
    Returns the max. speed possible (rev. per minute)
    Max. speed when time between 2 steps is minimal : 
    * prescaler = MINPRESCALER
    * cycles = MINCYCLES
    -----------------------------------------------------------------**/
u16 Stepper_getMaxSpeed()
{
    return (60 * System_getPeripheralFrequency() / this_number_of_steps / MINCYCLES / MINPRESCALER);
}

/** --------------------------------------------------------------------
    Returns the min. speed possible (rev. per minute)
    Min. speed when time between 2 steps is maximal : 
    * prescaler = MAXPRESCALER
    * cycles = MAXCYCLES
    -----------------------------------------------------------------**/
u16 Stepper_getMinSpeed()
{
    return (60 * System_getPeripheralFrequency() / this_number_of_steps / MAXCYCLES / MAXPRESCALER);
}

/**--------------------------------------------------------------------
    Sets the speed in revs per minute
    * ex1 : Stepper.setSpeed(60);
    * 60 revs <=> 1 min.
    * 1 rev.  <=> 1 sec. = 1000000 us
    * if Stepper = 200 steps per revolution
    * 1 step <=> 1000000/200 us = 5000 us
    * if Fosc = 48MHz, then Fcpu = Fosc / 4 = 12 MHz = 12 000 000 Hz
    * 1 cpu cycle = 1 / 12 000 000 sec. = 1/12 us
    * 1 step <=> (5000 * 12) cycles = 60000 cycles
    * Interrupt must be triggered every 60000 cycles

    * ex2 : Stepper.setSpeed(10);
    * 10 revs <=> 1 min.
    * 1 rev.  <=> 10 sec. = 10000000 us
    * if Stepper = 48 steps per revolution
    * 1 step <=> 10000000/48 us = 208333 us
    * if Fosc = 48MHz, then Fcpu = Fosc / 4 = 12 MHz = 12 000 000 Hz
    * 1 cpu cycle = 1 / 12 000 000 sec. = 1/12 us
    * 1 step <=> (208333 * 12) cycles = 2500000 cycles
    * Interrupt must be triggered every 2500000 cycles
    -----------------------------------------------------------------**/

void Stepper_setSpeed(u16 revolutionPerMinute)
{
    //u32 delay_us_per_step;              // delay between 2 steps
    u32 nb_cycles_per_us;               // number of cycles per us
    u32 cycles_per_step;                // CPU cycles between 2 steps
    u8  prescaler = 0;
    
    // 0 = 00 = 1:1 Prescale value
    // 1 = 01 = 1:2 Prescale value
    // 2 = 10 = 1:4 Prescale value
    // 3 = 11 = 1:8 Prescale value

    // number of cycles during 1 us
    nb_cycles_per_us = System_getPeripheralFrequency() / 1000000UL;

    // delay between 2 steps (1 mn = 60 * 1000 * 1000 us)
    this_delay_us_per_step = 60000000UL / revolutionPerMinute / this_number_of_steps;

    // number of CPU cycles between 2 steps
    cycles_per_step = this_delay_us_per_step * nb_cycles_per_us;
    
    while ( (cycles_per_step > 0xFFFF) && (prescaler < 4) )
    {
        cycles_per_step >>= 1; // /= 2;
        prescaler = prescaler + 1;
    }

    if (cycles_per_step >= 0xFFFF)
    {
        _TMRH_ = 0;
        _TMRL_ = 0;
    }
    else
    {
        _TMRH_ = highByte(0xFFFF - cycles_per_step);
        _TMRL_ =  lowByte(0xFFFF - cycles_per_step);
    }
    
    // stops interrupt
    INTCONbits.GIEH = 0;    // disable global HP interrupts
    INTCONbits.GIEL = 0;    // disable global LP interrupts

    // config. interrut
    #if defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f25k50) || defined(__18f45k50)
    T3GCONbits.TMR3GE = 0;  // disable timer gate
    #endif
    IPR2bits.TMR3IP = 1;    // interrupt has high priority
    PIR2bits.TMR3IF = 0;    // reset interrupt flag

    TMR3H = _TMRH_;         // load timer registers
    TMR3L = _TMRL_;

    T3CON = 0;
    T3CONbits.TMR3CS = 0;   // clock source is Fosc/4
    T3CONbits.T3CKPS = prescaler;// << 4; // bit 5-4 TxCKPS = prescaler
    T3CONbits.RD16 = 0;     // enables register r/w in two 8-bit operation

    // starts interrupt
    T3CONbits.TMR3ON = 1;   // Timer3 ON;
    PIE2bits.TMR3IE = 1;    // enable interrupt 
    INTCONbits.GIEL = 1;    // enable global LP interrupts
    INTCONbits.GIEH = 1;    // enable global HP interrupts
}

/**--------------------------------------------------------------------
    Moves the motor of steps_to_move steps.
    If the number is negative, the motor moves in the reverse direction.
    -----------------------------------------------------------------**/

//int Stepper_step(int steps_to_move, int microsteps, int revolutionPerMinute)
void Stepper_step(s32 steps_to_move)
{
    u32 delay_ms_per_move;
    
    // wait until previous command is finished
    while (this_steps_left > 0);

    // determine direction based on whether steps_to_move is + or -:
    if (steps_to_move > 0) this_direction = 1;
    if (steps_to_move < 0) this_direction = 0;

    // how many steps to take ?
    this_steps_left = steps_to_move > 0 ? steps_to_move : -steps_to_move;

    // wait until all steps have been processed
    delay_ms_per_move = ( this_steps_left * this_delay_us_per_step ) / 1040;
    Delayms(delay_ms_per_move);
}

/**--------------------------------------------------------------------
    Moves the motor forward or backwards.
    Private function.
    -----------------------------------------------------------------**/

static void Stepper_oneStep(u8 index)
{
    // Winding A (coil 1)
    digitalwrite(this_motor_pin_A, BitRead(index, 0)); // bit 0
    digitalwrite(this_motor_pin_B, BitRead(index, 1)); // bit 1

    // Winding B (coil 2)
    if (this_pin_count == 4)
    {
        digitalwrite(this_motor_pin_C, BitRead(index, 2)); // bit 2
        digitalwrite(this_motor_pin_D, BitRead(index, 3)); // bit 3
    }
}

/**--------------------------------------------------------------------
    Moves the motor forward or backwards.
    Private function.
    -----------------------------------------------------------------**/

static void Stepper_stepMotor(u16 thisStep)
{
    u8  current_microstep;
    u8  i;

    #ifdef __MICROSTEPPING__
    u16 inc, dec;
    #endif
    
    // wave-step
    if (this_number_of_microsteps == 0)
    {
        // step the motor to step number 0, 2, 4, or 6:
        i = steps_table [ (thisStep % 4) * 2 ];
        Stepper_oneStep(i);
    }

    // full-step
    else if (this_number_of_microsteps == 1)
    {
        // step the motor to step number 0, 2, 4, or 6:
        i = steps_table [ (thisStep % 4) * 2 ];
        Stepper_oneStep(i);
    }

    // half-step
    else if (this_number_of_microsteps == 2)
    {
        // step the motor to step number 0, 1, 2, 3, 4, 5, 6 or 7
        i = steps_table [ thisStep % 8 ];
        Stepper_oneStep(i);
    }
    
    // microstepping (4, 8, 16 or 32 microsteps / step)
    #ifdef __MICROSTEPPING__
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
        i = (current_microstep * this_jump) & INDEX_MASK;

        // from 0 to 90 degrees (from 0 to 1023)
        // increase current in next active coil
        inc = sine_table[i]; 

        // from 90 to 0 degrees (from 1023 to 0)
        // decrease current in active coil
        dec = sine_table[INDEX_MASK - i];

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
    #endif
}

/*  --------------------------------------------------------------------
    stepper_interrupt
    called from main.c
    --------------------------------------------------------------------
    millis  uses Timer0
    servos  uses Timer1
    pwm     uses Timer2
    dcf77   uses Timer3
    stepper uses Timer3
    ------------------------------------------------------------------*/

void stepper_interrupt()
{
    // check if the timer has overflowed
    if (PIR2bits.TMR3IF)
    {
        PIR2bits.TMR3IF = 0;
        // load the timer again for the next interrupt
        TMR3H = _TMRH_;
        TMR3L = _TMRL_;

        #ifdef __DEBUG__
        flag=flag+1;
        if (flag==100)
        {
            flag=0;
            #if defined(__18f26j50) || defined(__18f46j50) || \
                defined(__18f26j53) || defined(__18f46j53) || \
                defined(__18f27j53) || defined(__18f47j53)
            LATCbits.LATC2 ^= 1;
            #else
            LATAbits.LATA4 ^= 1;
            #endif
        }
        #endif
        
        // If there are still steps to move
        if (this_steps_left) // > 0
        {
            // increment the step number if clockwise motion
            if (this_direction) // == 1
            {
                this_current_step += 1;
                if (this_current_step == this_number_of_steps)
                    this_current_step = 0;
            } 

            // decrement the step number if anticlockwise motion
            else
            {
                // test if null first cause this_current_step is unsigned
                if (this_current_step == 0)
                    this_current_step = this_number_of_steps;
                this_current_step -= 1;
            }
            
            // step the motor
            //Stepper_oneStep(steps_table [ (this_current_step % 4) * 2 ]);
            Stepper_stepMotor(this_current_step);

            // One down so decrement the steps left
            this_steps_left -= 1;
        }
    }
}

#endif
