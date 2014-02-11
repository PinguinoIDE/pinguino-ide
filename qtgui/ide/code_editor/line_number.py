#! /usr/bin/python
#-*- coding: utf-8 -*-

from PySide.QtGui import QWidget
from PySide.QtGui import QPainter
from PySide import QtCore, QtGui

#from ..methods.backgrounds import BackgroundPallete

class LineNumber(QWidget):

    #----------------------------------------------------------------------
    def __init__(self, *args, **kwargs):
        super(LineNumber, self).__init__(*args, **kwargs)
        
        self.edit = None
        self.highest_line = 0
        self.current = 0
        
        self.setMinimumSize(QtCore.QSize(51, 0))
        self.setMaximumSize(QtCore.QSize(51, 16777215))
        #BackgroundPallete.set_background_pinguino(self)
        
        palette = QtGui.QPalette(self.palette())
        self.setAutoFillBackground(True)
        palette.setColor(QtGui.QPalette.Window, QtGui.QColor(175, 200, 225))
        self.setPalette(palette)        
               
        

    #----------------------------------------------------------------------
    def setTextEdit(self, edit):
        self.edit = edit

    #----------------------------------------------------------------------
    def update(self, *args):
        #width=50
        #if self.width() != width:
            #self.setFixedWidth(width)
        QWidget.update(self, *args)

    #----------------------------------------------------------------------
    def paintEvent(self, event):
        contents_y = self.edit.verticalScrollBar().value()
        page_bottom = contents_y + self.edit.viewport().height()
        font_metrics = self.fontMetrics()
        current_block = self.edit.document().findBlock(self.edit.textCursor().position())
        painter = QPainter(self)
        line_count = 0
        block = self.edit.document().begin()
        while block.isValid():
            line_count += 1
            position = self.edit.document().documentLayout().blockBoundingRect(block).topLeft()
            if position.y() > page_bottom:
                break
            bold = False
            if block == current_block:
                bold = True
                font = painter.font()
                font.setBold(True)
                painter.setFont(font)
                self.current = line_count
            painter.drawText(self.width() - font_metrics.width(str(line_count)) - 10,
                             round(position.y()) - contents_y + font_metrics.ascent(),
                             str(line_count))
            if bold:
                font = painter.font()
                font.setBold(False)
                painter.setFont(font)
            block = block.next()
        self.highest_line = line_count
        painter.end()
        QWidget.paintEvent(self, event)
        