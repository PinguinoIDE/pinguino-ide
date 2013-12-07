/*	----------------------------------------------------------------------------
	FILE:			system.c
	PROJECT:		pinguino
	PURPOSE:		
	PROGRAMER:		Régis Blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	16 nov. 2010
	LAST RELEASE:	06 feb. 2012
	----------------------------------------------------------------------------
	CHANGELOG:
	[23-02-11][Marcus Fazzi][Removed  asm("di")/asm("ei") from GetCP0Count/SetCP0Count]
	[30-01-12][Régis Blanchot][Added P32MX220F032D support]
	[06-02-12][Roland Haag][Added new clock settings]
	[13-05-12][Jean-Pierre Mandon][added P32MX250F128B and P32MX220F032B support]
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
	--------------------------------------------------------------------------*/

#ifndef __SYSTEM_C
#define __SYSTEM_C

#include <p32xxxx.h>
#include <const.h>
#include <typedef.h>

#if defined(PIC32_PINGUINO_220)||defined(GENERIC32MX250F128)||defined(GENERIC32MX220F032)
	#define CPUCOREMAXFREQUENCY	40000000L	// 40 MHz
#else
	#define CPUCOREMAXFREQUENCY	80000000L	// 80 MHz
#endif

#define CRYSTALFREQUENCY	8000000L	// 8 MHz
#define FLASHACCESSTIME		50			// 50 ns

// COSC<2:0>: Current Oscillator Selection bits
// NOSC<2:0>: New Oscillator Selection bits
#define FIRCOSCDIV		0b111 // Fast Internal RC Oscillator divided by OSCCON<FRCDIV> bits
#define FIRCOSCDIV16	0b110 // Fast Internal RC Oscillator divided by 16
#define LPIRCOSC		0b101 // Low-Power Internal RC Oscillator (LPRC)
#define SOSC			0b100 // Secondary Oscillator (SOSC)
#define POSCPLL			0b011 // Primary Oscillator with PLL module (XTPLL, HSPLL or ECPLL)
#define POSC			0b010 // Primary Oscillator (XT, HS or EC)
#define FRCOSCPLL		0b001 // Fast RC Oscillator with PLL module via Postscaler (FRCPLL)
#define FRCOSC			0b000 // Fast RC Oscillator (FRC)

// PLLODIV<2:0>: Output Divider for PLL
#define PLLODIV256		0b111 // PLL output divided by 256
#define PLLODIV64		0b110 // PLL output divided by 64
#define PLLODIV32		0b101 // PLL output divided by 32
#define PLLODIV16		0b100 // PLL output divided by 16
#define PLLODIV8		0b011 // PLL output divided by 8
#define PLLODIV4		0b010 // PLL output divided by 4
#define PLLODIV2		0b001 // PLL output divided by 2
#define PLLODIV1		0b000 // PLL output divided by 1

// FRCDIV<2:0>: Fast Internal RC Clock Divider bits
#define FRCDIV256		0b111 // FRC divided by 256
#define FRCDIV64		0b110 // FRC divided by 64
#define FRCDIV32		0b101 // FRC divided by 32
#define FRCDIV16		0b100 // FRC divided by 16
#define FRCDIV8			0b011 // FRC divided by 8
#define FRCDIV4			0b010 // FRC divided by 4
#define FRCDIV2			0b001 // FRC divided by 2 (default setting)
#define FRCDIV1			0b000 // FRC divided by 1

// PBDIV<1:0>: Peripheral Bus Clock Divisor
#define PBDIV8			0b11 // PBCLK is SYSCLK divided by 8 (default)
#define PBDIV4			0b10 // PBCLK is SYSCLK divided by 4
#define PBDIV2			0b01 // PBCLK is SYSCLK divided by 2
#define PBDIV1			0b00 // PBCLK is SYSCLK divided by 1

// PLLMULT<2:0>: PLL Multiplier bits
#define PLLMULT24		0b111 // Clock is multiplied by 24
#define PLLMULT21		0b110 // Clock is multiplied by 21
#define PLLMULT20		0b101 // Clock is multiplied by 20
#define PLLMULT19		0b100 // Clock is multiplied by 19
#define PLLMULT18		0b011 // Clock is multiplied by 18
#define PLLMULT17		0b010 // Clock is multiplied by 17
#define PLLMULT16		0b001 // Clock is multiplied by 16
#define PLLMULT15		0b000 // Clock is multiplied by 15

//#include "system-clocks.c"

/**
 * Implementation notes
 *
 * This implementation only handles the PLL setup with "PLL input
 * divider", "PLL output divider", "PLL multiplier", and "Peripheral
 * clock divider". Other clocking schemes are not supported.
 *
 * See @page 186, PIC32MX Family Reference Manual, PIC32MX Family
 * Clock Diagram
 */

