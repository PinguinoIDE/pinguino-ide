/*	----------------------------------------------------------------------------
	FILE:			bluetooth.c
	PROJECT:		pinguinoX
	PURPOSE:		
	PROGRAMER:		regis blabnchot <rblanchot@gmail.com>
	FIRST RELEASE:	28 oct. 2011
	LAST RELEASE:	
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
	
/*
	The library delay.c and the led toggle
	have been commented out for the MX220
	to make the compiled code smaller
*/

#ifndef __BLUETOOTH_C
#define __BLUETOOTH_C

#include <stdarg.h>				// variable args
#include <typedef.h>			// u8, u32, ... definitions
//#include <digitalw.c>			// toggle
#include <serial.c>				// Serial functions
//#include <delay.c>			// DelayMs
#include <itoa.c>				// ultoa()

//#define BT_DELAY		50		// Delay (ms) before sending a new command to the BT module

// BT return codes
#define BT_OK			111
#define BT_ERROR		112
#define BT_COMPLETE	113

u8 BT_ok(u8 uart_port)
{
	u8 i, ret;

	while ( !(ret = SerialRead(uart_port)) );
	switch(ret)
	{
		case 'O':// "OK"
			/*for (i=0; i<10; i++)
			{
				toggle(13);
				Delayms(100);
			}*/
			return BT_OK;
		case 'C':// "COMPLETE"
			return BT_COMPLETE;
		case 'E':// "ERROR"
			return BT_ERROR;
		default:
			return BT_ERROR;
	}
}

void BT_send(u8 uart_port, char *fmt,...)
{
	do {
		SerialPrintf(uart_port, fmt);
		//Delayms(BT_DELAY);
	} while ( BT_ok(uart_port) != BT_OK );
}

// Restore the current configuration settings back to the settings
// that were stored by the Factory Settings tool
// or settings that were stored to Flash.
void BT_restore(u8 uart_port)
{
	BT_send(uart_port,"AT&F\r\n");
}

// Restore the current configuration settings back to internal default values
void BT_reset(u8 uart_port)
{
	BT_send(uart_port,"ATZ\r\n");
}

// Change module name
void BT_setDeviceName(u8 uart_port, u8 * name)
{
	BT_send(uart_port,"AT+BTLNM=\"%s\"\r\n", name); 
}

// Get module name
u8 * BT_getDeviceName(u8 uart_port)
{
	BT_send(uart_port,"AT+BTLNM?\r\n");
/*
	while (!UART2SerialBuffer);
	return UART2SerialBuffer;
*/
}

// Set bluetooth device address (0x01)
void BT_setDeviceAddress(u8 uart_port, u8 * bdaddr)
{
	BT_send(uart_port,"AT+BTSET=1,%s\r\n", bdaddr);
}

// Get bluetooth device address
u8 * BT_getDeviceAddress(u8 uart_port)
{
	BT_send(uart_port,"AT+BTBDA?\r\n");
/*
	while (!UART2SerialBuffer);
	return UART2SerialBuffer;
*/
}

// Allows automatic Bluetooth connection
void BT_setAutoConnection(u8 uart_port)
{
	BT_send(uart_port,"AT+BTAUT=1,0\r\n");
}

void BT_setUARTSpeed(u8 uart_port, u32 baud_rate)
{
	char *string;

	// Change UART settings (baud rate, data bits, stop bits, parity, stop bits, flow control)
	// Enable RTS/CTS, DTR/DSR Flow control
	BT_send(uart_port,"AT+BTURT=%s,8,0,1,3\r\n", ultoa(baud_rate, string, 10));
}

// Disable security process when pairing
void BT_disableSecurity(u8 uart_port)
{
	BT_send(uart_port,"AT+BTSEC=0\r\n");
}

// Writes above setting to the BGB203 flash memory
void BT_writeFlash(u8 uart_port)
{
	BT_send(uart_port,"AT+BTFLS\r\n");
}

// Start Bluetooth Server
void BT_start(u8 uart_port)
{
	BT_send(uart_port,"AT+BTSRV=1\r\n\r\n");
}

// Get into command mode
void BT_setCommandMode(u8 uart_port)
{
	BT_send(uart_port,"+++");
}

// Configures the device not to echo received characters in command mode
void BT_echoOff(u8 uart_port)
{
	BT_send(uart_port,"ATE0\r\n");
}

// Configures the device to echo received characters in command mode
void BT_echoOn(u8 uart_port)
{
	BT_send(uart_port,"ATE1\r\n");
}

// Enter deep sleep mode
void BT_sleep(u8 uart_port)
{
	BT_send(uart_port,"AT+BTSLP\r\n");
}

u8 BT_init(u8 uart_port, u32 baud_rate)
{
	SerialConfigure(uart_port, UART_ENABLE, UART_RX_TX_ENABLED,	baud_rate);
//BT_echoOff(uart_port);
	BT_setCommandMode(uart_port); 
	BT_restore(uart_port);
	BT_setDeviceName(uart_port, "Pinguino");
// Pass through, DCE, enable escape sequence, disable entering in command moode with DTR/DSR, enable LED
//BT_send(uart_port,"AT+BTCFG=33\r\n");
	BT_setAutoConnection(uart_port);
	BT_setUARTSpeed(uart_port, baud_rate);
	BT_disableSecurity(uart_port);
	BT_writeFlash(uart_port);
	BT_start(uart_port);

	return ( BT_ok(uart_port) );
}

// 0x00 Delete any stored link key
// 0x01 Pair with remote device (initiate)
// 0x02 Allow another device to pair (wait for pair)
// 0x03 Configure Bluetooth address/Link key pair

u8 BT_connect(u8 uart_port, u8 * bdaddr)
{
	BT_send(uart_port,"AT+BTPAR=1,%s\r\n", bdaddr); // Pair with remote device
	BT_send(uart_port,"AT+BTCLT=%s,1\r\n", bdaddr); // on Port 1
	// the device will enter data mode if OK
	return ( BT_ok(uart_port) );
}

#endif /* __BLUETOOTH_C */

