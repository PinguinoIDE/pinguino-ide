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
import usb
import logging

from ..tools import Debugger
from .uploader import baseUploader

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
    BOOT_IVTSTART                   =    4      # long = 4 bytes
    BOOT_LEN1                       =    8      # long = 4 bytes
    BOOT_MEMFREE                    =    8      # long = 4 bytes
    BOOT_TYPE2                      =    12     # char = 1 byte
    BOOT_ADDR2                      =    13     # long = 4 bytes
    BOOT_LEN2                       =    17     # long = 4 bytes
    BOOT_TYPE3                      =    21     # char = 1 byte
    BOOT_ADDR3                      =    22     # long = 4 bytes
    BOOT_LEN3                       =    26     # long = 4 bytes

    BOOT_TYPE_LEN                   =    9      # char + long + long = 9 bytes

    BOOT_DEVID1                     =    8
    BOOT_DEVID2                     =    60

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
    WRITE_DEVICE_CMD                =    0x05    # to send a full RequestDataBlockSize to be programmed
    WRITE_COMPLETE_CMD              =    0x06    # if host send less than a RequestDataBlockSize to be programmed, or if it wished to program whatever was left in the buffer, it uses this command
    READ_DEVICE_CMD                 =    0x07    # the host sends this command in order to read out memory from the device. Used during verify (and read/export hex operations)
    RESET_DEVICE_CMD                =    0x08    # resets the microcontroller, so it can update the config bits (if they were programmed, and so as to leave the bootloader (and potentially go back into the main application)

    READ_DEVICE_CMD_SUPPORTED       =   False
    QUERY_DEVICE_CMD_SUPPORTED      =   False
    READ_DEVICE_BUFFER              =   []
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

    DEVICE_ID_ADDRESS               =    0xBF80F220

    # Table with supported USB devices
    # device_id:['CPU name']
    #-----------------------------------------------------------------------
    """
    : ['32MX110F016B'], 0x04A07053
    : ['32MX110F016C'], 0x04A09053
    : ['32MX110F016D'], 0x04A0B053
    : ['32MX120F032B'], 0x04A06053
    : ['32MX120F032C'], 0x04A08053
    : ['32MX120F032D'], 0x04A0A053
    : ['32MX130F064B'], 0x04D07053
    : ['32MX130F064C'], 0x04D09053
    : ['32MX130F064D'], 0x04D0B053
    : ['32MX150F128B'], 0x04D06053
    : ['32MX150F128C'], 0x04D08053
    : ['32MX150F128D'], 0x04D0A053
    : ['32MX210F016B'], 0x04A01053
    : ['32MX210F016C'], 0x04A03053
    : ['32MX210F016D'], 0x04A05053
    : ['32MX220F032B'], 0x04A00053
    : ['32MX220F032C'], 0x04A02053
    : ['32MX220F032D'], 0x04A04053
    : ['32MX230F064B'], 0x04D01053
    : ['32MX230F064C'], 0x04D03053
    : ['32MX230F064D'], 0x04D05053
    : ['32MX250F128B'], 0x04D00053
    : ['32MX250F128C'], 0x04D02053
    : ['32MX250F128D'], 0x04D04053

    : ['32MX330F064H'], 0x05600053
    : ['32MX330F064L'], 0x05601053
    : ['32MX430F064H'], 0x05602053
    : ['32MX430F064L'], 0x05603053
    : ['32MX350F128H'], 0x0570C053
    : ['32MX350F128L'], 0x0570D053
    : ['32MX450F128H'], 0x0570E053
    : ['32MX450F128L'], 0x0570F053
    : ['32MX350F256H'], 0x05704053
    : ['32MX350F256L'], 0x05705053
    : ['32MX450F256H'], 0x05706053
    : ['32MX450F256L'], 0x05707053
    : ['32MX370F512H'], 0x05808053
    : ['32MX370F512L'], 0x05809053
    : ['32MX470F512H'], 0x0580A053
    : ['32MX470F512L'], 0x0580B053

    : ['32MX360F512L'], 0x00938053
    : ['32MX360F256L'], 0x00934053
    : ['32MX340F128L'], 0x0092D053
    : ['32MX320F128L'], 0x0092A053
    : ['32MX340F512H'], 0x00916053
    : ['32MX340F256H'], 0x00912053
    : ['32MX340F128H'], 0x0090D053
    : ['32MX320F128H'], 0x0090A053
    : ['32MX320F064H'], 0x00906053
    : ['32MX320F032H'], 0x00902053
    : ['32MX460F512L'], 0x00978053
    : ['32MX460F256L'], 0x00974053
    : ['32MX440F128L'], 0x0096D053
    : ['32MX440F256H'], 0x00952053
    : ['32MX440F512H'], 0x00956053
    : ['32MX440F128H'], 0x0094D053
    : ['32MX420F032H'], 0x00942053

    : ['32MX534F064H'], 0x04400053
    : ['32MX534F064L'], 0x0440C053
    : ['32MX564F064H'], 0x04401053
    : ['32MX564F064L'], 0x0440D053
    : ['32MX564F128H'], 0x04403053
    : ['32MX564F128L'], 0x0440F053
    : ['32MX575F256H'], 0x04317053
    : ['32MX575F256L'], 0x04333053
    : ['32MX575F512H'], 0x04309053
    : ['32MX575F512L'], 0x0430F053
    : ['32MX664F064H'], 0x04405053
    : ['32MX664F064L'], 0x04411053
    : ['32MX664F128H'], 0x04407053
    : ['32MX664F128L'], 0x04413053
    : ['32MX675F256H'], 0x0430B053
    : ['32MX675F256L'], 0x04305053
    : ['32MX675F512H'], 0x0430C053
    : ['32MX675F512L'], 0x04311053
    : ['32MX695F512H'], 0x04325053
    : ['32MX695F512L'], 0x04341053
    : ['32MX764F128H'], 0x0440B053
    : ['32MX764F128L'], 0x04417053
    : ['32MX775F256H'], 0x04303053
    : ['32MX775F256L'], 0x04312053
    : ['32MX775F512H'], 0x0430D053
    : ['32MX775F512L'], 0x04306053
    : ['32MX795F512H'], 0x0430E053
    : ['32MX795F512L'], 0x04307053

    : ['32MZ1024ECG064'], 0x05103053
    : ['32MZ1024ECG100'], 0x0510D053
    : ['32MZ1024ECG124'], 0x05117053
    : ['32MZ1024ECG144'], 0x05121053
    : ['32MZ1024ECH064'], 0x05108053
    : ['32MZ1024ECH100'], 0x05112053
    : ['32MZ1024ECH124'], 0x0511C053
    : ['32MZ1024ECH144'], 0x05126053
    : ['32MZ2048ECG064'], 0x05104053
    : ['32MZ2048ECG100'], 0x0510E053
    : ['32MZ2048ECG124'], 0x05118053
    : ['32MZ2048ECG144'], 0x05122053
    : ['32MZ2048ECH064'], 0x05109053
    : ['32MZ2048ECH100'], 0x05113053
    : ['32MZ2048ECH124'], 0x0511D053
    : ['32MZ2048ECH144'], 0x05127053
    : ['32MZ1024ECM064'], 0x05130053
    : ['32MZ2048ECM064'], 0x05131053
    : ['32MZ1024ECM100'], 0x0513A053
    : ['32MZ2048ECM100'], 0x0513B053
    : ['32MZ1024ECM124'], 0x05144053
    : ['32MZ2048ECM124'], 0x05145053
    : ['32MZ1024ECM144'], 0x0514E053
    : ['32MZ2048ECM144'], 0x0514F053

    : ['32MX170F256B'], 0x06610053
    : ['32MX170F256D'], 0x0661A053
    : ['32MX270F256B'], 0x06600053
    : ['32MX270F256D'], 0x0660A053
    """

    devices_table = \
        {
            0x04A00053: ['32MX220F032B'],
            0x04A04053: ['32MX220F032D'],
            0x04D00053: ['32MX250F128B'],
            0x06600053: ['32MX270F256B'],
            0x00952053: ['32MX440F256H'],
            0x00978053: ['32MX460F512L'],
            0x0430E053: ['32MX795F512H'],
            0x04307053: ['32MX795F512L']
        }

