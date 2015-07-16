#! /usr/bin/python2
#-*- coding: utf-8 -*-

from . import constructor as Block

########################################################################
class Widgets(object):

    #----------------------------------------------------------------------
    def spin_int(self):

        pm = Block.Output("Output")
        pm.addSpinInt(0, -100, 100)
        return pm.getBlock()


    #----------------------------------------------------------------------
    def spin_float(self):

        pm = Block.Output("Output")
        pm.addSpinFloat(0.1, 0.1, -100, 100)
        return pm.getBlock()


    #----------------------------------------------------------------------
    def slider(self):

        pm = Block.Output("Output")
        pm.addSlider(0, 0, 1023)
        return pm.getBlock()


    #----------------------------------------------------------------------
    def choice_io(self):

        pm = Block.Output("Output")
        pm.addSpinChoice("INPUT", ["INPUT", "OUTPUT"])
        return pm.getBlock()


    #----------------------------------------------------------------------
    def choice_hl(self):

        pm = Block.Output("Output")
        pm.addSpinChoice("HIGH", ["HIGH", "LOW"])
        return pm.getBlock()


    #----------------------------------------------------------------------
    def choice_A(self):

        pm = Block.Output("Output")
        pm.addSpinChoice("A0", map(lambda x:"A{}".format(x), range(6)))
        return pm.getBlock()


    #----------------------------------------------------------------------
    def choice_D(self):

        pm = Block.Output("Output")
        pm.addSpinChoice("D0", map(lambda x:"D{}".format(x), range(14)))
        return pm.getBlock()


    #----------------------------------------------------------------------
    def choice_B(self):

        pm = Block.Output("Output")
        pm.addSpinChoice("True", ["True", "False"])
        return pm.getBlock()


    #----------------------------------------------------------------------
    def choice_B_b(self):

        pm = Block.OutputBool("Output")
        pm.addSpinChoice("True", ["True", "False"])
        return pm.getBlock()


    #----------------------------------------------------------------------
    def choice_user(self):

        pm = Block.Output("Output")
        pm.addSpinChoice("USERLED", ["GREENLED", "YELLOWLED", "USERBUTTON", "RUNLED", "USERLED"])
        return pm.getBlock()


    #----------------------------------------------------------------------
    def edit(self):

        pm = Block.Output("Output")
        pm.addEdit("\"string\"")
        return pm.getBlock()

    #----------------------------------------------------------------------
    def edit_bool(self):

        pm = Block.OutputBool("Output")
        pm.addEdit("\"string\"")
        return pm.getBlock()


    #----------------------------------------------------------------------
    def edit_c(self):

        pm = Block.Output("Output")
        pm.addEdit("'c'", "white")
        return pm.getBlock()


    #----------------------------------------------------------------------
    def convert(self):

        pm = Block.Output("Output")
        pm.addSpaceBool()
        return pm.getBlock()


    #----------------------------------------------------------------------
    def convert_(self):

        pm = Block.OutputBool("Output")
        pm.addSpace()
        return pm.getBlock()
