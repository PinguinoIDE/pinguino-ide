#! /usr/bin/python
# -*- coding: utf-8 -*-

#-------------------------------------------------------------------------------
# File: HelloWorld.py
#
# display string sent by pinguino device though usb bus
# 
# regis blanchot, september 2010
#
# rblanchot@gmail.com
#
#-------------------------------------------------------------------------------

import usb		# requires pyusb available at https://sourceforge.net/projects/pyusb/files/
import sys
import time
import Tkinter	# requires python-tk (apt-get install python-tk)
from Tkinter import *

#-------------------------------------------------------------------------------
# Program's window 
#-------------------------------------------------------------------------------

class Window:

	def __init__(self,root):
		self.root = root 
		self.canvas = Tkinter.Canvas(self.root)
		self.canvas.pack()
		self.canvas.config(background='black')
		self.canvas.config(width=400)
		self.canvas.config(height=200)

#-------------------------------------------------------------------------------
# Pinguino Class by Marin Purgar (marin.purgar@gmail.com)
#-------------------------------------------------------------------------------

class Pinguino():

	VENDOR = 0x04D8
	PRODUCT = 0xFEAA
	CONFIGURATION = 0x01 # if bootloader v4.x
	#CONFIGURATION = 0x03 # if bootloader v2.x
	INTERFACE = 0
   	ENDPOINT_IN = 0x81 # if bootloader v4.x
	#ENDPOINT_IN = 0x82 # if bootloader v2.x
	ENDPOINT_OUT = 0x01

	device = None
	handle = None

	def __init__(self,):
		for bus in usb.busses():
			for dev in bus.devices:
				if dev.idVendor == self.VENDOR and dev.idProduct == self.PRODUCT:
					self.device = dev
		return None

	def open(self):
		if not self.device:
			print >> sys.stderr, "Unable to find device!"
			return None
		try:
			self.handle = self.device.open()
			self.handle.setConfiguration(self.CONFIGURATION)
			self.handle.claimInterface(self.INTERFACE)
		except usb.USBError, err:
			print >> sys.stderr, err
			self.handle = None
		return self.handle

	def close(self):
		try:
			self.handle.releaseInterface()
		except Exception, err:
			print >> sys.stderr, err
		self.handle, self.device = None, None

	def read(self, length, timeout = 0):
		return self.handle.bulkRead(self.ENDPOINT_IN, length, timeout)

	def write(self, buffer, timeout = 0):
		return self.handle.bulkWrite(self.ENDPOINT_OUT, buffer, timeout)

#-------------------------------------------------------------------------------
# update
#-------------------------------------------------------------------------------

def update():
	INTERVAL = 100
	deg = unichr(176).encode("utf-8")
	myString = ''

	# get data from Pinguino board
	try:
		for i in pinguino.read(15, INTERVAL):
			myString += chr(i)
	except usb.USBError as err:
		pass

	if len(myString) > 0:
		#print 't =',t	# debug
		HelloWorld.canvas.create_text(200, 150, text=myString, font=("Helvetica", "18", "bold"), fill='red')
	else:
		HelloWorld.canvas.create_text(200, 150, text="Nothing received", font=("Helvetica", "12"), fill='red')
        
	# recall every 500ms
	HelloWorld.canvas.after(INTERVAL, update)

#-------------------------------------------------------------------------------
# main
#-------------------------------------------------------------------------------

if __name__ == "__main__":
	root = Tkinter.Tk()
	root.title('HelloWorld')
	HelloWorld = Window(root)
	pinguino = Pinguino()
	if pinguino.open() == None:
		print >> sys.stderr, "Unable to open Pinguino device!"
		sys.exit()
	update()
	root.mainloop()
	pinguino.close()