# ----------------------------------------------------------------------
    def resetDevice(self, handle):
# ----------------------------------------------------------------------
        """ Reset device """
        usbBuf = [self.RESET_DEVICE_CMD] * self.MAXPACKETSIZE

        try:
            handle.interruptWrite(self.OUT_EP, usbBuf, self.TIMEOUT)

        except Exception as e:
            logging.info(e)
            return self.ERR_USB_WRITE

        return self.ERR_NONE

# ----------------------------------------------------------------------
    def sendPacket(self, handle, usbBuf):
# ----------------------------------------------------------------------
        """ Send a packet to the bootloader """

        try:
            sent_bytes = handle.interruptWrite(self.OUT_EP, usbBuf, self.TIMEOUT)
        except Exception as e:
            logging.info(e)
            return self.ERR_USB_WRITE

        if sent_bytes == len(usbBuf):
            #logging.info("%d bytes successfully sent." % sent_bytes)
            return self.ERR_NONE

        else:
            logging.info("Sent %d/%d bytes" % (sent_bytes, len(usbBuf)))
            return self.ERR_USB_WRITE

# ----------------------------------------------------------------------
    def sendCommand(self, handle, command):
# ----------------------------------------------------------------------
        """ Send a command to the bootloader """

        usbBuf = [command] * self.MAXPACKETSIZE
        return self.sendPacket(handle, usbBuf)

