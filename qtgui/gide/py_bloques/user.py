#!/usr/bin/env python
#-*- coding: utf-8 -*-


from constructor import * 
kit = Kit()

########################################################################
class User():
    
    #----------------------------------------------------------------------
    def __init__(self):
        """constructor"""
    
    #----------------------------------------------------------------------
    def user_function(self):
        pm = Function("User")
        pm.addEdit("userFunc", None, (255, 255, 255))
        pm.addSyntax("(")
        pm.addSpace()
        pm.addSyntax(")")
        return pm.getBlock()
        
    #----------------------------------------------------------------------
    def more_args(self):
        pm = Output("User") 
        pm.addSpace()
        pm.addSyntax(",")
        pm.addDecorator(" ")
        pm.addSpace()
        return pm.getBlock()

    #----------------------------------------------------------------------
    def asign(self):
        pm = Linear("User")
        pm.addEdit("var1", None, (255, 255, 255))
        pm.addLabel("=")
        pm.addSpace()
        pm.fillWith(["spin-int"])
        return pm.getBlock()


    #----------------------------------------------------------------------
    def frameEdit(self):
        pm = FrameText("Tools")
        pm.bloque.append("Pinguino Rules")
        pm.bloque.append(['resize', 300, 100])
        return pm.getBlock()
