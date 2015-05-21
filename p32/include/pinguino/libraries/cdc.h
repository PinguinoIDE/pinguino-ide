/*  --------------------------------------------------------------------
    FILE:               cdc.h
    PROJECT:            pinguino 32
    PURPOSE:            usb cdc module based on the Microchip USB stack
    PROGRAMERS:         Jean-Pierre Mandon <jp.mandon@gmail.com>
    --------------------------------------------------------------------
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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
----------------------------------------------------------------------*/

#ifndef __CDC_H
#define __CDC_H

void USBCheckCDCRequest();
void CDCInitEP();
//void USBDeviceInit();
void USBDeviceAttach();
void putUSBUSART(char *, char);
char getsUSBUSART(char *, char);
void CDCTxService(void);

#endif
