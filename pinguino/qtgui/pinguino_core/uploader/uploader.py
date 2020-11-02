#!/usr/bin/env python
#-*- coding: utf-8 -*-

"""---------------------------------------------------------------------
      _____ _____ _   _  _____ _    _ _____ _   _  ____
     |  __ \_   _| \ | |/ ____| |  | |_   _| \ | |/ __ \
     | |__) || | |  \| | |  __| |  | | | | |  \| | |  | |
     |  ___/ | | | . ` | | |_ | |  | | | | | . ` | |  | |
     | |    _| |_| |\  | |__| | |__| |_| |_| |\  | |__| |
     |_|  _|_____|_| \_|\_____|\____/|_____|_| \_|\____/
         | |  | |     | |               | |
         | |  | |_ __ | | ___   __ _  __| | ___ _ __
         | |  | | '_ \| |/ _ \ / _` |/ _` |/ _ \ '__|
         | |__| | |_) | | (_) | (_| | (_| |  __/ |
          \____/| .__/|_|\___/ \__,_|\__,_|\___|_|
                | |
                |_|
    Author:         Regis Blanchot <rblanchot@gmail.com>
    --------------------------------------------------------------------
    2016-08-29      RB - added usb.core functions
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
-------------------------------------------------------------------------"""

import usb
#import usb.core
#import usb.util
import sys
import logging
import os
#import debugger

# from .. import Debugger

########################################################################
class baseUploader(object):

    # PyUSB Core module switch
    # ------------------------------------------------------------------

    PYUSB_USE_CORE                  =   0 # (0=legacy, 1=core)

    # Hex format record types
    # ------------------------------------------------------------------
    # Data_Record = 00
    # End_Of_File_Record = 01
    # Extended_Segment_Address_Record = 02
    # Start_Segment_Address_Record = 03
    # Extended_Linear_Address_Record = 04
    # Start_Linear_Address_Record = 05

    # Python3 compatibility (octals)
    Data_Record                     = 0o0
    End_Of_File_Record              = 0o1
    Extended_Segment_Address_Record = 0o2
    Start_Segment_Address_Record    = 0o3
    Extended_Linear_Address_Record  = 0o4
    Start_Linear_Address_Record     = 0o5


    # Error codes returned by various functions
    # ------------------------------------------------------------------
    ERR_NONE                        = 100
    ERR_CMD_ARG                     = 101
    ERR_CMD_UNKNOWN                 = 102
    ERR_DEVICE_NOT_FOUND            = 103
    ERR_USB_INIT1                   = 104
    ERR_USB_INIT2                   = 105
    ERR_USB_OPEN                    = 106
    ERR_USB_WRITE                   = 107
    ERR_USB_READ                    = 108
    ERR_HEX_OPEN                    = 109
    ERR_HEX_STAT                    = 110
    ERR_HEX_MMAP                    = 111
    ERR_HEX_SYNTAX                  = 112
    ERR_HEX_CHECKSUM                = 113
    ERR_HEX_RECORD                  = 114
    ERR_VERIFY                      = 115
    ERR_EOL                         = 116
    ERR_USB_ERASE                   = 117

    # Configuration
    # ------------------------------------------------------------------

    INTERFACE_ID                    = 0x00
    ACTIVE_CONFIG                   = 0x01
    VSC_ACTIVE_CONFIG               = 0x02

    # ------------------------------------------------------------------
    def add_report(self, message):
        """ display message in the log window """
        self.report.append(message)
        logging.info(message)

    # ------------------------------------------------------------------
    def getDevice(self, board):
        """ Scans connected USB devices until it finds a Pinguino board """

        logging.info("Looking for a Pinguino device ...")

        if self.PYUSB_USE_CORE:

            device = usb.core.find(idVendor=board.vendor, idProduct=board.product)
            if device is None :
                return self.ERR_DEVICE_NOT_FOUND
            else :
                return device

        else:

            busses = usb.busses()
            for bus in busses:
                for device in bus.devices:
                    if (device.idVendor, device.idProduct) == (board.vendor, board.product):
                        return device
            return self.ERR_DEVICE_NOT_FOUND

    # ------------------------------------------------------------------
    def initDevice(self, device):
        """ Init a Pinguino device """

        #logging.info("OS is %s" % os.getenv("PINGUINO_OS_NAME"))

        if os.getenv("PINGUINO_OS_NAME") == "linux":
            if device.idProduct == 0x003C: #self.P32_ID:
                # make sure the hid kernel driver is not active
                # functionality not available on Darwin or Windows
                if self.PYUSB_USE_CORE:
                    if device.is_kernel_driver_active(self.INTERFACE_ID):
                        try:
                            device.detach_kernel_driver(self.INTERFACE_ID)
                        except usb.core.USBError as e:
                            sys.exit("Could not detach kernel driver: %s" % str(e))
                else:
                    try:
                        handle.detachKernelDriver(self.INTERFACE_ID)
                    except:
                        #sys.exit("Could not detach kernel driver")
                        pass

                #logging.info("Kernel driver detached")


        if self.PYUSB_USE_CORE:
            try:
                device.set_configuration(self.ACTIVE_CONFIG)
            except usb.core.USBError as e:
                sys.exit("Could not set configuration: %s" % str(e))
            #logging.info("Configuration set")

            try:
                usb.util.claim_interface(device, self.INTERFACE_ID)
                #print("Claimed device")
            except:
                sys.exit("Could not claim the device: %s" % str(e))
            #logging.info("Interface claimed")

            return device

        else:
            handle = device.open()
            if handle:
                handle.setConfiguration(self.ACTIVE_CONFIG)
                handle.claimInterface(self.INTERFACE_ID)
                return handle
            return ERR_USB_INIT1

        #logging.info("Everything OK so far")

    # ------------------------------------------------------------------
    def closeDevice(self, handle):
        """ Close currently-open USB device """

        if self.PYUSB_USE_CORE:
            usb.util.release_interface(handle, self.INTERFACE_ID)
        else:
            handle.releaseInterface()

########################################################################
class Uploader(object):
    """Universal uploader class"""

    # ------------------------------------------------------------------
    def configure_uploader(self, hex_file, board):

        #debugger.Debugger(sys)

        self.hex_file = hex_file
        # self.filename = hex_file
        self.board = board

        if board.connect == "icsp":

            # TODO : interface here something like PICpgm (http://www.members.aon.at/electronics/pic/picpgm/)
            from .uploaderICSP import uploaderICSP as Uploader
            #self.logwindow("You choose a board without bootloader.\nYou should either change your board type\nor use a programmer to upload your application on your board", 1)
            #raise Exception("You choose a board without bootloader.\nYou should either change your board type\nor use a programmer to upload your application on your board")

        # elif board.connect == "boot2":
            # from .uploaderVSC import uploaderVSC as Uploader

        #elif board.connect == 'boot3':
        #    self.uploader = self.uploaderDLN(*parameters)

        else :

            if board.arch == 8:
                from .uploader8 import uploader8 as Uploader

            elif board.arch == 32:
                from .uploader32 import uploader32 as Uploader

        self.uploader = Uploader()


    # ------------------------------------------------------------------
    # @Debugger.debug_method
    def upload_hex(self):
        self.uploader.report = []
        # self.uploader.writeHex()
        self.uploader.uploadDevice(self.hex_file, self.board)
        return self.uploader.report
