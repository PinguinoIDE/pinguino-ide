// Using interrupt with Pinguino32
// Jean-Pierre MANDON 2011
// 12/06/2013 Moreno Manzini Use of __attribute__ ((nomips16)) for Interrupt call
// 23/03/2015 Regis Blanchot Simplified the code with some Pinguino's function

u8 counter=0;

// define here the code to execute when an interrupt occure
void Timer1Interrupt(void) 
{
    // Check Timer1 Interrupt flag
    if (Int.getFlag(INT_TIMER1))
    {
        // Clear the timer interrupt flag
        Int.clearFlag(INT_TIMER1);
        TMR1=0;		// reset the timer register
        counter++;		// increment the counter
    }
}

// configure timer 1 
void setup()
{
    // interrupt mode
    Int.configureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    T1CON=0;		// reset timer 1 configuration
    TMR1=0;		// reset timer 1 counter register
    PR1=0x9999;		// define the preload register
    // select interrupt priority and sub-priority
    Int.setVectorPriority(INT_TIMER1_VECTOR, 7, 3);
    // Clear the timer interrupt flag
    Int.clearFlag(INT_TIMER1);
    // Enable Timer1 Interrupt
    Int.enable(INT_TIMER1);
    // start Timer 1 and set prescaler
    T1CONSET=0x8010;
}

void loop()
{
    // counter is incremented at each timer 1 overflow
    CDC.printf("counter = %d\n\r",counter);
    delay(1000);
}
