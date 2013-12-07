// Using interrupt with Pinguino32
// Jean-Pierre MANDON 2011

// 12/06/2013 Moreno Manzini Use of __attribute__ ((nomips16)) for Interrupt call

#include <interrupt.c>

unsigned int counter=0;



//OLD CODE DEFINITION WITHOUT MIPS OPTION
/*
void ISR_wrapper_vector_4(void) __attribute__ ((section (".vector_4")));
// Put the ISR_wrapper in the good place

void ISR_wrapper_vector_4(void) { Tmr1Interrupt(); }
// ISR_wrapper will call the Tmr1Interrupt()

void Tmr1Interrupt(void) __attribute__ ((interrupt));
// Tmr1Interrupt is declared as an interrupt routine

// define here the code to execute when an interrupt occure
void Tmr1Interrupt(void)*/


//NEW CODE DEFINITION WITH MIPS AND __attribute__ ((nomips16))
 
//First Definition
void __attribute__ ((nomips16)) ISR_wrapper_vector_4(void) __attribute__ ((section (".vector_4")));
// Put the ISR_wrapper in the good place

//Second Definition
void __attribute__ ((nomips16))  Tmr1Interrupt(void) __attribute__ ((interrupt));
// Tmr1Interrupt is declared as an interrupt routine

//Third Definition
void __attribute__ ((nomips16)) ISR_wrapper_vector_4(void) { Tmr1Interrupt(); }
// ISR_wrapper will call the Tmr1Interrupt()

// define here the code to execute when an interrupt occure
void  __attribute__ ((nomips16))  Tmr1Interrupt(void) 
{
if (IFS0bits.T1IF)		// Timer Interrupt flag
	{
	TMR1=0;			// reset the timer register
	IFS0CLR=0x10;		// Clear the timer interrupt flag
	counter++;		// increment the counter
	}
}

// configure timer 1 
void init_timer1(void)
{
IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);	// interrupt mode (interrupt.c)
T1CON=0;		// reset timer 1 configuration
TMR1=0;			// reset timer 1 counter register
PR1=0x9999;		// define the preload register
IPC1SET=0x7;		// select interrupt priority and sub-priority
IFS0CLR=0x10;		// clear interrupt flag
IEC0SET=0x10;		// enable timer 1 interrupt
T1CONSET=0x8010;	// start timer 1 and set prescaler
}

void setup()
{
init_timer1();
}

void loop()
{
CDC.printf("%d\n\r",counter);	// counter is incremented at each timer 1 overflow
delay(1000);
}