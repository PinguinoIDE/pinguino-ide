/*	----------------------------------------------------------------------------
	FILE:			analog.c
	PROJECT:		pinguinoX
	PURPOSE:		
	PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
	FIRST RELEASE:	19 feb. 2011
	LAST RELEASE:	25 feb. 2012
	----------------------------------------------------------------------------
	CHANGELOG:
	[31-03-11][rblanchot@gamil.com][fixed conditional compilation for board support]
	// 08 nov. 2011 [jp.mandon@gmail.com] fixed a bug in analogRead ( stop and restart analog converter before sampling )
	// 25 feb. 2012 [jp.mandon@gmail.com] added support for PIC32_PINGUINO_220
	// 17 mar. 2012 [hgmvanbeek@gmail.com] added support for PIC32_PINGUINO_MICRO
	// 19 may. 2012 [jp.mandon@gmail.com] added support for PINGUINO32MX250 and PINGUINO32MX220
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

#ifndef __ANALOG__
#define __ANALOG__

#define pA	0
#define pB	1
#define pC	2
#define pD	3
#define pE	4
#define pF	5
#define pG	6

#define _0	1<<0
#define _1	1<<1
#define _2	1<<2
#define _3	1<<3
#define _4	1<<4
#define _5	1<<5
#define _6	1<<6
#define _7	1<<7
#define _8	1<<8
#define _9	1<<9
#define _10	1<<10
#define _11	1<<11
#define _12	1<<12
#define _13	1<<13
#define _14	1<<14
#define _15	1<<15
#define nil 1<<16

#if defined(PIC32_PINGUINO)

u16 __analogmask[]={0x0002,0x0004,0x0008,0x0010,0x0100,0x0200,0x0800,0x0400,
		    		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0002,0x0004,
		    		0x0008,0x0010,0x0100,0x0200,0x0800,0x0400};
u16 __bufmask[]=   {0x0004,0x0008,0x000C,0x0010,0x0020,0x0024,0x002C,0x0028,
		    		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0004,0x0008,
		    		0x000C,0x0010,0x0020,0x0024,0x002C,0x0028};
#endif

#if defined(PIC32_PINGUINO_OTG)

u16 __analogmask[]={0x0002,0x0004,0x0008,0x0010,0x0100,0x0200,0x0800,0x0400,
		    		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0002,0x0004,
		    		0x0008,0x0010,0x0100,0x0200,0x0800,0x0400};
u16 __bufmask[]=   {0x0004,0x0008,0x000C,0x0010,0x0020,0x0024,0x002C,0x0028,
		    		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0004,0x0008,
		    		0x000C,0x0010,0x0020,0x0024,0x002C,0x0028};
#endif

#if defined(PIC32_PINGUINO_MICRO)

u16 __analogmask[] = { _1,  _2,  _3,  _4, nil, nil, nil,  _8,   //  0- 7
					   _9, _10, _11, _12, nil, _14, nil, nil,   //  8-15  nil is a unavailable pin
					   _0, nil, nil, nil, nil, nil, nil, nil,   // 16-23
					  nil, nil, nil, nil, nil, nil, nil, nil }; // 24-31

u16 __bufmask[]    = {  4,   8,  12,  16,  32,  36,  40,  44,   //  0- 7
					   48,  56, nil, nil, nil, nil, nil, nil,   //  8-15
					    0, nil, nil, nil, nil, nil, nil, nil,   // 16-23
					  nil, nil, nil, nil, nil, nil, nil, nil }; // 24-31
#endif

#if defined(PIC32_PINGUINO_220)

u16 __portanalogmask[]=	{0x0002,0x0002,0x0001,0x0001,0x0001,0x0001,0x0000,0x0000,
						0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0002,0x0002,
						0x0001,0x0001,0x0001,0x0001,0x0000,0x0000};
u16 __analogmask[]=    	{0x0001,0x0002,0x0001,0x0002,0x0004,0x0008,0x0000,0x0000,
						0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,
						0x0004,0x0008,0x0000,0x0000,0x0000,0x0000};
u16 __bufmask[]=		{0x0018,0x001C,0x0008,0x000C,0x0010,0x0014,0x0000,0x0000,
						0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0018,0x001C,
						0x0008,0x000C,0x0010,0x0014,0x0000,0x0000};
#endif

#if defined(PINGUINO32MX250)||defined(PINGUINO32MX220)

u16 __analogmask[]=    	{_15,_14,_13,nil,nil,nil,nil,nil,
						nil,_3,_2,_1,_0,nil,nil,nil,
						nil,nil,nil,nil,nil,nil};
u16 __bufmask[]=		{0x0024,0x0028,0x002C,nil,nil,nil,nil,nil,
						nil,0x0014,0x0010,0x000C,0x0008,nil,nil,nil,
						nil,nil,nil,nil,nil,nil};
#endif


#if defined(EMPEROR460) || defined(EMPEROR795)
u16 __analogmask[]={0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
				    0x0020,0x0010,0x0008,0x0004,0x0002,0x0001,0x0000,0x0000,
				    0x0100,0x0200,0x0400,0x0800,0x0000,0x0000,0x0000,0x1000,
				    0x2000,0x4000,0x8000};
u16 __bufmask[]=   {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,				  
					0x0014,0x0010,0x000C,0x0008,0x0004,0x0000,0x0000,0x0000,				  
					0x001E,0x0022,0x0026,0x002A,0x0000,0x0000,0x0000,0x002E,
					0x0032,0x0036,0x003A};					
#endif

#if defined(UBW32_460) || defined(UBW32_795)
u16 __analogmask[]={0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
				    0x0020,0x0010,0x0008,0x0000,0x0000,0x0100,0x0200,0x0400,
				    0x0800,0x0000,0x0000,0x0000,0x1000,0x2000,0x4000,0x8000};
u16 __bufmask[]=   {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
					0x0014,0x0010,0x000C,0x0000,0x0000,0x0020,0x0024,0x0028,
					0x002C,0x0000,0x0000,0x0000,0x0030,0x0034,0x0038,0x003C};				    
#endif

#if defined(PIC32_PINGUINO_T795)
u16 __analogmask[]={  _3,  _4,  _6,  _7,  _9, _10, nil,	nil,	//0-7
		     nil, nil, nil, nil, nil, nil, nil, nil,	//8-15
		     nil, nil, _11, _12, nil, nil, nil, nil, 	//16-23
		     nil, nil, nil, nil, nil, nil, nil, nil, 	//24-31
		     nil,  _8, _14, _13, _15, nil, nil, nil,	//32-39
		     nil,  _5,  _2,  _1,  _0};			//40-44
u16 __bufmask[]=   {  12,  16,  24,  28,  36,  40, nil,	nil,	//0-7
		     nil, nil, nil, nil, nil, nil, nil, nil,	//8-15
		     nil, nil,  44,  48, nil, nil, nil, nil, 	//16-23
		     nil, nil, nil, nil, nil, nil, nil, nil, 	//24-31
		     nil,  32,  64,  52,  60, nil, nil, nil,	//32-39
		     nil,  20,   8,   4,   0};			//40-44
#endif


/* 2012-10-11 RB the followings are already defined in const.h
#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7
*/

