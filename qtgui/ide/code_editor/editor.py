#!/usr/bin/env python
#-*- coding: utf-8 -*-

import re
import pickle

from PySide import QtGui, QtCore
from PySide.QtCore import QPoint

from .autocompleter import PinguinoAutoCompleter
from .autocomplete_icons import CompleteIcons	
from .syntaxhighlighter import Highlighter        
from ..helpers.syntax import Autocompleter, Snippet
from ..helpers import constants as Constants
        
 
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
        #self.setAutoClose({})
        
        icons = CompleteIcons()
        
        self.completer.addItemsCompleter(Autocompleter["directive"], icons.iconDirectives)
        self.completer.addItemsCompleter(Autocompleter["reserved"], icons.iconReserved)
        
        self.completer.addItemsCompleter(Snippet.keys(), icons.iconSnippet)
        
        #namespaces = pickle.load(file(Constants.IDE_NAMESPACES_FILE, "r"))
        #self.completer.addItemsCompleter(namespaces["all"], icons.iconLibrary)
        
        #palette = QtGui.QPalette(self.palette())
        #self.setAutoFillBackground(True)
        #palette.setColor(QtGui.QPalette.Base, QtGui.QColor("#FFFFFF"))
        #self.setPalette(palette)          
        
        Highlighter(self)        

    
    ##----------------------------------------------------------------------
    #def setAutoClose(self, dic):
        #self.autoClose = dic
        
    #----------------------------------------------------------------------
    def mouseAction(self, event):
        if self.underMouse(): self.completer.hide()
        QtGui.QTextEdit.mousePressEvent(self, event)
        
        
    #----------------------------------------------------------------------
    def wheelEvent(self, event):
        
        if event.modifiers() == QtCore.Qt.ControlModifier:
            self.step_font_size(event.delta())
        else:
            super(CustomTextEdit, self).wheelEvent(event)
                
        
    #----------------------------------------------------------------------
    def step_font_size(self, delta):
        
        font = self.font()
        size = font.pointSize()
        if delta > 0: font.setPointSize(size+1)
        else: font.setPointSize(size-1)
        self.setFont(font)
                          
        
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
    def force_keyPressEvent(self, event):
        
        super(CustomTextEdit, self).keyPressEvent(event)
            
    #----------------------------------------------------------------------
    def __keyPressEvent__(self, event):
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
            #QtCore.Qt.ShiftModifier, 
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
        
        tc = self.textCursor()
        #tc.select(QtGui.QTextCursor.WordUnderCursor)
        tc.select(QtGui.QTextCursor.BlockUnderCursor)
        
        selected = tc.selectedText().split()
        if not selected: return
        #selected = selected[-1]
        
        #Si no cumple con el mínimo de letras
        try:
            if len(selected[-1]) < self.completer.spell:
                self.completer.hide()
        
            else:
                self.completer.popup(self.getPosPopup(), selected[-1])
                self.setFocus()
            
        except UnicodeEncodeError: return  #capturas tildes y caracteres especiales        