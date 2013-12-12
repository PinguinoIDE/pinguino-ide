/*	----------------------------------------------------------------------------
	FILE:			dcf77.c
	PROJECT:		pinguino
	PURPOSE:		Free running Clock synchronized by a DCF77 module
	PROGRAMMER:		Henk van Beek hgmvanbeek@gmail.com
	FIRST RELEASE:	20 mar. 2012
	LAST RELEASE:	20 apr. 2012
	----------------------------------------------------------------------------
	TODO :
	* load RTCC with DCF77 Time and Date
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
	----------------------------------------------------------------------------
	Based on a Timer1 ISR which interrupts every 20 ms a clock/date record
	"RTClock" is keeping time, day of the week and date.
	When for a full minute a signal is received from the dcf77 module,
	without errors, kept in a identical record "DCF77", the record "RTClock"
	is updated.
	The pulsetrain of 59 bits is buffered into Buff1 and Buff2.
	--------------------------------------------------------------------------*/

#ifndef _DCF77_C_
 #define _DCF77_C_

#include <typedef.h>			// u8, u16, ...
#include <digitalw.c>			// pinMode, ...
#ifdef DEBUG					// NB: Turn debugging on or off from the IDE
 #define UART1DEBUG
 #include <debug.c>
#endif
#include <interrupt.c>			// interrupts routines
#include <dcf77.h>
#include <bcd.c>

/*******************************************************************************
* if a puls is longer than 150 ms it is a "1", otherwise it is a "0".
*******************************************************************************/

#define DCF_Discr_150ms (150 / 20)

/*******************************************************************************
 * if during 1500 ms no puls has arrived means a start of a new pulstrain.
*******************************************************************************/

#define DCF_Sync_1500ms (1500 / 20)

/*******************************************************************************
* Global variables
*******************************************************************************/

u8  SignalPin, PrevSignal, BuffPntr, Cntr_sec, Nosync;
u16 PrevFlank;
u32 Buff1, Buff2;
u32 Cntr_20ms;

/*******************************************************************************
 * Appends a received bit at the DCF code chain
 ******************************************************************************/
void DCF77_appendSignal(u8 signal)
{
//	serial1printf("BuffPntr: %02d  ", BuffPntr);
	if ((signal > 0) && (BuffPntr < 32))
		Buff1 = Buff1 | (1 << BuffPntr);
	if ((signal > 0) && (BuffPntr > 31))
		Buff2 = Buff2 | (1 << (BuffPntr - 32));
//	serial1printf("Append Buff: %08x %08x  \r\n", DCF77.Buff1, DCF77.Buff2);
	BuffPntr++;
	if (BuffPntr > 59)
	{
		BuffPntr = 0;
		Buff1 	 = 0;
		Buff2 	 = 0;
	}
}

/*******************************************************************************
* Calculate even parity
*******************************************************************************/
u8 Parity_Even (u32 Bits)
{
  u8 i;
  u8 p = 0;
	for (i = 0; i < 32; i++)
		if ((Bits & (1 << i)) > 0)
			p = !p;
	return p;
}

/*******************************************************************************
 * Decodes DCF code after the 59 minute gap
 ******************************************************************************/
void Decode_Buffer(void)
{
  u32 Code;
  u8  Par, Par1, Par2, Par3;
//	Disp_Input_Binary(Buff1, Buff2);

	if (BuffPntr == 59)
	{
		// Minutes
		Code = (Buff1 >> 21) & 0x7F;
		Par  = Parity_Even(Code);
		Par1 = (Buff1 >> 28) & 1;
//		serial1printf("Par= %d Par1= %d\r\n", Par, Par1);
		if (Par == Par1)
		{
			Par1 = 11;
			DCF77.minutes = bcd2bin(Code);
//			serial1printf("mm= %02d \r\n", DCF77.mm);
		}

		// Hours
		Code = ((Buff1 >> 29) & 0x7) | ((Buff2 & 0x7) << 3);
		Par = Parity_Even(Code);
		Par2 = (Buff2 >> 3) & 1;
//		serial1printf("Par= %d Par2= %d\r\n", Par, Par2);
		if (Par == Par2)
		{
			Par2 = 12;
			DCF77.hours = bcd2bin(Code);
//			serial1printf("hh= %02d \r\n", DCF77.hh);
		}

		Code = (Buff2 >> 4) & 0x3FFFFF;
		Par = Parity_Even(Code);
		Par3 = (Buff2 >> 26) & 1;
//		serial1printf("Par= %d Par3= %d\r\n", Par, Par3);
		if (Par == Par3)
		{
			Par3 = 13;
			Code = (Buff2 >> 10) & 0x7;
			DCF77.dayofweek = bcd2bin(Code);
//			serial1printf("dw= %02d \r\n", DCF77.dw);

			Code = (Buff2 >> 4) & 0x3F;
			DCF77.dayofmonth = bcd2bin(Code);
//			serial1printf("da= %02d \r\n", DCF77.da);

			Code = (Buff2 >> 13) & 0x1F;
			DCF77.month = bcd2bin(Code);
//			serial1printf("mo= %02d \r\n", DCF77.mo);

			Code = (Buff2 >> 18) & 0xFF;
			DCF77.year = bcd2bin(Code);
//			serial1printf("yr= %02d \r\n", DCF77.yr);
		}
//		Part = Par1 + Par2 + Par3;
//		serial1printf("Part= %d\r\n", Part);
		if (Par1 + Par2 + Par3 == 36)
		{
			DCF77.seconds = 0;
			Nosync   = 0;
//			serial1printf("Tim=  %d:%02d \r\n", DCF77.hh, DCF77.mm);
//			serial1printf("Dat=: %02d-%02d-%02d  %1d\r\n", DCF77.da, DCF77.mo, DCF77.yr, DCF77.dw);
			RTClock = DCF77;
		}
	}
	// Reset Inputbuffer
	BuffPntr    = 0;
	Buff1 		= 0;
	Buff2 		= 0;
}