/** The indices are valid values for PLLIDIV */
static const uint32_t pllInputDivs[]  = {  1,  2,  3,  4,  5,  6, 10,  12 };

/** The indices are valid values for PLLODIV */
static const uint32_t pllOutputDivs[] = {  1,  2,  4,  8, 16, 32, 64, 256 };

/** The indices are valid values for PLLMULT */
static const uint32_t pllMuls[]       = { 15, 16, 17, 18, 19, 20, 21,  24 };

/** The indices are valid values for PBDIV */
static const uint32_t pbDivs[]        = { 1, 2, 4, 8 };

static const uint8_t pllInputDivsCount = 
  sizeof(pllInputDivs) / sizeof(pllInputDivs[0]);

static const uint8_t pllOutputDivsCount = 
  sizeof(pllOutputDivs) / sizeof(pllOutputDivs[0]);

static const uint8_t pllMulsCount = 
  sizeof(pllMuls) / sizeof(pllMuls[0]);

static const uint8_t pbDivsCount = 
  sizeof(pbDivs) / sizeof(pbDivs[0]);

/**
 * Error state, only used by the test cases
 */
typedef enum SystemClocksErrorEnum {
  Error_None                      = 0,
  Error_RequestedFrequencyTooHigh = 1,
  Error_NoCombinationFound        = 2,
} SystemClocksError;

/**
 * This struct is used to store the results after reading in the
 * current settings, and to calculate new clock settings in memory.
 *
 * Each function gets a pointer to this struct as the first parameter.
 *
 * If we used C++, this struct would be the class; and all functions
 * in this file would be member functions.
 */
typedef struct SystemClocksSettingsStruct {
  uint8_t PLLIDIV;
  uint8_t PLLODIV;
  uint8_t PLLMULT;
  uint8_t PBDIV;

  #ifdef EXTENDED_TEST_CASE
    // Error state
    SystemClocksError error;
  #endif
} SystemClocksSettings;

/*	----------------------------------------------------------------------------
	SystemUnlock() perform a system unlock sequence
	--------------------------------------------------------------------------*/

void SystemUnlock()
{
	SYSKEY = 0;				// ensure OSCCON is locked
	SYSKEY = 0xAA996655;	// Write Key1 to SYSKEY
	SYSKEY = 0x556699AA;	// Write Key2 to SYSKEY
}

/*	----------------------------------------------------------------------------
	SystemLock() relock OSCCON by relocking the SYSKEY
	--------------------------------------------------------------------------*/

void SystemLock()
{
	SYSKEY = 0x12345678;	// Write any value other than Key1 or Key2
}

/*	----------------------------------------------------------------------------
	Software Reset
	----------------------------------------------------------------------------
	assume interrupts are disabled
	assume the DMA controller is suspended
	assume the device is locked
	--------------------------------------------------------------------------*/

#ifndef __32MX220F032D__
void SystemReset()
{
	u16 dummy;

	SystemUnlock();
	// set SWRST bit to arm reset
	RSWRSTSET = 1;
	// read RSWRST register to trigger reset
	dummy = RSWRST;
	// prevent any unwanted code execution until reset occurs
	while(1);
}
#endif


/**
 * Read in all relevant clock settings
 *
 * @see system.c::GetSystemClock()
 *
 * @return Nothing (the result is put into the struct)
 */
void SystemClocksReadSettings(SystemClocksSettings *s) 
{
  s->PLLIDIV = DEVCFG2bits.FPLLIDIV;
  
  s->PLLODIV = OSCCONbits.PLLODIV;
  s->PLLMULT = OSCCONbits.PLLMULT;

  s->PBDIV   = OSCCONbits.PBDIV;
}

/**
 * Calculates the CPU frequency based on the values which are passed in.
 * This normally requires a call to SystemClocksReadSettings() before.
 *
 * @see system.c::GetSystemClock()
 *
 * @return The CPU frequency
 */
uint32_t SystemClocksGetCpuFrequency(const SystemClocksSettings *s) 
{
  return 
    CRYSTALFREQUENCY 
    / pllInputDivs [s->PLLIDIV]
    / pllOutputDivs[s->PLLODIV] 
    * pllMuls      [s->PLLMULT];
}

/**
 * Calculates the peripheral clock frequency based on the values which
 * are passed in. This normally requires a call to
 * SystemClocksReadSettings() before.
 *
 * @see system.c::GetPeripheralClock()
 *
 * @return The peripheral clock frequency
 */
