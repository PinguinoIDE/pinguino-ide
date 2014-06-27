#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

from PySide import QtGui, QtCore

from ...frames.insert_block import Ui_InsertBlock

########################################################################
class InsertBlock(QtGui.QDialog):

    def __init__(self, KIT):
        super(InsertBlock, self).__init__()

        self.insert = Ui_InsertBlock()
        self.insert.setupUi(self)

        self.setWindowTitle(os.getenv("NAME")+" - "+self.windowTitle())

        self.graphical = KIT

        self.connect(self.insert.lineEdit, QtCore.SIGNAL("textChanged(QString)"), self.update_blocks)
        self.connect(self.insert.listWidget, QtCore.SIGNAL("itemActivated(QListWidgetItem*)"), self.insert_block)
        #self.connect(self.insert.listWidget, QtCore.SIGNAL("itemEntered(QListWidgetItem*)"), self.insert_block)

        self.insert.lineEdit.keyPressEvent = self.line_edit_key
        self.insert.listWidget.keyPressEvent = self.list_key

        self.insert.lineEdit.setText(QtGui.QApplication.translate("Dialogs", "search block..."))
        self.insert.lineEdit.selectAll()

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
    def line_edit_key(self, event):

        key = event.key()
        if key in (QtCore.Qt.Key_Down, ):
            self.insert.listWidget.setFocus()
            self.insert.listWidget.setCurrentRow(0)

        if key in (QtCore.Qt.Key_Enter, QtCore.Qt.Key_Enter - 1):
            if self.insert.listWidget.count() == 1:
                self.insert_block(self.insert.listWidget.item(0))

        QtGui.QLineEdit.keyPressEvent(self.insert.lineEdit, event)


    #----------------------------------------------------------------------
    def list_key(self, event):

        key = event.key()
        if key in (QtCore.Qt.Key_Down,
                   QtCore.Qt.Key_Up,
                   QtCore.Qt.Key_Enter,
                   QtCore.Qt.Key_Enter - 1,
                   ):
            QtGui.QListWidget.keyPressEvent(self.insert.listWidget, event)
            return
        QtGui.QLineEdit.keyPressEvent(self.insert.lineEdit, event)


    #----------------------------------------------------------------------
    def update_blocks(self, text):

        bloques = []
        self.items = {}
        self.all_sets = self.graphical.get_all_sets()
        for key in self.all_sets.keys():
            if self.all_sets[key][2][0] == "label":
                label = self.all_sets[key][2][1]
                if label.lower().startswith(text.lower()):
                    bloques.append([key, self.all_sets[key]])
                    self.items[label] = [key, self.all_sets[key]]
        self.insert.listWidget.clear()
        self.insert.listWidget.addItems(self.items.keys())


    #----------------------------------------------------------------------
    def insert_block(self, list_widget):

        self.hide()
        work_area = self.graphical.get_work_area()
        block = self.items[list_widget.text()]
        name = block[1][0]
        args = block[1][:]
        baseName = list_widget.text()
        args[0] = ""
        args[1] = ""
        pos = self.cursor().pos() - QtCore.QPoint(10, 10)
        if name: work_area.new_bloq(name, args, pos, baseName)
        self.close()

