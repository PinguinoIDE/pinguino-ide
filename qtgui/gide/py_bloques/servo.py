#!/usr/bin/env python
#-*- coding: utf-8 -*-

from . import constructor as Block
from .constructor import Kit

Kit = Kit()

########################################################################
class Servo(object):
    
    Servo_attach = lambda n:Kit.addFunct("Servo", "servo.attach", 1, fill=["spin-int"])
    Servo_detach = lambda n:Kit.addFunct("Servo", "servo.detach", 1, fill=["spin-int"])
    
    #----------------------------------------------------------------------
    def Servo_write(self):
        pm = Block.Linear("Servo")
        pm.addLabel("servo.write")
        pm.addSyntax("(")
        pm.addArg("SERVO")
        pm.addSyntax(",")
        pm.addDecorator(" ")
        pm.addArg("DEGREES")
        pm.addSyntax(")")
        pm.fillWith(["spin-int", "spin-int"])
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def Servo_read(self):
        pm = Block.Output("Servo")
        pm.addLabel("servo.read")
        pm.addSyntax("(")
        pm.addSpace()
        pm.addSyntax(")")
        pm.fillWith(["spin-int"])
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def Servo_setMaximumPulse(self):
        pm = Block.Linear("Servo")
        pm.addLabel("servo.setMaximumPulse")
        pm.addSyntax("(")
        pm.addArg("SERVO")
        pm.addSyntax(",")
        pm.addDecorator(" ")
        pm.addArg("MICROSEC")
        pm.addSyntax(")")
        pm.fillWith(["spin-int", "spin-int"])
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def Servo_setMinimumPulse(self):
        pm = Block.Linear("Servo")
        pm.addLabel("servo.setMinimumPulse")
        pm.addSyntax("(")
        pm.addArg("SERVO")
        pm.addSyntax(",")
        pm.addDecorator(" ")
        pm.addArg("MICROSEC")
        pm.addSyntax(")")
        pm.fillWith(["spin-int", "spin-int"])
        return pm.getBlock()
    