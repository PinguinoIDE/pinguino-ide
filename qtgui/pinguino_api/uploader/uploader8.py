#!/usr/bin/env python
#-*- coding: iso-8859-15 -*-

"""---------------------------------------------------------------------
    Pinguino Universal Uploader

    Author:          Regis Blanchot <rblanchot@gmail.com>
    Last release:    2012-04-20

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

# This class is based on :
# - Diolan USB bootloader licenced (LGPL) by Diolan <http://www.diolan.com>
# - jallib USB bootloader licenced (BSD) by Albert Faber
# See also http://wiki.erazor-zone.de/wiki:projects:python:pyusb:pydoc
# Pinguino Device Descriptors : lsusb -v -d 04d8:feaa

#import sys
import os
#import usb            # checked in check.py

from uploader import baseUploader

class uploader8(baseUploader):
    """ upload .hex into pinguino device """

    # General Data Packet Structure (usbBuf)
    # ------------------------------------------------------------------
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
    # ------------------------------------------------------------------
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
    # ------------------------------------------------------------------
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
    # ------------------------------------------------------------------
    DATABLOCKSIZE                   =    32

    # USB Packet size
    # ------------------------------------------------------------------
    MAXPACKETSIZE                   =    64

    # bulk endpoints
    # ------------------------------------------------------------------
    IN_EP                           =    0x81    # endpoint for Bulk reads
    OUT_EP                          =    0x01    # endpoint for Bulk writes

    # configuration
    # ------------------------------------------------------------------

    # RB 2015-03-20 : moved to uploader.py
    #ACTIVE_CONFIG                   =    0x01
    #INTERFACE_ID                    =    0x00
    TIMEOUT                         =    10000       # 1200

    # Table with Microchip 8-bit USB devices
    # device_id:[PIC name]
    # ------------------------------------------------------------------

    devices_table = \
    {
        0x4740: ['18f13k50'],
        0x4700: ['18lf13k50'],

        0x4760: ['18f14k50'],
        0x4720: ['18f14k50'],

        0x2420: ['18f2450'],
        0x1260: ['18f2455'],
        0x2A60: ['18f2458'],
        0x4C00: ['18f24j50'],
        0x4CC0: ['18lf24j50'],
        0x5C60: ['18f24k50'],
        0x5CE0: ['18lf24k50'],

        0x1240: ['18f2550'],
        0x2A40: ['18f2553'],
        0x4C20: ['18f25j50'],
        0x4CE0: ['18lf25j50'],
        0x5C20: ['18f25k50'],
        0x5CA0: ['18lf25k50'],

        0x4C40: ['18f26j50'],
        0x4D00: ['18lf26j50'],

        0x5860: ['18f27j53'],

        0x1200: ['18f4450'],
        0x1220: ['18f4455'],
        0x2A20: ['18f4458'],
        0x4C60: ['18f44j50'],
        0x4D20: ['18lf44j50'],

        0x1200: ['18f4550'],
        0x2A00: ['18f4553'],
        0x4C80: ['18f45j50'],
        0x4D40: ['18lf45j50'],
        0x5C00: ['18f45k50'],
        0x5C80: ['18lf45k50'],

        0x4CA0: ['18f46j50'],
        0x4D60: ['18lf46j50'],

        0x58E0: ['18f47j53'],

        0x4100: ['18f65j50'],
        0x1560: ['18f66j50'],
        0x4160: ['18f66j55'],
        0x4180: ['18f67j50'],
        
        0x41A0: ['18f85j50'],
        0x41E0: ['18f86j50'],
        0x1F40: ['18f86j55'],
        0x4220: ['18f87j50']
    }

# ----------------------------------------------------------------------
    def sendCommand(self, handle, usbBuf):
# ----------------------------------------------------------------------
        """ send command to the bootloader """
        #self.txtWrite('[%s]' % ', '.join(map(hex, usbBuf)))
        sent_bytes = handle.bulkWrite(self.OUT_EP, usbBuf, self.TIMEOUT)
        #self.txtWrite(str(sent_bytes))

        if sent_bytes == len(usbBuf):
            #self.txtWrite("Block issued without problem.")
            # whatever is returned, USB packet size is always
            # 64 bytes long in high speed mode
            return handle.bulkRead(self.IN_EP, self.MAXPACKETSIZE, self.TIMEOUT)

        return self.ERR_USB_WRITE

# ----------------------------------------------------------------------
    def resetDevice(self, handle):
# ----------------------------------------------------------------------
        """ reset device """
        #usbBuf = [self.RESET_DEVICE_CMD] * self.MAXPACKETSIZE
        usbBuf = [0] * self.MAXPACKETSIZE
        # command code
        usbBuf[self.BOOT_CMD] = self.RESET_CMD
        # write data packet
        #usbBuf = self.sendCommand(usbBuf)
        try:
            handle.bulkWrite(self.OUT_EP, usbBuf, self.TIMEOUT)
        except:
            return self.ERR_USB_WRITE

        return self.ERR_NONE

# ----------------------------------------------------------------------
    def getVersion(self, handle):
# ----------------------------------------------------------------------
        """ get bootloader version """
        usbBuf = [0] * self.MAXPACKETSIZE
        # command code
        usbBuf[self.BOOT_CMD] = self.READ_VERSION_CMD
        # write data packet and get response
        usbBuf = self.sendCommand(handle, usbBuf)

        if usbBuf == self.ERR_USB_WRITE :
            return self.ERR_USB_WRITE

        # major.minor
        return str(usbBuf[self.BOOT_VER_MAJOR]) + "." + \
               str(usbBuf[self.BOOT_VER_MINOR])

# ----------------------------------------------------------------------
    def getDeviceID(self, handle):
# ----------------------------------------------------------------------
        """ read 2-byte device ID from location 0x3FFFFE """
        usbBuf = self.readFlash(handle, 0x3FFFFE, 2)

        if usbBuf == self.ERR_USB_WRITE:
            return self.ERR_USB_WRITE

        #print "BUFFER =", usbBuf
        dev1 = usbBuf[self.BOOT_DEV1]
        #print "DEV1 =", dev1
        dev2 = usbBuf[self.BOOT_DEV2]
        #print "DEV2 =", dev2
        device_id = (int(dev2) << 8) + int(dev1)
        #print device_id
        device_rev = device_id & 0x001F
        # mask revision number
        return device_id  & 0xFFE0

# ----------------------------------------------------------------------
    def getDeviceName(self, device_id):
# ----------------------------------------------------------------------
        for n in self.devices_table:
            if n == device_id:
                return self.devices_table[n][0]
        return self.ERR_DEVICE_NOT_FOUND

# ----------------------------------------------------------------------
    def eraseFlash(self, handle, address, numBlocks):
# ----------------------------------------------------------------------
        """ erase numBlocks of flash memory """
        usbBuf = [0] * self.MAXPACKETSIZE
        # command code
        usbBuf[self.BOOT_CMD] = self.ERASE_FLASH_CMD
        # number of blocks to erase
        usbBuf[self.BOOT_SIZE] = numBlocks
        # block address
        usbBuf[self.BOOT_ADDR_LO] = (address      ) & 0xFF
        usbBuf[self.BOOT_ADDR_HI] = (address >> 8 ) & 0xFF
        usbBuf[self.BOOT_ADDR_UP] = (address >> 16) & 0xFF
        # write data packet   
        #return self.sendCommand(usbBuf)
        handle.bulkWrite(self.OUT_EP, usbBuf, self.TIMEOUT)

# ----------------------------------------------------------------------
    def readFlash(self, handle, address, length):
# ----------------------------------------------------------------------
        """ read a block of flash """
        usbBuf = [0] * self.MAXPACKETSIZE
        # command code
        usbBuf[self.BOOT_CMD] = self.READ_FLASH_CMD 
        # size of block in bytes
        usbBuf[self.BOOT_CMD_LEN] = length
        # address of the block
        usbBuf[self.BOOT_ADDR_LO] = (address      ) & 0xFF
        usbBuf[self.BOOT_ADDR_HI] = (address >> 8 ) & 0xFF
        usbBuf[self.BOOT_ADDR_UP] = (address >> 16) & 0xFF
        # send request to the bootloader
        return self.sendCommand(handle, usbBuf)
        #handle.bulkWrite(self.OUT_EP, usbBuf, self.TIMEOUT)
        #return handle.bulkRead(self.IN_EP, self.BOOT_DATA_START + length, self.TIMEOUT)

# ----------------------------------------------------------------------
    def writeFlash(self, handle, address, datablock):
# ----------------------------------------------------------------------
        """ write a block of code
            first 5 bytes are for block description (BOOT_CMD, BOOT_CMD_LEN and BOOT_ADDR)
            data block size should be of DATABLOCKSIZE bytes
            total length is then DATABLOCKSIZE + 5 """
        usbBuf = [0xFF] * self.MAXPACKETSIZE
        # command code
        usbBuf[self.BOOT_CMD] = self.WRITE_FLASH_CMD 
        # size of block
        usbBuf[self.BOOT_CMD_LEN] = len(datablock)
        # block's address
        usbBuf[self.BOOT_ADDR_LO] = (address      ) & 0xFF
        usbBuf[self.BOOT_ADDR_HI] = (address >> 8 ) & 0xFF
        usbBuf[self.BOOT_ADDR_UP] = (address >> 16) & 0xFF
        # add data to the packet
        #self.txtWrite(hex(address))
        for i in range(len(datablock)):
            #self.txtWrite(hex(datablock[i]))
            usbBuf[self.BOOT_DATA_START + i] = datablock[i]
        # write data packet on usb device
        handle.bulkWrite(self.OUT_EP, usbBuf, self.TIMEOUT)
        #print usbBuf
        #usbBuf = self.sendCommand(usbBuf)
        #print usbBuf
        #if usbBuf == self.ERR_USB_WRITE :
        #    self.txtWrite("Write error")

# ----------------------------------------------------------------------
    def writeHex(self, handle, filename, board):
# ----------------------------------------------------------------------
        """ Parse the Hex File Format and send data to usb device """

        """
        [0]     Start code, one character, an ASCII colon ':'.
        [1:3]   Byte count, two hex digits, a number of bytes (hex digit pairs) in the data field. 16 (0x10) or 32 (0x20) bytes of data are the usual compromise values between line length and address overhead.
        [3:7]   Address, four hex digits, a 16-bit address of the beginning of the memory position for the data. Limited to 64 kilobytes, the limit is worked around by specifying higher bits via additional record types. This address is big endian.
        [7:9]   Record type, two hex digits, 00 to 05, defining the type of the data field.
        [9:*]   Data, a sequence of n bytes of the data themselves, represented by 2n hex digits.
        [*:*]   Checksum, two hex digits - the least significant byte of the two's complement of the sum of the values of all fields except fields 1 and 6 (Start code ":" byte and two hex digits of the Checksum). It is calculated by adding together the hex-encoded bytes (hex digit pairs), then leaving only the least significant byte of the result, and making a 2's complement (either by subtracting the byte from 0x100, or inverting it by XOR-ing with 0xFF and adding 0x01). If you are not working with 8-bit variables, you must suppress the overflow by AND-ing the result with 0xFF. The overflow may occur since both 0x100-0 and (0x00 XOR 0xFF)+1 equal 0x100. If the checksum is correctly calculated, adding all the bytes (the Byte count, both bytes in Address, the Record type, each Data byte and the Checksum) together will always result in a value wherein the least significant byte is zero (0x00).
                For example, on :0300300002337A1E
                03 + 00 + 30 + 00 + 02 + 33 + 7A = E2, 2's complement is 1E
        """

        data        = []
        old_address = 0 # or board.memstart ?
        max_address = 0
        address_Hi  = 0
        codesize    = 0

        # size of erased block
        # --------------------------------------------------------------

        # Pinguino x6j50 or x7j53, erased blocks are 1024-byte long
        if ("j" or "J") in board.proc :
            #self.add_report("x6j50, x6j53 or x7j53 chip")
            erasedBlockSize = 1024

        # Pinguino x455, x550 or x5k50, erased blocks are 64-byte long
        else:
            #self.add_report("x455, x550 or x5k50 chip")
            erasedBlockSize = 64

        # image of the whole PIC memory (above memstart)
        # --------------------------------------------------------------

        for i in range(board.memend - board.memstart):
            data.append(0xFF)

        # read hex file
        # --------------------------------------------------------------

        hexfile = open(filename,'r')
        lines = hexfile.readlines()
        hexfile.close()

        # calculate checksum and max_address
        # --------------------------------------------------------------

        for line in lines:
            byte_count = int(line[1:3], 16)
            # lower 16 bits (bits 0-15) of the data address
            address_Lo = int(line[3:7], 16)
            record_type= int(line[7:9], 16)
            
            # checksum calculation
            end = 9 + byte_count * 2 # position of checksum at end of line
            checksum = int(line[end:end+2], 16)
            cs = 0
            i = 1
            while i < end:
                cs = cs + (0x100 - int(line[i:i+2], 16) ) & 0xff # not(i)
                i = i + 2
            if checksum != cs:
                return self.ERR_HEX_CHECKSUM

            # extended linear address record
            if record_type == self.Extended_Linear_Address_Record:
                # upper 16 bits (bits 16-31) of the data address
                address_Hi = int(line[9:13], 16) << 16

            # data record
            elif record_type == self.Data_Record:

                address = address_Hi + address_Lo

                # max address
                if (address > old_address) and (address < board.memend):
                    max_address = address + byte_count
                    old_address = address

                # code size
                if (address >= board.memstart) and (address < board.memend):
                    codesize = codesize + byte_count

                # data append
                for i in range(byte_count):
                    #Caution : addresses are not always contiguous
                    #data.append(int(line[9 + (2 * i) : 11 + (2 * i)], 16))
                    data[address - board.memstart + i] = int(line[9 + (2 * i) : 11 + (2 * i)], 16)

            # end of file record
            elif record_type == self.End_Of_File_Record:
                break

            # unsupported record type
            else:
                return self.ERR_HEX_RECORD

        # max_address must be divisible by self.DATABLOCKSIZE
        # --------------------------------------------------------------

        max_address = max_address + erasedBlockSize - (max_address % erasedBlockSize)

        # erase memory from board.memstart to max_address 
        # --------------------------------------------------------------

        numBlocksMax = (board.memend - board.memstart) / erasedBlockSize
        numBlocks    = (max_address - board.memstart) / erasedBlockSize

        if numBlocks > numBlocksMax:
            return self.ERR_USB_ERASE

        if numBlocks < 256:
            self.eraseFlash(handle, board.memstart, numBlocks)

        else:
            numBlocks = numBlocks - 255
            upperAddress = board.memstart + 255 * erasedBlockSize
            # from board.memstart to board.memstart + 255 x 64 = 0x3FC0
            self.eraseFlash(handle, board.memstart, 255)
            # erase flash memory from board.memstart + 0x3FC0 to max_address
            self.eraseFlash(handle, upperAddress, numBlocks)

        # write blocks of DATABLOCKSIZE bytes
        # --------------------------------------------------------------

        for addr in range(board.memstart, max_address, self.DATABLOCKSIZE):
            index = addr - board.memstart
            self.writeFlash(handle, addr, data[index:index+self.DATABLOCKSIZE])

        self.add_report("%d bytes written." % codesize)

        return self.ERR_NONE

# ----------------------------------------------------------------------
    #def writeHex(self):
    def uploadDevice(self, filename, board):
# ----------------------------------------------------------------------

        # check file to upload
        # --------------------------------------------------------------

        if filename == '':
            self.add_report("No program to write")
            self.closeDevice(handle)
            return

        hexfile = open(filename, 'r')
        if hexfile == "":
            self.add_report("Unable to open %s" % filename)
            return
        hexfile.close()

        # search for a Pinguino board
        # --------------------------------------------------------------

        device = self.getDevice(board)

        if device == self.ERR_DEVICE_NOT_FOUND:
            self.add_report("Pinguino not found")
            self.add_report("If your device is connected,")
            self.add_report("press the Reset button to switch to bootloader mode.")
            return

        self.add_report("Pinguino found ...")

        handle = self.initDevice(device)

        if handle == self.ERR_USB_INIT1:
            self.add_report("Upload not possible")
            self.add_report("Try to restart the bootloader mode")
            return

        # find out the processor
        # --------------------------------------------------------------

        device_id = self.getDeviceID(handle)
        proc = self.getDeviceName(device_id)
        self.add_report(" - with PIC%s (id=%X)" % (proc, device_id))
        
        if proc != board.proc:
            self.add_report("Aborting: program compiled for %s but device has %s" % (board.proc, proc))
            self.closeDevice(handle)
            return

        # find out flash memory size
        # --------------------------------------------------------------

        memfree = board.memend - board.memstart;
        self.add_report(" - with %d bytes free (%d KB)" % (memfree, memfree/1024))
        self.add_report("   from 0x%05X to 0x%05X" % (board.memstart, board.memend))

        # find out bootloader version
        # --------------------------------------------------------------

        #product = handle.getString(device.iProduct, 30)
        #manufacturer = handle.getString(device.iManufacturer, 30)
        self.add_report(" - with USB bootloader v%s" % self.getVersion(handle))

        # start writing
        # --------------------------------------------------------------

        self.add_report("Uploading user program ...")
        status = self.writeHex(handle, filename, board)

        if status == self.ERR_HEX_RECORD:
            self.add_report("Aborting: record error")
            self.closeDevice(handle)
            return
            
        elif status == self.ERR_HEX_CHECKSUM:
            self.add_report("Aborting: checksum error")
            self.closeDevice(handle)
            return
            
        elif status == self.ERR_USB_ERASE:
            self.add_report("Aborting: erase error")
            self.closeDevice(handle)
            return
            
        elif status == self.ERR_NONE:
            self.add_report(os.path.basename(filename) + " successfully uploaded")

        # reset and start start user's app.
        # --------------------------------------------------------------

            #self.txtWrite("Resetting ...")
            self.add_report("Starting user program ...")
            self.resetDevice(handle)
            self.closeDevice(handle)
            return
            
        else:
            self.add_report("Aborting: unknown error")
            return
# ----------------------------------------------------------------------
