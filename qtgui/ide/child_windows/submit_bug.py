#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os

from PySide import QtGui, QtCore

from ...frames.submit_bug import Ui_SubmitBug

########################################################################
class SubmitBug(QtGui.QDialog):

    def __init__(self, parent, summary=""):
        super(SubmitBug, self).__init__()
        self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint |
                            QtCore.Qt.WindowSystemMenuHint |
                            QtCore.Qt.WindowStaysOnTopHint |
                            QtCore.Qt.Tool)

        file = QtCore.QFile(":ui/frames/submit_bug.ui")
        file.open(QtCore.QFile.ReadOnly)
        self.submit = loader.load(file, self)
        file.close()

        self.submit.setupUi(self)
        self.main = parent

        self.submit.plainTextEdit_details.insertPlainText(summary)

        self.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+self.windowTitle())

        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/logo/art/windowIcon.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.setWindowIcon(icon)

        self.connect(self.submit.pushButton_submit, QtCore.SIGNAL("clicked()"), self.submit_now)
        self.connect(self.submit.pushButton_cancel, QtCore.SIGNAL("clicked()"), self.close)
        self.connect(self.submit.checkBox_show_this_dialog, QtCore.SIGNAL("clicked()"), self.update_submit_dialog)

        self.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)

        self.center_on_screen()

    #----------------------------------------------------------------------
    def center_on_screen(self):

        screen = QtGui.QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)


    #----------------------------------------------------------------------
    def update_submit_dialog(self):

        self.main.configIDE.set("Features", "submmit_bugs", self.submit.checkBox_show_this_dialog.isChecked())


    #----------------------------------------------------------------------
    def submit_now(self):
        """"""

