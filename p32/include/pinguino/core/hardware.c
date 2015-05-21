/*	----------------------------------------------------------------------------
	FILE:				hardware.c
	PROJECT:			pinguinoX
	PURPOSE:		
	PROGRAMER:		Régis Blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	16 nov. 2010
	LAST RELEASE:	03 apr. 2011
	----------------------------------------------------------------------------
	CHANGELOG:
	[xx-03-11][Marcus Fazzi][Added support to Emperor Boards]
	[03-04-11][rblanchot@gamil.com][fixed conditional compilation for board support] 	----------------------------------------------------------------------------
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

#ifndef __HARDWARE_C
	#define __HARDWARE_C

	#include <const.h>
	#include <typedef.h>
	#include <delay.c>
	#include <digitalw.c>

/*	----------------------------------------------------------------------------
	SwitchDebounce()
	----------------------------------------------------------------------------
	Check twice button value in a short period of time (delayms)
	to make sure it's definitely pressed
	pin		: pin number where switch is connected
	delayms : period whithin button must keep same value (50ms should be a good value)
	return	: value is return only if it's stable
	--------------------------------------------------------------------------*/

u8 SwitchDebounce(u8 pin, u16 ms)
{
	u8 state1 = 0;
	u8 state2 = 0;

	while(1)
	{
		state1 = (digitalread(pin) == 1 ? 1 : 0);		// Met l'état de la pin dans state1
		Delayms(ms);									// anti-rebond 50 milisecondes
		state2 = (digitalread(pin) == 1 ? 1 : 0);		// Met l'état de la pin dans state2
		if(state2 == state1) return state1;				// Si l'etat est stable pendant 50 ms
	}
}

#endif /* __HARDWARE_C */

