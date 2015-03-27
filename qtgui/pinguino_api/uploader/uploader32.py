#!/usr/bin/env python
#-*- coding: iso-8859-15 -*-

"""-------------------------------------------------------------------------
    Pinguino Uploader for Pinguino 32

    (c) 2011-2014 Regis Blanchot <rblanchot@gmail.com>
    
    last update : 27 Mar. 2015

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

# Based on UBW32 / Microchip mphidflash software licensed (GNU GPL v3) by Phillip Burgess <pburgess@dslextreme.com>
# UBW32 sources at : http://vak-opensource.googlecode.com/svn/!svn/bc/117/trunk/utilities/ubw32/
# MPHIDFLASH sources at : http://mphidflash.googlecode.com/svn-history/r2/trunk/
# PyUSB Doc : http://wiki.erazor-zone.de/wiki:projects:python:pyusb:pydoc
# Device Descriptors : lsusb -v -d 04d8:003C

#import sys
import os
import time
#import usb
import logging

from ..tools import Debugger
from uploader import baseUploader

class uploader32(baseUploader):
    """ upload .hex into pinguino device """

    # Data block description
    BYTESPERADDRESS                 =    1
    MAXPACKETSIZE                   =    64
    DATABLOCKSIZE                   =    56        # MAXPACKETSIZE - Block Command Size

    # Response packet structure
    # ----------------------------------------------------------------------

    """
    struct __attribute__ ((packed))
    {
        unsigned char Command;
        unsigned char PacketDataFieldSize;
        unsigned char DeviceFamily;
        unsigned char Type1;
        unsigned long Address1;
        unsigned long Length1;
        unsigned char Type2;
        unsigned long Address2;
        unsigned long Length2;
        unsigned char Type3;        //End of sections list indicator goes here, when not programming the vectors, in that case fill with 0xFF.
        unsigned long Address3;
        unsigned long Length3;            
        unsigned char Type4;        //End of sections list indicator goes here, fill with 0xFF.
        unsigned char ExtraPadBytes[33];
    };
    """

    BOOT_CMD                        =    0      # char = 1 byte
    BOOT_PACKET_SIZE                =    1      # char = 1 byte
    BOOT_DEVICE_FAMILY              =    2      # char = 1 byte
    BOOT_TYPE1                      =    3      # char = 1 byte
    BOOT_ADDR1                      =    4      # long = 4 bytes
    BOOT_MEMSTART                   =    4      # long = 4 bytes
    BOOT_LEN1                       =    8      # long = 4 bytes
    BOOT_MEMFREE                    =    8      # long = 4 bytes
    BOOT_TYPE2                      =    12     # char = 1 byte
    BOOT_ADDR2                      =    13     # long = 4 bytes
    BOOT_LEN2                       =    17     # long = 4 bytes
    BOOT_TYPE3                      =    21     # char = 1 byte
    BOOT_ADDR3                      =    22     # long = 4 bytes
    BOOT_LEN3                       =    26     # long = 4 bytes

    BOOT_TYPE_LEN                   =    9      # char + long + long = 9 bytes

    BOOT_DEVID                      =    8

    BOOT_VER_MAJOR                  =    22
    BOOT_VER_MINOR                  =    26
    
    # Sent packet structure
    # ----------------------------------------------------------------------

    """
    struct __attribute__ ((packed))
    {
        unsigned char Command;
        unsigned long Address;
        unsigned char Size;
        unsigned char PadBytes[(TotalPacketSize - 6) - (RequestDataBlockSize)];    
        unsigned int Data[RequestDataBlockSize/WORDSIZE];
    };
    """

    BOOT_ADDR                       =    1      # 4 bytes
    BOOT_CMD_SIZE                   =    5      # 1 byte
    BOOT_CMD_PAD                    =    6      # 2 bytes unsused
    BOOT_CMD_DATA                   =    8      # 64 - 8 = 56 (DATABLOCKSIZE)

    # Command Definitions
    # ----------------------------------------------------------------------

    UNLOCKCONFIG_CMD                =    0x00    # sub-command for the ERASE_DEVICE_CMD
    LOCKCONFIG_CMD                  =    0x01    # sub-command for the ERASE_DEVICE_CMD
    QUERY_DEVICE_CMD                =    0x02    # what regions can be programmed, and what type of memory is the region
    UNLOCK_CONFIG_CMD               =    0x03    # for both locking and unlocking the config bits
    ERASE_DEVICE_CMD                =    0x04    # to start an erase operation, firmware controls which pages should be erased
    PROGRAM_DEVICE_CMD              =    0x05    # to send a full RequestDataBlockSize to be programmed
    PROGRAM_COMPLETE_CMD            =    0x06    # if host send less than a RequestDataBlockSize to be programmed, or if it wished to program whatever was left in the buffer, it uses this command
    GET_DATA_CMD                    =    0x07    # the host sends this command in order to read out memory from the device. Used during verify (and read/export hex operations)
    RESET_DEVICE_CMD                =    0x08    # resets the microcontroller, so it can update the config bits (if they were programmed, and so as to leave the bootloader (and potentially go back into the main application)

    GET_DATA_CMD_SUPPORTED          =   False
    QUERY_DEVICE_CMD_SUPPORTED      =   False
    GET_DATA_BUFFER                 =   []
    QUERY_DEVICE_BUFFER             =   []
    
    # Query Device Response
    # ----------------------------------------------------------------------

    TypeProgramMemory               =    0x01    # when the host sends a QUERY_DEVICE command, need to respond by populating a list of valid memory regions that exist in the device (and should be programmed)
    TypeEEPROM                      =    0x02
    TypeConfigWords                 =    0x03
    TypeEndOfTypeList               =    0xFF    # sort of serves as a "null terminator" like number, which denotes the end of the memory region list has been reached.

    # hid endpoints
    # ----------------------------------------------------------------------
    IN_EP                           =    0x81    # endpoint for Hid reads
    OUT_EP                          =    0x01    # endpoint for Hid writes

    # Device family
    # ----------------------------------------------------------------------
    DEVICE_FAMILY_PIC18             =    0x01
    DEVICE_FAMILY_PIC24             =    0x02
    DEVICE_FAMILY_PIC32             =    0x03

    # Configuration
    # ----------------------------------------------------------------------

    # RB 2015-03-20 : moved to uploader.py
    #INTERFACE_ID                    =    0x00
    #ACTIVE_CONFIG                   =    0x01
    TIMEOUT                         =    10000

    # Memory's area
    # ----------------------------------------------------------------------

    KSEG0_PROGRAM_FLASH             =    0x9D000000
    KSEG1_PROGRAM_FLASH             =    0xBD000000
    KSEG0_BOOT_FLASH                =    0x9FC00000
    KSEG1_BOOT_FLASH                =    0xBFC00000
    BOOT_FLASH_SIZE                 =    0x2FF0
    KSEG1_RAM                       =    0xA0000000
    DEVICE_ID_ADDRESS               =    0xBF80F220

    # Table with supported USB devices
    # device_id:['CPU name'] 
    #-----------------------------------------------------------------------

    devices_table = \
    {
        0x04A00053: ['32MX220F032B'],
        0x04A04053: ['32MX220F032D'],
        0x04D00053: ['32MX250F128B'],
        0x06600053: ['32MX270F256B'],
        0x00952053: ['32MX440F256H'],
        0x00952053: ['32MX460F512L'],   # to be fixed
        0x00952053: ['32MX795F512H']    # to be fixed
    }

# ----------------------------------------------------------------------
    def resetDevice(self):
# ----------------------------------------------------------------------
        """ Reset device """
        usbBuf = [self.RESET_DEVICE_CMD] * self.MAXPACKETSIZE

        try:
            self.handle.interruptWrite(self.OUT_EP, usbBuf, self.TIMEOUT)

        except:
            return self.ERR_USB_WRITE

        return self.ERR_NONE

# ----------------------------------------------------------------------
    def sendCMD(self, usbBuf):
# ----------------------------------------------------------------------
        """ Send a command to the bootloader """
        try:
            # send a command
            #time.sleep(0.5)
            sent_bytes = self.handle.interruptWrite(self.OUT_EP, usbBuf, self.TIMEOUT)

        except:
            return self.ERR_USB_WRITE

        if sent_bytes == len(usbBuf):
            logging.info("%d bytes successfully sent." % sent_bytes)
            return self.ERR_NONE

        else:
            logging.info("Sent %d/%d bytes" % (sent_bytes, len(usbBuf)))
            return self.ERR_USB_WRITE

# ----------------------------------------------------------------------
    def getResponse(self, usbBuf):
# ----------------------------------------------------------------------
        """ Send a command and get a response from the bootloader """
        try:
            # send a command
            #time.sleep(0.5)
            sent_bytes = self.handle.interruptWrite(self.OUT_EP, usbBuf, self.TIMEOUT)

        except:
            return self.ERR_USB_WRITE

        if sent_bytes != len(usbBuf):
            logging.info("Sent %d/%d bytes" % (sent_bytes, len(usbBuf)))
            return self.ERR_USB_WRITE

        else:
            logging.info("%d bytes successfully sent." % sent_bytes)

            try:
                # get the response
                #time.sleep(0.5)
                usbBuf = self.handle.interruptRead(self.IN_EP, self.MAXPACKETSIZE, self.TIMEOUT)
            except:
                logging.info("Nothing received")
                return self.ERR_USB_READ

            logging.info("usbBuf = %s" % str(usbBuf).strip('[]'))
            """
            for i in range(sent_bytes):
                logging.info("usbBuf[%d] = %02X" % (i, usbBuf[i]) )
            """
            
            return usbBuf

# ----------------------------------------------------------------------
    def getCommands(self):
# ----------------------------------------------------------------------
        """ Get supported commands """

        # self.QUERY_DEVICE_CMD
        usbBuf = [self.QUERY_DEVICE_CMD] * self.MAXPACKETSIZE
        self.QUERY_DEVICE_BUFFER = self.getResponse(usbBuf)

        if self.QUERY_DEVICE_BUFFER == self.ERR_USB_WRITE:
            return self.ERR_USB_WRITE

        if self.QUERY_DEVICE_BUFFER == self.ERR_USB_READ:
            logging.info("QUERY_DEVICE not supported")
            self.QUERY_DEVICE_CMD_SUPPORTED = False
        else:
            logging.info("QUERY_DEVICE supported")
            self.QUERY_DEVICE_CMD_SUPPORTED = True

        # self.GET_DATA_CMD
        usbBuf = [self.GET_DATA_CMD] * self.MAXPACKETSIZE
        # address to read
        usbBuf[self.BOOT_ADDR + 0] = (self.DEVICE_ID_ADDRESS      ) & 0xFF
        usbBuf[self.BOOT_ADDR + 1] = (self.DEVICE_ID_ADDRESS >> 8 ) & 0xFF
        usbBuf[self.BOOT_ADDR + 2] = (self.DEVICE_ID_ADDRESS >> 16) & 0xFF
        usbBuf[self.BOOT_ADDR + 3] = (self.DEVICE_ID_ADDRESS >> 24) & 0xFF
        # size of block to read
        usbBuf[self.BOOT_CMD_SIZE] = 4
        self.GET_DATA_BUFFER = self.getResponse(usbBuf)

        if self.GET_DATA_BUFFER == self.ERR_USB_WRITE:
            return self.ERR_USB_WRITE

        if self.GET_DATA_BUFFER == self.ERR_USB_READ:
            logging.info("GET_DATA not supported")
            self.GET_DATA_CMD_SUPPORTED = False
        else:
            logging.info("GET_DATA supported")
            self.GET_DATA_CMD_SUPPORTED = True

        return self.ERR_NONE
        
# ----------------------------------------------------------------------
    def getDeviceFamily(self):
# ----------------------------------------------------------------------
        """ Get device info """
        if self.QUERY_DEVICE_CMD_SUPPORTED == True:
            return self.QUERY_DEVICE_BUFFER[self.BOOT_DEVICE_FAMILY]
        else:
            return False

# ----------------------------------------------------------------------
    def getDeviceFCPU(self):
# ----------------------------------------------------------------------
        """ Get device info """
        if self.QUERY_DEVICE_CMD_SUPPORTED == True:
            # CPU frequency
            fcpu = (self.QUERY_DEVICE_BUFFER[self.BOOT_ADDR2 + 0]      ) | \
                   (self.QUERY_DEVICE_BUFFER[self.BOOT_ADDR2 + 1] <<  8) | \
                   (self.QUERY_DEVICE_BUFFER[self.BOOT_ADDR2 + 2] << 16) | \
                   (self.QUERY_DEVICE_BUFFER[self.BOOT_ADDR2 + 3] << 24)
            return fcpu
        else:
            return False

# ----------------------------------------------------------------------
    def getDeviceFPB(self):
# ----------------------------------------------------------------------
        """ Get device info """
        if self.QUERY_DEVICE_CMD_SUPPORTED == True:
            # Peripheral frequency
            fpb  = (self.QUERY_DEVICE_BUFFER[self.BOOT_LEN2 + 0]      ) | \
                   (self.QUERY_DEVICE_BUFFER[self.BOOT_LEN2 + 1] <<  8) | \
                   (self.QUERY_DEVICE_BUFFER[self.BOOT_LEN2 + 2] << 16) | \
                   (self.QUERY_DEVICE_BUFFER[self.BOOT_LEN2 + 3] << 24)
            return fpb
        else:
            return False

# ----------------------------------------------------------------------
    def getDeviceID(self):
# ----------------------------------------------------------------------
        """ Get 4-byte device ID """
        if self.GET_DATA_CMD_SUPPORTED == True:
            devid = (self.GET_DATA_BUFFER[self.BOOT_DEVID + 0]      ) | \
                    (self.GET_DATA_BUFFER[self.BOOT_DEVID + 1] <<  8) | \
                    (self.GET_DATA_BUFFER[self.BOOT_DEVID + 2] << 16) | \
                    (self.GET_DATA_BUFFER[self.BOOT_DEVID + 3] << 24)
            return devid
        else:
            return False

# ----------------------------------------------------------------------
    def getVersion(self):
# ----------------------------------------------------------------------
        """ get bootloader version """
        if self.QUERY_DEVICE_CMD_SUPPORTED == True:
            major = self.QUERY_DEVICE_BUFFER[self.BOOT_VER_MAJOR]
            minor = self.QUERY_DEVICE_BUFFER[self.BOOT_VER_MINOR]
            logging.info("Major = %d" % major)
            logging.info("Minor = %d" % minor)
            return str(major) + "." + str(minor)
        else:
            return False

# ----------------------------------------------------------------------
    def getDeviceFlashStart(self):
# ----------------------------------------------------------------------
        """ Get user program start address """
        if self.QUERY_DEVICE_CMD_SUPPORTED == True:
            start = (self.QUERY_DEVICE_BUFFER[self.BOOT_MEMSTART + 0]      ) | \
                    (self.QUERY_DEVICE_BUFFER[self.BOOT_MEMSTART + 1] <<  8) | \
                    (self.QUERY_DEVICE_BUFFER[self.BOOT_MEMSTART + 2] << 16) | \
                    (self.QUERY_DEVICE_BUFFER[self.BOOT_MEMSTART + 3] << 24)
            return start
        else:
            return False
            
# ----------------------------------------------------------------------
    def getDeviceFlashFree(self):
# ----------------------------------------------------------------------
        """ Get available free memory """
        if self.QUERY_DEVICE_CMD_SUPPORTED == True:
            free = (self.QUERY_DEVICE_BUFFER[self.BOOT_MEMFREE + 0]      ) | \
                   (self.QUERY_DEVICE_BUFFER[self.BOOT_MEMFREE + 1] <<  8) | \
                   (self.QUERY_DEVICE_BUFFER[self.BOOT_MEMFREE + 2] << 16)
            return free
        else:
            return False
            
# ----------------------------------------------------------------------
    def getDeviceName(self, device_id):
# ----------------------------------------------------------------------
        """ Get device name from device ID """
        for n in self.devices_table:
            if n == device_id:
                return self.devices_table[n][0]
        return self.ERR_DEVICE_NOT_FOUND

# ----------------------------------------------------------------------
    def readFlash(self, address, length):
# ----------------------------------------------------------------------
        """ read a block of flash """
        # command code
        usbBuf = [self.GET_DATA_CMD] * self.MAXPACKETSIZE

        # address
        usbBuf[self.BOOT_ADDR + 0] = (address      ) & 0xFF
        usbBuf[self.BOOT_ADDR + 1] = (address >> 8 ) & 0xFF
        usbBuf[self.BOOT_ADDR + 2] = (address >> 16) & 0xFF
        usbBuf[self.BOOT_ADDR + 3] = (address >> 24) & 0xFF

        # size of block to read
        usbBuf[self.BOOT_CMD_SIZE] = length

        # send request to the bootloader and return response
        return self.getResponse(usbBuf)
            
# ----------------------------------------------------------------------
    def eraseFlash(self):
# ----------------------------------------------------------------------
        """ Erase the whole flash memory """
        usbBuf = [self.ERASE_DEVICE_CMD] * self.MAXPACKETSIZE
        return self.sendCMD(usbBuf)

# ----------------------------------------------------------------------
    def writeFlash(self, address, block):
# ----------------------------------------------------------------------
        """ Write a block of code """
        
        # Convert Virtual Address to Physical Address if necessary
        # as NVMADDR only accept physical address
        #address = address & 0x1FFFFFFF

        length = len(block)

        logging.info("writing %d-byte block at 0x%08X" % (length, address))

        if length == 0:
            # Short data packets need flushing
            usbBuf = [self.PROGRAM_COMPLETE_CMD] * self.MAXPACKETSIZE
            return self.sendCMD(usbBuf)

        """
        struct __attribute__ ((packed))
        {
            unsigned char Command;
            unsigned long Address;
            unsigned char Size;
            unsigned char PadBytes[(TotalPacketSize - 6) - (RequestDataBlockSize)];    
            unsigned int Data[RequestDataBlockSize/WORDSIZE];
        };
        """

        # command code
        usbBuf = [self.PROGRAM_DEVICE_CMD] * self.MAXPACKETSIZE

        # block start address
        usbBuf[self.BOOT_ADDR + 0] = (address      ) & 0xFF
        usbBuf[self.BOOT_ADDR + 1] = (address >> 8 ) & 0xFF
        usbBuf[self.BOOT_ADDR + 2] = (address >> 16) & 0xFF
        usbBuf[self.BOOT_ADDR + 3] = (address >> 24) & 0xFF

        # block's address (0x12345678 => "12345678")
        #address = "%08X" % (address / self.BYTESPERADDRESS)
        #usbBuf[self.BOOT_ADDR + 0] = int(address[6:8], 16)
        #usbBuf[self.BOOT_ADDR + 1] = int(address[4:6], 16)
        #usbBuf[self.BOOT_ADDR + 2] = int(address[2:4], 16)
        #usbBuf[self.BOOT_ADDR + 3] = int(address[0:2], 16)

        # data's length in bytes
        usbBuf[self.BOOT_CMD_SIZE] = length
        
        # pad bytes
        # 64 bytes (USB Packet Size ) - 6 bytes (Command) = 58 not divisible by 4
        # 58 bytes - 2 bytes = 56 bytes (Data block size) which is divisible by 4
        usbBuf[self.BOOT_CMD_PAD + 0] = 0
        usbBuf[self.BOOT_CMD_PAD + 1] = 0

        # add data 'right justified' within packet
        # note : the bootloader will 'left justified' all data at reception
        for i in range(length):
            usbBuf[self.MAXPACKETSIZE - length + i] = block[i]

        #logging.info(str(usbBuf).strip('[]'))

        # write data packet on usb device
        status = self.sendCMD(usbBuf)

        if (status == self.ERR_NONE) and (length < self.DATABLOCKSIZE):
            # Short data packets need flushing
            self.add_report("Short data packet at 0x%08X flushed" % address)
            #usbBuf = [self.PROGRAM_COMPLETE_CMD] * self.MAXPACKETSIZE
            usbBuf[self.BOOT_CMD] = self.PROGRAM_COMPLETE_CMD
            status = self.sendCMD(usbBuf)

        return status

# ----------------------------------------------------------------------
    def hexWrite(self, filename, board):
# ----------------------------------------------------------------------
        """
            Parse the Hex File Format and send data to usb device
            [0]      Start code, one character, an ASCII colon ':'.
            [1:3]    Byte count, two hex digits, a number of bytes (hex digit pairs) in the data field. 16 (0x10) or 32 (0x20) bytes of data are the usual compromise values between line length and address overhead.
            [3:7]    Address, four hex digits, a 16-bit address of the beginning of the memory position for the data. Limited to 64 kilobytes, the limit is worked around by specifying higher bits via additional record types. This address is big endian.
            [7:9]    Record type, two hex digits, 00 to 05, defining the type of the data field.
            [9:*]    Data, a sequence of n bytes of the data themselves, represented by 2n hex digits.
            [*:*]    Checksum, two hex digits - the least significant byte of the two's complement of the sum of the values of all fields except fields 1 and 6 (Start code ":" byte and two hex digits of the Checksum). It is calculated by adding together the hex-encoded bytes (hex digit pairs), then leaving only the least significant byte of the result, and making a 2's complement (either by subtracting the byte from 0x100, or inverting it by XOR-ing with 0xFF and adding 0x01). If you are not working with 8-bit variables, you must suppress the overflow by AND-ing the result with 0xFF. The overflow may occur since both 0x100-0 and (0x00 XOR 0xFF)+1 equal 0x100. If the checksum is correctly calculated, adding all the bytes (the Byte count, both bytes in Address, the Record type, each Data byte and the Checksum) together will always result in a value wherein the least significant byte is zero (0x00).
                     For example, on :0300300002337A1E
                     03 + 00 + 30 + 00 + 02 + 33 + 7A = E2, 2's complement is 1E
        """

        k0_prog_flash_memory = []
        #k0_boot_flash_memory = []
        #k1_boot_flash_memory = []
        old_k0pfm_address = 0
        #old_k0bfm_address = 0
        #old_k1bfm_address = 0
        max_k0pfm_address = 0
        #max_k0bfm_address = 0
        #max_k1bfm_address = 0
        address_Hi  = 0
        codesize    = 0

        #self.add_report("board.memstart = 0x%08X\r\n" % board.memstart)
        #self.add_report("board.memend = 0x%08X\r\n" % board.memend)

        # image of the Program Flash Memory
        # --------------------------------------------------------------

        for i in range(board.memend - board.memstart):
            k0_prog_flash_memory.append(0xFF)

        # image of the Boot Flash Memory
        # --------------------------------------------------------------

        """
        for i in range(self.BOOT_FLASH_SIZE):
            k0_boot_flash_memory.append(0xFF)
            k1_boot_flash_memory.append(0xFF)
        """
        
        # load hex file
        # ----------------------------------------------------------------------

        hexfile = open(filename, 'r')
        lines = hexfile.readlines()
        hexfile.close()

        # read each line in file
        # --------------------------------------------------------------

        for line in lines:

            byte_count = int(line[1:3], 16)
            # lower 16 bits (bits 0-15) of the data address
            address_Lo = int(line[3:7], 16)
            record_type= int(line[7:9], 16)

            # checksum calculation
            # ----------------------------------------------------------

            end = 9 + byte_count * 2 # position of checksum at end of line
            checksum = int(line[end:end+2], 16)

            cs = 0
            i = 1
            while i < end:
                cs = cs + (0x100 - int(line[i:i+2], 16) ) & 0xff # eq. to not(i)
                i = i + 2

            if checksum != cs:
                return self.ERR_HEX_CHECKSUM

            # extended linear address record
            # ----------------------------------------------------------

            if record_type == self.Extended_Linear_Address_Record:

                # upper 16 bits (bits 16-31) of the data address
                address_Hi = int(line[9:13], 16) << 16

            # data record
            # ----------------------------------------------------------

            elif record_type == self.Data_Record or \
                 record_type == self.Start_Linear_Address_Record:

                address = address_Hi + address_Lo
                #self.add_report("address = 0x%08X" % address)

                if (address >= board.memstart) and (address < board.memend):

                    # max pfm address
                    if (address > old_k0pfm_address):
                        max_k0pfm_address = address + byte_count
                        old_k0pfm_address = address
                        #self.add_report("max_k0pfm_address = %d" % max_k0pfm_address)

                    # code size
                    if (address >= board.memstart):
                        codesize = codesize + byte_count
                        #self.add_report("codesize = %d" % codesize)

                    # pfm data append
                    for i in range(byte_count):
                        k0_prog_flash_memory[address - board.memstart + i] = \
                            int(line[9 + (2 * i) : 11 + (2 * i)], 16)

                """
                if (address >= self.KSEG0_BOOT_FLASH) and \
                   (address < (self.KSEG0_BOOT_FLASH + self.BOOT_FLASH_SIZE) ):

                    # max k0 bfm address
                    if (address > old_k0bfm_address):
                        max_k0bfm_address = address + byte_count
                        old_k0bfm_address = address
                        #self.add_report("max_k0bfm_address = %d" % max_k0bfm_address)

                    # k0 bfm data append
                    for i in range(byte_count):
                        k0_boot_flash_memory[address - self.KSEG0_BOOT_FLASH + i] = \
                            int(line[9 + (2 * i) : 11 + (2 * i)], 16)

                if (address >= self.KSEG1_BOOT_FLASH) and \
                   (address < (self.KSEG1_BOOT_FLASH + self.BOOT_FLASH_SIZE) ):

                    # max k1 bfm address
                    if (address > old_k1bfm_address):
                        max_k1bfm_address = address + byte_count
                        old_k1bfm_address = address
                        #self.add_report("max_k1bfm_address = %d" % max_k1bfm_address)

                    # k1 bfm data append
                    for i in range(byte_count):
                        k1_boot_flash_memory[address - self.KSEG1_BOOT_FLASH + i] = \
                            int(line[9 + (2 * i) : 11 + (2 * i)], 16)
                """

            # end of file record
            # ----------------------------------------------------------

            elif record_type == self.End_Of_File_Record:

                break

            # unsupported record type
            # ----------------------------------------------------------

            else:

                self.add_report("Caution : unsupported record type (%d) in hex file" % record_type)
                self.add_report("Line %s" % line)
                #return self.ERR_HEX_RECORD

        # max_k0pfm_address must be divisible by self.DATABLOCKSIZE
        # --------------------------------------------------------------

        #max_k0pfm_address = max_k0pfm_address + self.MAXPACKETSIZE - (max_k0pfm_address % self.MAXPACKETSIZE)

        #max_k0pfm_address = max_k0pfm_address + 64 - (max_k0pfm_address % 64)
        #max_k0pfm_address = max_k0pfm_address + self.DATABLOCKSIZE - (max_k0pfm_address % self.DATABLOCKSIZE)
        
        #Correction André du 13/10/2014
        max_k0pfm_address = max_k0pfm_address + self.DATABLOCKSIZE - (codesize % self.DATABLOCKSIZE)
        
        #Correction Régis du 13/10/2014
        #max_k0pfm_address = self.DATABLOCKSIZE * ( 1 + int(max_k0pfm_address / self.DATABLOCKSIZE) )
        
        #max_k0bfm_address = max_k0bfm_address + 64 - (max_k0bfm_address % 64)
        #max_k1bfm_address = max_k1bfm_address + 64 - (max_k1bfm_address % 64)
        #self.add_report("max_k0pfm_address = 0x%08X" % max_k0pfm_address)
        #self.add_report("max_k0bfm_address = 0x%08X" % max_k0bfm_address)
        #self.add_report("max_k1bfm_address = 0x%08X" % max_k1bfm_address)

        # write blocks of DATABLOCKSIZE bytes in k0 pfm
        # --------------------------------------------------------------

        for addr in range(board.memstart, max_k0pfm_address, self.DATABLOCKSIZE):
            index = addr - board.memstart
            #self.add_report("Writing block at 0x%08X" % addr)
            #self.add_report("index = %d" % index)
            #self.add_report("Block=")
            #for i in range(self.DATABLOCKSIZE):
            #    self.add_report("[%x]" % k0_prog_flash_memory[index+i])
            status = self.writeFlash(addr, k0_prog_flash_memory[index:index+self.DATABLOCKSIZE])
            if (status != self.ERR_NONE):
                return status

        logging.info("Writing process complete...")
        usbBuf = [self.PROGRAM_COMPLETE_CMD] * self.MAXPACKETSIZE
        #usbBuf[self.BOOT_CMD] = self.PROGRAM_COMPLETE_CMD
        status = self.sendCMD(usbBuf)

        self.add_report("%d bytes written." % codesize)

        """
        # write blocks of DATABLOCKSIZE bytes in k0 bfm 
        # --------------------------------------------------------------

        for addr in range(self.KSEG0_BOOT_FLASH, max_k0bfm_address, self.DATABLOCKSIZE):
            index = addr - self.KSEG0_BOOT_FLASH
            #self.add_report("index = %d" % index)
            #self.add_report("data = %s" % k0_boot_flash_memory[index:index+self.DATABLOCKSIZE])
            self.writeFlash(addr, k0_boot_flash_memory[index:index+self.DATABLOCKSIZE])

        usbBuf = [self.PROGRAM_COMPLETE_CMD] * self.MAXPACKETSIZE
        #usbBuf[self.BOOT_CMD] = self.PROGRAM_COMPLETE_CMD
        status = status + self.sendCMD(usbBuf)

        # write blocks of DATABLOCKSIZE bytes in k1 bfm 
        # --------------------------------------------------------------

        for addr in range(self.KSEG1_BOOT_FLASH, max_k1bfm_address, self.DATABLOCKSIZE):
            index = addr - self.KSEG1_BOOT_FLASH
            #self.add_report("index = %d" % index)
            #self.add_report("data = %s" % k1_boot_flash_memory[index:index+self.DATABLOCKSIZE])
            self.writeFlash(addr, k1_boot_flash_memory[index:index+self.DATABLOCKSIZE])

        usbBuf = [self.PROGRAM_COMPLETE_CMD] * self.MAXPACKETSIZE
        #usbBuf[self.BOOT_CMD] = self.PROGRAM_COMPLETE_CMD
        status = status + self.sendCMD(usbBuf)
        """
        
        # end
        # --------------------------------------------------------------

        return status
        #return self.ERR_NONE

# ----------------------------------------------------------------------
    def writeHex(self):
# ----------------------------------------------------------------------

        # check file to upload
        # --------------------------------------------------------------

        if self.filename == '':
            self.add_report("No program to write")
            return

        hexfile = open(self.filename, 'r')

        if hexfile == "":
            self.add_report("Unable to open %s" % self.filename)
            return

        hexfile.close()

        # search for a Pinguino board
        # --------------------------------------------------------------

        self.device = self.getDevice()
        #self.add_report("Device = %s" % self.device)

        if self.device == self.ERR_DEVICE_NOT_FOUND:

            self.add_report("Pinguino not found")
            self.add_report("If your device is connected switch to bootloader mode.")
            return

        self.add_report("Pinguino found ...")

        self.handle = self.initDevice()
        #self.add_report("Handle = %s" % self.handle)

        if self.handle == self.ERR_USB_INIT1:

            self.add_report("Upload not possible")
            self.add_report("Try to restart the bootloader mode")
            return

        #elif self.handle == None:
        #    return

        #self.add_report("%s - %s" % (handle.getString(device.iProduct, 30), handle.getString(device.iManufacturer, 30))
        #self.add_report("%s" % handle.getString(device.iProduct, 30)

        # find out supported commands
        # --------------------------------------------------------------

        if self.getCommands() == self.ERR_USB_WRITE:
            self.add_report("Aborting : write error!")
            self.closeDevice()
            return

        # find out processor family
        # --------------------------------------------------------------

        logging.info("Getting processor family ...")
        fam = self.getDeviceFamily()
        if fam:
            logging.info("Device family = %d" % fam)
            if  fam != self.DEVICE_FAMILY_PIC32:
                self.add_report("Aborting : not a PIC32 family device.")
                self.closeDevice()
                return

        # find out processor name
        # --------------------------------------------------------------

        logging.info("Getting processor name ...")
        devid = self.getDeviceID()
        if devid:
            logging.info("Device ID = %08X" % devid)
            # mask device id to get revision number
            rev = ( ( devid >> 24 ) & 0xF0 ) >> 4
            # mask revision number to get device id
            pid  = devid & 0x0FFFFFFF
            # get proc. name
            proc = self.getDeviceName(pid)
            self.add_report(" - with PIC%s (ID 0x%08X, rev. A%01X)" % (proc, pid, rev))
            if proc != self.board.proc:
                self.add_report("Aborting: program compiled for %s but device has %s" % (self.board.proc, proc))
                self.closeDevice()
                return
                
        # find out clock frequencies
        # --------------------------------------------------------------

        logging.info("Getting clock frequencies ...")
        fcpu = self.getDeviceFCPU()
        fpb = self.getDeviceFPB()
        if fcpu and fpb:
            logging.info("FCPU = %dHz" % fcpu)
            logging.info("FPB  = %dHz" % fpb)
            self.add_report("   System  clock %.3f MHz" % ( fcpu/1000000.0 ))
            self.add_report("   Periph. clock %.3f MHz" % (  fpb/1000000.0 ))

        # find out flash memory size
        # addresses MUST BE virtual NOT physical
        # --------------------------------------------------------------

        logging.info("Getting flash memory size ...")
        memstart = self.getDeviceFlashStart()
        memfree  = self.getDeviceFlashFree()
        if memstart and memfree:
            memend   = memstart + memfree

            # Convert KSEG1 to KSEG0
            # and Physical to Virtual address
            if memstart > 0xBD000000:
                memstart = memstart - 0x20000000
            memstart = memstart | 0x80000000
            if memend  > 0xBD000000:
                memend   = memend   - 0x20000000
            memend   = memend   | 0x80000000
            logging.info("memstart=0x%08X" % memstart)
            logging.info("memend=0x%08X" % memend)

            #self.add_report("memstart=0x%08X" % memstart)
            #self.add_report("memend=0x%08X" % memend)
            #self.add_report("board.memend=0x%08X" % self.board.memend)
            #self.add_report("memfree=0x%08X" % memfree)

            if self.board.memstart != memstart:
                self.add_report("Conflict : free flash memory should start at 0x%08X not 0x%08X" % (memstart, self.board.memstart))
                self.add_report("This issue has been automatically fixed.")
                self.add_report("Please report it at https://github.com/PinguinoIDE/pinguino-ide/issues")
                self.board.memstart = memstart

            if self.board.memend != memend:
                self.add_report("Conflict : free flash memory should stop at 0x%08X not 0x%08X" % (memend, self.board.memend))
                self.add_report("This issue has been automatically fixed.")
                self.add_report("Please report it at https://github.com/PinguinoIDE/pinguino-ide/issues")
                self.board.memend = memend

        memfree = self.board.memend - self.board.memstart
        self.add_report(" - with %d bytes free (%d KB)" % (memfree, memfree/1024))
        self.add_report("   from 0x%08X to 0x%08X" % (self.board.memstart, self.board.memend))
            
        # find out bootloader version
        # --------------------------------------------------------------

        logging.info("Getting bootloader version ...")
        version = self.getVersion()
        if version:
            self.add_report(" - with Pinguino USB HID Bootloader v%s" % version)
        else:
            self.add_report(" - with Microchip USB HID Bootloader")

        ################################################################
        # Uncomment to test only the QUERY_DEVICE and GET_DATA commands
        ################################################################
        #self.closeDevice()
        #return
        ################################################################

        # start erasing
        # --------------------------------------------------------------

        self.add_report("Erasing flash memory ...")
        status = self.eraseFlash()
        if status != self.ERR_NONE:
            self.add_report("Aborting: erase error!")
            self.closeDevice()
            return

        # start writing
        # --------------------------------------------------------------

        self.add_report("Uploading user program ...")
        status = self.hexWrite(self.filename, self.board)
        if status != self.ERR_NONE:
            self.add_report("Aborting: write error code %d" % status)
            self.closeDevice()
            return

        self.add_report("%s successfully uploaded." % os.path.basename(self.filename))

        # reset and start start user's app.
        # --------------------------------------------------------------

        #self.add_report("Resetting ...")
        self.add_report("Starting user program ...")
        logging.info("Sending Reset command ...")
        status = self.resetDevice()
        if status != self.ERR_NONE:
            self.closeDevice()
        
        return
        
# ----------------------------------------------------------------------
