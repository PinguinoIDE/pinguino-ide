#! /usr/bin/python2
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
        pm.fillWith([("edit", "i=1"), ("edit-bool", "i<10"), ("edit", "i++")])
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
        pm.addSyntax("):")
        pm.fillWith(["spin-int"])
        return pm.getBlock()


    #----------------------------------------------------------------------
    def return_(self):

        pm = Block.Linear("Control")
        pm.addLabel("return")
        return pm.getBlock()


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
