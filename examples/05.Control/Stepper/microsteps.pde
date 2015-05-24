/* 
	Stepper Motor Control - one revolution

	This program drives a unipolar or bipolar stepper motor. 
	The motor can be connected to any digital pins of the Pinguino.
 
  However, if microsteps are used and greater or equal to 4,
  the motor must connected to PWM pins.

	The motor should revolve one revolution in one direction, then
	one revolution in the other direction with different microsteps
  resolution.  

  2011-2014	Régis Blanchot
*/

// change this to fit the number of steps per revolution for your motor
const u16 stepsPerRevolution  = 48;
// number of microsteps per steps
const u16 microstepsPerStep[]   = {0, 1, 2, 4, 8, 16, 32};
// change this to fit the number of revolution per minute you want
const u16 revolutionPerMinute = 1;

void setup()
{
    pinMode(USERLED, OUTPUT);    

    Stepper.init(stepsPerRevolution, 4, 5, 6, 7);   // CCP4 to CCP7            
}

void loop()
{
    u8 i;
    u16 s, ms, rev;
    
    for (i=0; i<6; i++)
    {
        ms = microstepsPerStep[i];
        CDC.printf("microsteps = %d\r\n", ms);
        Stepper.setMicrostep(ms);
        Stepper.setSpeed(revolutionPerMinute);

        rev = stepsPerRevolution * ms;
        
        CDC.printf("\tclockwise rotation\r\n");
        for (s=0; s<rev; s++)
            Stepper.step(1);
        delay(1000);
        toggle(USERLED);
        
        CDC.printf("\tcounter-clockwise rotation\r\n");
        for (s=0; s<rev; s++)
            Stepper.step(-1);
        delay(1000);
        toggle(USERLED);
    }
}
