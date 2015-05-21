/*
 * File			DS1306.c
 *
 * Description		Support for Dallas Semiconductor DS1306 Real Time Clock, using SPI
 *			Porting from Arduino C++ to PINGUINO32 C with added features to use emulated SPI port. 
 *
 * Author		Moreno Manzini
 *
 * Version		1.0
 *
 * License		This software is released under the terms of the Mozilla Public License (MPL) version 2.0
 * 			Full details of licensing terms can be found in the "LICENSE" file, distributed with this code
 *
 * 			Original author Chris Bearman
 *			Original project https://github.com/cjbearman/ds1306-arduino
 *
 *   18/06/2013 	Ver. 1 Rev. 0 First release --- Moreno Manzini ( moreno at mediacom dot it )
*/


#include <DS1306.h>
#include <digitalw.c>
#include <spi.c>
#include <spiloc.c>
#include <string.h>     // memset

 
SPI_Loc_Conf DS1306_SPILoc;
// Class Properties
u8 DS1306_CE;			// Chip enable line
BOOL DS1306_writeHours24 = TRUE;			// True (default) means time/alarm writes use 24 hour form
u8 DS1306_LocalSPI;


// Must call initialize prior to using any other method in this class (except constructor)
void DS1306_init(u8 LocalSPI, u8 LoCE,u8 LoSCK, u8 LoSDO, u8 LoSDI) 
{
	unsigned char cr;
	// Initialize SPI Bus

  DS1306_CE = LoCE;
  DS1306_LocalSPI = LocalSPI;

	// Initialize the chip enable, LOW
	pinmode(DS1306_CE, OUTPUT);
	digitalwrite(DS1306_CE, LOW);
  
  
  switch (DS1306_LocalSPI)
   {
    case 0:
     DS1306_SPILoc.CPOL = FALSE;    // Clock Polarity
     DS1306_SPILoc.CPHA = TRUE;    // Sample position
     DS1306_SPILoc.DelayuS = 0;    // Delay Clock uS
     DS1306_SPILoc.LoSCK = LoSCK;
     DS1306_SPILoc.LoSDO = LoSDO;
     DS1306_SPILoc.LoSDI = LoSDI;
     SPILoc_Init (&DS1306_SPILoc);
    break;
    case 1:
    case 2:
     SPI_begin();
     break;
   }


	// digitalread control register
	cr = DS1306_read(DS1306_CR);

	// Rewrite control register, disabling write protect
	// disabling write protect
	cr = cr & ~ (1 << DS1306_CR_WP);
	DS1306_write(DS1306_CR, cr);
}




// Time set uses hours (when DS1306_writeHours24 = true) (default), hours12/ampm (when DS1306_writeHours24 = false)
void DS1306_Hours24(BOOL Hours24)
{
 DS1306_writeHours24 = Hours24;
}

  

// Set the current time
// Time set uses hours (when DS1306_writeHours24 = true), hours12/ampm (when DS1306_writeHours24 = false)
void DS1306_setTime(const ds1306time *time)
{
	unsigned char buf[DS1306_SIZE_DATETIME];
	DS1306_encodeTimePacket(buf, time);
	DS1306_writen(DS1306_DATETIME, buf, DS1306_SIZE_DATETIME);
}




// Reads len bytes from register in address into data
void DS1306_readn(unsigned char address, unsigned char *data, int len)
{
 int i;

	// Select the DS1306 by raising it's chip enable line
	digitalwrite(DS1306_CE, HIGH);

 switch (DS1306_LocalSPI)
  {
   case 0:
    SPILoc_Write(&DS1306_SPILoc, address);
  	 for (i = 0; i < len; i++)
     data[i] = SPILoc_Read(&DS1306_SPILoc);
    break;
   case 1:
   case 2:
    SPI_write(address);
  	 for (i = 0; i < len; i++)
     data[i] = SPI_read();
    break;
  }

 digitalwrite(DS1306_CE, LOW);
}


// Read a single byte register
unsigned char DS1306_read(unsigned char address)
{
	unsigned char buf;
	DS1306_readn(address, &buf, 1);
	return buf;
}


// Write SPI to register "address" with specified data, bursting for the given length
void DS1306_writen(unsigned char address, const unsigned char *data, int len)
{
 int i;

	digitalwrite(DS1306_CE, HIGH);
 switch (DS1306_LocalSPI)
  {
   case 0:
    SPILoc_Write(&DS1306_SPILoc, address | DS1306_WRITE_OFFSET);
   	for (i = 0; i < len; i++)
     SPILoc_Write(&DS1306_SPILoc, data[i]);
    break;
   case 1:
   case 2:
    SPI_write(address | DS1306_WRITE_OFFSET);
    for (i = 0; i < len; i++)
     SPI_write(data[i]);
    break;
  }
	digitalwrite(DS1306_CE, LOW);
}

 
// Write a single byte register
void DS1306_write(unsigned char address, const unsigned char value)
{
	DS1306_writen(address, &value, 1);
}