# ----------------------------------------------------------------------
    def getResponse(self, handle):
# ----------------------------------------------------------------------
        """ Get a response from the bootloader """
        """ Assumes a command has been sent before """

        try:
            usbBuf = handle.interruptRead(self.IN_EP, self.MAXPACKETSIZE, self.TIMEOUT)
        except Exception as e:
            logging.info("Received nothing : %s" % e)
            return self.ERR_USB_READ

        logging.info("Received a packet")
        """
        logging.info("Received : %s" % str(usbBuf).strip('[]'))
        for i in range(sent_bytes):
            logging.info("usbBuf[%d] = %02X" % (i, usbBuf[i]) )
        """

        return usbBuf

# ----------------------------------------------------------------------
    def getCommands(self, handle, board):
# ----------------------------------------------------------------------
        """ Get supported commands """

        # 1/ try QUERY_DEVICE_CMD

        logging.info("Trying with QUERY_DEVICE ...")
        status = self.sendCommand(handle, self.QUERY_DEVICE_CMD)
        if status == self.ERR_NONE:
            self.QUERY_DEVICE_BUFFER = self.getResponse(handle)
            if self.QUERY_DEVICE_BUFFER == self.ERR_USB_READ:
                logging.info("QUERY_DEVICE not supported")
                self.QUERY_DEVICE_CMD_SUPPORTED = False
            else:
                logging.info("QUERY_DEVICE supported")
                self.QUERY_DEVICE_CMD_SUPPORTED = True
        else :
            return status

        # 2/ try GET_DATA_CMD

        if board.proc != '32MX220F032D' and \
           board.proc != '32MX220F032B' and \
           board.proc != '32MX250F128B':

            logging.info("Trying with GET_DATA ...")
            usbBuf = [self.READ_DEVICE_CMD] * self.MAXPACKETSIZE
            usbBuf[self.BOOT_ADDR + 0] = (self.DEVICE_ID_ADDRESS      ) & 0xFF
            usbBuf[self.BOOT_ADDR + 1] = (self.DEVICE_ID_ADDRESS >> 8 ) & 0xFF
            usbBuf[self.BOOT_ADDR + 2] = (self.DEVICE_ID_ADDRESS >> 16) & 0xFF
            usbBuf[self.BOOT_ADDR + 3] = (self.DEVICE_ID_ADDRESS >> 24) & 0xFF
            usbBuf[self.BOOT_CMD_SIZE] = 4

            status = self.sendPacket(handle, usbBuf)
            if status == self.ERR_NONE:
                self.READ_DEVICE_BUFFER = self.getResponse(handle)
                if self.READ_DEVICE_BUFFER == self.ERR_USB_READ:
                    logging.info("GET_DATA not supported")
                    self.READ_DEVICE_CMD_SUPPORTED = False
                else:
                    logging.info("GET_DATA supported")
                    self.READ_DEVICE_CMD_SUPPORTED = True
            else :
                return status

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
        if self.READ_DEVICE_CMD_SUPPORTED == True:
            devid1 = (self.READ_DEVICE_BUFFER[self.BOOT_DEVID1 + 0]      ) | \
                (self.READ_DEVICE_BUFFER[self.BOOT_DEVID1 + 1] <<  8) | \
                (self.READ_DEVICE_BUFFER[self.BOOT_DEVID1 + 2] << 16) | \
                (self.READ_DEVICE_BUFFER[self.BOOT_DEVID1 + 3] << 24)
            devid2 = (self.READ_DEVICE_BUFFER[self.BOOT_DEVID2 + 0]      ) | \
                (self.READ_DEVICE_BUFFER[self.BOOT_DEVID2 + 1] <<  8) | \
                (self.READ_DEVICE_BUFFER[self.BOOT_DEVID2 + 2] << 16) | \
                (self.READ_DEVICE_BUFFER[self.BOOT_DEVID2 + 3] << 24)
            # at least one is null
            devid = devid1 + devid2
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
            if major == 0 and minor == 0:
                logging.info("No")
                return False
            else:
                logging.info("Yes, major = %d" % major)
                logging.info("Yes, minor = %d" % minor)
                return str(major) + "." + str(minor)
        else:
            return False

