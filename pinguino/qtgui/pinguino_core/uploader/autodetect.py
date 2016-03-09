#! /usr/bin/python2
#-*- coding: iso-8859-15 -*-

"""-------------------------------------------------------------------------
    Pinguino Bootloader Auto Detection

    Author:          Regis Blanchot <rblanchot@gmail.com>
    Last release:    2012-12-15

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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
-------------------------------------------------------------------------"""

#import sys
#import os
#import usb            # checked in check.py

from uploader import baseUploader

class autodetect(baseUploader):
    """ detect bootloader version """

    # General Data Packet Structure (usbBuf)
    # --------------------------------------------------------------------------
    #    __________________
    #    |    COMMAND     |   0       [CMD]
    #    |      LEN       |   1       [LEN]
    #    |     ADDRL      |   2       [        ]  [addrl]
    #    |     ADDRH      |   3       [ADR.pAdr]: [addrh]
    #    |     ADDRU      |   4       [        ]  [addru]
    #    |                |   5       [DATA]
    #    |                |
    #    .      DATA      .
    #    .                .
    #    |                |   62
    #    |________________|   63
    #
    # --------------------------------------------------------------------------
    BOOT_CMD                        =    0
    BOOT_CMD_LEN                    =    1
    BOOT_ADDR_LO                    =    2
    BOOT_ADDR_HI                    =    3
    BOOT_ADDR_UP                    =    4
    BOOT_DATA_START                 =    5

    BOOT_DEV1                       =    5
    BOOT_DEV2                       =    6

    BOOT_VER_MINOR                  =    2
    BOOT_VER_MAJOR                  =    3

    BOOT_SIZE                       =    1

    # Bootloader commands
    # --------------------------------------------------------------------------
    READ_VERSION_CMD                =    0x00
    READ_FLASH_CMD                  =    0x01
    WRITE_FLASH_CMD                 =    0x02
    ERASE_FLASH_CMD                 =    0x03
    #READ_EEDATA_CMD                =    0x04
    #WRITE_EEDATA_CMD               =    0x05
    #READ_CONFIG_CMD                =    0x06
    #WRITE_CONFIG_CMD               =    0x07
    RESET_CMD                       =    0xFF

    # Data Block's size to write
    # --------------------------------------------------------------------------
    DATABLOCKSIZE                   =    32

    # USB Packet size
    # --------------------------------------------------------------------------
    MAXPACKETSIZE                   =    64

    # bulk endpoints
    # --------------------------------------------------------------------------
    IN_EP                           =    0x81    # endpoint for Bulk reads
    OUT_EP                          =    0x01    # endpoint for Bulk writes

    # configuration
    # --------------------------------------------------------------------------
    ACTIVE_CONFIG                   =    0x01
    INTERFACE_ID                    =    0x00
    TIMEOUT                         =    1200

# ------------------------------------------------------------------------------
    def initDevice(self):
# ------------------------------------------------------------------------------
#   TODO: to move in uploader.py ?
# ------------------------------------------------------------------------------
        """ init pinguino device """
        #conf = self.device.configurations[0]
        #iface = conf.interfaces[0][0]
        handle = self.device.open()
        if handle:
            ##handle.detachKernelDriver(iface.interfaceNumber)
            #handle.setConfiguration(conf)
            #handle.claimInterface(iface)
            ##handle.setAltInterface(iface)
            handle.setConfiguration(self.ACTIVE_CONFIG)
            handle.claimInterface(self.INTERFACE_ID)
            return handle
        return self.ERR_USB_INIT1
# ------------------------------------------------------------------------------
    def sendCMD(self, usbBuf):
# ------------------------------------------------------------------------------
        """ send command to the bootloader """
        sent_bytes = self.handle.bulkWrite(self.OUT_EP, usbBuf, self.TIMEOUT)
        #print sent_bytes
        if sent_bytes == len(usbBuf):
            #print "Block issued without problem."
            # whatever is returned, USB packet size is always 64 bytes long in high speed mode
            return self.handle.bulkRead(self.IN_EP, self.MAXPACKETSIZE, self.TIMEOUT)
        else:
            return self.ERR_USB_WRITE
# ------------------------------------------------------------------------------
    def detect(self):
# ------------------------------------------------------------------------------

        # search for a Pinguino board
        # ----------------------------------------------------------------------

        self.device = self.getDevice()
        if self.device == self.ERR_DEVICE_NOT_FOUND:
            self.add_report("Pinguino not found\n")
            self.add_report("Is your device connected and/or in bootloader mode ?\n")
            return
        else:
            self.add_report("Pinguino found ...\n")

        self.handle = self.initDevice()
        if self.handle == self.ERR_USB_INIT1:
            # wrong active config
            # bootloader version is 1.x or 2.x
            return (2,12)

        # find out bootloader version
        # ----------------------------------------------------------------------

        usbBuf = [0] * self.MAXPACKETSIZE
        # command code
        usbBuf[self.BOOT_CMD] = self.READ_VERSION_CMD
        # write data packet and get response
        usbBuf = self.sendCMD(usbBuf)
        if usbBuf != self.ERR_USB_WRITE:
            # major.minor
            version = ( usbBuf[self.BOOT_VER_MAJOR], \
                        usbBuf[self.BOOT_VER_MINOR] )
        self.closeDevice()
        return version
# ------------------------------------------------------------------------------
