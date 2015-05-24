/*
 * File			DS1306.h
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



#ifndef __DS1306_RTC_
#define __DS1306_RTC_

/* Memory Locations */
#define DS1306_DATETIME			0x00
#define DS1306_ALARM0			0x07
#define DS1306_ALARM1			0x0B
#define DS1306_CR 				0x0F
#define DS1306_SR				0x10
#define DS1306_TCR				0x11
#define DS1306_USER_START		0x20
#define DS1306_USER_END			0x7F

/* Buffer sizes required for reading / writing time/date and alarms to the RTC */
#define DS1306_SIZE_DATETIME	7
#define DS1306_SIZE_ALARM		4

/* Bit Position of key register parameters (CR) */
#define DS1306_CR_WP			6
#define DS1306_CR_1HZ			2
#define DS1306_CR_AIE1			1
#define DS1306_CR_AIE0			0

/* Bit Position of key register parameters (SR) */
#define DS1306_SR_IRQF1			1
#define DS1306_SR_IRQF0			0

/* Bit Positions of key register parameters (TCR) */
#define DS1306_TCR_TCS			7
#define DS1306_TCR_DS			3
#define DS1306_TCR_RS			1

/* Days of week (suggested, see spec) */
#define DS1306_SUNDAY			1
#define DS1306_MONDAY			2
#define DS1306_TUESDAY			3
#define DS1306_WEDNESDAY		4
#define DS1306_THURSDAY			5
#define DS1306_FRIDAY			6
#define DS1306_SATURDAY			7

/* "Any" designator, for alarms */
#define DS1306_ANY				0x80

/* Write offset used when writing DS1306 registers */
#define DS1306_WRITE_OFFSET		0x80

/* Representation of the current time/date */
typedef struct {
	unsigned char seconds;
	unsigned char minutes;
	unsigned char hours;
	unsigned char hours12;
	char ampm;					// 'A' = AM, 'P' = PM, anything else is hours12 undefined
	unsigned char dow;
	unsigned char day;
	unsigned char month;
	unsigned char year;
} ds1306time;

/* Representation of an alarm */
typedef struct {
	unsigned char seconds;
	unsigned char minutes;
	unsigned char hours;
	unsigned char hours12;
	char ampm;					// 'A' = AM, 'P' = PM, anything else is hours12 undefined
	unsigned char dow;
} ds1306alarm;

	// Initialize DS1306 using ce as chip enable line, turn on osc
	void DS1306_Init(u8 LocalSPI, u8 LoCE,u8 LoSCK, u8 LoSDO, u8 LoSDI);

        // Time set uses hours (when DS1306_writeHours24 = true) (default), hours12/ampm (when DS1306_writeHours24 = false)
        void DS1306_Hours24(BOOL Hours24);

	// Primary clock (time/date) operations
	void DS1306_setTime(const ds1306time *time);
	void DS1306_getTime(ds1306time *time);

	// Alarm management operations
	void DS1306_setAlarm(int alarm, const ds1306alarm *time);
	void DS1306_getAlarm(int alarm, ds1306alarm *time);
	BOOL DS1306_getAlarmState(unsigned int alarm);
	void DS1306_getAlarmBothState(BOOL *state1, BOOL *state2);
	void DS1306_clearAlarmState(unsigned int alarm);
	void DS1306_clearAlarmBothState();
	BOOL DS1306_getAlarmEnabled(unsigned int alarm);
	void DS1306_getAlarmBothEnabled(BOOL *enabled1, BOOL *enabled2);
	void DS1306_enableAlarm(unsigned int alarm);
	void DS1306_disableAlarm(unsigned int alarm);
	void DS1306_enableBothAlarms();
	void DS1306_disableBothAlarms();

	// 1Hz state
	BOOL DS1306_get1HzState();
	void DS1306_set1HzState(BOOL enabled);

	// Trickle charge management
	BOOL DS1306_enableTrickleCharge(unsigned char numDiodes, unsigned char kRes);
	void DS1306_disableTrickleCharge();
	BOOL DS1306_getTrickleChargeState(unsigned char *numDiodes, unsigned char *kRes);

	// User memory management
	BOOL DS1306_writeUser(unsigned char addr, const char *buf, int num);
	BOOL DS1306_readUser(unsigned char addr, char *buf, int num);

	// Write Protection management
	BOOL DS1306_isWriteProtected();
	void DS1306_setWriteProtection(BOOL on);

	// Direct Register access (use for direct access to registers, if needed)
	void DS1306_readn(unsigned char address, unsigned char *data, int len);
	unsigned char DS1306_read(unsigned char address);
	void DS1306_writen(unsigned char address, const unsigned char *data, int len);
	void DS1306_write(unsigned char address, const unsigned char value);


	// Encode a time / alarm packet
	void DS1306_encodeTimePacket(unsigned char *buf, const ds1306time *time);
	void DS1306_encodeAlarmPacket(unsigned char *buf, const ds1306alarm *alarm);

	// Decode a time / alarm packet
	void DS1306_decodeTimePacket(const unsigned char *buf, ds1306time *time);
	void DS1306_decodeAlarmPacket(const unsigned char *buf, ds1306alarm *alarm);

	// Hour parameter management
	void DS1306_decodeHourByte(unsigned char hourByte, unsigned char *hour24, unsigned char *hour12, char *ampm);
	unsigned char DS1306_encodeHourByte(unsigned char hour24, unsigned char hour12, char ampm);

	// Parameter encode / decode
	unsigned char DS1306_encodeBCD7(unsigned char value);
	unsigned char DS1306_encodeBCD7M(unsigned char value, unsigned char mask);
	unsigned char DS1306_encodeBCD8(unsigned char value);
	unsigned char DS1306_decodeBCD7(unsigned char value);
	unsigned char DS1306_decodeBCD7M(unsigned char value, unsigned char mask);
	unsigned char DS1306_decodeBCD8(unsigned char value);

 
#endif /* __DS1306_RTC_ */ 