/*******************************************************************************
 * Called every 20 ms by ISR
 * Builds DCF code by calling DCF77_appendsignal
 ******************************************************************************/
void DCF77_scanSignal(void)
{
  u16 LengthPulse;
  u16 CurrFlank;
  u8  CurrSignal ;
    CurrSignal = digitalread(SignalPin);
	if (CurrSignal != PrevSignal)
	{
		if (CurrSignal == 1)
		{
//			digitalwrite(GREENLED, HIGH);
			/* At a raising flank */
			CurrFlank = Cntr_20ms;
			if ((CurrFlank - PrevFlank) > DCF_Sync_1500ms)
				Decode_Buffer();
			PrevFlank = CurrFlank;
		}
		else
		{
//			digitalwrite(GREENLED, LOW);
			/* At a falling flank */
			LengthPulse = Cntr_20ms - PrevFlank;
			if (LengthPulse < DCF_Discr_150ms)
				DCF77_appendSignal(0);
			else
				DCF77_appendSignal(1);
		}
		PrevSignal = CurrSignal;
	}
}

/*******************************************************************************
* Update Time/Date record (every second by ISR)
*******************************************************************************/
void Update_Time (void)
{
  u8 Mnth[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (RTClock.seconds == 0)
    	RTClock.nosync = Nosync;
	RTClock.seconds++;								// Seconds
	if (RTClock.seconds > 59)
	{
		Nosync++;
		RTClock.seconds = 0;
		RTClock.minutes++;							// Minutes
		if (RTClock.minutes > 59)
		{
			RTClock.minutes = 0;
			RTClock.hours++;						// Hours
			if (RTClock.hours > 23)
			{
				RTClock.hours = 0;
				RTClock.dayofweek++;				// Day of Week
				if (RTClock.dayofweek > 7)
					RTClock.dayofweek = 1;
				RTClock.dayofmonth++;				// Date
				if (RTClock.dayofmonth > Mnth[RTClock.month-1])
				{
					RTClock.dayofmonth = 1;
					RTClock.month++;				// Month
					if (RTClock.month > 12)
					{
						RTClock.month = 1;
						RTClock.year++;				// Year
						if (RTClock.year > 99)
							RTClock.year = 0;
					}
				}
			}
		}
	}
}

/*******************************************************************************
* Initialize the DCF77 library.
*******************************************************************************/
void DCF77_start(u8 dcfPin)
{
	SignalPin  = dcfPin;
	pinmode(SignalPin, INPUT);

	PrevSignal	= 0;
	PrevFlank	= 0;
	BuffPntr	= 0;
	Buff1		= 0;
	Buff2		= 0;
	Nosync		= 1;

/*  Timer1 Configuration
	   The timer clock prescale (TCKPS) is 1:64
	   The TMR1 Count register increments on every PBCLK clock cycle
	   PBCLK = CPUCLK / 2 = 40MHz => TMR1 inc. every 64/40MHZ = 1,6 us
	   20 ms = 20000 us => 20000 / 1.6 = 12500 cycles
*/
	IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
	T1CON    = 0x20;					// prescaler 1:64, internal peripheral clock
	TMR1     = 0;						// clear timer register
    #if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
    PR1      = 6250;					// load period register
    #else
    PR1      = 12500;	                // load period register
    #endif
	IPC1SET  = 7;						// select interrupt priority and sub-priority
	IFS0CLR  = 1 << INT_TIMER1_VECTOR;	// clear interrupt flag
	IEC0SET  = 1 << INT_TIMER1_VECTOR;	// enable timer 1 interrupt
	T1CONSET = 0x8000;					// start timer 1
}

/*******************************************************************************
* Timer 1 interrupt (Vector 4)
*******************************************************************************/
// Put the ISR_wrapper in the good place
void ISR_wrapper_vector_4(void) __attribute__ ((section (".vector_4")));

// ISR_wrapper will call the DCF77_timer1Interrupt()
void ISR_wrapper_vector_4(void) { Timer1Interrupt(); }

// Tmr1Interrupt is declared as an interrupt routine
void Timer1Interrupt(void) __attribute__ ((interrupt));

void Timer1Interrupt()
{
	// is this an TMR1 interrupt ?
	if (IntGetFlag(INT_TIMER1))
	{
		// Update time every second
		Cntr_20ms++;
		Cntr_sec++;
    	if (Cntr_sec > 49)
    	{
      		Cntr_sec = 0;
      		Update_Time();
		}

		DCF77_scanSignal();
		IntClearFlag(INT_TIMER1);
	}
}

/*******************************************************************************
 * Displays the DCF code chain
 ******************************************************************************/
void Disp_Input_Binary(u32 Bits1, u32 Bits2)
{
  u8 BitC;
	serial1printf("Bits= %08x %08x \r\n", Bits1, Bits2);
    for (BitC = 32; BitC > 0; BitC--)
    {
		if ((Bits2 & (1 << (BitC-1))) > 0)
			serial1write('1');
		else
			serial1write('0');
	}
    for (BitC = 32; BitC > 0; BitC--)
    {
		if ((Bits1 & (1 << (BitC-1))) > 0)
			serial1write('1');
		else
			serial1write('0');
	}
	serial1println("  ");
}

#endif
