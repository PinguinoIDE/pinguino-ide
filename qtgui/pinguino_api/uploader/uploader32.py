#!/usr/bin/env python
#-*- coding: iso-8859-15 -*-

"""-------------------------------------------------------------------------
    Pinguino Uploader for Pinguino 32

    (c) 2011-2014 Regis Blanchot <rblanchot@gmail.com>

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
#import usb

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
    2    unsigned char Command;
    56    unsigned char PacketDataFieldSize;
    3    unsigned char DeviceFamily;
    1    unsigned char Type1;
    0 48 0 189    unsigned long Address1; BD003000
    0 208 1 96   unsigned long Length1;   6001D000
    255    unsigned char Type2;
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
    BOOT_LEN1                       =    8      # long = 4 bytes
    BOOT_TYPE2                      =    12     # char = 1 byte
    BOOT_ADDR2                      =    13     # long = 4 bytes
    BOOT_LEN2                       =    17     # long = 4 bytes
    BOOT_TYPE3                      =    21     # char = 1 byte
    BOOT_ADDR3                      =    22     # long = 4 bytes
    BOOT_LEN3                       =    26     # long = 4 bytes

    BOOT_TYPE_LEN                   =    9      # char + long + long = 9 bytes

    BOOT_DEVID1                     =    6
    BOOT_DEVID2                     =    7
    BOOT_DEVID3                     =    8
    BOOT_DEVID4                     =    9

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

    BOOT_ADDR                       =    1
    BOOT_ADDR_LSBLO                 =    1
    BOOT_ADDR_LSBHI                 =    2
    BOOT_ADDR_MSBLO                 =    3
    BOOT_ADDR_MSBHI                 =    4
    BOOT_CMD_SIZE                   =    5
    BOOT_CMD_PAD                    =    6
    BOOT_CMD_DATA                   =    7
    

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

    # configuration
    ACTIVE_CONFIG                   =    1

    # Device family
    DEVICE_FAMILY_PIC18             =    0x01
    DEVICE_FAMILY_PIC24             =    0x02
    DEVICE_FAMILY_PIC32             =    0x03

    # Configuration
    # ----------------------------------------------------------------------

    INTERFACE_ID                    =    0x00
    ACTIVE_CONFIG                   =    1
    TIMEOUT                         =    1000

    # Table with supported USB devices
    # device_id:[PIC name, flash size(in bytes), eeprom size (in bytes)] 
    #-----------------------------------------------------------------------

    devices_table = \
    {  
        0x04A00053: ['32MX220F032B' , 0x9D008000, 0x9D003180 ], #32K
        0x04A04053: ['32MX220F032D' , 0x9D008000, 0x9D003000 ], #32K
        0x04D00053: ['32MX250F128B' , 0x9D020000, 0x9D003180 ], #128K
        0x66000053: ['32MX270F256B' , 0x9D040000, 0x9D003180 ]  #256K
    }

# ----------------------------------------------------------------------
    def initDevice(self):
# ----------------------------------------------------------------------
        """ init pinguino device """
        handle = self.device.open()
        if handle:
            try:
                handle.detachKernelDriver(self.INTERFACE_ID)
            
            except:
                #usb.USBError as msg:
                #self.add_report(msg.message)
                pass

            handle.setConfiguration(self.ACTIVE_CONFIG)
            handle.claimInterface(self.INTERFACE_ID)

            return handle
        return self.ERR_USB_INIT1

# ----------------------------------------------------------------------
    def resetDevice(self):
# ----------------------------------------------------------------------
        """ reset device """
        usbBuf = [self.RESET_DEVICE_CMD] * self.MAXPACKETSIZE
        #usbBuf[self.BOOT_CMD] = self.RESET_DEVICE_CMD
        return self.sendCMD(usbBuf)

# ----------------------------------------------------------------------
    def getDeviceFamily(self):
# ----------------------------------------------------------------------
        """ get device info """
        usbBuf = [self.QUERY_DEVICE_CMD] * self.MAXPACKETSIZE
        #usbBuf[self.BOOT_CMD] = self.QUERY_DEVICE_CMD
        usbBuf = self.getResponse(usbBuf)
        if usbBuf == self.ERR_USB_WRITE:
            return self.ERR_USB_WRITE
        return usbBuf[self.BOOT_DEVICE_FAMILY]

# ----------------------------------------------------------------------
    def getDeviceID(self):
# ----------------------------------------------------------------------
        """ get 4-byte device ID from location 0xBF80F220 / 0x9F80F220 """

        usbBuf = self.readFlash(0xBF80F220, 4)

        if usbBuf == self.ERR_USB_WRITE:
            return self.ERR_USB_WRITE, self.ERR_USB_WRITE

        devid = (usbBuf[60]      ) | \
                (usbBuf[61] <<  8) | \
                (usbBuf[62] << 16) | \
                (usbBuf[63] << 24)

        # mask device id to get revision number
        device_rev = ( ( devid >> 24 ) & 0xF0 ) >> 4
        # mask revision number to get device id
        device_id  = devid & 0x0FFFFFFF

        return device_id, device_rev

# ----------------------------------------------------------------------
    def getDeviceFlash(self):
# ----------------------------------------------------------------------
        """ get size of program memory area """
        usbBuf = [self.QUERY_DEVICE_CMD] * self.MAXPACKETSIZE
        #usbBuf[self.BOOT_CMD] = self.QUERY_DEVICE_CMD
        usbBuf = self.getResponse(usbBuf)
        
        if usbBuf == self.ERR_USB_WRITE:
            return self.ERR_USB_WRITE, self.ERR_USB_WRITE
            #print "Error: no response from the bootloader"

        """
        bf=0
        j=3
        while usbBuf[j] != self.TypeEndOfTypeList:
            if usbBuf[j] == self.TypeProgramMemory:
                bf = (usbBuf[j + 5]      ) | \
                     (usbBuf[j + 6] <<  8) | \
                     (usbBuf[j + 7] << 16) | \
                     (usbBuf[j + 8] << 24)
            j = j + 9
        """

        # user program address
        ps = (usbBuf[self.BOOT_ADDR1 + 0]      ) | \
             (usbBuf[self.BOOT_ADDR1 + 1] <<  8) | \
             (usbBuf[self.BOOT_ADDR1 + 2] << 16) | \
             (usbBuf[self.BOOT_ADDR1 + 3] << 24)

        # free memory
        bf = (usbBuf[self.BOOT_LEN1 + 0]      ) | \
             (usbBuf[self.BOOT_LEN1 + 1] <<  8) | \
             (usbBuf[self.BOOT_LEN1 + 2] << 16)
         
        #      | \
        #     (usbBuf[BOOT_LEN1 + 3] << 24)

        return ps, bf
        
# ----------------------------------------------------------------------
    def getDeviceName(self, device_id):
# ----------------------------------------------------------------------
        for n in self.devices_table:
            if n == device_id:
                return self.devices_table[n][0]
        return self.ERR_DEVICE_NOT_FOUND

# ----------------------------------------------------------------------
    def sendCMD(self, usbBuf):
# ----------------------------------------------------------------------
        """ Send a command to the bootloader """
        sent_bytes = self.handle.interruptWrite(self.OUT_EP, usbBuf, self.TIMEOUT)
        if sent_bytes == len(usbBuf):
            return self.ERR_NONE
        else:
            return self.ERR_USB_WRITE

# ----------------------------------------------------------------------
    def getResponse(self, usbBuf):
# ----------------------------------------------------------------------
        """ Send a command and get a response from the bootloader """
        sent_bytes = self.handle.interruptWrite(self.OUT_EP, usbBuf, self.TIMEOUT)
        if sent_bytes == len(usbBuf):
            try:
                usbBuf = self.handle.interruptRead(self.IN_EP, self.MAXPACKETSIZE, self.TIMEOUT)
            except:
                return self.ERR_USB_WRITE
            return usbBuf
        return self.ERR_USB_WRITE

# ----------------------------------------------------------------------
    def eraseFlash(self):
# ----------------------------------------------------------------------
        """ erase the whole flash memory """
        usbBuf = [self.ERASE_DEVICE_CMD] * self.MAXPACKETSIZE
        #usbBuf[self.BOOT_CMD] = self.ERASE_DEVICE_CMD
        return self.sendCMD(usbBuf)

# ----------------------------------------------------------------------
    def writeFlash(self, address, block):
# ----------------------------------------------------------------------
        """ write a block of code """
        length = len(block)
        if length == 0:
            # Short data packets need flushing
            usbBuf = [self.PROGRAM_COMPLETE_CMD] * self.MAXPACKETSIZE
            #usbBuf[self.BOOT_CMD] = self.PROGRAM_COMPLETE_CMD
            return self.sendCMD(usbBuf)
        # command code
        usbBuf = [self.PROGRAM_DEVICE_CMD] * self.MAXPACKETSIZE
        #usbBuf[self.BOOT_CMD] = self.PROGRAM_DEVICE_CMD
        # block's address (0x12345678 => "12345678")
        address = "%08X" % (address / self.BYTESPERADDRESS)
        usbBuf[self.BOOT_ADDR + 0] = int(address[6:8], 16)    # 78 = (address      ) & 0xFF
        usbBuf[self.BOOT_ADDR + 1] = int(address[4:6], 16)    # 56 = (address >>  8) & 0xFF
        usbBuf[self.BOOT_ADDR + 2] = int(address[2:4], 16)    # 34 = (address >> 16) & 0xFF
        usbBuf[self.BOOT_ADDR + 3] = int(address[0:2], 16)    # 12 = (address >> 24)
        # data's length
        usbBuf[self.BOOT_CMD_SIZE] = length
        # add data 'right justified' within packet
        for i in range(length):
            usbBuf[self.MAXPACKETSIZE - length + i] = block[i]
        # write data packet on usb device
        status = self.sendCMD(usbBuf)
        if (status == self.ERR_NONE) and (length < self.DATABLOCKSIZE):
            # Short data packets need flushing
            #usbBuf = [self.PROGRAM_COMPLETE_CMD] * self.MAXPACKETSIZE
            usbBuf[self.BOOT_CMD] = self.PROGRAM_COMPLETE_CMD
            status = self.sendCMD(usbBuf)
        return status

# ----------------------------------------------------------------------
    def readFlash(self, address, length):
# ----------------------------------------------------------------------
        """ read a block of flash """
        #usbBuf = [self.BOOT_CMD] * self.MAXPACKETSIZE
        # command code
        usbBuf = [self.GET_DATA_CMD] * self.MAXPACKETSIZE
        #usbBuf[self.BOOT_CMD] = self.GET_DATA_CMD
        # address
        usbBuf[self.BOOT_ADDR + 0] = (address      ) & 0xFF
        usbBuf[self.BOOT_ADDR + 1] = (address >> 8 ) & 0xFF
        usbBuf[self.BOOT_ADDR + 2] = (address >> 16) & 0xFF
        usbBuf[self.BOOT_ADDR + 3] = (address >> 24) & 0xFF
        # size of block to read
        usbBuf[self.BOOT_CMD_SIZE] = length
        # send request to the bootloader
        return self.getResponse(usbBuf)

# ----------------------------------------------------------------------
    def hexWrite(self, filename, board):
# ----------------------------------------------------------------------
        """ Parse the Hex File Format and send data to usb device """

        """
        [0]        Start code, one character, an ASCII colon ':'.
        [1:3]    Byte count, two hex digits, a number of bytes (hex digit pairs) in the data field. 16 (0x10) or 32 (0x20) bytes of data are the usual compromise values between line length and address overhead.
        [3:7]    Address, four hex digits, a 16-bit address of the beginning of the memory position for the data. Limited to 64 kilobytes, the limit is worked around by specifying higher bits via additional record types. This address is big endian.
        [7:9]    Record type, two hex digits, 00 to 05, defining the type of the data field.
        [9:*]    Data, a sequence of n bytes of the data themselves, represented by 2n hex digits.
        [*:*]    Checksum, two hex digits - the least significant byte of the two's complement of the sum of the values of all fields except fields 1 and 6 (Start code ":" byte and two hex digits of the Checksum). It is calculated by adding together the hex-encoded bytes (hex digit pairs), then leaving only the least significant byte of the result, and making a 2's complement (either by subtracting the byte from 0x100, or inverting it by XOR-ing with 0xFF and adding 0x01). If you are not working with 8-bit variables, you must suppress the overflow by AND-ing the result with 0xFF. The overflow may occur since both 0x100-0 and (0x00 XOR 0xFF)+1 equal 0x100. If the checksum is correctly calculated, adding all the bytes (the Byte count, both bytes in Address, the Record type, each Data byte and the Checksum) together will always result in a value wherein the least significant byte is zero (0x00).
                For example, on :0300300002337A1E
                03 + 00 + 30 + 00 + 02 + 33 + 7A = E2, 2's complement is 1E
        """

        data        = []
        old_address = 0 # or self.board.memstart ?
        max_address = 0
        address_Hi  = 0
        codesize    = 0

        bufLen      = 0
        addrSave    = 0

        # image of the whole PIC memory (above memstart)
        # --------------------------------------------------------------

        #self.add_report("board.memstart = %d" % board.memstart)
        #self.add_report("board.memend = %d" % board.memend)
        for i in range(board.memend - board.memstart):
            data.append(0xFF)

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
            elif record_type == self.Data_Record:

                address = address_Hi + address_Lo
                #self.add_report("%d to be written" % address)

                # max address
                if (address > old_address) and (address < board.memend):
                    max_address = address + byte_count
                    old_address = address
                    #self.add_report("max_address = %d" % max_address)

                # code size
                if (address >= board.memstart) and (address < board.memend):
                    codesize = codesize + byte_count
                    #self.add_report("codesize = %d" % codesize)

                # data append
                for i in range(byte_count):
                    #Caution : addresses are not always contiguous
                    #data.append(int(line[9 + (2 * i) : 11 + (2 * i)], 16))
                    data[address - board.memstart + i] = int(line[9 + (2 * i) : 11 + (2 * i)], 16)

            # end of file record
            # ----------------------------------------------------------
            elif record_type == self.End_Of_File_Record:

                break

            # unsupported record type
            # ----------------------------------------------------------
            else:

                return self.ERR_HEX_RECORD

        # max_address must be divisible by self.DATABLOCKSIZE
        # --------------------------------------------------------------

        #max_address = max_address + self.MAXPACKETSIZE - (max_address % self.MAXPACKETSIZE)
        max_address = max_address + 64 - (max_address % 64)
        #self.add_report("max_address = 0x%X" % max_address)

        # write blocks of DATABLOCKSIZE bytes
        # --------------------------------------------------------------

        for addr in range(board.memstart, max_address, self.DATABLOCKSIZE):
            index = addr - board.memstart
            #self.add_report("index = %d" % index)
            #self.add_report("data = %s" % data[index:index+self.DATABLOCKSIZE])
            self.writeFlash(addr, data[index:index+self.DATABLOCKSIZE])

        usbBuf = [self.PROGRAM_COMPLETE_CMD] * self.MAXPACKETSIZE
        #usbBuf[BOOT_CMD] = self.PROGRAM_COMPLETE_CMD
        #status = self.sendCMD(usbBuf)

        self.add_report("%d bytes written" % codesize)

        #return status
        return self.ERR_NONE

# ----------------------------------------------------------------------
    def writeHex(self):
# ----------------------------------------------------------------------

        # check file to upload
        # --------------------------------------------------------------

        if self.filename == '':
            self.add_report("No program to write")
            self.closeDevice()
            return

        hexfile = open(self.filename, 'r')
        if hexfile == "":
            self.add_report("Unable to open %s" % self.filename)
            return
        hexfile.close()

        # search for a Pinguino board
        # --------------------------------------------------------------

        self.device = self.getDevice()

        if self.device == self.ERR_DEVICE_NOT_FOUND:
            self.add_report("Pinguino not found")
            self.add_report("If your device is connected switch to bootloader mode.")
            return

        self.add_report("Pinguino found ...")

        #import sys
        #reload(sys)
        #sys.stdout.write("*** BREAKPOINT ***\r\n")

        self.handle = self.initDevice()

        if self.handle == self.ERR_USB_INIT1:
            self.add_report("Upload not possible")
            self.add_report("Try to restart the bootloader mode")
            return

        elif self.handle == None:
            return

        #self.add_report("%s - %s" % (handle.getString(device.iProduct, 30), handle.getString(device.iManufacturer, 30))
        #self.add_report("%s" % handle.getString(device.iProduct, 30)

        # find out processor
        # --------------------------------------------------------------

        if self.getDeviceFamily() != self.DEVICE_FAMILY_PIC32:
            self.add_report("Error: not a PIC32 family device")
            self.closeDevice()
            return

        device_id, device_rev = self.getDeviceID()
        if device_id == self.ERR_USB_WRITE or device_rev == self.ERR_USB_WRITE:
            self.add_report("Error: impossible to read the flash memory")
            self.closeDevice()
            return

        proc = self.getDeviceName(device_id)
        self.add_report(" - with PIC%s (id=0x%08X, rev.%01X)" % (proc, device_id, device_rev))

        """
        if proc != self.board.proc:
            self.add_report("Error: Program compiled for %s but device has %s" % (self.board.proc, proc))
            self.closeDevice()
            return
        """

        # find out flash memory size
        # --------------------------------------------------------------

        memstart, memfree = self.getDeviceFlash()
        memend   = memstart + memfree
        memstart = memstart + 0x80000000
        memend   = memend   + 0x80000000
        self.add_report(" - with %d bytes free (%d KB)" % (memfree, memfree/1024))
        self.add_report(" - from 0x%08X to 0x%08X" % (memstart, memend))

        # start erasing
        # --------------------------------------------------------------

        #self.add_report("Erasing ..."
        status = self.eraseFlash()
        if status != self.ERR_NONE:
            self.add_report("Erase Error!")
            self.closeDevice()
            return

        # start writing
        # --------------------------------------------------------------

        self.add_report("Uploading user program ...")
        status = self.hexWrite(self.filename, self.board)
        if status != self.ERR_NONE:
            self.add_report("Write Error!")
            self.closeDevice()
            return

        self.add_report("%s successfully uploaded" % os.path.basename(self.filename))

        # reset and start start user's app.
        # --------------------------------------------------------------

        #self.add_report("Resetting ...")
        self.add_report("Starting user program ...")
        status = self.resetDevice()
        if status != self.ERR_NONE:
            self.closeDevice()
            return
# ----------------------------------------------------------------------
