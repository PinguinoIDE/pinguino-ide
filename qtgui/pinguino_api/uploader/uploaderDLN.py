#!/usr/bin/env python
#-*- coding: iso-8859-15 -*-

"""-------------------------------------------------------------------------
	Pinguino Universal Uploader

	Author:			Regis Blanchot <rblanchot@gmail.com> 
	Last release:	2012-01-24
	
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

# This class is based on :
# - Diolan USB bootloader licenced (LGPL) by Diolan <http://www.diolan.com>
# - jallib USB bootloader licenced (BSD) by Albert Faber
# See also PyUSB Doc. http://wiki.erazor-zone.de/wiki:projects:python:pyusb:pydoc
# Device Descriptors : lsusb -v -d 04d8:feaa

import sys
import os
#import usb			# checked in check.py

class uploaderDLN:
	""" upload .hex into pinguino device """

	# Bootloader commands
	# --------------------------------------------------------------------------

	DLN_WRITE_FLASH_CMD				=	0x01
	DLN_ERASE_FLASH_CMD				=	0x02
	DLN_GET_FW_VER_CMD				=	0x03
	DLN_RESET_CMD  					=	0x04
	DLN_READ_DEVID_CMD				=	0x05
	DLN_UNKNOWN_CMD					=	0xFF

	# boot command offset in bytes
	BOOT_CMD						=	0
	BOOT_ECHO						=	1
	BOOT_ADDR_LO					=	2
	BOOT_ADDR_HI					=	3
	BOOT_ADDR_UP					=	4
	BOOT_SIZE						=	5
	BOOT_CODE						=	6
	BOOT_VER_MAJOR					=	2
	BOOT_VER_MINOR					=	3
	BOOT_VER_SUBMINOR				=	4

	DLN_BLOCKSIZE					=	32 #64 - DLN_BOOT_CMD_SIZE

	BOOT_CMD_SIZE					=	64
	BOOT_DATA_SIZE					=	BOOT_CMD_SIZE - BOOT_CODE

	# HID endpoints
	DLN_IN_EP						=	0x81	# endpoint for HID reads
	DLN_OUT_EP						=	0x01	# endpoint for HID writes

	# configuration
	DLN_ACTIVE_CONFIG				=	0x01
	DLN_INTERFACE_ID				=	0x00
	DLN_TIMEOUT						=	500

	MAX_HID_RETRY					=	4

	# Table with supported USB devices (from JAL USB HID Bootloader)
	# device_id:[PIC name, flash size(in bytes), eeprom size (in bytes)] 
	# --------------------------------------------------------------------------

	devices_table = \
		{  
			0x4740: ['18f13k50'	, 0x02000, 0x80 ],
			0x4700: ['18lf13k50', 0x02000, 0x80 ],

			0x4760: ['18f14k50'	, 0x04000, 0xFF ],
			0x4720: ['18f14k50'	, 0x04000, 0xFF ],

			0x2420: ['18f2450'	, 0x04000, 0x00 ],
			0x1260: ['18f2455'	, 0x06000, 0xFF ],
			0x2A60: ['18f2458'	, 0x06000, 0xFF ],

			0x4C00: ['18f24J50'	, 0x04000, 0x00 ],
			0x4CC0: ['18lf24J50', 0x04000, 0x00 ],
			
			0x1240: ['18f2550'	, 0x08000, 0xFF ],
			0x2A40: ['18f2553'	, 0x08000, 0xFF ],

			0x4C20: ['18f25J50'	, 0x08000, 0x00 ],
			0x4CE0: ['18lf25J50', 0x08000, 0x00 ],
			
			0x4C40: ['18f26J50'	, 0x10000, 0x00 ],
			0x4D00: ['18lf26J50', 0x10000, 0x00 ],
			
			0x1200: ['18f4450'	, 0x04000, 0x00 ],  #FIXME: Duplicate key 0x1200
			0x1220: ['18f4455'	, 0x06000, 0x00 ],
			0x2A20: ['18f4458'	, 0x06000, 0xFF ],
			
			0x4C60: ['18f44J50'	, 0x04000, 0x00 ],
			0x4D20: ['18lf44J50', 0x04000, 0x00 ],
			
			0x1200: ['18f4550'	, 0x08000, 0xFF ],  #FIXME: Duplicate key 0x1200
			0x2A00: ['18f4553'	, 0x08000, 0xFF ],
			
			0x4C80: ['18f45J50'	, 0x08000, 0x00 ],
			0x4D40: ['18lf45J50', 0x08000, 0x00 ],
			
			0x4CA0: ['18f46J50'	, 0x10000, 0x00 ],
			0x4D60: ['18f46J50'	, 0x10000, 0x00 ],
			
			0x4100: ['18f65J50'	, 0x08000, 0x00 ],
			0x1560: ['18f66J50'	, 0x10000, 0x00 ],
			0x4160: ['18f66J55'	, 0x18000, 0x00 ],
			0x4180: ['18f67J50'	, 0x20000, 0x00 ],
			0x41A0: ['18f85J50'	, 0x08000, 0x00 ],
			0x41E0: ['18f86J50'	, 0x10000, 0x00 ],
			0x1F40: ['18f86J55'	, 0x18000, 0x00 ],
			0x4220: ['18f87J50'	, 0x20000, 0x00 ]
		}

# ------------------------------------------------------------------------------
	def initDevice(self, device):
# ------------------------------------------------------------------------------
		""" init pinguino device """
		conf = device.configurations[0]
		iface = conf.interfaces[0][0]
		handle = device.open()
		if handle:
			if sys.platform == 'win32':
				handle.setConfiguration(conf)
				handle.claimInterface(iface)
				handle.setAltInterface(iface)
			if sys.platform == 'linux2':
				handle.detachKernelDriver(iface.interfaceNumber)
				handle.setConfiguration(conf)
				handle.claimInterface(iface)
				handle.setAltInterface(iface)
			return handle
		return self.ERR_USB_INIT1
# ------------------------------------------------------------------------------
	def usbWrite(self, usbBuf):  
# ------------------------------------------------------------------------------
		"""
		controlMsg(requestType, request, buffer, value=0, index=0, timeout=100) -> bytesWritten|buffer
		Performs a control request to the default control pipe on a device.
		Arguments:
			requestType: specifies the direction of data flow, the type of request
				and the recipient.
				= Host to Device, Class type, Interface recipient
				= usb.TYPE_CLASS | usb.RECIP_INTERFACE | usb.ENDPOINT_OUT
				= 0x21
			request: specifies the request.
				= SET_REPORT
				= 0x9
			buffer: if the transfer is a write transfer, buffer is a sequence 
				with the transfer data, otherwise, buffer is the number of
				bytes to read.
			value: specific information to pass to the device. (default: 0)
				= 0
			index: specific information to pass to the device. (default: 0)
				= 0
			timeout: operation timeout in miliseconds. (default: 100)
		Returns the number of bytes written.
		"""
		sent_bytes = handle.controlMsg(0x21, 0x09, usbBuf, 0x00, 0x00, self.DLN_TIMEOUT)
		if sent_bytes == len(usbBuf):
			return self.ERR_NONE
		else:		
			return self.ERR_USB_WRITE
# ------------------------------------------------------------------------------
	def transaction(self, handle, usbBuf):
# ------------------------------------------------------------------------------
		"""
		Write a data packet to currently-open USB device 
		Return 64 bytes from the device

		interruptRead(endpoint, size, timeout=100) -> buffer
		Performs a interrupt read request to the endpoint specified.
		Arguments:
			endpoint: endpoint number.
			size: number of bytes to read.
			timeout: operation timeout in miliseconds. (default: 100)
		Returns a tuple with the data read.
		"""
		retry = 0
		while retry < self.MAX_HID_RETRY:
			status = self.usbWrite(handle, usbBuf)
			if status == self.ERR_NONE:
				return handle.interruptRead(1, 64, self.DLN_TIMEOUT)
				#return handle.interruptRead(1, len(usbBuf), self.DLN_TIMEOUT)
			else:
				retry = retry + 1
		return self.ERR_USB_WRITE
# ------------------------------------------------------------------------------
	def reset(self, handle):
# ------------------------------------------------------------------------------
		""" reset device """
		usbBuf = [0] * 64
		# command code
		usbBuf[self.BOOT_CMD] = self.DLN_RESET_CMD
		# write data packet
		return self.usbWrite(handle, usbBuf)
# ------------------------------------------------------------------------------
	def getVersion(self, handle):
# ------------------------------------------------------------------------------
		""" get bootloader version """
		usbBuf = [0] * 64
		# command code
		usbBuf[self.BOOT_CMD] = self.DLN_GET_FW_VER_CMD
		# write data packet and get response
		usbBuf = self.transaction(handle, usbBuf)
		if usbBuf == self.ERR_USB_WRITE:
			return self.ERR_USB_WRITE
		else:		
			# major.minor.subminor
			return	str(usbBuf[self.BOOT_VER_MAJOR]) + "." + \
					str(usbBuf[self.BOOT_VER_MINOR]) + "." + \
					str(usbBuf[self.BOOT_VER_SUBMINOR])
# ------------------------------------------------------------------------------
	def getDeviceID(self, handle):
# ------------------------------------------------------------------------------
		""" read Device ID """
		usbBuf = [0] * 64
		# command code
		usbBuf[self.BOOT_CMD] = self.DLN_READ_DEVID_CMD
		# write data packet and get response
		usbBuf = self.transaction(handle, usbBuf)
		if usbBuf == self.ERR_USB_WRITE:
			return self.ERR_USB_WRITE
		else:		
			dev1 = usbBuf[self.BOOT_CODE + 0]
			dev2 = usbBuf[self.BOOT_CODE + 1]
			device_id = ( int(dev2) << 8 ) + int(dev1)
			deivce_rev = device_id & 0x001F
			# mask revision number
			return device_id  & 0xFFE0
# ------------------------------------------------------------------------------
	def getDeviceFlash(self, device_id, board):
# ------------------------------------------------------------------------------
		for n in self.devices_table:
			if n == device_id:
				return self.devices_table[n][1] - board.memstart			
		return self.ERR_DEVICE_NOT_FOUND
# ------------------------------------------------------------------------------
	def getDeviceName(self, device_id):
# ------------------------------------------------------------------------------
		for n in self.devices_table:
			if n == device_id:
				return self.devices_table[n][0]
		return self.ERR_DEVICE_NOT_FOUND
# ------------------------------------------------------------------------------
	def eraseFlash(self, handle, address, size64):
# ------------------------------------------------------------------------------
		""" erase n * 64-byte blocks of flash memory """
		usbBuf = [0] * 64
		# command code
		usbBuf[self.BOOT_CMD] = self.DLN_ERASE_FLASH_CMD
		# echo is used to link between command and response
		usbBuf[self.BOOT_ECHO] = 123
		# block address (must be divisible by 64)
		usbBuf[self.BOOT_ADDR_LO] = (address      ) & 0xFF
		usbBuf[self.BOOT_ADDR_HI] = (address >> 8 ) & 0xFF
		usbBuf[self.BOOT_ADDR_UP] = (address >> 16) & 0xFF
		# size in 64 byte blocks
		usbBuf[self.BOOT_SIZE] = size64
		# write data packet and get response
		#print usbBuf
		usbBuf = self.transaction(handle, usbBuf)
		#print usbBuf
# ------------------------------------------------------------------------------
	def writeFlash(self, handle, address, block):
# ------------------------------------------------------------------------------
		""" write a block of code """
		usbBuf = [0xFF] * 64
		usbBuf[self.BOOT_CMD] = self.DLN_WRITE_FLASH_CMD 
		# echo is used to link between command and response
		usbBuf[self.BOOT_ECHO] = 123
		# block's address (must be divisible by 2)
		#print hex(address)
		usbBuf[self.BOOT_ADDR_LO] = (address      ) & 0xFF
		usbBuf[self.BOOT_ADDR_HI] = (address >> 8 ) & 0xFF
		usbBuf[self.BOOT_ADDR_UP] = (address >> 16) & 0xFF
		# size must be divisible by 8
		size = len(block)
		if (size % 8 != 0):
			size = (size / 8 + 1) * 8
		usbBuf[self.BOOT_SIZE] = size 
		# add data to the packet
		for i in range(size):
			usbBuf[self.BOOT_CODE + i] = block[i]
		# write data packet on usb device
		#print usbBuf
		usbBuf = self.transaction(handle, usbBuf)
		#print usbBuf
# ------------------------------------------------------------------------------
	def hexWrite(self, handle, filename, board):
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

		data		= []
		old_address = 0
		max_address	= 0
		address_Hi	= 0
		codesize	= 0

		# read hex file
		# ----------------------------------------------------------------------

		fichier = open(filename,'r')
		lines = fichier.readlines()
		fichier.close()

		# 1st pass : calculate checksum and max_address
		# ----------------------------------------------------------------------

		for line in lines:
			byte_count = int(line[1:3], 16)
			address_Lo = int(line[3:7], 16) # lower 16 bits (bits 0-15) of the data address
			record_type= int(line[7:9], 16)
			
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
				address = (address_Hi << 16) + address_Lo

			# code size
			if address >= board.memstart:
				codesize = codesize + byte_count

			# max address
			if (address > old_address) and (address < board.memend):
				max_address = address + byte_count
				old_address = address

		max_address = max_address + 64 - (max_address % 64)
		#print memstart, max_address, memend

		# fill data sequence with 0xFF
		# ----------------------------------------------------------------------

		for i in range(max_address - board.memstart):
			data.append(0xFF)

		# 2nd pass : parse bytes from line into data
		# ----------------------------------------------------------------------

		address_Hi	= 0

		for line in lines:
			byte_count = int(line[1:3], 16)
			address_Lo = int(line[3:7], 16) # four hex digits
			record_type= int(line[7:9], 16)

			# 32-bit address
			address = (address_Hi << 16) + address_Lo
			#print hex(address)

			# data record
			if record_type == self.Data_Record:
				if (address >= board.memstart) and (address < board.memend):
					#print hex(address)
					for i in range(byte_count):
						#print address - memstart + i
						#print int(line[9 + (2 * i) : 11 + (2 * i)], 16)
						data[address - board.memstart + i] = int(line[9 + (2 * i) : 11 + (2 * i)], 16)
					
			# end of file record
			elif record_type == self.End_Of_File_Record:
				break

			# extended linear address record
			elif record_type == self.Extended_Linear_Address_Record:
				address_Hi = int(line[9:13], 16) # upper 16 bits (bits 16-31) of the data address
				#print "address_Hi = " + hex(address_Hi)

			# unsupported record type
			else:
				return self.ERR_HEX_RECORD

		# erase memory from memstart to max_address 
		# ----------------------------------------------------------------------

		size64 = (max_address - board.memstart) / 64
		if size64 > 511:
			return ERR_USB_ERASE
		if size64 < 256:
			self.eraseFlash(handle, board.memstart, size64)
		else:
			# erase flash memory from memstart to memstart + 0x4000
			self.eraseFlash(handle, board.memstart, 255)
			# erase flash memory from memstart + 0x4000 to max_address
			size64 = size64 - 255
			self.eraseFlash(handle, board.memstart + 0x4000, size64)

		# write 32-bit blocks
		# ----------------------------------------------------------------------

		usbBuf = []
		for addr in range(board.memstart, max_address):
			index = addr - board.memstart
			#print hex(addr)
			if addr % self.DLN_BLOCKSIZE == 0:
				if usbBuf != []:
					#print usbBuf
					self.writeFlash(handle, addr - self.DLN_BLOCKSIZE, usbBuf)
				usbBuf = []
			if data[index] != []:
				#print data[addr - memstart]
				usbBuf.append(data[index])
		#print "%d bytes written." % codesize

		return self.ERR_NONE
# ------------------------------------------------------------------------------
	def writeHex(self, output, filename, board):
# ------------------------------------------------------------------------------
		fichier = open(filename, 'r')
		if fichier == "":
			self.add_report(output, "Unable to open %s" % filename)
			return
		fichier.close()

		device = self.getDevice(board)
		if device == self.ERR_DEVICE_NOT_FOUND:
			self.add_report(output, "Pinguino not found")
			self.add_report(output, "Is your device connected and/or in bootloader mode ?")
			return
		else:
			self.add_report(output, "Pinguino found")

		handle = self.initDevice(device)
		if handle == self.ERR_USB_INIT1:
			self.add_report(output, "Upload not possible")
			self.add_report(output, "Try to restart the bootloader mode")
			return

		device_id = self.getDeviceID(handle)
		proc = self.getDeviceName(device_id)
		if proc != board.proc:
			self.add_report(output, "Compiled for %s but device has %s" % (board.proc, proc))
			return
		#memend = self.getDeviceFlash(device_id)
		self.add_report(output, "%s (id=%s)" % (proc, hex(device_id)))
		self.add_report(output, "%d bytes free" % (board.memend - board.memstart))

		#product = handle.getString(device.iProduct, 30)
		#manufacturer = handle.getString(device.iManufacturer, 30)
		self.add_report(output, "Pinguino HID bootloader %s" % self.getVersion(handle))

		if filename == '':
			self.add_report(output, "No program to write")
			self.closeDevice(handle)
			return

		self.add_report(output, "Writing ...")
		status = self.hexWrite(handle, filename, board)
		if status == self.ERR_NONE:
			self.add_report(output, os.path.basename(filename) + " successfully uploaded")
		if status == self.ERR_HEX_RECORD:
			self.add_report(output, "Record error")
			self.closeDevice(handle)
			return
		if status == self.ERR_HEX_CHECKSUM:
			self.add_report(output, "Checksum error")
			self.closeDevice(handle)
			return
		if status == self.ERR_USB_ERASE:
			self.add_report(output, "Erase error")
			self.closeDevice(handle)
			return

		# Disable EIS and Start User App.
		self.add_report(output, "Resetting ...")
		self.reset(handle)
		self.closeDevice(handle)
		return
# ------------------------------------------------------------------------------