// Encode value as BCD
unsigned char DS1306_encodeBCD7(unsigned char value)
{
	return ((value & DS1306_ANY) ? DS1306_ANY : (((value / 10) << 4) | (value % 10)));
}

// Encode value as BCD, apply bitmask (AND)
unsigned char DS1306_encodeBCD7M(unsigned char value, unsigned char mask)
{
	return ((value & DS1306_ANY) ? DS1306_ANY : (((value / 10) << 4) | (value % 10)) & mask);
}

// Encode value as BCD, full 8 bits
unsigned char DS1306_encodeBCD8(unsigned char value)
{
	return (((value / 10) << 4) | (value % 10));
}

// Decode BCD
unsigned char DS1306_decodeBCD7(unsigned char value)
{
	return ((value & DS1306_ANY) ? DS1306_ANY : ((((value & 0xF0) >> 4) * 10) + (value & 0x0F)));
}

unsigned char DS1306_decodeBCD7M(unsigned char value, unsigned char mask)
{

	return ((value & DS1306_ANY) ? DS1306_ANY : DS1306_decodeBCD7(value & mask));
}

// Decode BCD, full 8 bits
unsigned char DS1306_decodeBCD8(unsigned char value)
{
	return ((((value & 0xF0) >> 4) * 10) + (value & 0x0F));
}



// Encodes a time packet
// CJB - need to extend 12 hour based encoding
void DS1306_encodeTimePacket(unsigned char *buf, const ds1306time *time)
{
	buf[0] = DS1306_encodeBCD7M(time->seconds, 0x7F);
	buf[1] = DS1306_encodeBCD7M(time->minutes, 0x7F);
	buf[2] = DS1306_encodeHourByte(time->hours, time->hours12, time->ampm);
	buf[3] = DS1306_encodeBCD7M(time->dow, 0x07);
	buf[4] = DS1306_encodeBCD7M(time->day, 0x3F);
	buf[5] = DS1306_encodeBCD7M(time->month, 0x3F);
	buf[6] = DS1306_encodeBCD8(time->year);
}

// Encode an alarm packet
// CJB - need to extend 12 hour based encoding
void DS1306_encodeAlarmPacket(unsigned char *buf, const ds1306alarm *alarm)
{
	buf[0] = DS1306_encodeBCD7M(alarm->seconds, 0x7F);
	buf[1] = DS1306_encodeBCD7M(alarm->minutes, 0x7F);
	buf[2] = DS1306_encodeHourByte(alarm->hours, alarm->hours12, alarm->ampm);
	buf[3] = DS1306_encodeBCD7M(alarm->dow, 0x07);
}

// Decode a time response from the DS1306
void DS1306_decodeTimePacket(const unsigned char *buf, ds1306time *time)
{
	time->seconds = DS1306_decodeBCD7M(buf[0], 0x7F);
	time->minutes = DS1306_decodeBCD7M(buf[1], 0x7F);
	DS1306_decodeHourByte(buf[2], &(time->hours), &(time->hours12), &(time->ampm));
	time->dow = DS1306_decodeBCD7M(buf[3], 0x07);
	time->day = DS1306_decodeBCD7M(buf[4], 0x3F);
	time->month = DS1306_decodeBCD7M(buf[5], 0x3F);
	time->year = DS1306_decodeBCD8(buf[6]);
}

// Decodes an alarm response from the DS1306
void DS1306_decodeAlarmPacket(const unsigned char *buf, ds1306alarm *alarm)
{
	alarm->seconds = DS1306_decodeBCD7M(buf[0], 0x7F);
	alarm->minutes = DS1306_decodeBCD7M(buf[1], 0x7F);
	DS1306_decodeHourByte(buf[2], &(alarm->hours), &(alarm->hours12), &(alarm->ampm));
	alarm->dow = DS1306_decodeBCD7M(buf[3], 0x07);
}

// Encodes an hour byte using selected default format for write, honoring "ALL" (for alarms)
unsigned char DS1306_encodeHourByte(unsigned char hour24, unsigned char hour12, char ampm)
{
	if ((DS1306_writeHours24 && ((hour24 & DS1306_ANY))) || (!DS1306_writeHours24 && (hour12 & DS1306_ANY))) {
		return DS1306_ANY;
	} else {
		if (DS1306_writeHours24) {
			// Writing 24 hour format times
			return DS1306_encodeBCD7M(hour24, 0x3F);
		} else {
			return (DS1306_encodeBCD7M(hour12, 0x1F) | 0x40 | (ampm == 'P' ? 0x20 : 0x00));
		}
	}
}