# ----------------------------------------------------------------------
    def getDeviceFlashStart(self):
# ----------------------------------------------------------------------
        """ Get user program start address """
        if self.QUERY_DEVICE_CMD_SUPPORTED == True:
            start = (self.QUERY_DEVICE_BUFFER[self.BOOT_IVTSTART + 0]      ) | \
                (self.QUERY_DEVICE_BUFFER[self.BOOT_IVTSTART + 1] <<  8) | \
                (self.QUERY_DEVICE_BUFFER[self.BOOT_IVTSTART + 2] << 16) | \
                (self.QUERY_DEVICE_BUFFER[self.BOOT_IVTSTART + 3] << 24)
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
    def readFlash(self, handle, address, length):
# ----------------------------------------------------------------------
        """ read a block of bytes in flash memory"""

        # command code
        usbBuf = [self.READ_DEVICE_CMD] * self.MAXPACKETSIZE

        # address
        usbBuf[self.BOOT_ADDR + 0] = (address      ) & 0xFF
        usbBuf[self.BOOT_ADDR + 1] = (address >> 8 ) & 0xFF
        usbBuf[self.BOOT_ADDR + 2] = (address >> 16) & 0xFF
        usbBuf[self.BOOT_ADDR + 3] = (address >> 24) & 0xFF

        # size of block to read
        usbBuf[self.BOOT_CMD_SIZE] = length

        # send request to the bootloader and return response
        return self.sendPacketWithResponse(usbBuf)

# ----------------------------------------------------------------------
    def eraseFlash(self, handle):
# ----------------------------------------------------------------------
        """ Erase the whole flash memory """
        return self.sendCommand(handle, self.ERASE_DEVICE_CMD)

# ----------------------------------------------------------------------
    def writeFlash(self, handle, address, block):
# ----------------------------------------------------------------------
        """ Write a block of code """

        # Convert Virtual Address to Physical Address if necessary
        # as NVMADDR only accept physical address
        #address = address & 0x1FFFFFFF

        length = len(block)

        #logging.info("writing %d-byte block at 0x%08X" % (length, address))

        #logging.info(str(block).strip('[]'))

        if length == 0:
            logging.info("Zero-length data packet at 0x%08X flushed" % address)
            return self.sendCommand(handle, self.WRITE_COMPLETE_CMD)

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
        usbBuf = [self.WRITE_DEVICE_CMD] * self.MAXPACKETSIZE

        # block start address
        usbBuf[self.BOOT_ADDR + 0] = (address      ) & 0xFF
        usbBuf[self.BOOT_ADDR + 1] = (address >> 8 ) & 0xFF
        usbBuf[self.BOOT_ADDR + 2] = (address >> 16) & 0xFF
        usbBuf[self.BOOT_ADDR + 3] = (address >> 24) & 0xFF

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
        status = self.sendPacket(handle, usbBuf)
        #logging.info("status = %d" % status)

        if (status == self.ERR_NONE) and (length < self.DATABLOCKSIZE):
            # Short data packets need flushing
            logging.info("Short data packet at 0x%08X flushed" % address)
            #usbBuf = [self.WRITE_COMPLETE_CMD] * self.MAXPACKETSIZE
            #usbBuf[self.BOOT_CMD] = self.WRITE_COMPLETE_CMD
            status = self.sendCommand(handle, self.WRITE_COMPLETE_CMD)

        return status

