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

// TMR1 Overflow Interrupt Function
void Timer1Interrupt(void)
{
    if (Int.getFlag(INT_TIMER1)) // Timer Interrupt flag
    {
        // Clear the timer interrupt flag
        Int.clearFlag(INT_TIMER1);
        CounterO++;		   // increment the CounterO
    }
}

// TMR4 Overflow Interrupt Function
void Timer4Interrupt(void)
{
    TmpCounterRes = TMR1;
    TmpCounterO = CounterO;
    if (Int.getFlag(INT_TIMER4))	// Timer Interrupt flag
    {
        // Clear the timer interrupt flag
        Int.clearFlag(INT_TIMER4);
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

void setup()
{
    System.setCpuFrequency(80000000); // should be 80 MHz
    System.setPeripheralFrequency(80000000); // should be 80 MHz
    Int.configureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR); // interrupt mode 
 
    // TMR1 Init
    
    // reset timer 1 configuration
    T1CON=0;
    // reset timer 1 Counter register
    TMR1=0;
    // define the preload register
    PR1=0x0FFFF;
    // select interrupt priority and sub-priority
    Int.setVectorPriority(INT_TIMER1_VECTOR, 7, 3);
    // Clear the timer interrupt flag
    Int.clearFlag(INT_TIMER1);
    // Enable Timer1 Interrupt
    Int.enable(INT_TIMER1);
    // start timer 1 and set prescaler to 1 internal clock
    T1CONSET=0x8000;
    // decomment to measure external clock on P31 (Conn1.1) I/O pin
    //T1CONSET=0x8002;

    // TMR4 Init Function
    
    // reset timer 4 configuration
    T4CON=0;
    // reset timer 4 counter register
    TMR4=0;
    // define the preload register
    PR4=31250-1;
    // select interrupt priority and sub-priority
    Int.setVectorPriority(INT_TIMER4_VECTOR, 7, 3);
    // Clear the timer interrupt flag
    Int.clearFlag(INT_TIMER4);
    // Enable Timer1 Interrupt
    Int.enable(INT_TIMER4);
    // start timer 4 and set prescaler to 256
    T4CONSET=0x8070;
}

void loop()
{
    Frequency = VCounterO;
    Frequency *= 65536;
    Frequency += VCounterRes;
    Frequency -= PrevRes;
    CDC.printf("Freq= %u Hz , OvF= %d , Cnt= %d , Res=%d\n\r",Frequency,VCounterO,VCounterRes,PrevRes);	
    delay(1000);
}
