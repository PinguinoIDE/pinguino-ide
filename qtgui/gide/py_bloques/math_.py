#!/usr/bin/env python
#-*- coding: utf-8 -*-

from . import constructor as Block
from .constructor import Kit

Kit = Kit()

########################################################################
class Math_(object):
    
    sin = lambda n:Kit.addOper("Math", "sin", "output", ["space"])
    cos = lambda n:Kit.addOper("Math", "cos", "output", ["space"])
    tan = lambda n:Kit.addOper("Math", "tan", "output", ["space"])
    cot = lambda n:Kit.addOper("Math", "cot", "output", ["space"])
    sec = lambda n:Kit.addOper("Math", "sec", "output", ["space"])
    csc = lambda n:Kit.addOper("Math", "csc", "output", ["space"])
    
    add = lambda n:Kit.addOper("Math", "+", "output", ["space", "space"])
    sub = lambda n:Kit.addOper("Math", "-", "output", ["space", "space"])
    pro = lambda n:Kit.addOper("Math", "*", "output", ["space", "space"])
    div = lambda n:Kit.addOper("Math", "/", "output", ["space", "space"])
    ala = lambda n:Kit.addOper("Math", "^", "output", ["space", "space"])
    
    
    may = lambda n:Kit.addOper("Math", ">", "output-bool", ["space", "space"])   
    mayeq = lambda n:Kit.addOper("Math", ">=", "output-bool", ["space", "space"])   
    men = lambda n:Kit.addOper("Math", "<", "output-bool", ["space", "space"])   
    meneq = lambda n:Kit.addOper("Math", "<=", "output-bool", ["space", "space"])
    
    equ = lambda n:Kit.addOper("Math", "==", "output-bool", ["space", "space"])  
    nequ = lambda n:Kit.addOper("Math", "!=", "output-bool", ["space", "space"])  
    equ_b = lambda n:Kit.addOper("Math", "==", "output-bool", ["space_bool", "space_bool"])  
    nequ_b = lambda n:Kit.addOper("Math", "!=", "output-bool", ["space_bool", "space_bool"])    
    
    and_ = lambda n:Kit.addOper("Math", "@and@", "output-bool", ["space_bool", "space_bool"])  
    or_ = lambda n:Kit.addOper("Math", "@or@", "output-bool", ["space_bool", "space_bool"])  
    not_ = lambda n:Kit.addOper("Math", "@not@", "output-bool", ["space_bool", "space_bool"])  


    #----------------------------------------------------------------------
    def map_(self):
        pm = Block.Output("Math")
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
        pm = Block.Output("Math")
        pm.addLabel("(")
        pm.addSpace()
        pm.addLabel(")")
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def agrup_bool(self):
        pm = Block.Output("Math")
        pm.addLabel("(")
        pm.addSpaceBool()
        pm.addLabel(")")
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def incre__(self):
        pm = Block.Output("Math")
        pm.addSpace()
        pm.addLabel("++")
        return pm.getBlock()       

    #----------------------------------------------------------------------
    def decre__(self):
        pm = Block.Output("Math")
        pm.addSpace()
        pm.addLabel("--")
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def pre_incre(self):
        pm = Block.Output("Math")
        pm.addLabel("++")
        pm.addSpace()
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def pre_decre(self):
        pm = Block.Output("Math")
        pm.addLabel("--")
        pm.addSpace()
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def linear__(self):
        pm = Block.Linear("Math")
        pm.addSpace()
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def linearb__(self):
        pm = Block.Linear("Math")
        pm.addSpaceBool()
        return pm.getBlock()       

    #----------------------------------------------------------------------
    def pre_decre(self):
        pm = Block.Output("Math")
        pm.addLabel("--")
        pm.addSpace()
        return pm.getBlock()   

