#!/usr/bin/env python
#-*- coding: utf-8 -*-

#from PyQt4 import QtCore, QtGui
#from PyQt4.Qt import QApplication, QPoint
#from snippet import snippet as snippet_complete
#import sys, re
#from PyQt4.QtGui import QListWidget, QListWidgetItem, QIcon

import re
#from ..snippets. snippets import snippet as snippet_complete
from ..helpers.syntax import Autocompleter, Snippet

from PySide import QtGui, QtCore
from PySide.QtCore import QPoint
from .autocompleter import PinguinoAutoCompleter

from .autocomplete_icons import CompleteIcons	
        
from .syntaxhighlighter import Highlighter                
 
########################################################################
class CustomTextEdit(QtGui.QTextEdit):

    #----------------------------------------------------------------------
    def __init__(self, *args, **kwargs):
        super(CustomTextEdit, self).__init__(*args, **kwargs)
        self.completer = PinguinoAutoCompleter()
        self.completer.text_edit = self
        self.cursorC = QtGui.QCursor()
        self.mousePressEvent = self.mouseAction  
        self.completer.setFont(self.font())
        self.connect(self.completer, QtCore.SIGNAL("itemDoubleClicked(QListWidgetItem*)"), self.insertItem)
        self.completer.keyPressEvent = self.keyPressEvent_autocompleter
        self.resize(500, 500)
        self.completer.setFont(self.font())
        self.setAutoClose({})
        
        icons = CompleteIcons()
        
        self.completer.addItemsCompleter(Autocompleter["directive"], icons.iconDirectives)
        self.completer.addItemsCompleter(Autocompleter["reserved"], icons.iconReserved)
        
        self.completer.addItemsCompleter(Snippet.keys(), icons.iconSnippet)
        
        
        Highlighter(self)
        
        
    #----------------------------------------------------------------------
    def setAutoClose(self, dic):
        self.autoClose = dic
        
    #----------------------------------------------------------------------
    def mouseAction(self,event):
        if self.underMouse(): self.completer.hide()
        QtGui.QTextEdit.mousePressEvent(self, event)
        
        
    #----------------------------------------------------------------------
    def insertItem(self, completion):
        self.insert(completion.text())
        
        
    #----------------------------------------------------------------------
    def insert(self, completion):
        if completion == None: return
        selected = completion
        tc = self.textCursor()
        tc.select(QtGui.QTextCursor.WordUnderCursor)
        tc.removeSelectedText()
        
        if completion in Snippet.keys():
            pos = tc.position()
            tc.insertText(Snippet[str(completion)][1].replace("\t", " "*4))
            tc.setPosition(pos + Snippet[str(completion)][0])
            self.setTextCursor(tc)
        elif re.match("(.+) +\[.+\]", str(completion)) != None:
            ins = re.match("(.+) +\[.+\]", str(completion)).group(1)
            tc.insertText(ins)
        else: tc.insertText(completion)        
        self.completer.hide()
        self.setFocus()
        
        
        
    #----------------------------------------------------------------------
    def getPosPopup(self):
        pos = self.pos()
        pos1 = self.mapToGlobal(pos)
        pos2 = QPoint()
        cRect = self.cursorRect()
        pos2.setX(cRect.x())
        pos2.setY(cRect.y())
        return pos1 + pos2
    
    #----------------------------------------------------------------------
    def autoInsert(self, event):
        key = event.text()
        tc = self.textCursor()
        
        def accept(insert):
            QtGui.QTextEdit.keyPressEvent(self, event)
            tc.insertText(insert)
            tc.setPosition(tc.position() - len(insert))
            self.setTextCursor(tc)
            return True

        #if key=="[" and self.autoClose["brackets"]: return accept("]")
        #elif key=="{" and self.autoClose["keys"]: return accept("}")
        #elif key=='"' and self.autoClose["doublequotation"]: return accept('"')
        #elif key=="'" and self.autoClose["singlequotation"]: return accept("'")
        #elif key=="(" and self.autoClose["parentheses"]: return accept(")")
        #else: return False

    #----------------------------------------------------------------------
    def keyPressEvent_autocompleter(self, event):
        """"""
        #desplazar
        if event.key() in (QtCore.Qt.Key_Up, QtCore.Qt.Key_Down):
            self.completer.setFocus()
            if event.key() == QtCore.Qt.Key_Up: self.completer.up()
            elif event.key() == QtCore.Qt.Key_Down: self.completer.down()
            return        
        
        #insertar
        elif event.key() in (QtCore.Qt.Key_Enter, QtCore.Qt.Key_Enter-1, QtCore.Qt.Key_Tab):
            self.insert(self.completer.currentItem().text())
            self.completer.hide()
            return
        
        self.keyPressEvent(event)
        
            
    #----------------------------------------------------------------------
    def keyPressEvent(self, event):
        self.setFocus()
        
        if event.key() in (
            QtCore.Qt.Key_Enter,
            QtCore.Qt.Key_Escape,
            QtCore.Qt.Key_Space,
            QtCore.Qt.Key_Control,
            QtCore.Qt.Key_Shift,
            QtCore.Qt.Key_Alt,
            QtCore.Qt.Key_Backtab,
            QtCore.Qt.Key_Up,
            QtCore.Qt.Key_Down, 
            QtCore.Qt.Key_Right, 
            QtCore.Qt.Key_Left,
            ) or event.modifiers() in (
            QtCore.Qt.ControlModifier,
            QtCore.Qt.ShiftModifier, 
            QtCore.Qt.AltModifier, 
            ):
            self.completer.hide()
            super(CustomTextEdit, self).keyPressEvent(event)
            return
        
        #Si el autocerrado está activado
        if self.autoInsert(event): return

        if event.key() == QtCore.Qt.Key_Tab:
            self.insert(" "*4)
            return
        
        super(CustomTextEdit, self).keyPressEvent(event)
        
        self.show_autocomplete_if_conditions()
        
        
    #----------------------------------------------------------------------
    def show_autocomplete_if_conditions(self):
        """"""
        tc=self.textCursor()
        tc.select(QtGui.QTextCursor.WordUnderCursor)
        #Si no cumple con el mínimo de letras
        try:
            if len(str(tc.selectedText())) < self.completer.spell:
                self.completer.hide()
                #self.setFocus()
        except UnicodeEncodeError: return  #capturas tildes y caracteres especiales
        
        else:
            self.completer.popup(self.getPosPopup(), str(tc.selectedText()))
            self.setFocus()            