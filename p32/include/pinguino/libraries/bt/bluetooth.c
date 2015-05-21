/*	----------------------------------------------------------------------------
	FILE:			bluetooth.c
	PROJECT:		pinguino
	PURPOSE:		BGB203 basic functions
	PROGRAMER:		regis blabnchot <rblanchot@gmail.com>
	FIRST RELEASE:	28 oct. 2011
	LAST RELEASE:	04 mar. 2012
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

#ifndef __BLUETOOTH_C
#define __BLUETOOTH_C

#include <stdlib.h>				// malloc, ...
#include <stdarg.h>				// variable args
#include <string.h>				// strlen, ...
#include <typedef.h>			// u8, u32, ... definitions
#include <digitalw.c>			// toggle
#include <serial.c>				// Serial functions
#include <delay.c>				// DelayMs
#include <itoa.c>				// ultoa()
#include <__cdc.c>

#define BT_DELAY		10		// Delay (ms) before sending a new command to the BT module
#define CRLF			"/r/n"	// <CR><LF>

// BT return codes
typedef enum
{
	BT_OK		=	111,
	BT_ERROR,
	BT_COMPLETE
} BT_STATUS;

// BT response
typedef struct
{
	u8 *command;
	u8 *data;
	u8 *status;			// OK or ERROR
	BT_STATUS code;		// BT_OK or BT_ERROR
} BT_RESPONSE;

//	----------------------------------------------------------------------------
//	Get a complete response from the module
//	----------------------------------------------------------------------------

u8 *BT_getBuffer(u8 uart_port, u8 *buffer)
{
	u8 c, i = 0;
	
	// wait until something is received
	while (!SerialAvailable(uart_port));

	do {
		c = SerialRead(uart_port);			// return 255 (-1) if no reception
		if (c != 255) buffer[i++] = c;
	} while (c != 255);// && (i < 80) );
	// C string must be null-terminated
	//BT_BUFFER[i] = '\0';
	buffer[i] = '\0';
	//CDCprintf("buffer = [%s]\r\n", BT_BUFFER);

	//return BT_BUFFER;
	return buffer;
}

//	----------------------------------------------------------------------------
//	Get status response
//	----------------------------------------------------------------------------

BT_STATUS BT_getStatusCode(u8 * status)
{
	u8 length;
	BT_STATUS code;
	
	length = strlen(status);
	
	// check last char if status is OK, ERROR or COMPLETE
	switch (status[length-1])
	{
		case 'K': return BT_OK;
		case 'R': return BT_ERROR;
		case 'E': return BT_COMPLETE;
	}
}

//	----------------------------------------------------------------------------
//	Get a short response from the module
//	<CR><LF><status><CR><LF>
//	with status = OK or ERROR
//	----------------------------------------------------------------------------

BT_RESPONSE BT_getResponse(u8 uart_port, BT_RESPONSE response)
{
	//u8 *buffer;
	u8 *buffer = (u8*) malloc(128);

	// get a complete response from the module
	buffer = BT_getBuffer(uart_port, buffer);
	//CDCprintf("buffer=[%s]", buffer);

	response.command = NULL;
	response.data    = strtok(buffer, CRLF); // to remove the first CRLF 
	response.data    = strtok(NULL, CRLF);
	response.status  = strtok(NULL, CRLF);
	response.code    = BT_getStatusCode(response.status);

	return response;
}

//	----------------------------------------------------------------------------
//	Get an extended response from the module
//	<CR><LF><command><delimiter><data><CR><LF><status><CR><LF>
//	----------------------------------------------------------------------------

BT_RESPONSE BT_getExtendedResponse(u8 uart_port)
{
	u8 length;
	u8 *buffer;
	BT_RESPONSE response;
	
	// get a complete response from the module
	//buffer = BT_getBuffer(uart_port, buffer);

	// response's format is :
	//    <CR><LF><status><CR><LF> with status = OK or ERROR
/*	
	response.command = strtok(buffer, CRLF); // to remove the first CRLF 
	response.command = strtok(buffer, CRLF);
	response.data    = strtok(NULL, CRLF);
	response.status  = strtok(NULL, CRLF);
	response.code    = BT_getStatusCode(response.status);

	return response;
*/
}

//	----------------------------------------------------------------------------
//	Send an AT command to the module over the UART Port
//	----------------------------------------------------------------------------

BT_RESPONSE BT_sendCommand(u8 uart_port, u8 *fmt, ...)
{
	SerialFlush(uart_port);
	SerialPrintf(uart_port, fmt);
	//CDCprintf(fmt);
	Delayms(BT_DELAY);
}

//	----------------------------------------------------------------------------
//	Get into command mode
//	----------------------------------------------------------------------------

