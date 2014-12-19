
#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

from ...frames.board_config_advance import Ui_AdvanceBoardConfig
from ...pinguino_api.pinguino_config import PinguinoConfig
from PySide import QtGui, QtCore

########################################################################
class BoardConfigAdvance(QtGui.QDialog):

    def __init__(self, parent, boardname):
        super(BoardConfigAdvance, self).__init__()
        self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint |
                            QtCore.Qt.WindowSystemMenuHint |
                            QtCore.Qt.WindowStaysOnTopHint |
                            QtCore.Qt.Tool)

        self.board_config = Ui_AdvanceBoardConfig()
        self.board_config.setupUi(self)
        self.main = parent

        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/logo/art/windowIcon.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.setWindowIcon(icon)

        #FIXME: add correct options
        self.HEAPSIZE = {"512 byte": 512,
                         "1024 byte": 1024,}
        self.board_config.comboBox_heapsize.clear()
        self.board_config.comboBox_heapsize.addItems(self.HEAPSIZE.keys())

        self.OPTIMIZATION = "-O2 -O3 -Os".split()
        self.board_config.comboBox_optimization.clear()
        self.board_config.comboBox_optimization.addItems(self.OPTIMIZATION)

        self.load_config()

        self.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+self.windowTitle())

        self.connect(self.board_config.pushButton_accept, QtCore.SIGNAL("clicked()"), self.accept_config)
        self.connect(self.board_config.pushButton_defaul, QtCore.SIGNAL("clicked()"), self.restore_default)

        self.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)

        #self.board_config.pushButton_advance.setStyleSheet("""
        #QPushButton{
            #font-family: inherit;
            #font-weight: bold;
        #}
        #""")

        self.center_on_screen()


    #----------------------------------------------------------------------
    def center_on_screen(self):

        size=self.size()
        desktopSize = QtGui.QDesktopWidget().screenGeometry()
        top = (desktopSize.height() / 2) - (size.height() / 2)
        left = (desktopSize.width() / 2) - (size.width() / 2)
        self.move(left, top)


    #----------------------------------------------------------------------
    def restore_default(self):

        self.board_config.checkBox_mips16.setChecked(True)
        self.board_config.comboBox_heapsize.setCurrentIndex(0)
        self.board_config.comboBox_optimization.setCurrentIndex(1)


    #----------------------------------------------------------------------
    def accept_config(self):

        self.save_config()
        self.main.configIDE.save_config()
        self.close()

    #----------------------------------------------------------------------
    def save_config(self):

        self.main.configIDE.set("Board", "mips16", self.board_config.checkBox_mips16.isChecked())
        heapsize = self.HEAPSIZE[self.board_config.comboBox_heapsize.currentText()]
        self.main.configIDE.set("Board", "heapsize", heapsize)
        self.main.configIDE.set("Board", "optimization", self.board_config.comboBox_optimization.currentText())


    #----------------------------------------------------------------------
    def load_config(self):

        #Called in the parent frame
        #self.main.configIDE.load_config()

        mips16 = self.main.configIDE.config("Board", "mips16", True)
        self.board_config.checkBox_mips16.setChecked(mips16)

        heapsize = self.main.configIDE.config("Board", "heapsize", 512)
        heapsize_values = self.HEAPSIZE.values()
        index = heapsize_values.index(heapsize)
        self.board_config.comboBox_heapsize.setCurrentIndex(index)

        optimization = self.main.configIDE.config("Board", "optimization", "-O3")
        index = self.OPTIMIZATION.index(optimization)
        self.board_config.comboBox_optimization.setCurrentIndex(index)


    #----------------------------------------------------------------------
    def close(self):

        PinguinoConfig.update_pinguino_extra_options(self.main.configIDE, self.main.pinguinoAPI)
        super(BoardConfigAdvance, self).close()