// Decodes an hour byte, honoring "ALL" (for alarms)
void DS1306_decodeHourByte(unsigned char hourByte, unsigned char *hour24, unsigned char *hour12, char *ampm)
{
	if (hourByte & DS1306_ANY) {
		*hour12 = DS1306_ANY;
		*hour24 = DS1306_ANY;
		*ampm = 0;
	} else {
		if (hourByte & 0x40) {
			// 12 hour mode
			*hour12 = DS1306_decodeBCD7M(hourByte, 0x1F);
			if (hourByte & 0x20) {
				// PM
				if (*hour12 == 12) {
					*hour24 = 12;
				} else {
					*hour24 = 12 + *hour12;
				}
				*ampm = 'P';
			} else {
				// AM
				*hour24 = (*hour12 == 12) ? 0 : *hour12;
				*ampm = 'A';
			}
		} else {
			// 24 hour mode
			*hour24 = DS1306_decodeBCD7M(hourByte, 0x3F);
			if (*hour24 == 0) {
				*hour12 = 12;
				*ampm = 'A';
			} else if (*hour24 == 12) {
				*hour12 = 12;
				*ampm = 'P';
			} else if (*hour24 < 12) {
				*ampm = 'A';
				*hour12 = *hour24;
			} else {
				*ampm = 'P';
				*hour12 = *hour24 - 12;
			}
		}
	}
}



// Retrieve current time
void DS1306_getTime(ds1306time *time)
{
	unsigned char buf[DS1306_SIZE_DATETIME];

    memset((__data *)time, 0, sizeof(ds1306time));

	DS1306_readn(DS1306_DATETIME, buf, DS1306_SIZE_DATETIME);
	DS1306_decodeTimePacket(buf, time);
}

// Set an alarm
// alarm must be 0 or 1 else nothing is done
// Time set uses hours (when DS1306_writeHours24 = true), hours12/ampm (when DS1306_writeHours24 = false)
// Set any field (except ampm of course) to DS1306_ANY to indicate that alarm fires on any value in that field
void DS1306_setAlarm(int alarm, const ds1306alarm *time)
{
	if (alarm < 2) {
		unsigned char buf[DS1306_SIZE_ALARM];
		DS1306_encodeAlarmPacket(buf, time);
		DS1306_writen(alarm == 0 ? DS1306_ALARM0 : DS1306_ALARM1, buf, DS1306_SIZE_ALARM);
	}
}

// Retrieve an alarm
// alarm must be 0 or 1 else nothing is done except resetting (zeroing) the response structure
// Be on the look out for DS1306_ANY in any alarm field (except ampm) to indicate whether the alarm fires on any value
// in that field
void DS1306_getAlarm(int alarm, ds1306alarm *time)
{
    memset((__data *)time, 0, sizeof(ds1306time));

	if (alarm < 2) {
		unsigned char buf[DS1306_SIZE_ALARM];
		DS1306_readn(alarm == 0 ? DS1306_ALARM0 : DS1306_ALARM1, buf, DS1306_SIZE_ALARM);
		DS1306_decodeAlarmPacket(buf, time);
	}
}

// Retrieve the alarm state of an individual alarm where alarm = 0 or 1
// A true return means the alarm has triggered
BOOL DS1306_getAlarmState(unsigned int alarm)
{
	if (alarm > 1) return false;
	return ((DS1306_read(DS1306_SR) & (1 << alarm)) ? true : false);
}

// Retrieve state of both alarms
// true values indicate that the alarm has triggered
void DS1306_getAlarmBothState(BOOL *state1, BOOL *state2)
{
	unsigned char sr = DS1306_read(DS1306_SR);
	*state1 = (sr & 0x01) ? true : false;
	*state2 = (sr & 0x02) ? true : false;
}

// Clear the state of an individual alarm where alarm = 0 or 1
void DS1306_clearAlarmState(unsigned int alarm)
{
	if (alarm > 1) return;

	DS1306_write(DS1306_SR, DS1306_read(DS1306_SR) & ~ (1 << alarm));
}

// Clear state of both alarms
void DS1306_clearAlarmBothState()
{
	DS1306_write(DS1306_SR, 0);
}

// Returns the enabled state of an individual alarm where alarm = 0 or 1
// A true return means the alarm is enabled
BOOL DS1306_getAlarmEnabled(unsigned int alarm)
{
	if (alarm > 1) return false;
	return((DS1306_read(DS1306_CR) & (1 << alarm)) ? true : false);
}

// Returns the alarm enablement state of both alarms (true = enabled, false = disabled)
void DS1306_getAlarmBothEnabled(BOOL *enabled1, BOOL *enabled2)
{
	unsigned char cr = DS1306_read(DS1306_CR);
	*enabled1 = (cr & 0x01) ? true : false;
	*enabled2 = (cr & 0x01) ? true : false;
}