# ----------------------------------------------------------------------
    def writeHex(self, handle, filename, board):
# ----------------------------------------------------------------------
        """
            Parse the Hex File Format and send data to usb device
            [0]      Start code, one character, an ASCII colon ':'.
            [1:3]    Byte count, two hex digits, a number of bytes (hex digit pairs) in the data field. 16 (0x10) or 32 (0x20) bytes of data are the usual compromise values between line length and address overhead.
            [3:7]    Address, four hex digits, a 16-bit address of the beginning of the memory position for the data. Limited to 64 kilobytes, the limit is worked around by specifying higher bits via additional record types. This address is big endian.
            [7:9]    Record type, two hex digits, 00 to 05, defining the type of the data field.
            [9:*]    Data, a sequence of n bytes of the data themselves, represented by 2n hex digits.
            [*:*]    Checksum, two hex digits - the least significant byte of the two's complement of the sum of the values of all fields except fields 1 and 6 (Start code ":" byte and two hex digits of the Checksum). It is calculated by adding together the hex-encoded bytes (hex digit pairs), then leaving only the least significant byte of the result, and making a 2's complement (either by subtracting the byte from 0x100, or inverting it by XOR-ing with 0xFF and adding 0x01). If you are not working with 8-bit variables, you must suppress the overflow by AND-ing the result with 0xFF. The overflow may occur since both 0x100-0 and'], 0x00 XOR 0xFF)+1 equal 0x100. If the checksum is correctly calculated, adding all the bytes (the Byte count, both bytes in Address, the Record type, each Data byte and the Checksum) together will always result in a value wherein the least significant byte is zero'], 0x00).
                     For example, on :0300300002337A1E
                     03 + 00 + 30 + 00 + 02 + 33 + 7A = E2, 2's complement is 1E
        """

        program_memory = []
        last_address = 0
        max_address  = 0
        min_address  = 0xFFFFFFFF
        address_Hi   = 0
        codesize     = 0

        # Memory image (IVT + RESET VECTOR + STARTUP SEQUENCE + USER APP.)
        # --------------------------------------------------------------

        logging.info("writing from min 0x%08X to max 0x%08X ..." % (board.ivtstart,board.memend))
        for i in range(board.memend - board.ivtstart):
            program_memory.append(0xFF)

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
            # ivtstart = where IVT starts (eBase)
            # memstart = where user's program starts
            # ----------------------------------------------------------

            elif record_type == self.Data_Record:

                address = address_Hi + address_Lo
                #logging.info("address = 0x%08X" % address)

                if (address >= board.ivtstart) and (address < board.memend):

                    # min program address
                    if (min_address > address):
                        min_address = address

                    # max program address
                    if (address > last_address):
                        max_address = address + byte_count
                        last_address = address
                        #logging.info("max_address = 0x%08X" % max_address)

                    # user's program code size
                    if (address >= board.memstart):
                        codesize = codesize + byte_count
                        #logging.info("codesize = %d" % codesize)

                    # program data append
                    for i in range(byte_count):
                        program_memory[address - board.ivtstart + i] = \
                            int(line[9 + (2 * i) : 11 + (2 * i)], 16)

            # Reset Vector
            # ----------------------------------------------------------

            elif record_type == self.Start_Linear_Address_Record:

                logging.info("Reset Vector = 0x%08X" % int(line[9:17], 16))

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

        # min and max address must be divisible by self.DATABLOCKSIZE
        # --------------------------------------------------------------

        # 13/10/2014 - fixed by André
        #max_address = max_address + self.DATABLOCKSIZE - (codesize % self.DATABLOCKSIZE)
        #max_address = max_address + self.DATABLOCKSIZE - (max_address % self.DATABLOCKSIZE)
        #logging.info("program max. address = 0x%08X" % max_address)

        # trim the memory image
        # --------------------------------------------------------------
        """
        logging.info("first byte to write at 0x%08X" % min_address)
        logging.info("last  byte to write at 0x%08X" % max_address)
        logging.info("index min. = %d (0x%X)" % \
            ((min_address - board.ivtstart),(min_address - board.ivtstart)))
        del program_memory[:(min_address - board.ivtstart)]
        logging.info("index max. = %d (0x%X)" % \
            ((max_address - board.ivtstart),(max_address - board.ivtstart)))
        del program_memory[(max_address - board.ivtstart):]
        """
        # write blocks of DATABLOCKSIZE bytes in program memory
        # --------------------------------------------------------------

        min_address = board.ivtstart;

        logging.info("writing from 0x%08X to 0x%08X ..." % (min_address,max_address))
        for addr in range(min_address, max_address, self.DATABLOCKSIZE):
            index = addr - min_address
            #logging.info("address, index = 0x%08X, %d" % (addr, index))
            status = self.writeFlash(handle, addr, program_memory[index:index+self.DATABLOCKSIZE])
            if (status != self.ERR_NONE):
                return status

        # end
        # --------------------------------------------------------------

        logging.info("Writing completed")
        self.add_report("%d bytes written." % codesize)
        status = self.sendCommand(handle, self.WRITE_COMPLETE_CMD)

        return status

