/*----------------------------------------------------- 
Author:  Moreno Manzini
Date: 12/06/2013
Description:
High Accuracy Period Meter with a 12.5nS resolution.
Measure error on a 40mS signal about +-25nS

12/06/2013 Ver. 1 Rev. 1 c. Moreno Manzini ( moreno at mediacom dot it )

Licence Creative Commons 3.0 http://creativecommons.org/licenses/by-sa/3.0/
  
Interrupt routines inspired to http://blog.pinguino.cc/?p=225

-----------------------------------------------------*/

//****** Comment the line to obtain a REAL Period Meter *****
#define Test40mSPeriodTMR4          1
//****** Comment the line to obtain a REAL Period Meter *****


#define ledPinG			32 // the number of the Green LED pin
#define Clk     		18 // INT1 Period Clock

#define ClkPeriodFor10  125     //TMR1 Clock period nS * 10 



#include <interrupt.c>

/*
Useful INTx pin
                            Vector  INTCON  IFS0/IEC0 
                                    EP bit  hex val
INT0    RF6     BUT(2.3)*   3       0       0x8
INT1    RD8     P18(1.14)   7       1       0x80
INT2    RD9     SDA**       11      2       0x800
INT3    RD10    SCL**       15      3       0x8000
INT4    RD11    P19(1.13)   19      4       0x80000

* INT0 pin was connected to pin 1.13 through jumper BUT_J (Normally closed).
Is possible use, BUT, pin 1.13 or RD4 (P13) as source of signal to generate the interrupt.
ATTENTION!! THE BUT I/O HAD A 100nF CONDENSATOR CONNECTED ON ITS PIN, THIS ACTS AS A LOW PASS FILTER.

** These pin are used on UEXT connector, is better use them only in case of real necessity.
*/

long unsigned int CounterO=0;
unsigned int TmpCounterRes=0;
long unsigned int TmpCounterO=0;
BOOL DataValid = false;

long unsigned int ActStart,PrevStart,Period;

 

// Put the ISR_wrapper in the good place
void __attribute__ ((nomips16)) ISR_wrapper_vector_4(void) __attribute__ ((section (".vector_4")));

// TMR1 Overflow Interrupt Function
void __attribute__ ((nomips16)) Tmr1Interrupt(void) __attribute__ ((interrupt));

// ISR_wrapper will call the Tmr1Interrupt()
void __attribute__ ((nomips16)) ISR_wrapper_vector_4(void) { Tmr1Interrupt(); }


#ifdef Test40mSPeriodTMR4
// Put the ISR_wrapper in the good place
void __attribute__ ((nomips16)) ISR_wrapper_vector_16(void) __attribute__ ((section (".vector_16")));

// TMR4 Overflow Interrupt Function
void __attribute__ ((nomips16)) Tmr4Interrupt(void) __attribute__ ((interrupt));

// ISR_wrapper will call the Tmr4Interrupt()
void __attribute__ ((nomips16)) ISR_wrapper_vector_16(void) { Tmr4Interrupt(); }
#endif


// Put the ISR_wrapper in the good place
void __attribute__ ((nomips16)) ISR_wrapper_vector_7(void) __attribute__ ((section (".vector_7")));

// INT1 transition
void __attribute__ ((nomips16)) Int1Interrupt(void) __attribute__ ((interrupt));

// ISR_wrapper will call the Int1Interrupt()
void __attribute__ ((nomips16)) ISR_wrapper_vector_7(void) { Int1Interrupt(); }





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
void InitTmr1(void)
{
 IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);	// interrupt mode (interrupt.c)
 T1CON=0;		       // reset timer 1 configuration
 TMR1=0;			     // reset timer 1 Counter register (VERY DANGEROUS OPERATION USE ONLY AT INIT TIME)
 PR1=0x0FFFF;		  // define the preload register
 IPC1SET=0x7;		  // select interrupt priority and sub-priority
 IFS0CLR=0x10;		  // clear interrupt flag
 IEC0SET=0x10;		  // enable timer 1 interrupt
 T1CONSET=0x8000;	// start timer 1 and set prescaler to 1 internal clock
}



#ifdef Test40mSPeriodTMR4
// TMR4 Overflow Interrupt Function
void __attribute__ ((nomips16)) Tmr4Interrupt(void)
{
 if (IFS0bits.T4IF)		// Timer Interrupt flag
  {   IFS0CLR=0x10000;		// Clear the timer interrupt flag
   toggle(ledPinG);
   toggle(Clk);
  }
}



// TMR4 Init Function
void InitTmr4(void)
{
 IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);	// interrupt mode (interrupt.c)
 T4CON=0;		            // reset timer 4 configuration
 TMR4=0;			            // reset timer 4 Counter register (VERY DANGEROUS OPERATION USE ONLY AT INIT TIME)
 PR4=6250-1;		         // define the preload register
 IPC4SET=0x7;		         // select interrupt priority and sub-priority
 IFS0CLR=0x10000;		    // clear interrupt flag
 IEC0SET=0x10000;		    // enable timer 4 interrupt
 T4CONSET=0x8070;	      // start timer 4 and set prescaler to 256
}
#endif 
 
 


// INT1 Transition Interrupt Function
void __attribute__ ((nomips16)) Int1Interrupt(void)
{
 TmpCounterRes = TMR1;
 TmpCounterO = CounterO;
 if (IFS0bits.INT1IF)		// Timer Interrupt flag
  {
   IFS0bits.INT1IF=0;      // clear interrupt flag
   PrevStart = ActStart;
   ActStart = TmpCounterO;
   ActStart *= 65536;
   ActStart += TmpCounterRes;
   Period = ActStart - PrevStart;
   Period *= ClkPeriodFor10;
   DataValid = true;
  }
}



// INT1 Init Function
void InitInt1(void)
{
 IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);	// interrupt mode (interrupt.c)
 INTCONbits.INT1EP=0x1;   //Interrupt on Rising Edge
 IPC1bits.INT1IP=0x4;		  // select interrupt priority 
 IPC1bits.INT1IS=0x3;		  // select interrupt sub-priority
 IFS0bits.INT1IF=0;       // clear interrupt flag
 IEC0bits.INT1IE=1;       // enable INT1 interrupt
}



void setup()
{
 System.setCpuFrequency(80000000); // should be 80 MHz
 System.setPeripheralFrequency(80000000); // should be 80 MHz
 
#ifdef Test40mSPeriodTMR4
 pinMode(ledPinG, OUTPUT); 
 digitalWrite(ledPinG,HIGH); 
 pinMode(Clk, OUTPUT); 
 digitalWrite(Clk,HIGH); 
#endif


 InitTmr1();
#ifdef Test40mSPeriodTMR4
 InitTmr4();
#endif
 InitInt1();
}



void loop()
{
 if (DataValid)
  {
   DataValid = false;
   CDC.printf("Period= %u.%u nS*10\n\r",Period/10,Period%10);	
  }
}