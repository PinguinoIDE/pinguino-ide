#!/usr/bin/env python
#-*- coding: utf-8 -*-
#-*- coding: iso-8859-15 -*-

"""---------------------------------------------------------------------
    Pinguino ICSP Uploader

    Author:         Regis Blanchot <rblanchot@gmail.com>
    --------------------------------------------------------------------
    2018-05-26 - RB - first release   

    --------------------------------------------------------------------
    This library is free software you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA  02110-1301  USA
---------------------------------------------------------------------"""

###  ___
### |   |---1---VPP/MCRL
### | I |---2---VDD
### | C |---3---GND
### | S |---4---PGD
### | P |---5---PGC
### |___|---6---PGM

import sys
import os
#import usb            # checked in check.py

from .uploader import baseUploader

class uploaderICSP(baseUploader):
    """-----------------------------------------------------------------
    Upload .hex into Pinguino Devices via ICSP.
    Supported programmers are :
    - Pickit 2
    - Pickit 3
    -----------------------------------------------------------------"""

    ### Device IDs
    MICROCHIP_VENDOR_ID     = 0x04D8
    PICKIT2_PRODUCT_ID      = 0x0033
    PICKIT3_PRODUCT_ID      = 0x900A

    ### Endpoints
    PK2_INT_WRITE           = 0x01
    PK2_INT_READ            = 0x81
    INT_PKT_SZ              = 0x40

    ### Commands
    PK2_SET_POS             = 0xB9
    PK2_SETUP               = 0xB8
    PK2_READ                = 0xAC
    PK2_PADDING             = 0xAD
    PK2_RUN_SCRIPT          = 0xA6
    PK2_CHECK_VOLTAGE       = 0xA3
    PK2_CHECK_STATUS        = 0xA2
    PK2_SET_VPP             = 0xA1 # Logic Input Level
    PK2_SET_VDD             = 0xA0 # Supply Level

    PK2_NOP                 = 0x5A
    PK2_GET_VERSION         = 0x76
    PK2_GET_VERSION_MPLAB   = 0x41
    PK2_SET_VDD_4(v)        = 0xA0, (uint8_t)((v)*2048+672), (uint8_t)(((v)*2048+672)/256), (uint8_t)((v)*36)
    PK2_SET_VDD_PK3(v)      = 0xA0, (uint8_t)(v / 0.125F), (uint8_t)(v / 0.125F /256)
    PK2_SET_VPP_4(v)        = 0xA1, 0x40, (uint8_t)((v)*18.61), (uint8_t)((v)*13)
    PK2_SET_VPP_PK3(v)      = 0xA1, (uint8_t)(v / 0.125F), (uint8_t)(v / 0.125F /256)
    PK2_READ_STATUS         = 0xA2
    PK2_READ_VDD_VPP        = 0xA3
    PK2_DOWNLOAD_SCRIPT(idx, len)    0xA4, (idx), (len)
    PK2_RUN_SCRIPT(idx, cnt)    0xA5, (idx), (cnt)
    PK2_EXEC_SCRIPT_2(len)  = 0xA6, (len)
    PK2_CLR_DLOAD_BUFF      = 0xA7
    PK2_DOWNLOAD_DATA_2(len)  0xA8, (len)
    PK2_CLR_ULOAD_BUFF      = 0xA9
    PK2_UPLOAD_DATA         = 0xAA
    PK2_CLR_SCRIPT_BUFF     = 0xAB
    PK2_UPLOAD_DATA_NO_LEN  = 0xAC
    PK2_END_OF_BUFFER       = 0xAD
    PK2_RESET               = 0xAE

    ### Scripting Commands (from pk2cmd source, only the used ones are here)
    PK2_SCR_VPP_ON          = 0xFB
    PK2_SCR_VPP_OFF         = 0xFA
    PK2_SCR_VPP_PWM_ON      = 0xF9
    PK2_SCR_MCLR_GND_ON     = 0xF7
    PK2_SCR_MCLR_GND_OFF    = 0xF6
    PK2_SCR_SET_ICSP_PINS   = 0xF3
    PK2_SCR_DELAY_LONG      = 0xE8
    PK2_SCR_ICSP_STATE_BUF  = 0xDC
    PK2_SCR_SET_AUX         = 0xCF
    PK2_SCR_AUX_BUF         = 0xCE

    PK2_SCR_VDD_ON          = 0xFF
    PK2_SCR_VDD_OFF         = 0xFE
    PK2_SCR_VDD_GND_ON      = 0xFD
    PK2_SCR_VDD_GND_OFF     = 0xFC
    PK2_SCR_VPP_ON          = 0xFB
    PK2_SCR_VPP_OFF         = 0xFA
    PK2_SCR_VPP_PWM_ON      = 0xF9
    PK2_SCR_VPP_PWM_OFF     = 0xF8
    PK2_SCR_MCLR_GND_ON     = 0xF7
    PK2_SCR_MCLR_GND_OFF    = 0xF6
    PK2_SCR_BUSY_LED_ON     = 0xF5
    PK2_SCR_BUSY_LED_OFF    = 0xF4
    PK2_SCR_WRITE_BYTE_LITERAL(v)	0xF2, (v)
    PK2_SCR_WRITE_BYTE_BUFFER	0xF1
    PK2_SCR_READ_BYTE_BUFFER	0xF0
    PK2_SCR_READ_BYTE       = 0xEF
    PK2_SCR_WRITE_BITS_LITTERAL(b, v)	0xEE, (b), (v)
    PK2_SCR_WRITE_BITS_BUFFER(v)	0xED, (v)
    PK2_SCR_READ_BITS_BUFFER(v)		0xEC, (v)
    PK2_SCR_READ_BITS(v)			0xEB, (v)
    PK2_SCR_SET_ICSP_SPEED(us) 0xEA,(us)
    PK2_SCR_SET_ICSP_DELAY_2(us) 0xEA,(us)
    PK2_SCR_SET_PINS_2(dd, cd, dv, cv) 0xF3, (((cd)!=0) | (((dd)!=0)<<1) | (((cv)!=0)<<2) | (((dv)!=0)<<3))
    PK2_SCR_GET_PINS        = 0xDC
    PK2_SCR_LOOP_3(rel, cnt)    0xE9, rel, cnt
    PK2_SCR_DELAY_2(sec)    ((sec)>0.0054528?0xE8:0xE7), (uint8_t)((sec)>0.0054528?(.999+(sec)/.00546):(.999+(sec)/.0000213))
    ### SPI commands
    PK2_SCR_SET_AUX_2(ad, av)   0xCF, (((ad)!=0) | (((av)!=0)<<1))
    PK2_SCR_SPI_SETUP_PINS_4    SCR_SET_PINS_2(1,0,0,0), SCR_SET_AUX_2(0,0)
    PK2_SCR_SPI             = 0xC3
    PK2_SCR_SPI_LIT_2(v)    = 0xC7, (v)
    SPI_WR_BYTE_BUF         = 0xC6
    SPI_RD_BYTE_BUF         = 0xC5
    PK2_SCR_SPI_RDWR_BYTE_LIT(v)	0xC4, (v)
    PK2_SCR_SPI_RDWR_BYTE_BUF   = 0xC3
    ### I2C commands
    PK2_SCR_I2C_START           = 0xCD
    PK2_SCR_I2C_STOP            = 0xCC
    PK2_SCR_I2C_WR_BYTE_LIT(v)  = 0xCB, (v)
    PK2_SCR_I2C_WR_BYTE_BUF     = 0xCA
    PK2_SCR_I2C_RD_BYTE_ACK     = 0xC9
    PK2_SCR_I2C_RD_BYTE_NACK    = 0xC8

    ### Pin Constants for I/O
    PK2_IO_ICSP_1_IN        = 0x02
    PK2_IO_ICSP_1_SET       = 0x08
    PK2_IO_ICSP_2_IN        = 0x01
    PK2_IO_ICSP_2_SET       = 0x04
    PK2_IO_AUX_1_IN         = 0x01
    PK2_IO_AUX_1_SET        = 0x02

    ### Banks
    PK2_BANK_0              = 0x06
    PK2_BANK_1              = 0x07

    ### Samples taken
    PK2_SAMPLES             = 1024

    ### Cursors
    PK2_CURSOR1             = 1
    PK2_CURSOR2             = 2
    PK2_CURSOR_BOTH         = 3

    ### Unit is the device matched that we want to use
    def find_device(self, unit):
        devices = []
        ### Find the PicKit devices plugged in right now
        for bus in usb.busses() :
            for device in bus.devices :
                if device.idVendor == MICROCHIP_VENDOR_ID:
                    if device.idProduct == PICKIT2_PRODUCT_ID:
                        self.add_report("Pickit 2 found ...")
                        devices.append(device)
                    if device.idProduct == PICKIT3_PRODUCT_ID:
                        self.add_report("Pickit 3 found ...")
                        devices.append(device)

        ### Make sure we have at least one!
        if   len(devices) < 1:
            sys.stderr.write("No PicKit USB device found!\n")
            sys.exit(-1)
        elif len(devices) > 1:
            sys.stderr.write("Several PicKit USB devices found!\n")
            sys.exit(-1)
        else:
            return devices[unit]

    ### Get the VDD and VPP levels
    def check_voltages(self):
        done = False
        try:
            ### Send the command
            msg = chr(PK2_CHECK_VOLTAGE) + chr(PK2_PADDING) * 63
            self.devhandle.interruptWrite(PK2_INT_WRITE, msg)
        except:
            pass
    
        while not done:
            try:
                ret = self.devhandle.interruptRead(PK2_INT_READ, INT_PKT_SZ)
            ### We ignore failures here because we're just polling for
            ### the return from the device
            except:
                pass
            else:
                done = True

        ### This equation is from the pk2cmd program from microchip
        vdd = (float(ret[1] << 8 | ret[0]) / 0x10000) * 5.0
        vpp = (float(ret[3] << 8 | ret[2]) / 0x10000) * 13.7
        return vdd, vpp

    ### Set the vpp voltage level
    ### This function is based on one from pk2cmd
    def set_vpp(self, voltage, threshold):
        ### Message Header
        msg = chr(PK2_SET_VPP) + chr(0x40)
        ### Find ADC threshold
        adc = int(float(voltage) * 18.61)
        ### Find Fault threshold
        fault = int(float(voltage * threshold) * 18.61)

        ### Send the msg to the device
        try:
            msg = msg + chr(adc) + chr(fault) + chr(PK2_PADDING) * 60
            self.devhandle.interruptWrite(PK2_INT_WRITE, msg)
        except:
            pass
    
    ### Set the vdd voltage level
    ### This function is based on one from pk2cmd
    def set_vdd(self, voltage, threshold):
        ### Voltage must be >= 2.5, setting too low prevents vpp pump from working
        if voltage < 2.5:
            voltage = 2.5
        ### Find the capture and compare value
        ccp = int(float(voltage * 32) + 10.5) << 6
        ### Find the fault value
        fault = int((float(threshold * voltage) / 5) * 255)
        ### ~4.12v maximum due to diode droop
        if fault > 210:
            fault = 210

        ### Message Header
        msg = chr(PK2_SET_VDD)
        ### Send the msg to the device
        try:
            msg = msg + chr(ccp & 0xFF) + chr((ccp >> 8) & 0xFF) + chr(fault & 0xFF) + chr(PK2_PADDING) * 60
            self.devhandle.interruptWrite(PK2_INT_WRITE, msg)
        except:
            pass
    
    ### Checks the status of the device, a list of the possible responses
    ### can be found in the IO Probe text file
    def check_status(self):
        ### Check the device status
        msg = chr(PK2_CHECK_STATUS) + chr(PK2_PADDING) * 63
        try:
            ### Send status msg
            self.devhandle.interruptWrite(PK2_INT_WRITE, msg)
        except:
            pass

        ### Get status back
        done = False
        while not done:
            try:
                ret = self.devhandle.interruptRead(PK2_INT_READ, INT_PKT_SZ)
            ### We ignore failures here because we're just polling for
            ### the return from the device
            except:
                pass
            else:
                done = True

        return (ret[1] << 8) | ret[0]
        
    #######################################################################
    ### I/O Probe Components
    #######################################################################
    ### Generate the byte used to set the ICSP Pins
    def gen_icsp_byte(self):
        icsp_pins = 0

        ### Channel 1
        if self.chan_io[1] == "Input":
            icsp_pins = icsp_pins | PK2_IO_ICSP_1_IN
        elif self.chan_val[1] == 1:
            icsp_pins = icsp_pins | PK2_IO_ICSP_1_SET
        
        ### Channel 2
        if self.chan_io[2] == "Input":
            icsp_pins = icsp_pins | PK2_IO_ICSP_2_IN
        elif self.chan_val[2] == 1:
            icsp_pins = icsp_pins | PK2_IO_ICSP_2_SET

        return icsp_pins
    
    ### Generate the byte used to set the AUX Pins
    def gen_aux_byte(self):
        aux_pins = 0

        ### Aux pin
        if self.chan_io[3] == "Input":
            aux_pins = aux_pins | PK2_IO_AUX_1_IN
        elif self.chan_val[3] == 1:
            aux_pins = aux_pins | PK2_IO_AUX_1_SET
        return aux_pins

    ### Setup the I/O Module
    def io_setup(self):
        msg = chr(PK2_RUN_SCRIPT)
        ### 9 Commands in this script
        msg = msg + chr(0x09)
        ### Set Ch. 0 to 0/1
        if self.chan_val[0] == 0:
            msg = msg + chr(PK2_SCR_VPP_OFF)
            msg = msg + chr(PK2_SCR_MCLR_GND_ON)
        else:
            msg = msg + chr(PK2_SCR_VPP_ON)
            msg = msg + chr(PK2_SCR_MCLR_GND_OFF)
        ### Turn on VPP PWM
        msg = msg + chr(PK2_SCR_VPP_PWM_ON)
        ### Set ICSP Pins
        msg = msg + chr(PK2_SCR_SET_ICSP_PINS) + chr(self.gen_icsp_byte())
        ### Set AUX Pins
        msg = msg + chr(PK2_SCR_SET_AUX) + chr(self.gen_aux_byte())
        ### Delay
        msg = msg + chr(PK2_SCR_DELAY_LONG) + chr(0x14)
        ### Padding
        msg = msg + chr(PK2_PADDING) * 53
        
        ### Send the msg to the device
        try:
            self.devhandle.interruptWrite(PK2_INT_WRITE, msg)
        except:
            pass

        ### Check device status
        ### FIXME What are we checking here?
        self.check_status()

    ### Sets the pins direction and value
    def io_setup_pins(self):
        msg = chr(PK2_RUN_SCRIPT)
        ### 6 Commands in this script
        msg = msg + chr (0x06)
        ### Set Ch. 0 to 0/1
        if self.chan_val[0] == 0:
            msg = msg + chr(PK2_SCR_VPP_OFF)
            msg = msg + chr(PK2_SCR_MCLR_GND_ON)
        else:
            msg = msg + chr(PK2_SCR_VPP_ON)
            msg = msg + chr(PK2_SCR_MCLR_GND_OFF)
        ### Set ICSP Pins
        msg = msg + chr(PK2_SCR_SET_ICSP_PINS) + chr(self.gen_icsp_byte())
        ### Set AUX Pins
        msg = msg + chr(PK2_SCR_SET_AUX) + chr(self.gen_aux_byte())
        ### Padding
        msg = msg + chr(PK2_PADDING) * 56

        ### Send the msg to the device
        try:
            self.devhandle.interruptWrite(PK2_INT_WRITE, msg)
        except:
            pass

        ### Save the local copy of the pin value and io state
        self.last_chan_val = self.chan_val[:]
        self.last_chan_io = self.chan_io[:]

        ### Check device status
        ### FIXME What are we checking here?
        self.check_status()

    ### Requests an I/O report
    def io_request_pins(self):
        ### Send the data request    
        msg = chr(PK2_RUN_SCRIPT)
        ### 2 Instructions
        msg = msg + chr(0x02)
        ### Request the state buffer? 
        msg = msg + chr(PK2_SCR_ICSP_STATE_BUF)
        ### Request the aux state buffer?
        msg = msg + chr(PK2_SCR_AUX_BUF)
        ### Unknown
        msg = msg + chr(0xAA)
        ### Padding
        msg = msg + chr(PK2_PADDING) * 59

        ### Send the msg to the device
        try:
            self.devhandle.interruptWrite(PK2_INT_WRITE, msg)
        except:
            pass

        done = False
        ### Get response
        while not done:
            try:
                ret = self.devhandle.interruptRead(PK2_INT_READ, INT_PKT_SZ)
            ### We ignore failures here because we're just polling for
            ### the return from the device
            except:
                pass
            else:
                done = True
        return ret
        
    ### Run the I/O Module
    def run_io(self):
        ### Check to make sure we have a voltage on the device
        vdd, vpp = self.check_voltages()
        ### If the input voltage is < 2.5v, it won't work
        if vdd < 2.5: 
            err = gtk.MessageDialog(None, gtk.DIALOG_MODAL, gtk.MESSAGE_ERROR, gtk.BUTTONS_OK, "You must connect VDD (pin 2) to a source between 2.5v and 5.0v")
            err.set_title("Check Source Voltage")
            err.run()
            err.destroy()
            return False

        ### These values were determined by doing the reverse of the
        ### equations in these functions using the captured data
        ### Set VPP
        self.set_vpp(vdd, 0.7)
        ### Set VDD
        self.set_vdd(vdd, 0.85)
        
        ### Setup the IO Module
        self.io_setup()
                
        ### Start the io thread running            
        self.io_running = True

        ### Start the thread
        self.io_thread = io_thread(self)
        self.io_thread.start()
        return True
    