# ----------------------------------------------------------------------
    def uploadDevice(self, filename, board):
# ----------------------------------------------------------------------

        # check file to upload
        # --------------------------------------------------------------

        if filename == '':
            self.add_report("No program to write")
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
            self.add_report("If your device is connected switch to bootloader mode.")
            return

        self.add_report("Pinguino found ...")

        handle = self.initDevice(device)

        if handle == self.ERR_USB_INIT1:

            self.add_report("Upload not possible")
            self.add_report("Try to restart the bootloader mode")
            return

        # find out supported commands
        # --------------------------------------------------------------

        logging.info("Checking supported commands ...")
        status = self.getCommands(handle, board)

        if status == self.ERR_USB_WRITE:
            self.add_report("Aborting : write error!")
            self.closeDevice(handle)
            return

        """
        if not self.READ_DEVICE_CMD_SUPPORTED or \
           not self.QUERY_DEVICE_CMD_SUPPORTED:
            logging.info("Reseting ...")
            self.closeDevice(handle)
            device = self.getDevice(board)
            handle = self.initDevice(device)
            #logging.info("Device reset")
            #self.resetDevice(handle)
            #handle.reset()
            #time.sleep(5)
            #handle.releaseInterface()
            #time.sleep(5)
            #handle.claimInterface(self.INTERFACE_ID)
            #handle.clearHalt(self.IN_EP)
            #handle.resetEndpoint(self.IN_EP)
            #self.initDevice(device)
        """

        # find out processor family
        # --------------------------------------------------------------

        logging.info("Getting processor family ...")
        fam = self.getDeviceFamily()
        if fam:
            logging.info("Yes, Device family = %d" % fam)
            if  fam != self.DEVICE_FAMILY_PIC32:
                self.add_report("Aborting : not a PIC32 family device.")
                self.closeDevice(handle)
                return
        else:
            logging.info("No")

        # find out processor name
        # --------------------------------------------------------------

        logging.info("Getting processor name ...")
        devid = self.getDeviceID()
        if devid:
            logging.info("Yes, Device ID = %08X" % devid)
            # mask device id to get revision number
            rev = ( ( devid >> 24 ) & 0xF0 ) >> 4
            # mask revision number to get device id
            pid  = devid & 0x0FFFFFFF
            # get proc. name
            proc = self.getDeviceName(pid)
            self.add_report(" - with PIC%s (ID 0x%08X, rev. A%01X)" % (proc, pid, rev))
            if proc != board.proc:
                self.add_report("Aborting: program compiled for %s but device has %s" % (board.proc, proc))
                self.closeDevice(handle)
                return
        else:
            logging.info("No, assuming it's a PIC%s" % board.proc)
            self.add_report(" - with supposed PIC%s" % board.proc)

        # find out clock frequencies
        # --------------------------------------------------------------

        logging.info("Getting clock frequencies ...")
        fcpu = self.getDeviceFCPU()
        fpb = self.getDeviceFPB()
        if fcpu and fpb:
            logging.info("Yes, FCPU = %dHz" % fcpu)
            logging.info("Yes, FPB  = %dHz" % fpb)
            self.add_report("   System  clock %.3f MHz" % ( fcpu/1000000.0 ))
            self.add_report("   Periph. clock %.3f MHz" % (  fpb/1000000.0 ))
        else:
            logging.info("No")

        # find out flash memory size
        # --------------------------------------------------------------
        logging.info("Getting flash memory infos ...")
        memstart = self.getDeviceFlashStart()
        memfree  = self.getDeviceFlashFree()
        if memstart and memfree:
            logging.info("Yes, Prog. starts at 0x%08X" % memstart)
            #logging.info("Yes, free  = 0x%08X" % memfree)
            # Convert KSEG1 to KSEG0
            # and Physical to Virtual address
            if memstart >= 0xBD000000:
                memstart = memstart - 0x20000000
            memstart = memstart | 0x80000000
            ivtstart = board.ivtstart
            memend   = memstart + memfree
            #memfree  = memend - memstart

            if board.memstart != memstart:
                logging.info("Conflict : memstart should be 0x%08X not 0x%08X" % (memstart, board.memstart))
                logging.info("This issue has been automatically fixed.")
                logging.info("Please report it at https://github.com/PinguinoIDE/pinguino-ide/issues")
                board.memstart = memstart

            if board.memend != memend:
                logging.info("Conflict : memend should be 0x%08X not 0x%08X" % (memend, board.memend))
                logging.info("This issue has been automatically fixed.")
                logging.info("Please report it at https://github.com/PinguinoIDE/pinguino-ide/issues")
                board.memend = memend

            logging.info("Yes, ivtstart = 0x%08X" % ivtstart)
            logging.info("Yes, memstart = 0x%08X" % memstart)
            logging.info("Yes, memend   = 0x%08X" % memend)
            logging.info("Yes, memfree  = 0x%X bytes free" % memfree)

        else:
            logging.info("No")
            memfree = board.memend - board.memstart

        self.add_report(" - with %d bytes free (%d KB)" % (memfree, memfree/1024))
        self.add_report("   from 0x%08X to 0x%08X" % (board.memstart, board.memend))

        # find out bootloader version
        # --------------------------------------------------------------

        logging.info("Getting bootloader version ...")
        version = self.getVersion()
        if version:
            self.manufacturer = "Pinguino"
            self.add_report(" - with Pinguino USB HID Bootloader v%s" % version)
        else:
            self.manufacturer = "Microchip"
            self.add_report(" - with Microchip USB HID Bootloader")

        # start erasing
        # --------------------------------------------------------------

        self.add_report("Erasing flash memory ...")
        status = self.eraseFlash(handle)
        if status != self.ERR_NONE:
            self.add_report("Aborting: erase error!")
            self.closeDevice(handle)
            return

        # start writing
        # --------------------------------------------------------------

        self.add_report("Uploading user program ...")
        status = self.writeHex(handle, filename, board)
        if status != self.ERR_NONE:
            self.add_report("Aborting: write error code %d" % status)
            self.closeDevice(handle)
            return

        self.add_report("%s successfully uploaded." % os.path.basename(filename))

        # reset and start user's app.
        # --------------------------------------------------------------

        logging.info("Sending Reset command ...")
        self.add_report("Starting user program ...")
        status = self.resetDevice(handle)
        logging.info("Device Reset failed ...")
        if status != self.ERR_NONE:
            self.closeDevice(handle)

        return

# ----------------------------------------------------------------------
