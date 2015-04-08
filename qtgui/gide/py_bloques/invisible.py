#! /usr/bin/python2
#-*- coding: utf-8 -*-

from . import constructor as Block

########################################################################
class NoVisible(object):

    #----------------------------------------------------------------------
    def edit_value(self):

        pm = Block.Output("None")
        pm.addEdit("value", "white")
        return pm.getBlock()


    #----------------------------------------------------------------------
    def number_base(self):

        pm = Block.Output("None")
        pm.addSpinChoice("10", ["2", "8", "10", "16"])
        return pm.getBlock()

