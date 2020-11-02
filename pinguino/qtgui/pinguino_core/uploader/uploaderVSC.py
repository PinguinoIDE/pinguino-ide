#! /usr/bin/python2
#-*- coding: iso-8859-15 -*-

"""-------------------------------------------------------------------------
    Pinguino Universal Uploader

    (c) 2011 Regis Blanchot <rblanchot@gmail.com>

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

# This class is based on :
# - Docker software licensed (LGPL) by Pierre Gaufillet <pierre.gaufillet@magic.fr>
# - Vasco  software licensed (LGPL) by Jean-Pierre Mandon <jp.mandon@gmail.com>
# PyUSB Doc can be found here: http://wiki.erazor-zone.de/wiki:projects:python:pyusb:pydoc
# To get the Device Descriptors : lsusb -v -d 04d8:feaa

import sys
import os
#import usb			# checked in check.py

from .uploader import baseUploader

class uploaderVSC(baseUploader):
    """ upload .hex into pinguino device """

    # Vasco Bulk bootloader commands
    # --------------------------------------------------------------------------

    VSC_ERASE_FLASH_CMD				=	0x00
    VSC_WRITE_FLASH_CMD				=	0x01
    VSC_READ_FLASH_CMD				=	0x02
    VSC_VALID_APPLICATION_CMD		=	0x03
    VSC_RESET_DEVICE_CMD			=	0x04
    VSC_SECTION_DESCRIPTOR_CMD		=	0x05

    # boot command size in bytes
    VSC_BOOT_CMD_SIZE				=	4
    VSC_BLOCKSIZE					=	32

    # bulk endpoints
    VSC_IN_EP						=	0x82	# endpoint for Bulk reads
    VSC_OUT_EP						=	0x01	# endpoint for Bulk writes

    # configuration
    VSC_TIMEOUT						=	200

    # Table with supported USB devices
    # device_id:[PIC name, flash size(in bytes), eeprom size (in bytes)]
    # --------------------------------------------------------------------------

    devices_table = \
    {
        0x1260: ['18f2455'],
        0x1240: ['18f2550'],
        0x1220: ['18f4455'],
        0x1200: ['18f4550']
    }

# ------------------------------------------------------------------------------
    def usbWrite(self, handle, usbBuf):
# ------------------------------------------------------------------------------
        """	Write a data packet to currently-open USB device """
        sent_bytes = handle.bulkWrite(self.VSC_OUT_EP, usbBuf, self.VSC_TIMEOUT)
        if (sent_bytes):
            return self.ERR_NONE
        else:
            return self.ERR_USB_WRITE
# ------------------------------------------------------------------------------
    def eraseBlock(self, handle, address):
# ------------------------------------------------------------------------------
        """ erase 64 bytes of flash memory """
        # command
        cmd = self.VSC_ERASE_FLASH_CMD
        # block address
        address = "%06X" % address
        addr_lo = int(address[4:6],16)
        addr_hi = int(address[2:4],16)
        addr_up = int(address[0:2],16)
        # write data packet
        usbBuf = chr(cmd) + chr(addr_lo) + chr(addr_hi) + chr(addr_up)
        self.usbWrite(handle, usbBuf)
# ------------------------------------------------------------------------------
    #def issueBlock(self, address, block):
    def writeFlash(self, handle, address, datablock):
# ------------------------------------------------------------------------------
        """ write a block of code """
        # command
        cmd = self.VSC_WRITE_FLASH_CMD
        # block's address
        address = "%06X" % address
        readbyte1 = int(address[4:6], 16)
        readbyte2 = int(address[2:4], 16)
        readbyte3 = int(address[0:2], 16)
        # add data to the packet
        usbBuf = chr(cmd) + chr(readbyte1) + chr(readbyte2) + chr(readbyte3)
        for i in range(len(datablock)):
            usbBuf = usbBuf + chr(datablock[i])
        # write data packet on usb device
        self.usbWrite(handle, usbBuf)
# ------------------------------------------------------------------------------
    #def hexWrite(self):
    def writeHex(self, handle, filename, board):
# ------------------------------------------------------------------------------
        """ Parse the Hex File Format and send data to usb device """

        """
        [0]		Start code, one character, an ASCII colon ':'.
        [1:3]	Byte count, two hex digits, a number of bytes (hex digit pairs) in the data field. 16 (0x10) or 32 (0x20) bytes of data are the usual compromise values between line length and address overhead.
        [3:7]	Address, four hex digits, a 16-bit address of the beginning of the memory position for the data. Limited to 64 kilobytes, the limit is worked around by specifying higher bits via additional record types. This address is big endian.
        [7:9]	Record type, two hex digits, 00 to 05, defining the type of the data field.
        [9:*]	Data, a sequence of n bytes of the data themselves, represented by 2n hex digits.
        [*:*]	Checksum, two hex digits - the least significant byte of the two's complement of the sum of the values of all fields except fields 1 and 6 (Start code ":" byte and two hex digits of the Checksum). It is calculated by adding together the hex-encoded bytes (hex digit pairs), then leaving only the least significant byte of the result, and making a 2's complement (either by subtracting the byte from 0x100, or inverting it by XOR-ing with 0xFF and adding 0x01). If you are not working with 8-bit variables, you must suppress the overflow by AND-ing the result with 0xFF. The overflow may occur since both 0x100-0 and (0x00 XOR 0xFF)+1 equal 0x100. If the checksum is correctly calculated, adding all the bytes (the Byte count, both bytes in Address, the Record type, each Data byte and the Checksum) together will always result in a value wherein the least significant byte is zero (0x00).
                For example, on :0300300002337A1E
                03 + 00 + 30 + 00 + 02 + 33 + 7A = E2, 2's complement is 1E
        """

        data = []
        old_address = 0
        max_address = 0
        address_Hi	= 0
        codesize	= 0

        # read hex file
        # ----------------------------------------------------------------------

        fichier = open(filename,'r')
        lines = fichier.readlines()
        fichier.close()

        # 1st pass : calculate checksum and max address
        # ----------------------------------------------------------------------

        i = 0
        for line in lines:
            byte_count = int(line[1:3], 16)
            address_Lo = int(line[3:7], 16) # lower 16 bits (bits 0-15) of the data address
            record_type= int(line[7:9], 16)
            i+=1

            address = (address_Hi << 16) + address_Lo

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
                address_Hi = int(line[9:13], 16) << 16 # upper 16 bits (bits 16-31) of the data address

            # code size
            if address >= board.memstart:
                codesize = codesize + byte_count

            # max address
            if (address > old_address) and (address < board.memend):
                max_address = address + byte_count
                old_address = address

        max_address = max_address + 64 - (max_address % 64)
        #self.txtWrite(output, "%d bytes to write\n" % codesize)

        # fill data sequence with 0xFF
        # ----------------------------------------------------------------------

        for i in range(board.memstart, max_address + 64):
            #print i
            data.append(0xFF)

        # 2nd pass : parse bytes from line into data
        # ----------------------------------------------------------------------

        address_Hi	= 0

        for line in lines:
            byte_count = int(line[1:3], 16)
            address_Lo = int(line[3:7], 16) # four hex digits
            record_type= int(line[7:9], 16)

            address = (address_Hi << 16) + address_Lo

            # data record
            if record_type == self.Data_Record:
                if (address >= board.memstart) and (address < board.memend):
                    for i in range(byte_count):
                        data[address - board.memstart + i] = int(line[9 + (2 * i) : 11 + (2 * i)], 16)

            # end of file record
            elif record_type == self.End_Of_File_Record:
                break

            # extended linear address record
            elif record_type == self.Extended_Linear_Address_Record:
                address_Hi = int(line[9:13], 16) # upper 16 bits (bits 16-31) of the data address

            # unsupported record type
            else:
                return self.ERR_HEX_RECORD

        # erase and write blocks
        # ----------------------------------------------------------------------

        usbBuf = []
        for addr in range(board.memstart, max_address + 64):
            if addr % 64 == 0:
                self.eraseBlock(handle, addr)
            if addr % self.VSC_BLOCKSIZE == 0:
                if usbBuf != []:
                    self.writeFlash(handle, addr - self.VSC_BLOCKSIZE, usbBuf)
                usbBuf = []
            if data[addr - board.memstart] != []:
                usbBuf.append(data[addr - board.memstart])

        return self.ERR_NONE
# ------------------------------------------------------------------------------
    #def writeHex(self):
    def uploadDevice(self, filename, board):
# ------------------------------------------------------------------------------

        # check file to upload
        # ----------------------------------------------------------------------

        if filename == '':
            self.add_report("No program to write")
            self.closeDevice(handle)
            return

        fichier = open(filename, 'r')
        if fichier == "":
            self.add_report("Unable to open %s" % filename)
            return
        fichier.close()

        # search for a Pinguino board
        # ----------------------------------------------------------------------

        device = self.getDevice(board)
        if device is self.ERR_DEVICE_NOT_FOUND:
            self.add_report("Pinguino not found")
            self.add_report("If your device is connected,")
            self.add_report("press the Reset button to switch to bootloader mode.")
            return
        else:
            self.add_report("Pinguino found")

        handle = self.initDevice(device)


        if handle is self.ERR_USB_INIT1:
            self.add_report("Upload not possible")
            self.add_report("Try to restart the bootloader mode")
            return

        # find out bootloader version
        # ----------------------------------------------------------------------

        #product = handle.getString(device.iProduct, 30)
        #manufacturer = handle.getString(device.iManufacturer, 30)
        #self.add_report("Bootloader v2.12")

        # start writing
        # ----------------------------------------------------------------------

        #self.add_report("Writing ...")
        #status = self.hexWrite()
        status = self.writeHex(handle, filename, board)
        if status == self.ERR_NONE:
            self.add_report(os.path.basename(filename) + " successfully uploaded")
        if status == self.ERR_HEX_RECORD:
            self.add_report("Record error")
        if status == self.ERR_HEX_CHECKSUM:
            self.add_report("Checksum error")

        self.closeDevice(handle)
# ------------------------------------------------------------------------------
