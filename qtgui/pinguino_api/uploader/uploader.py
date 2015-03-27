#!/usr/bin/env python
#-*- coding: utf-8 -*-

import usb
import sys
import logging
#import debugger

from ..tools import Debugger

########################################################################
class baseUploader(object):

    # Hex format record types
    # --------------------------------------------------------------------------
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
    # --------------------------------------------------------------------------
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
    # ----------------------------------------------------------------------

    INTERFACE_ID                    = 0x00
    ACTIVE_CONFIG                   = 0x01

# ------------------------------------------------------------------------------
    def __init__(self, hex_file, board):
        self.hex_file = hex_file
        self.filename = hex_file
        self.board = board
        self.report = []


# ------------------------------------------------------------------------------
    def add_report(self, message):
        """ display message in the log window """
        self.report.append(message)
        # DEBUGGING MESSAGE
        logging.info(message)

# ------------------------------------------------------------------------------
    def getDevice(self):
        """ Scans connected USB devices until it finds a Pinguino board """
        busses = usb.busses()
        for bus in busses:
            for device in bus.devices:
                logging.info("Found device %s:%s" % (hex(device.idVendor),hex(device.idProduct)))
                if (device.idVendor, device.idProduct) == (self.board.vendor, self.board.product):
                    """
                    self.configuration = device.configurations[0]
                    logging.info("Configuration = %s" % self.configuration)
                    self.interface = self.configuration.interfaces[0][0]
                    logging.info("Interface = %s" % self.interface)
                    self.endpoints = []
                    self.pipes = []
                    for ep in self.interface.endpoints:
                        self.endpoints.append(ep)
                        self.pipes.append(ep.address)
                    """
                    return device
        return self.ERR_DEVICE_NOT_FOUND

# ----------------------------------------------------------------------
    def initDevice(self):
        """ Init a Pinguino device """
        handle = self.device.open()
        if handle:
            
            try:
                handle.detachKernelDriver(0)
            except usb.USBError:
                pass

            #handle.setConfiguration(self.ACTIVE_CONFIG)
            #handle.setConfiguration(self.configuration)
            try:
                handle.setConfiguration(self.ACTIVE_CONFIG)
            except usb.USBError:
                pass
                
            #handle.claimInterface(self.INTERFACE_ID)
            #handle.claimInterface(self.interface)
            try:
                handle.claimInterface(self.INTERFACE_ID)
            except usb.USBError:
                pass

            """
            #handle.setAltInterface(self.INTERFACE_ID)
            #handle.setAltInterface(self.interface)
            try:
                handle.setAltInterface(self.INTERFACE_ID)
            except usb.USBError:
                pass
            """

            return handle
        return self.ERR_USB_INIT1

# ------------------------------------------------------------------------------
    def closeDevice(self):
        """ Close currently-open USB device """
        try:
            self.handle.releaseInterface()
        except:
            pass

########################################################################
class Uploader(object):
    """Universal uploader class"""

    #----------------------------------------------------------------------
    def __init__(self, hex_file, board):

        #debugger.Debugger(sys)

        if board.bldr == "noboot":

            # TODO : interface here something like PICpgm (http://www.members.aon.at/electronics/pic/picpgm/)
            #self.logwindow("You choose a board without bootloader.\nYou should either change your board type\nor use a programmer to upload your application on your board", 1)
            raise Exception("You choose a board without bootloader.\nYou should either change your board type\nor use a programmer to upload your application on your board")

        elif board.bldr == "boot2":
            from uploaderVSC import uploaderVSC as Uploader

        #elif board.bldr == 'boot3':
        #    self.uploader = self.uploaderDLN(*parameters)

        elif board.bldr == "boot4":
            from uploader8 import uploader8 as Uploader

        elif board.bldr == "microchip":
            from uploader32 import uploader32 as Uploader

        self.uploader = Uploader(hex_file, board)


    #----------------------------------------------------------------------
    @Debugger.debug_method
    def write_hex(self):

        self.uploader.writeHex()
        return self.uploader.report

