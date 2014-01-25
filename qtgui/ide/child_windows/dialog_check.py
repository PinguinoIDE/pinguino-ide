#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import sys

from PySide import QtGui, QtCore

from ..methods.constants import TAB_NAME
from ...frames.dialog_check import Ui_DialogCheck

########################################################################
class DialogCheck(QtGui.QDialog):
    
    def __init__(self, IDE, config_option, message=None):
        super(DialogCheck, self).__init__()
        
        self.config = IDE.configIDE
        show_ = self.config.config("Check Dialogs", config_option, True)
        
        if not show_:
            super(DialogCheck, self).close()
            return
            
        self.config_option = config_option
        
        self.dialog = Ui_DialogCheck()
        self.dialog.setupUi(self)
        
        self.setWindowTitle(TAB_NAME)
        
        if message:
            self.show_message(message)
        
        self.connect(self.dialog.pushButton, QtCore.SIGNAL("clicked()"), self.close)
        
    #----------------------------------------------------------------------
    def close(self):
        """"""
        self.config.set("Check Dialogs", self.config_option, self.dialog.checkBox.isChecked())
        self.config.save_config()
        super(DialogCheck, self).close()
        
        
    #----------------------------------------------------------------------
    def show_message(self, message):
        try:
            self.dialog.textBrowser.insertHtml(message)
            self.show()
            return True
        except:
            return False