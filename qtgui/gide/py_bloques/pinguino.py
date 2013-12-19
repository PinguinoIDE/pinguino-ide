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

    CDC_read = lambda n:Kit.addFunct("Communication", "CDC.read", 1, t="output", fill=[])
    CDC_write = lambda n:Kit.addFunct("Communication", "CDC.write", 1, t="linear", fill=["edit-c"])
    CDC_getstring = lambda n:Kit.addFunct("Communication", "CDC.getString", 0, t="output")
    CDC_getkey = lambda n:Kit.addFunct("Communication", "CDC.getKey", 0, t="output")
    CDC_print = lambda n:Kit.addFunct("Communication", "CDC.print", 1, t="linear", fill=["edit"])
    CDC_println = lambda n:Kit.addFunct("Communication", "CDC.println", 1, t="linear", fill=["edit"])

    Serial_available = lambda n:Kit.addFunct("Communication", "Serial.available", 0, t="output-bool", fill=[])
    Serial_begin = lambda n:Kit.addFunct("Communication", "Serial.begin", 1, t="linear", fill=["spin-int"])
    Serial_getKey = lambda n:Kit.addFunct("Communication", "Serial.getKey", 0, t="output", fill=[])
    Serial_getString = lambda n:Kit.addFunct("Communication", "Serial.getString", 0, t="output", fill=[])
    Serial_print = lambda n:Kit.addFunct("Communication", "Serial.print", 1, t="linear", fill=["edit"])
    Serial_println = lambda n:Kit.addFunct("Communication", "Serial.println", 1, t="linear", fill=["edit"])
    Serial_read = lambda n:Kit.addFunct("Communication", "Serial.read", 0, t="output", fill=[])
    Serial_write = lambda n:Kit.addFunct("Communication", "Serial.write", 1, t="linear", fill=["edit-c"])
    
    
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
        st.addSyntax("vars")
        return st.getBlock()
    

    #----------------------------------------------------------------------
    def types(self):
        st = Block.Output("Output")
        st.addSpinChoice("int", ["char*", "char", "int", "float", "u8", "u16", "u32"])
        st.addSyntax(" ")
        st.addEdit("45")
        return st.getBlock()
    
    #----------------------------------------------------------------------
    def CDC_printf(self):
        st = Block.Linear("Communication")
        st.addLabel("CDC.printf")
        st.addSyntax("(")
        st.addSpace()
        st.addLabel(",")
        st.addSpace()
        st.addSyntax(")")
        st.fillWith(["edit", "more_args"])
        return st.getBlock()
        


    ##Communication ands Editable

    #----------------------------------------------------------------------
    def hack_linear(self):
        st = Block.Linear("Editable")
        st.addEdit("edit...", None, (255, 255, 255))
        st.addContextHelp("Any Communicationand here.")
        return st.getBlock()
    
    #----------------------------------------------------------------------
    def hack_function(self):
        st = Block.Function("Editable")
        st.addEdit("edit...", None, (255, 255, 255))
        st.addContextHelp("Any Communicationand here.")
        return st.getBlock()
    
    #----------------------------------------------------------------------
    def hack_nested(self):
        st = Block.Nested("Editable")
        st.addEdit("edit...", None, (255, 255, 255))
        st.addContextHelp("Any Communicationand here.")
        return st.getBlock()
    
    #----------------------------------------------------------------------
    def hack_nestednext(self):
        st = Block.NestedNext("Editable")
        st.addEdit("edit...", None, (255, 255, 255))
        st.addContextHelp("Any Communicationand here.")
        return st.getBlock()
    
    #----------------------------------------------------------------------
    def hack_output(self):
        st = Block.Output("Editable")
        st.addEdit("edit...", None, (255, 255, 255))
        st.addContextHelp("Any Communicationand here.")
        return st.getBlock()
    
    #----------------------------------------------------------------------
    def hack_output_b(self):
        st = Block.OutputBool("Editable")
        st.addEdit("edit...", None, (255, 255, 255))
        st.addContextHelp("Any Communicationand here.")
        return st.getBlock()

        
