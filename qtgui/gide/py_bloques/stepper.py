#! /usr/bin/python2
#-*- coding: utf-8 -*-

from . import constructor as Block
from .constructor import Kit

Kit = Kit()

########################################################################
class Stepper(object):

    Stepper_setSpeed = lambda n:Kit.addFunct("Stepper", "Stepper.setSpeed", 1, fill=["spin-int"], str_help="in revolutions per minute")
    Stepper_setMicrostep = lambda n:Kit.addFunct("Stepper", "Stepper.setMicrostep", 1, fill=["spin-int"])
    Stepper_step = lambda n:Kit.addFunct("Stepper", "Stepper.step", 1, fill=["spin-int"])


    #----------------------------------------------------------------------
    def Stepper_init(self):

        pm = Block.Linear("Stepper")
        pm.addLabel("Stepper.init")
        pm.addSyntax("(")
        pm.addArg("STEPS")
        pm.addSyntax(",")
        pm.addDecorator(" ")
        pm.addDecorator("PINS=")
        for i in range(3):
            pm.addSpace()
            pm.addSyntax(",")
        pm.addSpace()
        pm.addSyntax(")")
        return pm.getBlock()
