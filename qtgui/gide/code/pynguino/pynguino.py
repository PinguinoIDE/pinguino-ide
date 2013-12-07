#!/usr/bin/env python
#-*- coding: utf-8 -*-

"""-------------------------------------------------------------------------
    pynguino.py

    author:		Yeison Cardona
    contact:		yeison.eng@gmail.com 
    first release:	09/Dec/2012
    last release:	09/Dec/2012

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
-------------------------------------------------------------------------"""

import os, time
from serial import Serial

HIGH = "HIGH"
LOW = "LOW"
INPUT = "INPUT"
OUTPUT = "OUTPUT"

########################################################################
class Pinguino:
    """"""

    pinMode = lambda self, pin, mode : self.Pinguino.write("pinMode(%d,%s)"%(pin, mode))
    digitalWrite = lambda self, pin, write : self.Pinguino.write("digitalWrite(%d,%s)"%(pin, write))
    analogWrite = lambda self, pin, analog : self.Pinguino.write("digitalWrite(%d,%d)"%(pin, analog))
    #delay = lambda self, time_ : self.Pinguino.write("delay(%d)"%(time_))
    delay = lambda self, time_ : time.sleep(time_/1000.0)
    
    delayMicroseconds = lambda self, time_ : self.Pinguino.write("delayMicroseconds(%d)"%(time_))
    toggle = lambda self, pin : self.Pinguino.write("toggle(%d)"%(pin))    
    
    allOutput = lambda self : self.Pinguino.write("allOutput")  
    allInput = lambda self : self.Pinguino.write("allInput")  
    allHigh = lambda self : self.Pinguino.write("allHigh")  
    allLow = lambda self : self.Pinguino.write("allLow")    
    
    #----------------------------------------------------------------------
    def __init__(self):
        if os.name == "posix": self.PORT_NAME = "/dev/ttyACM%d"
        elif os.name == "nt": self.PORT_NAME = "COM%d"
        
    #----------------------------------------------------------------------
    def connect(self, puerto=0):
        try:
            self.Pinguino = Serial(self.PORT_NAME%puerto, timeout=0.1, baudrate=9600)
            return True
        except: return False

    #----------------------------------------------------------------------
    def recursiveConnect(self, maximo=20):
        for i in range(maximo):
            if self.connect(i): return True
        return False

    #----------------------------------------------------------------------
    def digitalRead(self, pin):
        self.Pinguino.write("digitalRead(%d)"%pin)
        try: return int(self.Pinguino.readline())
        except: return self.digitalRead(pin)
    
    #----------------------------------------------------------------------
    def analogRead(self, pin):
        self.Pinguino.write("analogRead(%d)"%pin)
        r = self.Pinguino.readline()
        if r == "":
            r = self.Pinguino.readline()
        return int(r)
        

if __name__=='__main__':
    pass