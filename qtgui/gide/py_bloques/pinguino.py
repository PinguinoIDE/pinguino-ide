#!/usr/bin/env python
#-*- coding: utf-8 -*-

from . import constructor as Block
from .constructor import Kit

Kit = Kit()

########################################################################
class Pinguino(object):
    
    pinMode = lambda n:Kit.addFunct("Pinguino", "pinMode", 2, fill=["spin-int", "choice-io"])
    delay = lambda n:Kit.addFunct("Pinguino", "delay", 1, fill=["spin-int"])
    delayMicroseconds = lambda n:Kit.addFunct("Pinguino", "delayMicroseconds", 1, fill=["spin-int"])
    toggle = lambda n:Kit.addFunct("Pinguino", "toggle", 1, fill=["spin-int"])
    
    digitalWrite = lambda n:Kit.addFunct("Pinguino", "digitalWrite", 2, fill=["spin-int", "choice-hl"])
    analogWrite = lambda n:Kit.addFunct("Pinguino", "analogWrite", 2, fill=["spin-int", "slider"])
    digitalRead = lambda n:Kit.addFunct("Pinguino", "digitalRead", 1, fill=["spin-int"])
    analogRead = lambda n:Kit.addFunct("Pinguino", "analogRead", 1, fill=["spin-int"])
    
    digitalRead_o = lambda n:Kit.addFunct("Pinguino", "digitalRead", 1, t="output", fill=["spin-int"])
    analogRead_o = lambda n:Kit.addFunct("Pinguino", "analogRead", 1, t="output", fill=["spin-int"])
    
    digitalRead_b = lambda n:Kit.addFunct("Pinguino", "digitalRead", 1, t="output-bool", fill=["spin-int"])
    analogRead_b = lambda n:Kit.addFunct("Pinguino", "analogRead", 1, t="output-bool", fill=["spin-int"])
    
    #----------------------------------------------------------------------
    def setup(self):
        st = Block.Function("Pinguino")
        st.addLabel("setup")
        st.addSyntax("()")
        return st.getBlock()

    #----------------------------------------------------------------------
    def loop(self):
        st = Block.Function("Pinguino")
        st.addLabel("loop")
        st.addSyntax("()")
        return st.getBlock()        

    #----------------------------------------------------------------------
    def define(self):
        st = Block.Linear("Pinguino")
        st.addLabel("#define")
        st.addSyntax(" ")
        st.addSpace()
        st.addSyntax(" ")
        st.addSpace()
        st.addSyntax(" //")
        st.fillWith(["edit-value", "edit-value"])
        return st.getBlock()

    #----------------------------------------------------------------------
    def global_vars(self):
        st = Block.Function("Pinguino")
        st.addLabel("global")
        st.addSyntax(" variables")
        return st.getBlock()
    

    #----------------------------------------------------------------------
    def types(self):
        st = Block.Output("Output")
        st.addSpinChoice("int", ["char*", "char", "int", "float", "u8", "u16", "u32"])
        st.addSyntax(" ")
        st.addEdit("45")
        return st.getBlock()
    
