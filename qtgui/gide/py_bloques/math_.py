#!/usr/bin/env python
#-*- coding: utf-8 -*-

from constructor import * 
kit = Kit()

########################################################################
class Math_():
    
    #----------------------------------------------------------------------
    def __init__(self):
        """constructor"""
    
        
    sin = lambda n:kit.addOper("Math", "sin", "output", ["space"])
    cos = lambda n:kit.addOper("Math", "cos", "output", ["space"])
    tan = lambda n:kit.addOper("Math", "tan", "output", ["space"])
    cot = lambda n:kit.addOper("Math", "cot", "output", ["space"])
    sec = lambda n:kit.addOper("Math", "sec", "output", ["space"])
    csc = lambda n:kit.addOper("Math", "csc", "output", ["space"])
    
    add = lambda n:kit.addOper("Math", "+", "output", ["space", "space"])
    sub = lambda n:kit.addOper("Math", "-", "output", ["space", "space"])
    pro = lambda n:kit.addOper("Math", "*", "output", ["space", "space"])
    div = lambda n:kit.addOper("Math", "/", "output", ["space", "space"])
    ala = lambda n:kit.addOper("Math", "^", "output", ["space", "space"])
    
    
    may = lambda n:kit.addOper("Math", ">", "output-bool", ["space", "space"])   
    mayeq = lambda n:kit.addOper("Math", ">=", "output-bool", ["space", "space"])   
    men = lambda n:kit.addOper("Math", "<", "output-bool", ["space", "space"])   
    meneq = lambda n:kit.addOper("Math", "<=", "output-bool", ["space", "space"])
    
    equ = lambda n:kit.addOper("Math", "==", "output-bool", ["space", "space"])  
    nequ = lambda n:kit.addOper("Math", "!=", "output-bool", ["space", "space"])  
    equ_b = lambda n:kit.addOper("Math", "==", "output-bool", ["space_bool", "space_bool"])  
    nequ_b = lambda n:kit.addOper("Math", "!=", "output-bool", ["space_bool", "space_bool"])    
    
    and_ = lambda n:kit.addOper("Math", "@and@", "output-bool", ["space_bool", "space_bool"])  
    or_ = lambda n:kit.addOper("Math", "@or@", "output-bool", ["space_bool", "space_bool"])  
    not_ = lambda n:kit.addOper("Math", "@not@", "output-bool", ["space_bool", "space_bool"])  


    #----------------------------------------------------------------------
    def map_(self):
        pm = Output("Math")
        pm.addLabel("map")
        pm.addSyntax("(")
        pm.addSpace()
        pm.addSyntax(",")
        pm.addDecorator(" ")
        pm.addSpace()       
        pm.addSyntax(",")
        pm.addDecorator("~") 
        pm.addSpace()   
        pm.addSyntax(",")
        pm.addDecorator(" ")
        pm.addSpace()       
        pm.addSyntax(",")
        pm.addDecorator("~") 
        pm.addSpace()  
        pm.addSyntax(")")
        return pm.getBlock()
        
    
    #----------------------------------------------------------------------
    def agrup(self):
        pm = Output("Math")
        pm.addLabel("(")
        pm.addSpace()
        pm.addLabel(")")
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def agrup_bool(self):
        pm = Output("Math")
        pm.addLabel("(")
        pm.addSpaceBool()
        pm.addLabel(")")
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def incre__(self):
        pm = Output("Math")
        pm.addSpace()
        pm.addLabel("++")
        return pm.getBlock()       

    #----------------------------------------------------------------------
    def decre__(self):
        pm = Output("Math")
        pm.addSpace()
        pm.addLabel("--")
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def pre_incre(self):
        pm = Output("Math")
        pm.addLabel("++")
        pm.addSpace()
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def pre_decre(self):
        pm = Output("Math")
        pm.addLabel("--")
        pm.addSpace()
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def linear__(self):
        pm = Linear("Math")
        pm.addSpace()
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def linearb__(self):
        pm = Linear("Math")
        pm.addSpaceBool()
        return pm.getBlock()       

    #----------------------------------------------------------------------
    def pre_decre(self):
        pm = Output("Math")
        pm.addLabel("--")
        pm.addSpace()
        return pm.getBlock()   

