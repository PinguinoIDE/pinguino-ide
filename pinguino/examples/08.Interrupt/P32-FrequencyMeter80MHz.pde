/*----------------------------------------------------- 
Author:  Moreno Manzini
Date: 12/06/2013
Description:
Easy frequency meter which measure Peripheral clock fixed at 80MHz with Pinguino32
For measure external clock on P31 (Conn1.1) pin use T1CONSET=0x8002; instead of T1CONSET=0x8000; in init_timer1 function
Max internal error about +- 2 count +- 0,025ppm

12/06/2013 Ver. 1 Rev. 1 c. Moreno Manzini ( moreno at mediacom dot it )

Licence Creative Commons 3.0 http://creativecommons.org/licenses/by-sa/3.0/
  
Interrupt routines inspired to http://blog.pinguino.cc/?p=225

-----------------------------------------------------*/


#include <interrupt.c>




#define NumSample100mS 10 //1Sec total




unsigned int CounterO=0;
unsigned int VCounterRes=0;
unsigned int TmpCounterRes=0;
unsigned int VCounterO=0;
unsigned int TmpCounterO=0;
unsigned int CtnPulse100mS=0;
unsigned int PrevRes=0;
unsigned int ActRes=0;

DWORD   Frequency;



// Put the ISR_wrapper in the good place
void __attribute__ ((nomips16)) ISR_wrapper_vector_4(void) __attribute__ ((section (".vector_4")));

// TMR1 Overflow Interrupt Function
void __attribute__ ((nomips16)) Tmr1Interrupt(void) __attribute__ ((interrupt));

// ISR_wrapper will call the Tmr1Interrupt()
void __attribute__ ((nomips16)) ISR_wrapper_vector_4(void) { Tmr1Interrupt(); }


// Put the ISR_wrapper in the good place
void __attribute__ ((nomips16)) ISR_wrapper_vector_16(void) __attribute__ ((section (".vector_16")));

// TMR1 Overflow Interrupt Function
void __attribute__ ((nomips16)) Tmr4Interrupt(void) __attribute__ ((interrupt));

// ISR_wrapper will call the Tmr1Interrupt()
void __attribute__ ((nomips16)) ISR_wrapper_vector_16(void) { Tmr4Interrupt(); }



// TMR1 Overflow Interrupt Function
void __attribute__ ((nomips16)) Tmr1Interrupt(void)
{
 if (IFS0bits.T1IF)		// Timer Interrupt flag
	{
	 IFS0CLR=0x10;		// Clear the timer interrupt flag
	 CounterO++;		   // increment the CounterO
	}
}



// TMR1 Init Function
void init_timer1(void)
{
IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);	// interrupt mode (interrupt.c)
T1CON=0;		     // reset timer 1 configuration
TMR1=0;			     // reset timer 1 Counter register (VERY DANGEROUS OPERATION USE ONLY AT INIT TIME)
PR1=0x0FFFF;		  // define the preload register
IPC1SET=0x7;		  // select interrupt priority and sub-priority
IFS0CLR=0x10;		// clear interrupt flag
IEC0SET=0x10;		// enable timer 1 interrupt
T1CONSET=0x8000;	// start timer 1 and set prescaler to 1 internal clock
// decomment to measure external clock on P31 (Conn1.1) I/O pin
//T1CONSET=0x8002;	// start timer 1 and set prescaler to 1 external clock
}



// TMR4 Overflow Interrupt Function
void __attribute__ ((nomips16)) Tmr4Interrupt(void)
{
 TmpCounterRes = TMR1;
 TmpCounterO = CounterO;
if (IFS0bits.T4IF)		// Timer Interrupt flag
	{
	 IFS0CLR=0x10000;		// Clear the timer interrupt flag
	 CtnPulse100mS++;		// increment the CounterO
   if (CtnPulse100mS >= NumSample100mS)
    {
     VCounterRes = TmpCounterRes;
     PrevRes = ActRes;
     ActRes = TmpCounterRes;
     VCounterO = TmpCounterO;
     CounterO = 0;
     CtnPulse100mS = 0;
	  }
  }
}



// TMR4 Init Function
void init_timer4(void)
{
IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);	// interrupt mode (interrupt.c)
T4CON=0;		            // reset timer 4 configuration
TMR4=0;			            // reset timer 4 Counter register (VERY DANGEROUS OPERATION USE ONLY AT INIT TIME)
PR4=31250-1;		         // define the preload register
IPC4SET=0x7;		         // select interrupt priority and sub-priority
IFS0CLR=0x10000;		    // clear interrupt flag
IEC0SET=0x10000;		    // enable timer 4 interrupt
T4CONSET=0x8070;	      // start timer 4 and set prescaler to 256
}



void setup()
{
System.setCpuFrequency(80000000); // should be 80 MHz
System.setPeripheralFrequency(80000000); // should be 80 MHz
 
init_timer1();
init_timer4();
}



void loop()
{
 Frequency = VCounterO;
 Frequency *= 65536;
 Frequency += VCounterRes;
 Frequency -= PrevRes;
 CDC.printf("Freq= %u Hz , OvF= %d , Cnt= %d , Res= %d\n\r",Frequency,VCounterO,VCounterRes,PrevRes);	
 delay(1000);
}