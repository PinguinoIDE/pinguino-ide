#!/usr/bin/env python
#-*- coding: utf-8 -*-

from . import constructor as Block

########################################################################
class Control(object):
    
    #----------------------------------------------------------------------
    def if_(self):
        pm = Block.Nested("Control")
        pm.addLabel("if")
        pm.addSyntax("(")
        pm.addSpaceBool()
        pm.addSyntax(")")
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def else_(self):
        pm = Block.NestedNext("Control")
        pm.addLabel("else")
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def else_if(self):
        pm = Block.NestedNext("Control")
        pm.addLabel("else if")
        pm.addSyntax("(")
        pm.addSpaceBool()
        pm.addSyntax(")")
        return pm.getBlock()
        
    #----------------------------------------------------------------------
    def while_(self):
        pm = Block.Nested("Control")
        pm.addLabel("while")
        pm.addSyntax("(")
        pm.addSpaceBool()
        pm.addSyntax(")")
        return pm.getBlock()
        
    #----------------------------------------------------------------------
    def for_(self):
        pm = Block.Nested("Control")
        pm.addLabel("for")
        pm.addSyntax("(")
        pm.addSpace()
        pm.addSyntax(";")
        pm.addSpaceBool()
        pm.addSyntax(";")
        pm.addSpace()
        pm.addSyntax(")")
        return pm.getBlock()


    #----------------------------------------------------------------------
    def repeat(self):
        pm = Block.Nested("Control")
        pm.addSyntax("//")
        pm.addLabel("Repeat")
        pm.addSyntax("\n")
        pm.addSyntax("for(__i__=0;__i__<")
        pm.addSpace()
        pm.addSyntax(";__i__=__i__+1)")
        pm.fillWith(["spin-int"])
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def switch(self):
        pm = Block.Nested("Control")
        pm.addLabel("switch")
        pm.addSyntax("(")
        pm.addSpace()
        pm.addSyntax(")")
        return pm.getBlock()

    #----------------------------------------------------------------------
    def case(self):
        pm = Block.NestedNext("Control")
        pm.addLabel("case")
        pm.addSyntax("(")
        pm.addSpace()
        pm.addSyntax(")")
        pm.fillWith(["spin-int"])
        return pm.getBlock()

    #----------------------------------------------------------------------
    def return_(self):
        pm = Block.Linear("Control")
        pm.addLabel("return")
        return pm.getBlock()

    ##----------------------------------------------------------------------
    #def return__(self):
        #pm = Block.Linear("Control")
        #pm.addLabel("return")
        #pm.addSyntax(" ")
        #pm.addSpinChoice("int", ["char*", "char", "int", "float", "u8", "u16", "u32"])
        #pm.addSyntax(" (")
        #pm.addSpace()
        #pm.addSyntax(")")
        #return pm.getBlock()
    
    #----------------------------------------------------------------------
    def return___(self):
        pm = Block.Linear("Control")
        pm.addLabel("return")
        pm.addSyntax(" ")
        pm.addSpace()
        return pm.getBlock()
        

    #----------------------------------------------------------------------
    def break_(self):
        pm = Block.Linear("Control")
        pm.addLabel("break")
        return pm.getBlock()

