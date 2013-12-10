#!/usr/bin/env python
#-*- coding: utf-8 -*-

from . import constructor as Block

########################################################################
class User(object):
    
    #----------------------------------------------------------------------
    def user_function(self):
        pm = Block.Function("User")
        pm.addEdit("userFunc", None, (255, 255, 255))
        pm.addSyntax("(")
        pm.addSpace()
        pm.addSyntax(")")
        return pm.getBlock()
        
    #----------------------------------------------------------------------
    def more_args(self):
        pm = Block.Output("User") 
        pm.addSpace()
        pm.addSyntax(",")
        pm.addDecorator(" ")
        pm.addSpace()
        return pm.getBlock()

    #----------------------------------------------------------------------
    def asign(self):
        pm = Block.Linear("User")
        pm.addEdit("var1", None, (255, 255, 255))
        pm.addLabel("=")
        pm.addSpace()
        pm.fillWith(["spin-int"])
        return pm.getBlock()


    #----------------------------------------------------------------------
    def frameEdit(self):
        pm = Block.FrameText("Tools")
        pm.bloque.append("Pinguino Rules")
        pm.bloque.append(['resize', 300, 100])
        return pm.getBlock()
