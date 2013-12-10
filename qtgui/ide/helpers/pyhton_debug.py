#!/usr/bin/env python
#-*- coding: utf-8 -*-

from .dialogs import Dialogs

########################################################################
class Stderr(file):
    """"""
    #----------------------------------------------------------------------
    @classmethod
    def write(self, message):
        """"""
        #Dialogs.error_message(None, message)
        self.plainTextEdit_output.insertPlainText(message)
        



########################################################################
class Stdout(file):
    """"""
    #----------------------------------------------------------------------
    @classmethod
    def write(self, message):
        """"""
        #Dialogs.error_message(None, message)
        self.plainTextEdit_output.insertPlainText(message)
        
                
                
                
    
    