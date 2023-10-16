#!/usr/bin/env python
#-*- coding: utf-8 -*-
#-*- coding: iso-8859-15 -*-

"""---------------------------------------------------------------------
         _____ _____ _   _  _____ _    _ _____ _   _  ____             
        |  __ \_   _| \ | |/ ____| |  | |_   _| \ | |/ __ \            
        | |__) || | |  \| | |  __| |  | | | | |  \| | |  | |           
        |  ___/ | | | . ` | | |_ | |  | | | | | . ` | |  | |           
        | |    _| |_| |\  | |__| | |__| |_| |_| |\  | |__| |           
   ___  |_|   |_____|_| \_|\_____|\____/|_____|_| \_|\____/_           
  / _ \      | |   (_) |   | |  | |     | |               | |          
 | (_) |_____| |__  _| |_  | |  | |_ __ | | ___   __ _  __| | ___ _ __ 
  > _ <______| '_ \| | __| | |  | | '_ \| |/ _ \ / _` |/ _` |/ _ \ '__|
 | (_) |     | |_) | | |_  | |__| | |_) | | (_) | (_| | (_| |  __/ |   
  \___/      |_.__/|_|\__|  \____/| .__/|_|\___/ \__,_|\__,_|\___|_|   
                                  | |                                  
                                  |_|                                   
    Author:         Regis Blanchot <rblanchot@gmail.com>
    --------------------------------------------------------------------
    2013-11-13 - RB - first release   
    2015-09-08 - RB - fixed numBlocks > numBlocksMax when used with XC8
    2015-11-09 - RB - added PIC16F1459 ID
    2016-08-29 - RB - added usb.core functions
    2016-11-12 - RB - added python3 support (bytearray)
    2016-11-23 - RB - changed constant size DATABLOCKSIZE to variable writeBlockSize
    2019-03-05 - RB - uses arrays of bytes instead of lists for Python 3 compatibility
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

# This class is based on :
# - Diolan USB bootloader licenced (LGPL) by Diolan <http://www.diolan.com>
# - jallib USB bootloader licenced (BSD) by Albert Faber
# See also http://wiki.erazor-zone.de/wiki:projects:python:pyusb:pydoc
# Pinguino Device Descriptors : lsusb -v -d 04d8:feaa

import sys
import os
import array
#import logging

from .uploader import baseUploader

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

    BOOT_REV1                       =    5
    BOOT_REV2                       =    6
    BOOT_DEV1                       =    7
    BOOT_DEV2                       =    8

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
    TIMEOUT                         =    1200

    # Table with Microchip 8-bit USB devices
    # device_id:[PIC name]
    # ------------------------------------------------------------------

    devices_table = \
    {
        # 16F
        0x3020: ['16f1454'],
        0x3021: ['16f1455'],
        0x3023: ['16f1459'],
        0x3024: ['16lf1454'],
        0x3025: ['16lf1455'],
        0x3027: ['16lf1459'],

        # 18F
        0x4740: ['18f13k50'],
        0x4700: ['18lf13k50'],

        0x4760: ['18f14k50'],
        0x4720: ['18lf14k50'],

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
    def usbWrite(self, device, usbBuf):
# ----------------------------------------------------------------------
        """ send bytes to the bootloader """

        usbBuf = array.array('B', usbBuf)

        #if sys.version_info.major >= 3:
        #    usbBuf = str(bytearray(usbBuf))

        if self.PYUSB_USE_CORE:
            sent_bytes = device.write(self.OUT_EP, usbBuf, self.TIMEOUT)
        else:
            sent_bytes = device.bulkWrite(self.OUT_EP, usbBuf, self.TIMEOUT)

        return sent_bytes

# ----------------------------------------------------------------------
    def usbRead(self, device):
# ----------------------------------------------------------------------
        """ read bytes from the bootloader
            whatever is returned, USB packet size is always 64 bytes
            long (self.MAXPACKETSIZE) in high speed mode
        """

        usbBuf = array.array('B')

        if self.PYUSB_USE_CORE:
            usbBuf = device.read(self.IN_EP, self.MAXPACKETSIZE, self.TIMEOUT)
        else:
            usbBuf = device.bulkRead(self.IN_EP, self.MAXPACKETSIZE, self.TIMEOUT)

        #if sys.version_info.major >= 3:
        #    return str(bytearray(usbBuf))
        #else:
        #    return usbBuf
        
        return usbBuf

# ----------------------------------------------------------------------
    def sendCommand(self, device, usbBuf):
# ----------------------------------------------------------------------
        """ send command to the bootloader """
        
        #self.add_report('[%s]' % ', '.join(map(hex, usbBuf)))
        sent_bytes = self.usbWrite(device, usbBuf)
        #self.add_report(str(sent_bytes))

        if sent_bytes != len(usbBuf):
            #self.add_report("Block not issued.")
            return self.ERR_USB_WRITE

        #self.add_report("Block issued without problem.")
        usbBuf = self.usbRead(device)
        #self.add_report('[%s]' % ', '.join(map(hex, usbBuf)))
        return usbBuf

# ----------------------------------------------------------------------
    def resetDevice(self, device):
# ----------------------------------------------------------------------
        """ reset device """
        
        #usbBuf = [self.RESET_DEVICE_CMD] * self.MAXPACKETSIZE
        usbBuf = [0] * self.MAXPACKETSIZE
        # command code
        usbBuf[self.BOOT_CMD] = self.RESET_CMD
        # write data packet
        #usbBuf = self.sendCommand(usbBuf)
        try:
            self.usbWrite(device, usbBuf)
        except:
            return self.ERR_USB_WRITE

        return self.ERR_NONE

# ----------------------------------------------------------------------
    def getVersion(self, device):
# ----------------------------------------------------------------------
        """ get bootloader version """
        
        usbBuf = [0] * self.MAXPACKETSIZE
        # command code
        usbBuf[self.BOOT_CMD] = self.READ_VERSION_CMD
        # write data packet and get response
        usbBuf = self.sendCommand(device, usbBuf)

        if usbBuf == self.ERR_USB_WRITE :
            return self.ERR_USB_WRITE

        # major.minor
        return str(usbBuf[self.BOOT_VER_MAJOR]) + "." + \
               str(usbBuf[self.BOOT_VER_MINOR])

# ----------------------------------------------------------------------
    def getDeviceID(self, device, board):
# ----------------------------------------------------------------------
        """ read 2-byte device ID from
            PIC18F : 0x3FFFFE
            PIC16F : 0x8005        """

        if (board.family == '16F'):
            # REVISION & DEVICE ID
            usbBuf = self.flashRead(device, 0x8005, 4)
            if usbBuf == self.ERR_USB_WRITE:
                return self.ERR_USB_WRITE, self.ERR_USB_WRITE
            rev1 = usbBuf[self.BOOT_REV1]
            rev2 = usbBuf[self.BOOT_REV2]
            device_rev = (int(rev2) << 8) + int(rev1)
            dev1 = usbBuf[self.BOOT_DEV1]
            dev2 = usbBuf[self.BOOT_DEV2]
            device_id  = (int(dev2) << 8) + int(dev1)
        else:
            # REVISION & DEVICE ID
            usbBuf = self.flashRead(device, 0x3FFFFE, 2)
            if usbBuf == self.ERR_USB_WRITE:
                return self.ERR_USB_WRITE, self.ERR_USB_WRITE
            #self.add_report("usbBuf = %s" % usbBuf)
            dev1 = usbBuf[self.BOOT_REV1]
            dev2 = usbBuf[self.BOOT_REV2]
            device_id  = (int(dev2) << 8) + int(dev1)
            device_id  = device_id & 0xFFE0
            device_rev = device_id & 0x001F

        return device_id, device_rev

# ----------------------------------------------------------------------
    def getDeviceName(self, device_id):
# ----------------------------------------------------------------------
        for n in self.devices_table:
            if n == device_id:
                return self.devices_table[n][0]
        return self.ERR_DEVICE_NOT_FOUND

# ----------------------------------------------------------------------
    def flashErase(self, device, address, numBlocks):
# ----------------------------------------------------------------------
        """ erase numBlocks of flash memory """
        
        usbBuf = [0] * self.MAXPACKETSIZE
        # command code
        usbBuf[self.BOOT_CMD] = int(self.ERASE_FLASH_CMD)
        # number of blocks to erase
        usbBuf[self.BOOT_SIZE] = int(numBlocks)
        #usbBuf[self.BOOT_SIZE] = int(numBlocks)
        # block address
        usbBuf[self.BOOT_ADDR_LO] = int((address      ) & 0xFF)
        usbBuf[self.BOOT_ADDR_HI] = int((address >> 8 ) & 0xFF)
        usbBuf[self.BOOT_ADDR_UP] = int((address >> 16) & 0xFF)
        # write data packet
        #return self.sendCommand(usbBuf)
        self.usbWrite(device, usbBuf)

# ----------------------------------------------------------------------
    def flashRead(self, device, address, length):
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
        return self.sendCommand(device, usbBuf)
        #device.write(self.OUT_EP, usbBuf, self.TIMEOUT)
        #return device.read(self.IN_EP, self.BOOT_DATA_START + length, self.TIMEOUT)

# ----------------------------------------------------------------------
    def flashWrite(self, device, address, datablock):
# ----------------------------------------------------------------------
        """ write a block of code
            first 5 bytes are for block description (BOOT_CMD, BOOT_CMD_LEN and BOOT_ADDR)
            data block size should be of writeBlockSize bytes
            total length is then writeBlockSize + 5 """
            
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
        self.usbWrite(device, usbBuf)
        #print usbBuf
        #usbBuf = self.sendCommand(usbBuf)
        #print usbBuf
        #if usbBuf == self.ERR_USB_WRITE :
        #    self.txtWrite("Write error")

# ----------------------------------------------------------------------
    def writeHex(self, device, filename, board):
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

        # Addresses are doubled in the PIC16F HEX file
        if board.family == '16F':
            MEMSTART = board.memstart * 2
            MEMEND   = board.memend   * 2
        else:
            MEMSTART = board.memstart
            MEMEND   = board.memend

        #self.add_report("MEMSTART = 0x%X" % MEMSTART)
        #self.add_report("MEMEND   = 0x%X" % MEMEND)

        data        = []
        old_max_address = MEMSTART
        old_min_address = MEMEND
        max_address = 0
        min_address = 0
        address_Hi  = 0
        codesize    = 0

        # size of write block
        # ------------------------------------------------------------------

        if   "13k50" in board.proc :
            writeBlockSize = 8
        elif "14k50" in board.proc :
            writeBlockSize = 16
        else :
            writeBlockSize = 32

        # size of erase block
        # --------------------------------------------------------------

        # Pinguino 18Fx6j50 or 18Fx7j53
        # Erased block size is 1024-byte long
        if ("j" or "J") in board.proc :
            #self.add_report("x6j50, x6j53 or x7j53 chip")
            eraseBlockSize = 1024

        # Pinguino 16f145x, 18Fx455, 18Fx550 or 18Fx5k50
        # Erased block size is 64-byte long
        else:
            #self.add_report("x455, x550 or x5k50 chip")
            eraseBlockSize = 64

        # image of the whole PIC memory (above memstart)
        # --------------------------------------------------------------

        for i in range(MEMEND - MEMSTART):
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
                #self.add_report("address : 0x%X" % address)

                # min address
                if (address < old_min_address) and (address >= MEMSTART):
                    min_address = address
                    old_min_address = address
                    #self.add_report("min. address : 0x%X" % min_address)

                # max address
                if (address > old_max_address) and (address < MEMEND):
                    max_address = address + byte_count
                    old_max_address = address
                    #self.add_report("max. address : 0x%X" % max_address)

                # code size
                if (address >= MEMSTART) and (address < MEMEND):
                    codesize = codesize + byte_count
                    #self.add_report("0x%04X\t[%02d]\t[%05d]" % (address, byte_count, codesize))

                # data append
                for i in range(byte_count):
                    if ((address + i) < MEMEND):
                        #Caution : addresses are not always contiguous
                        #data.append(int(line[9 + (2 * i) : 11 + (2 * i)], 16))
                        #data[address - board.memstart + i] = int(line[9 + (2 * i) : 11 + (2 * i)], 16)
                        data[address - min_address + i] = int(line[9 + (2 * i) : 11 + (2 * i)], 16)
                        #self.add_report("0x%X=[%s]" % (address - min_address + i, int(line[9 + (2 * i) : 11 + (2 * i)], 16)))
                        #self.add_report("index=%d" % (address - min_address + i))

            # end of file record
            elif record_type == self.End_Of_File_Record:
                break

            # unsupported record type
            else:
                return self.ERR_HEX_RECORD

        # max_address must be divisible by writeBlockSize
        # --------------------------------------------------------------

        #min_address = min_address - eraseBlockSize - (min_address % eraseBlockSize)
        max_address = max_address + eraseBlockSize - (max_address % eraseBlockSize)
        if (max_address > MEMEND):
            max_address = MEMEND

        #self.add_report("min_address = 0x%X" % min_address)
        #self.add_report("max_address = 0x%X" % max_address)

        # erase memory from board.memstart to max_address
        # --------------------------------------------------------------

        numBlocksMax = (MEMEND      - MEMSTART) / eraseBlockSize
        numBlocks    = (max_address - MEMSTART) / eraseBlockSize
        #self.add_report("numBlocks    : %d" % numBlocks)
        #self.add_report("numBlocksMax : %d" % numBlocksMax)

        if  numBlocks > numBlocksMax:
            numBlocks = numBlocksMax
            #return self.ERR_USB_ERASE

        if  numBlocks < 256:
            self.flashErase(device, board.memstart, numBlocks)

        else:
            numBlocks = numBlocks - 255
            upperAddress = board.memstart + 255 * eraseBlockSize
            # from board.memstart to board.memstart + 255 x 64 = 0x3FC0
            self.flashErase(device, board.memstart, 255)
            # erase flash memory from board.memstart + 0x3FC0 to max_address
            self.flashErase(device, upperAddress, numBlocks)

        # write blocks of writeBlockSize bytes
        # --------------------------------------------------------------

        #for addr8 in range(board.memstart, max_address, writeBlockSize):
        for addr8 in range(min_address, max_address, writeBlockSize):
            #index = addr8 - board.memstart
            index = addr8 - min_address
            # the addresses are doubled in the PIC16F HEX file
            if board.family == '16F':
                addr16 = addr8 / 2
                self.flashWrite(device, addr16, data[index:index+writeBlockSize])
                #self.add_report("addr8=0x%X addr16=0x%X" % (addr8, addr16))
                #self.add_report("0x%X  [%s]" % (addr16, data[index:index+writeBlockSize]))
            else:
                self.flashWrite(device, addr8,  data[index:index+writeBlockSize])
                #self.add_report("0x%X  [%s]" % (addr8, data[index:index+writeBlockSize]))

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
            self.closeDevice(device)
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

        device = self.initDevice(device)

        if device == self.ERR_USB_INIT1:
            self.add_report("Upload not possible")
            self.add_report("Try to restart the bootloader mode")
            return

        # find out the processor
        # --------------------------------------------------------------

        device_id, device_rev = self.getDeviceID(device, board)
        proc = self.getDeviceName(device_id)

        if proc == self.ERR_DEVICE_NOT_FOUND:
            self.add_report("Aborting: unknown PIC (id=0x%X)" % device_id)
            self.closeDevice(device)
            return
        else:
            self.add_report(" - with PIC%s (id=0x%X, rev=%x)" % (proc, device_id, device_rev))

        if proc != board.proc:
            self.add_report("Aborting: program compiled for %s but device has %s" % (board.proc, proc))
            self.closeDevice(device)
            return

        # find out flash memory size
        # --------------------------------------------------------------

        memfree = board.memend - board.memstart;
        self.add_report(" - with %d bytes free (%.2f/%d KB)" % (memfree, memfree/1024, board.memend/1024))
        self.add_report("   from 0x%05X to 0x%05X" % (board.memstart, board.memend))

        # find out bootloader version
        # --------------------------------------------------------------

        #product = device.getString(device.iProduct, 30)
        #manufacturer = device.getString(device.iManufacturer, 30)
        self.add_report(" - with USB bootloader v%s" % self.getVersion(device))

        # start writing
        # --------------------------------------------------------------

        self.add_report("Uploading user program ...")
        status = self.writeHex(device, filename, board)

        if status == self.ERR_HEX_RECORD:
            self.add_report("Aborting: record error")
            self.closeDevice(device)
            return

        elif status == self.ERR_HEX_CHECKSUM:
            self.add_report("Aborting: checksum error")
            self.closeDevice(device)
            return

        elif status == self.ERR_USB_ERASE:
            self.add_report("Aborting: erase error")
            self.closeDevice(device)
            return

        elif status == self.ERR_NONE:
            self.add_report(os.path.basename(filename) + " successfully uploaded")

        # reset and start start user's app.
        # --------------------------------------------------------------

            #self.txtWrite("Resetting ...")
            self.add_report("Starting user program ...")
            # Device can't be closed because it just has been reseted
            self.closeDevice(device)
            self.resetDevice(device)
            return

        else:
            self.add_report("Aborting: unknown error")
            return
# ----------------------------------------------------------------------
