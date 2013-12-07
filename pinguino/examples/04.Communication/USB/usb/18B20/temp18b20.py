#! /usr/bin/python
# -*- coding: utf-8 -*-

#-------------------------------------------------------------------------------
# File: temp18b20.py
#
# display time and temperature
# temperature is sent by pinguino device though usb bus
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

curtime = ''
curtemp = ''
timeline = ''
templine = ''

#-------------------------------------------------------------------------------
# Program window 
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
	CONFIGURATION = 0x01 # on Pinguino 26J50
	#CONFIGURATION = 0x03 # on Pinguino 2550 ?
	INTERFACE = 0
	#ENDPOINT_IN = 0x82 #2550
   	ENDPOINT_IN = 0x81 #26j50
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
	global curtime
	global curtemp
	global timeline
	global templine
	deg = unichr(176).encode("utf-8")
	t = ''

	# get temperature
	try:
		t = pinguino.read(4, INTERVAL)
	except usb.USBError as err:
		pass
	if len(t) > 0:
		#print 't =',t	# debug
		if t[0] == 1:
			sign = '-'
		else:
			sign = ''
		newtemp = '%s%d%s%d%s%s' % (sign,t[1], '.', t[2]*256+t[3], deg, 'C')
		if newtemp != curtemp:
			curtemp = newtemp
			if templine:
				Temp18b20.canvas.delete(templine)			
			templine = Temp18b20.canvas.create_text(200, 150, text=curtemp, font=("Helvetica", "48", "bold"), fill='red')

	# get time
	newtime = time.strftime('%H:%M:%S')
	if newtime != curtime:
		curtime = newtime
		if timeline:
			Temp18b20.canvas.delete(timeline)			
		timeline = Temp18b20.canvas.create_text(200, 050, text=curtime, font=("Helvetica", "24", "bold"), fill='red')

	# recall every 500ms
	Temp18b20.canvas.after(INTERVAL, update)

#-------------------------------------------------------------------------------
# main
#-------------------------------------------------------------------------------

if __name__ == "__main__":
	root = Tkinter.Tk()
	root.title('Temp18B20')
	Temp18b20 = Window(root)
	pinguino = Pinguino()
	if pinguino.open() == None:
		print >> sys.stderr, "Unable to open Pinguino device!"
		sys.exit()
	update()
	root.mainloop()
	pinguino.close()

