#!/usr/bin/env python
#-*- coding: utf-8 -*-

from . import constructor as Block

########################################################################
class NoVisible(object):
    
    #----------------------------------------------------------------------
    def edit_value(self):
        pm = Block.Output("None")
        pm.addEdit("value", "white")
        return pm.getBlock()    
    
