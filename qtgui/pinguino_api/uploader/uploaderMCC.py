#!/usr/bin/env python
#-*- coding: iso-8859-15 -*-

"""-------------------------------------------------------------------------
	Pinguino Uploader for Pinguino 32

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

# Based on UBW32 / Microchip mphidflash software licensed (GNU GPL v3) by Phillip Burgess <pburgess@dslextreme.com>
# UBW32 sources at : http://vak-opensource.googlecode.com/svn/!svn/bc/117/trunk/utilities/ubw32/
# MPHIDFLASH sources at : http://mphidflash.googlecode.com/svn-history/r2/trunk/
# PyUSB Doc : http://wiki.erazor-zone.de/wiki:projects:python:pyusb:pydoc
# Device Descriptors : lsusb -v -d 04d8:003C

import sys
import os
#import usb

# Hex format record types
# --------------------------------------------------------------------------

Data_Record						=	 00
End_Of_File_Record				=	 01
Extended_Segment_Address_Record	=	 02
Start_Segment_Address_Record	=	 03
Extended_Linear_Address_Record	=	 04
Start_Linear_Address_Record		=	 05

# Microchip Hid bootloader commands
# sources : http://mphidflash.googlecode.com/svn-history/r2/trunk/
# --------------------------------------------------------------------------

VENDOR_ID						=	0x04D8
PRODUCT_ID						=	0x003C
INTERFACE_ID					=	0x00
ACTIVE_CONFIG					=	1
TIMEOUT							=	0

# Data block description 
BYTESPERADDRESS					=	1
MAXPACKETSIZE					=	64
BLOCKSIZE						=	56		# MAXPACKETSIZE - Block Command Size

# Command Definitions
UNLOCKCONFIG_CMD				=	0x00	# sub-command for the ERASE_DEVICE_CMD
LOCKCONFIG_CMD					=	0x01	# sub-command for the ERASE_DEVICE_CMD
QUERY_DEVICE_CMD				=	0x02	# what regions can be programmed, and what type of memory is the region
UNLOCK_CONFIG_CMD				=	0x03	# for both locking and unlocking the config bits
ERASE_DEVICE_CMD				=	0x04	# to start an erase operation, firmware controls which pages should be erased
PROGRAM_DEVICE_CMD				=	0x05	# to send a full RequestDataBlockSize to be programmed
PROGRAM_COMPLETE_CMD			=	0x06	# if host send less than a RequestDataBlockSize to be programmed, or if it wished to program whatever was left in the buffer, it uses this command
GET_DATA_CMD					=	0x07	# the host sends this command in order to read out memory from the device. Used during verify (and read/export hex operations)
RESET_DEVICE_CMD				=	0x08	# resets the microcontroller, so it can update the config bits (if they were programmed, and so as to leave the bootloader (and potentially go back into the main application)

# Query Device Response
TypeProgramMemory				=	0x01	# when the host sends a QUERY_DEVICE command, need to respond by populating a list of valid memory regions that exist in the device (and should be programmed)
TypeEEPROM						=	0x02
TypeConfigWords					=	0x03
TypeEndOfTypeList				=	0xFF	# sort of serves as a "null terminator" like number, which denotes the end of the memory region list has been reached.

# hid endpoints
IN_EP							=	0x81	# endpoint for Hid reads
OUT_EP							=	0x01	# endpoint for Hid writes

# configuration
ACTIVE_CONFIG					=	1

# Device family
DEVICE_FAMILY_PIC18				=	0x01
DEVICE_FAMILY_PIC24				=	0x02
DEVICE_FAMILY_PIC32				=	0x03

# Error codes returned by various functions
# --------------------------------------------------------------------------

ERR_NONE						=	0
ERR_CMD_ARG						=	1
ERR_CMD_UNKNOWN					=	2
ERR_DEVICE_NOT_FOUND			=	3
ERR_USB_INIT1					=	4
ERR_USB_INIT2					=	5
ERR_USB_OPEN					=	6
ERR_USB_WRITE					=	7
ERR_USB_READ					=	8
ERR_HEX_OPEN					=	9
ERR_HEX_STAT					=	10
ERR_HEX_MMAP					=	11
ERR_HEX_SYNTAX					=	12
ERR_HEX_CHECKSUM				=	13
ERR_HEX_RECORD					=	14
ERR_VERIFY						=	15
ERR_EOL							=	16
ERR_FILE_NOT_FOUND				=	17

# ------------------------------------------------------------------------------
def getDevice(vendor, product):
	""" get list of USB devices and search for pinguino """
	busses = usb.busses()
	for bus in busses:
		for device in bus.devices:
			if device.idVendor == vendor and device.idProduct == product:
				return device
	return ERR_DEVICE_NOT_FOUND
# ------------------------------------------------------------------------------
def initDevice(device):
	""" init pinguino device """
	handle = device.open()
	if handle:
		try:
			# make sure the hiddev kernel driver is not active
			handle.detachKernelDriver(INTERFACE_ID)
		except usb.USBError:
			pass
		handle.setConfiguration(ACTIVE_CONFIG)
		handle.claimInterface(INTERFACE_ID)
		return handle
	return ERR_USB_INIT1
# ------------------------------------------------------------------------------
def usbWrite(handle, usbBuf):
	"""	Write a MAXPACKETSIZE bytes data packet to currently-open USB device """
	sent_bytes = handle.interruptWrite(OUT_EP, usbBuf, TIMEOUT)
	if sent_bytes == len(usbBuf): 
		return ERR_NONE
	else:
		return ERR_USB_WRITE
# ------------------------------------------------------------------------------
def issueBlock(handle, address, block):
	""" write a block of code """
	length = len(block)
	if length == 0:
		# Short data packets need flushing
		usbBuf = [PROGRAM_COMPLETE_CMD] * MAXPACKETSIZE
		#usbBuf[0] = PROGRAM_COMPLETE_CMD
		return usbWrite(handle, usbBuf)
	# command code
	usbBuf = [PROGRAM_DEVICE_CMD] * MAXPACKETSIZE
	#usbBuf[0] = PROGRAM_DEVICE_CMD 
	# block's address (0x12345678 => "12345678")
	address = "%08X" % (address / BYTESPERADDRESS)
	usbBuf[1] = int(address[6:8], 16)	# 78 = (address      ) & 0xFF
	usbBuf[2] = int(address[4:6], 16)	# 56 = (address >>  8) & 0xFF
	usbBuf[3] = int(address[2:4], 16)	# 34 = (address >> 16) & 0xFF
	usbBuf[4] = int(address[0:2], 16)	# 12 = (address >> 24)
	# data's length
	usbBuf[5] = length
	# add data 'right justified' within packet
	for i in range(length):
		usbBuf[MAXPACKETSIZE - length + i] = block[i]
	# write data packet on usb device
	status = usbWrite(handle, usbBuf)
	if (status == ERR_NONE) and (len < BLOCKSIZE):
		# Short data packets need flushing
		#usbBuf = [PROGRAM_COMPLETE_CMD] * MAXPACKETSIZE
		usbBuf[0] = PROGRAM_COMPLETE_CMD
		status = usbWrite(handle, usbBuf)
	return status
# ------------------------------------------------------------------------------
def hexWrite(handle, filename):
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

	hexBuf		= []
	bufLen		= 0
	addrHi		= 0
	addrSave	= 0
	addr32		= 0
	codesize	= 0

	# load hex file
	# ----------------------------------------------------------------------

	fichier = open(filename, 'r')
	if fichier == "":
		return ERR_FILE_NOT_FOUND
	lines = fichier.readlines()
	fichier.close()

	# read each line in file
	# ----------------------------------------------------------------------

	for line in lines:
		byte_count = int(line[1:3], 16)
		addrLo = int(line[3:7], 16) # lower 16 bits (bits 0-15) of the data address
		record_type= int(line[7:9], 16)

		# checksum calculation
		end = 9 + byte_count * 2 # position of checksum at end of line
		checksum = int(line[end:end+2], 16)
		cs = 0
		i = 1
		while i < end:
			cs = cs + (0x100 - int(line[i:i+2], 16) ) & 0xff # eq. to not(i)
			i = i + 2
		if checksum != cs:
			return ERR_HEX_CHECKSUM

		# data record
		if record_type == Data_Record:
			# count bytes
			codesize += byte_count
			# If new record address is not contiguous with prior record,
			# issue accumulated hex data (if any) and start anew
			if (addrHi + addrLo) != addr32:
				addr32 = addrHi + addrLo
				if bufLen:
					status = issueBlock(handle, addrSave, hexBuf)
					if status != ERR_NONE: return status
					bufLen = 0
					hexBuf = []
				addrSave = addr32
			# Parse bytes from line into hexBuf
			for i in range(byte_count):
				hexBuf.append(int(line[9 + (2 * i) : 11 + (2 * i)], 16))
				bufLen += 1
				# If buffer is full, issue block and start anew
				if bufLen == BLOCKSIZE:
					status = issueBlock(handle, addrSave, hexBuf)
					if status != ERR_NONE: return status
					bufLen = 0
					hexBuf = []
				# Increment address, wraparound as per hexfile spec
				if addr32 == 0xffffffff:
					# Wraparound.  If any hex data, issue and start anew
					if bufLen:
						status = issueBlock(handle, addrSave, hexBuf)
						if status != ERR_NONE: return status
						bufLen = 0
						hexBuf = []
					addr32 = 0
				else:
					addr32 += 1
				# If issueBlock() used, save new address for next block
				if not bufLen:
					addrSave = addr32;

		# end of file record
		elif record_type == End_Of_File_Record:
			break

		# extended linear address record
		elif record_type == Extended_Linear_Address_Record:
			# upper 16 bits (bits 16-31) of the data address
			addrHi = int(line[9:13], 16) << 16
			addr32 = addrHi + addrLo
			# Assume this means a noncontiguous address jump; issue block
			# and start anew.  The prior noncontiguous address code should
			# already have this covered, but in the freak case of an
			# extended address record with no subsequent data, make sure
			# the last of the data is issued.
			if bufLen:
				status = issueBlock(handle, addrSave, hexBuf)
				if status != ERR_NONE: return status
				bufLen = 0;
				hexBuf = []
				addrSave = addr32;

		# unsupported record type
		else:
			return ERR_HEX_RECORD

	# At end of file, issue any residual data (counters reset at top)
	if bufLen:
		status = issueBlock(handle, addrSave, hexBuf)
		if status != ERR_NONE: return status
	# Make sure last data is flushed (issueBlock() does this
	# automatically if less than 56 bytes...but if the last packet
	# is exactly this size, an explicit flush is done here)
	if bufLen == BLOCKSIZE:
		usbBuf = [PROGRAM_COMPLETE_CMD] * MAXPACKETSIZE
		#usbBuf[0] = PROGRAM_COMPLETE_CMD
		return usbWrite(handle, usbBuf)
		
	print "%d bytes written" % codesize

	return ERR_NONE

# ------------------------------------------------------------------------------
# ------------------------------------------------------------------------------
# -----------------------------------MAIN---------------------------------------
# ------------------------------------------------------------------------------
# ------------------------------------------------------------------------------

filename = "examples/1.Basics/Blink/Blink.hex"
fichier = open(filename, 'r')
if fichier == "":
	print "Unable to open " + filename
	sys.exit(0)
fichier.close()

device = getDevice(VENDOR_ID, PRODUCT_ID)
if device == ERR_DEVICE_NOT_FOUND:
	print "Pinguino not found"
	print "Is your device connected and/or in bootloader mode ?"
	sys.exit(0)
else:
	print "Pinguino found"

handle = initDevice(device)
if handle == ERR_USB_INIT1:
	print "Upload not possible"
	print "Try to restart the bootloader mode"
	sys.exit(0)

#print "%s - %s" % (handle.getString(device.iProduct, 30), handle.getString(device.iManufacturer, 30))
print "%s" % handle.getString(device.iProduct, 30)

#print "Querying ..."
usbBuf = [QUERY_DEVICE_CMD] * MAXPACKETSIZE
#usbBuf[0] = QUERY_DEVICE_CMD
status = usbWrite(handle, usbBuf)
if status != ERR_NONE:
	print "Query Error!"
	handle.releaseInterface()
	sys.exit(0)
usbBuf = handle.interruptRead(OUT_EP, MAXPACKETSIZE, TIMEOUT)
if usbBuf[2] == DEVICE_FAMILY_PIC18:
	BYTESPERADDRESS = 1
	print "PIC18 Processor"
elif usbBuf[2] == DEVICE_FAMILY_PIC24:
	BYTESPERADDRESS = 2
	print "PIC24 Processor"
elif usbBuf[2] == DEVICE_FAMILY_PIC32:
	BYTESPERADDRESS = 1
	print "PIC32 Processor"
else:
	BYTESPERADDRESS = 1
	print "Unknown"
j=3
while usbBuf[j] != TypeEndOfTypeList:
	if usbBuf[j] == TypeProgramMemory:
		bf = usbBuf[j + 5] | usbBuf[j + 6] << 8 | usbBuf[j + 7] << 16 | usbBuf[j + 8] << 24
		print "%d bytes free" % bf
	j = j + 9

#print "Erasing ..."
usbBuf = [ERASE_DEVICE_CMD] * MAXPACKETSIZE
#usbBuf[0] = ERASE_DEVICE_CMD
status = usbWrite(handle, usbBuf)
if status != ERR_NONE:
	print "Erase Error!"
	handle.releaseInterface()
	sys.exit(0)

#print "Writing ..."
status = hexWrite(handle, filename)
if status != ERR_NONE:
	print "Write Error!"
	handle.releaseInterface()
	sys.exit(0)

#print "Resetting ..."
usbBuf = [RESET_DEVICE_CMD] * MAXPACKETSIZE
#usbBuf[0] = RESET_DEVICE_CMD
status = usbWrite(handle, usbBuf)
if status != ERR_NONE:
	print "Reset Error!"
	
handle.releaseInterface()
print "Ready"
# ------------------------------------------------------------------------------
