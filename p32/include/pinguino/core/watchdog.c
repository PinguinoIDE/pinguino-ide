//	watchdog.c
//	pic32-pinguino watchdog lib
//	djpark@astsb.info

#ifndef __WATCHDOG__
#define __WATCHDOG__

//--	watchdog in lib
#define EnableWDT()         (WDTCONSET = _WDTCON_ON_MASK)
#define DisableWDT()        (WDTCONCLR = _WDTCON_ON_MASK)
#define ClearWDT()          (WDTCONSET = _WDTCON_WDTCLR_MASK)
#define ClearEventWDT()     (RCONCLR = _RCON_WDTO_MASK)
#define ReadEventWDT()      (RCONbits.WDTO)
#define ReadPostscalerWDT() (WDTCONbits.WDTPSTA)

u8 boot_from_watchdog = 0;

void init_watchdog()
{
	// enable watchdog (8.2 seconds)
	if (ReadEventWDT())
		boot_from_watchdog = 1;
	//EnableWDT();
	DisableWDT();
	ClearEventWDT();
	ClearWDT();
}

#define EventWDT() (boot_from_watchdog)

#endif	// __WATCHDOG__

