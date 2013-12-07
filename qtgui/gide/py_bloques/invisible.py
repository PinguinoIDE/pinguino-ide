#!/usr/bin/env python
#-*- coding: utf-8 -*-


from constructor import * 
kit = Kit()

########################################################################
class NoVisible():
    
    #----------------------------------------------------------------------
    def __init__(self):
        """constructor"""
    
    #----------------------------------------------------------------------
    def edit_value(self):
        pm = Output("None")
        pm.addEdit("value", "white")
        return pm.getBlock()    
    