// Enable an alarm where alarm = 0 or 1
void DS1306_enableAlarm(unsigned int alarm)
{
	if (alarm > 1) return;
	DS1306_write(DS1306_CR, DS1306_read(DS1306_CR) | (1 << alarm));
}

// Disable an alarm where alarm = 0 or 1
void DS1306_disableAlarm(unsigned int alarm)
{
	if (alarm > 1) return;
	DS1306_write(DS1306_CR, DS1306_read(DS1306_CR) & ~ (1 << alarm));
}

// Enable both alarms
void DS1306_enableBothAlarms()
{
	DS1306_write(DS1306_CR, DS1306_read(DS1306_CR) | 0x03);
}

// Disable both alarms
void DS1306_disableBothAlarms()
{
	DS1306_write(DS1306_CR, DS1306_read(DS1306_CR) & ~ 0x03);
}

// Enable trickle charging
// Must provide number of diodes (1 or 2)
// and KOhm resistance (2, 4 or 8)
// Any other values will fail (false returned, no changes made)
BOOL DS1306_enableTrickleCharge(unsigned char numDiodes, unsigned char kRes)
{
	unsigned char b;

	if (numDiodes < 1 || numDiodes > 2) return false;
    
    b = 0xA0 | (numDiodes << 2);
	switch(kRes) {
		case 2	:	b |= 0x01; break;
		case 4	:	b |= 0x02; break;
		case 8	:	b |= 0x03; break;
		default	:	return false;
	}

	DS1306_writen(DS1306_TCR, &b, 1);
}

// Disable trickle charging
void DS1306_disableTrickleCharge()
{
	unsigned char b = 0;
	DS1306_writen(DS1306_TCR, &b, 1);
}

// Retrieve trickle charging state
// Returns true (Trickle enabled), false (Trickle disabled)
// When enabled, numDiodes and kRes will be set to (1, 2) and (2, 4, 8) respectively
// When disabled numDiodes and kRes will be set to 0
BOOL DS1306_getTrickleChargeState(unsigned char *numDiodes, unsigned char *kRes)
{
	unsigned char b;
	DS1306_readn(DS1306_TCR, &b, 1);

	*numDiodes = 0;
	*kRes = 0;

	if ((b & 0xF0) != 0xA0) return false;	// TCR disabled

	switch(b & 0x03) {
		case 0x01	:	*kRes = 2; break;
		case 0x02	:	*kRes = 4; break;
		case 0x03	:	*kRes = 8; break;
		default		:	return false;
	}

	// TCR is enabled if we got this far
	*numDiodes = (b & 0x0C) >> 2;

	return true;
}

// Write num elements of user memory, starting at addr
// Will fail and return false if write does not fall within the bounds of user memory space
// Will return true provided write is valid
BOOL DS1306_writeUser(unsigned char addr, const char *buf, int num)
{
	if (addr >= DS1306_USER_START && addr < DS1306_USER_END && (addr + num - 1) <= DS1306_USER_END) {
		DS1306_writen(addr, (const unsigned char *) buf, num);
		return true;
	} else {
		return false;
	}
}

// Read num elements of user memory, starting at addr
// Will fail and return false if write does not fall within the bounds of user memory space
// Will return true provided read is valid
BOOL DS1306_readUser(unsigned char addr, char *buf, int num)
{
	memset((__data *)buf, 0, num);
	if (addr >= DS1306_USER_START && addr < DS1306_USER_END && (addr + num - 1) <= DS1306_USER_END) {
		DS1306_readn(addr, (unsigned char *) buf, num);
		return true;
	} else {
		return false;
	}
}

// Returns true if DS1306 is write protected
BOOL DS1306_isWriteProtected()
{
	return ((DS1306_read(DS1306_CR) & (1 << DS1306_CR_WP)) ? true : false);
}

// Set's the write protection on (true) or off for the DS1306
void DS1306_setWriteProtection(BOOL on)
{
	unsigned char cr = DS1306_read(DS1306_CR);
	if (on) {
		cr |= (1 << DS1306_CR_WP);
	} else {
		cr &= ~ (1 << DS1306_CR_WP);
	}
	DS1306_write(DS1306_CR, cr);
}

// Get state of 1hz pin
BOOL DS1306_get1HzState()
{
	unsigned char cr = DS1306_read(DS1306_CR);
	return ((cr & (1 << DS1306_CR_1HZ)) ? true : false);
}

// Set state of 1hz pin
void DS1306_set1HzState(BOOL enabled)
{
	unsigned char cr = DS1306_read(DS1306_CR);
	if (enabled) {
		cr |= (1 << DS1306_CR_1HZ);
	} else {
		cr &= ~ (1 << DS1306_CR_1HZ);
	}
	DS1306_write(DS1306_CR, cr);
}
