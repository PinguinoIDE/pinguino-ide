#!/usr/bin/env python
#-*- coding: utf-8 -*-


from constructor import * 
kit = Kit()

########################################################################
class Widgets():
    
    #----------------------------------------------------------------------
    def __init__(self):
        """constructor"""
    
    #----------------------------------------------------------------------
    def spin_int(self):
        pm = Output("Output")
        pm.addSpinInt(0, -100, 100)
        return pm.getBlock()
        
        
    #----------------------------------------------------------------------
    def spin_float(self):
        pm = Output("Output")
        pm.addSpinFloat(0.1, 0.1, -100, 100)
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def slider(self):
        pm = Output("Output")
        pm.addSlider(0, 0, 1023)
        return pm.getBlock()
        
    #----------------------------------------------------------------------
    def choice_io(self):
        pm = Output("Output")
        pm.addSpinChoice("INPUT", ["INPUT", "OUTPUT"])
        return pm.getBlock()
        
    #----------------------------------------------------------------------
    def choice_hl(self):
        pm = Output("Output")
        pm.addSpinChoice("HIGH", ["HIGH", "LOW"])
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def choice_A(self):
        pm = Output("Output")
        pm.addSpinChoice("A0", map(lambda x:"A%d"%x, range(6)))
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def choice_D(self):
        pm = Output("Output")
        pm.addSpinChoice("D0", map(lambda x:"D%d"%x, range(14)))
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def choice_B(self):
        pm = Output("Output")
        pm.addSpinChoice("True", ["True", "False"])
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def choice_B_b(self):
        pm = OutputBool("Output")
        pm.addSpinChoice("True", ["True", "False"])
        return pm.getBlock()        

    #----------------------------------------------------------------------
    def choice_user(self):
        pm = Output("Output")
        pm.addSpinChoice("GREENLED", ["GREENLED", "YELLOWLED", "USERBUTTON", "RUNLED"])
        return pm.getBlock()

    #----------------------------------------------------------------------
    def edit(self):
        pm = Output("Output")
        pm.addEdit("\"string\"")
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def edit_c(self):
        pm = Output("Output")
        pm.addEdit("'c'", "white")
        return pm.getBlock()

    #----------------------------------------------------------------------
    def convert(self):
        pm = Output("Output")
        pm.addSpaceBool()
        return pm.getBlock()  

    #----------------------------------------------------------------------
    def convert_(self):
        pm = OutputBool("Output")
        pm.addSpace()
        return pm.getBlock()          