uint32_t SystemClocksGetPeripheralFrequency(const SystemClocksSettings *s) 
{
  return SystemClocksGetCpuFrequency(s) / pbDivs[s->PBDIV];
}

/**
 * Calculate the necessary values in order to have the CPU running at
 * the desired frequency.
 *
 * @see system.c::SystemConfig()
 *
 * @return Nothing (the result is put into the struct)
 */
void SystemClocksCalcCpuClockSettings(SystemClocksSettings *s, 
				      uint32_t cpuFrequency) 
{
  if (cpuFrequency <= CPUCOREMAXFREQUENCY) 
    {
      #ifdef EXTENDED_TEST_CASE
        s->error = Error_None;
      #endif
    } 
  else 
    {
      cpuFrequency = CPUCOREMAXFREQUENCY;

      #ifdef EXTENDED_TEST_CASE
        s->error = Error_RequestedFrequencyTooHigh;
      #endif
    }

  uint8_t pllOutputDivIndex;
  for (pllOutputDivIndex = 0; 
       pllOutputDivIndex < pllOutputDivsCount; 
       pllOutputDivIndex++)
    {
      uint8_t pllMulIndex;
      for (pllMulIndex = 0; pllMulIndex < pllMulsCount; pllMulIndex++)
	{
	  if ( (cpuFrequency 
		* pllInputDivs[s->PLLIDIV]
		* pllOutputDivs[pllOutputDivIndex])
	       == (CRYSTALFREQUENCY * pllMuls[pllMulIndex]) )
	    {
	      s->PLLODIV = pllOutputDivIndex;
	      s->PLLMULT = pllMulIndex;
	      // Match found
	      return;
	    }
	}
    }
  
  /* 
   * No combination found -> try to get max CPU frequency. This
   * depends from pllInputDiv.
   *
   * The selected values will result in the max CPU frequency, if
   * the pllInputDiv is 1 or 2.
   *
   * With pllInputDiv > 2 we cannot reach the max frequency.
   */
  if (pllInputDivs[s->PLLIDIV] >= 2) 
    {
      s->PLLODIV = PLLODIV1; // /1

      #ifdef EXTENDED_TEST_CASE
        s->error = Error_NoCombinationFound;
      #endif
    }
  else 
    {
      s->PLLODIV = PLLODIV2; // /2

      #ifdef EXTENDED_TEST_CASE
        s->error = Error_NoCombinationFound;
      #endif
    }

  s->PLLMULT = PLLMULT20; // x20
}

/**
 * Calculate the necessary values in order to have the peripheral
 * clock running at the desired frequency. As this value is only a
 * divider, this normally requires a call to
 * SystemClocksCalcCpuClockSettings() before, because the CPU
 * frequency is needed to find the divider value.
 *
 * @see system.c::SystemConfig()
 *
 * @return Nothing (the result is put into the struct)
 */
void SystemClocksCalcPeripheralClockSettings(SystemClocksSettings *s, 
					     uint32_t peripheralFrequency) 
{
  #ifdef EXTENDED_TEST_CASE
    s->error = Error_None;
  #endif

  if (peripheralFrequency > CPUCOREMAXFREQUENCY) 
    {
      peripheralFrequency = CPUCOREMAXFREQUENCY;

      #ifdef EXTENDED_TEST_CASE
        s->error = Error_RequestedFrequencyTooHigh;
      #endif
    }

  const uint32_t cpuFrequency = SystemClocksGetCpuFrequency(s);

  uint8_t i;
  for (i = 0; i < pbDivsCount; i++) 
    {
      if (cpuFrequency == peripheralFrequency * pbDivs[i]) 
	{
	  s->PBDIV = i;
	  // Match found
	  return;
	}
    }

  // No match: Use default value
  s->PBDIV = PBDIV1; // /1
  #ifdef EXTENDED_TEST_CASE
    s->error = Error_NoCombinationFound;
  #endif
}

/**
 * Write the clock settings into the registers; this effectively applies the
 * new settings, and will change the CPU frequency, peripheral
 * frequency, and the number of flash wait states.
 *
 * This normally requires a call to
 * SystemClocksCalcCpuClockSettings(), and
 * SystemClocksCalcPeripheralClockSettings() before.
 *
 * @see system.c::SystemConfig()
 *
 * @return Nothing
 */
