#!/usr/bin/env python
#-*- coding: utf-8 -*-

from . import constructor as Block
from .constructor import Kit

Kit = Kit()

########################################################################
class Functions(object):
    
    #----------------------------------------------------------------------
    def repeat(self):
        pm = Block.Nested("Functions")
        pm.addSyntax("//")
        pm.addLabel("Repeat")
        pm.addSyntax("\n")
        pm.addSyntax("for(__i__=0;__i__<")
        pm.addSpace()
        pm.addSyntax(";__i__=__i__+1)")
        pm.fillWith(["spin-int"])
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def button_pressed(self):
        """"""
        pm = Block.Nested("Functions")
        pm.addSyntax("//")
        pm.addLabel("Button Pressed")
        pm.addSyntax("\n")
        pm.addSyntax("__j__ = ")
        pm.addSpace()
        pm.addSyntax(";\n")
        pm.addSyntax("if (digitalRead(__j__))\n")
        pm.addSyntax("{\n")
        pm.addSyntax("    while (digitalRead(__j__)){}")
        pm.fillWith(["spin-int"])
        pm.addContextHelp("Normally open")
        return pm.getBlock()
        
    
    #----------------------------------------------------------------------
    def button_released(self):
        """"""
        pm = Block.Nested("Functions")
        pm.addSyntax("//")
        pm.addLabel("Button Released")
        pm.addSyntax("\n")
        pm.addSyntax("__j__ = ")
        pm.addSpace()
        pm.addSyntax(";\n")
        pm.addSyntax("if (!digitalRead(__j__))\n")
        pm.addSyntax("{\n")
        pm.addSyntax("    while (!digitalRead(__j__)){}")
        pm.fillWith(["spin-int"])
        pm.addContextHelp("Normally closed")
        return pm.getBlock()