BT_RESPONSE BT_setCommandMode(u8 uart_port)
{
	BT_sendCommand(uart_port,"+++\r");
	BT_sendCommand(uart_port,"+++\r");
	BT_sendCommand(uart_port,"+++\r");
	//return BT_getResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Configures the device not to echo received characters in command mode
//	----------------------------------------------------------------------------

BT_RESPONSE BT_echoOff(u8 uart_port)
{
	BT_sendCommand(uart_port,"ATE0\r");
	//return BT_getResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Configures the device to echo received characters in command mode
//	----------------------------------------------------------------------------

BT_RESPONSE BT_echoOn(u8 uart_port)
{
	BT_sendCommand(uart_port,"ATE1\r");
	//return BT_getResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Restore the current configuration settings back to the settings
//	that were stored by the Factory Settings tool
//	or settings that were stored to Flash.
//	----------------------------------------------------------------------------

BT_RESPONSE BT_restore(u8 uart_port)
{
	BT_sendCommand(uart_port,"AT&F\r");
	//return BT_getResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Restore the current configuration settings back to internal default values
//	----------------------------------------------------------------------------

BT_RESPONSE BT_reset(u8 uart_port)
{
	BT_sendCommand(uart_port,"ATZ\r");
	//return BT_getResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Ask the module for its firmware version 
//	----------------------------------------------------------------------------

BT_RESPONSE BT_getFirmware(u8 uart_port)
{
	BT_sendCommand(uart_port,"ATI\r");
	//return BT_getExtendedResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Change module name
//	----------------------------------------------------------------------------

BT_RESPONSE BT_setDeviceName(u8 uart_port, u8 * name)
{
	BT_sendCommand(uart_port,"AT+BTLNM=\"%s\"\r", name); 
	return BT_getExtendedResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Get module name
//	----------------------------------------------------------------------------

BT_RESPONSE BT_getDeviceName(u8 uart_port)
{
	BT_sendCommand(uart_port,"AT+BTLNM?\r");
	return BT_getExtendedResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Set bluetooth device address (0x01)
//	----------------------------------------------------------------------------

BT_RESPONSE BT_setDeviceAddress(u8 uart_port, u8 * bdaddr)
{
	BT_sendCommand(uart_port,"AT+BTSET=1,%s\r", bdaddr);
	return BT_getExtendedResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Get bluetooth device address
//	----------------------------------------------------------------------------

BT_RESPONSE BT_getDeviceAddress(u8 uart_port)
{
	BT_sendCommand(uart_port,"AT+BTBDA?\r");
	return BT_getExtendedResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Allows automatic Bluetooth connection
//	----------------------------------------------------------------------------

BT_RESPONSE BT_setAutoConnection(u8 uart_port)
{
	BT_sendCommand(uart_port,"AT+BTAUT=1,0\r");
	return BT_getExtendedResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	
//	----------------------------------------------------------------------------

BT_RESPONSE BT_setUARTSpeed(u8 uart_port, u32 baud_rate)
{
	char *string;

	// Change UART settings (baud rate, data bits, stop bits, parity, stop bits, flow control)
	// Enable RTS/CTS, DTR/DSR Flow control
	BT_sendCommand(uart_port,"AT+BTURT=%s,8,0,1,3\r", ultoa(baud_rate, string, 10));
	return BT_getExtendedResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Disable security process when pairing
//	----------------------------------------------------------------------------

BT_RESPONSE BT_disableSecurity(u8 uart_port)
{
	BT_sendCommand(uart_port,"AT+BTSEC=0\r");
	return BT_getExtendedResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Writes above setting to the BGB203 flash memory
//	----------------------------------------------------------------------------

BT_RESPONSE BT_writeFlash(u8 uart_port)
{
	BT_sendCommand(uart_port,"AT+BTFLS\r");
	return BT_getExtendedResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Start Bluetooth Server
//	----------------------------------------------------------------------------

BT_RESPONSE BT_start(u8 uart_port)
{
	BT_sendCommand(uart_port,"AT+BTSRV=1\r");
	return BT_getExtendedResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Enter deep sleep mode
//	----------------------------------------------------------------------------

BT_RESPONSE BT_sleep(u8 uart_port)
{
	BT_sendCommand(uart_port,"AT+BTSLP\r");
	return BT_getExtendedResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Search for new devices for s seconds
//	----------------------------------------------------------------------------

BT_RESPONSE BT_search(u8 uart_port, u8 s)
{
	// cancels any current commands
	BT_sendCommand(uart_port,"AT+BTCAN\r");
	BT_sendCommand(uart_port,"AT+BTINQ=%d\r", s);
	return BT_getExtendedResponse(uart_port);
}

//	----------------------------------------------------------------------------
//	Initialize the BGB203 Bluetooth module
//	return : name of the device (Pinguino)
//	----------------------------------------------------------------------------

BT_RESPONSE BT_init(u8 uart_port, u32 baud_rate)
{
	BT_RESPONSE response;

	// 115200 bauds is the default configuration value
	//SerialConfigure(uart_port, UART_ENABLE, UART_RX_TX_ENABLED,	115200);

	response = BT_setCommandMode(uart_port); 
	//BT_getFirmware(uart_port);
	//BT_echoOff(uart_port);
	//BT_restore(uart_port);
	//BT_setDeviceName(uart_port, "Pinguino");
	// Pass through, DCE, enable escape sequence, disable entering in command mode with DTR/DSR, enable LED
	//BT_sendCommand(uart_port,"AT+BTCFG=33");
	//BT_setAutoConnection(uart_port);
/*
	if (baud_rate != 115200)
	{
		BT_setUARTSpeed(uart_port, baud_rate);
		// new UART speed
		SerialConfigure(uart_port, UART_ENABLE, UART_RX_TX_ENABLED,	baud_rate);
	}
*/
	//BT_disableSecurity(uart_port);
	//BT_writeFlash(uart_port);
	//BT_start(uart_port);
	return response;
}

//	----------------------------------------------------------------------------
//
// 0x00 Delete any stored link key
// 0x01 Pair with remote device (initiate)
// 0x02 Allow another device to pair (wait for pair)
// 0x03 Configure Bluetooth address/Link key pair
//	----------------------------------------------------------------------------

BT_RESPONSE BT_connect(u8 uart_port, u8 * bdaddr)
{
	BT_RESPONSE response;
	
	BT_sendCommand(uart_port,"AT+BTPAR=1,%s\r", bdaddr); // Pair with remote device
	response = BT_getExtendedResponse(uart_port);
	if (response.code == BT_ERROR) return response;
	BT_sendCommand(uart_port,"AT+BTCLT=%s,1\r", bdaddr); // on Port 1
	response = BT_getExtendedResponse(uart_port);
	// the device will enter data mode if OK
	return response;
}

#endif /* __BLUETOOTH_C */