void SystemClocksWriteSettings(const SystemClocksSettings *s) 
{
  SystemUnlock();
  
  /**
   * @page 186 
   *
   * PIC32MX Family Clock Diagram
   *
   * @page 189
   *
   * OSCCON: Oscillator Control Register
   */

  /* 
   * bit 10-8 NOSC<2:0>: New Oscillator Selection bits
   *     011 = Primary Oscillator with PLL module (XTPLL, HSPLL or ECPLL)
   */
  OSCCONbits.NOSC = POSCPLL;

  /*
   * bit 29-27 PLLODIV<2:0>: Output Divider for PLL
   */
  OSCCONbits.PLLODIV = s->PLLODIV;
  
  /*
   * bit 18-16 PLLMULT<2:0>: PLL Multiplier bits
   */
  OSCCONbits.PLLMULT = s->PLLMULT;
  
  /*
   * bit 0 OSWEN: Oscillator Switch Enable bit
   *   1 = Initiate an osc switch to selection specified by NOSC2:NOSC0 bits
   *   0 = Oscillator switch is complete
   */
  OSCCONbits.OSWEN = 1;
  // Busy wait until osc switch has been completed
  while (OSCCONbits.OSWEN == 1) 
    {
      asm("nop");
    }
  
  /*
   * bit 20-19 PBDIV<1:0>: Peripheral Bus Clock Divisor
   */
  OSCCONbits.PBDIV = s->PBDIV;
  
  // Set wait states
#if defined(PIC32_PINGUINO_220)||defined(GENERIC32MX250F128)||defined(GENERIC32MX220F032)
	// TODO
	PMMODEbits.WAITB = 0b00;								// Data wait of 1 TPB
#else
	CHECON = (SystemClocksGetCpuFrequency(s) / 20) - 1;		// FlashWaitStates
#endif
  
  //
  
  SystemLock();
}

/*	----------------------------------------------------------------------------
	GetSystemClock
	--------------------------------------------------------------------------*/

u32 GetSystemClock(void)
{
	SystemClocksSettings s;
	SystemClocksReadSettings(&s);
	return SystemClocksGetCpuFrequency(&s);
}

/*	----------------------------------------------------------------------------
	GetPeripheralClock()
	--------------------------------------------------------------------------*/

u32 GetPeripheralClock()
{
  SystemClocksSettings s;
  SystemClocksReadSettings(&s);
  return SystemClocksGetPeripheralFrequency(&s);
}

/*	----------------------------------------------------------------------------
	SetFlashWaitStates()
	--------------------------------------------------------------------------*/

void SetFlashWaitStates_old()
{
	SystemUnlock();
#if defined(PIC32_PINGUINO_220)||defined(GENERIC32MX250F128)||defined(GENERIC32MX220F032)
	PMMODEbits.WAITB = 0b00;					// Data wait of 1 TPB
#else
	CHECON = (GetSystemClock() / 20) - 1;		// FlashWaitStates
#endif
	SystemLock();
}

/*	----------------------------------------------------------------------------
	SystemSetSystemClock
	--------------------------------------------------------------------------*/

void SystemSetSystemClock(u32 cpuCoreFrequency)
{
	SystemClocksSettings s;
	SystemClocksReadSettings(&s);
	SystemClocksCalcCpuClockSettings(&s, cpuCoreFrequency);
	SystemClocksWriteSettings(&s);
}

/*	----------------------------------------------------------------------------
	SystemSetPeripheralClock()
	--------------------------------------------------------------------------*/

void SystemSetPeripheralClock(u32 peripheralFrequency)
{
	SystemClocksSettings s;
	SystemClocksReadSettings(&s);
	SystemClocksCalcPeripheralClockSettings(&s, peripheralFrequency);
	SystemClocksWriteSettings(&s);
}

/*	----------------------------------------------------------------------------
	SystemConfig()
	--------------------------------------------------------------------------*/

void SystemConfig(u32 cpuCoreFrequency)
{
	SystemClocksSettings s;
	SystemClocksReadSettings(&s);
	SystemClocksCalcCpuClockSettings(&s, cpuCoreFrequency);
	SystemClocksCalcPeripheralClockSettings(&s, cpuCoreFrequency / 2);
	SystemClocksWriteSettings(&s);

	DDPCONbits.JTAGEN=0;		// PORTA is used as digital instead of JTAG
}

/*	----------------------------------------------------------------------------
	GetCP0Count()
	--------------------------------------------------------------------------*/

u32 MIPS32 GetCP0Count()
{
	//u32 count;
	//asm("di"); // Disable all interrupts
	//count = _CP0_GET_COUNT();
	//asm("ei"); // Enable all interrupts
	//return count;
	return _CP0_GET_COUNT();
}

/*	----------------------------------------------------------------------------
	SetCP0Count()
	--------------------------------------------------------------------------*/

void MIPS32 SetCP0Count(u32 count)
{
	//asm("di"); // Disable all interrupts
	_CP0_SET_COUNT(count);
	//asm("ei"); // Enable all interrupts
}

#endif	/* __SYSTEM_C */

