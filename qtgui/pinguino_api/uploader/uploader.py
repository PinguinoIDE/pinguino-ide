#!/usr/bin/env python
#-*- coding: utf-8 -*-

import usb
import sys
import logging
import os
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
    VSC_ACTIVE_CONFIG               = 0x02

# # ------------------------------------------------------------------------------
    # def __init__(self, hexfile, board):
        # self.report = []

# ------------------------------------------------------------------------------
    def add_report(self, message):
        """ display message in the log window """
        self.report.append(message)
        # DEBUGGING MESSAGE
        logging.info(message)

# ------------------------------------------------------------------------------
    def getDevice(self, board):
        """ Scans connected USB devices until it finds a Pinguino board """
        logging.info("Looking for a Pinguino device ...")
        busses = usb.busses()
        for bus in busses:
            for device in bus.devices:
                logging.info("Found device 0x%04X:0x%04X" % (device.idVendor, device.idProduct))
                if (device.idVendor, device.idProduct) == (board.vendor, board.product):
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
                    if board.bldr == "boot2":
                        self.ACTIVE_CONFIG = self.VSC_ACTIVE_CONFIG

                    return device
        return self.ERR_DEVICE_NOT_FOUND

# ----------------------------------------------------------------------
    def initDevice(self, device):
        """ Init a Pinguino device """

        handle = device.open()

        if handle:

            logging.info("OS is %s" % os.getenv("PINGUINO_OS_NAME"))

            if os.getenv("PINGUINO_OS_NAME") == "linux":
                if device.idProduct == 0x003C: #self.P32_ID:
                    # make sure the hid kernel driver is not active
                    # functionality not available on Darwin or Windows
                    handle.detachKernelDriver(self.INTERFACE_ID)
                    logging.info("Kernel driver detached")


            #handle.setConfiguration(self.configuration)
            handle.setConfiguration(self.ACTIVE_CONFIG)
            logging.info("Configuration set")

            #handle.claimInterface(self.interface)
            handle.claimInterface(self.INTERFACE_ID)
            logging.info("Interface claimed")

            logging.info("Everything OK so far")
            return handle

        return self.ERR_USB_INIT1

# ------------------------------------------------------------------------------
    def closeDevice(self, handle):
        """ Close currently-open USB device """
        try:
            handle.releaseInterface()
        except Exception as e:
            logging.info(e)
            pass
        logging.info("Device closed")

########################################################################
class Uploader(object):
    """Universal uploader class"""

    #----------------------------------------------------------------------
    def __init__(self, hexfile, board):

        #debugger.Debugger(sys)

        self.hexfile = hexfile
        #self.filename = hexfile
        self.board = board

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

        self.uploader = Uploader()


    #----------------------------------------------------------------------
    @Debugger.debug_method
    def upload(self):
        self.uploader.report = []
        self.uploader.uploadDevice(self.hexfile, self.board)
        return self.uploader.report

