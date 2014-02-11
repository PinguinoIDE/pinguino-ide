#!/usr/bin/env python
#-*- coding: utf-8 -*-

from .dialogs import Dialogs

########################################################################
class Stderr(file):
    
    #----------------------------------------------------------------------
    @classmethod
    def write(cls, message):
        Dialogs.error_message(None, message)
        