void SetAnalog(u8 pin)
{
	#if defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_OTG)
	if ((pin==4)||(pin==18)) TRISDbits.TRISD9=1;   // analog input 18 is shared with I2C
	if ((pin==5)||(pin==19)) TRISDbits.TRISD10=1;  // analog input 19 is shared with I2C
	#endif
	#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
		#if defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
			ANSELBSET=__analogmask[pin];
			TRISBSET=__analogmask[pin];
		#else
			switch (__portanalogmask[pin])
				{
				case 1:	ANSELBSET=__analogmask[pin];
						TRISBSET=__analogmask[pin];
						break;
				case 2: ANSELCSET=__analogmask[pin];
						TRISCSET=__analogmask[pin];
						break;
				}		
		#endif
	#else
	TRISBSET=__analogmask[pin];
	AD1PCFGCLR=__analogmask[pin];
	#endif
}

u8 IsDigital(u8 pin)
{
	#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
		#if defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
			if (ANSELB&__analogmask[pin]) return 1;
			else return 0;
		#else
			switch (__portanalogmask[pin])
				{
				case 1:	if (ANSELB&__analogmask[pin]) return 1;
						else return 0;
						break;
				case 2: if (ANSELC&__analogmask[pin]) return 1;
						else return 0;
						break;
				}
		#endif
	#else	
	if ((AD1PCFG&__analogmask[pin])!=0) return 1;
	else return 0;
	#endif
}
	
void analog_init(void)
{
	AD1CSSL = 0;
	AD1CON1=0x00E0;
	AD1CON2=0;
	AD1CON3 = 0x8F00;
	AD1CON1bits.ADON = 1;
}

u16 analogRead(u8 pin)
{
	
	AD1CON1bits.ADON = 0;				  // stop analogic converter
	if (IsDigital(pin)) SetAnalog(pin);	  // set analog
	AD1CHS=(__bufmask[pin]/4)<<16;		  // select channel
	AD1CON1bits.ADON = 1;				  // start analogic converter
	AD1CON1bits.SAMP=1;					  // start sampling
	while (!AD1CON1bits.DONE);			  // wait for conversion
	return(ADC1BUF0+(8*(AD1CON2bits.BUFS&0x01))); // return result
}

#endif
