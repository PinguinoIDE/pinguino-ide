#!/usr/bin/env python
#-*- coding: utf-8 -*-

import re
#import pickle

from PySide import QtGui, QtCore
from PySide.QtCore import QPoint

from .autocompleter import PinguinoAutoCompleter
from .autocomplete_icons import CompleteIcons
from .pinguino_highlighter import Highlighter
from ..methods.syntax import Autocompleter, Snippet
#from ..methods import constants as Constants
#from ..methods.decorators import Decorator


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

        #namespaces = pickle.load(open(Constants.IDE_NAMESPACES_FILE, "r"))
        #self.completer.addItemsCompleter(namespaces["all"], icons.iconLibrary)

        #palette = QtGui.QPalette(self.palette())
        #self.setAutoFillBackground(True)
        #palette.setColor(QtGui.QPalette.Base, QtGui.QColor("#FFFFFF"))
        #self.initialize()
        #self.setPalette(palette)

        self.next_ignore = None

        #Highlighter(self)

        Highlighter(self.document())

        #self.set_highlighter()

        self.setStyleSheet("""
        QTextEdit {
            background-color: #FFF;
            font-family: ubuntu mono;
            font-weight: normal;
            font-size: 12pt;
            }

        """)



    ##----------------------------------------------------------------------
    #@Decorator.call_later()
    #def set_highlighter(self):
        #""""""
        #Highlighter(self)


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
        if delta > 0: size = size + 1
        else: size = size - 1

        self.setStyleSheet("""
        QTextEdit {
            background-color: #FFF;
            font-family: ubuntu mono;
            font-weight: normal;
            font-size: %dpt;
            }

        """%size)


    #----------------------------------------------------------------------
    def insertItem(self, completion):

        self.insert(completion.text())


    #----------------------------------------------------------------------

    def insert(self, completion):
        if not completion: return
        #selected = completion
        tc = self.textCursor()

        self.smart_under_selection(tc)

        tc.removeSelectedText()

        if completion in Snippet.keys():
            pos = tc.position()

            text_position = Snippet[completion].find("[!]")
            text_insert = Snippet[completion].replace("[!]", "")
            position_in_line = tc.positionInBlock()

            tc.insertText(text_insert.replace("\n", "\n"+" "*position_in_line))
            tc.setPosition(pos + text_position)

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
            selected = tc.selectedText()
            tc.insertText(key + selected + insert)
            tc.movePosition(tc.Left, tc.MoveAnchor)
            tc.setPosition(tc.position()-len(selected), tc.MoveAnchor)
            tc.setPosition(tc.position()+len(selected), tc.KeepAnchor)
            return tc

        if key == "[":
            self.next_ignore = "]"
            return accept("]")

        elif key == "{":
            self.next_ignore = "}"
            return accept("}")

        elif key == '"':
            self.next_ignore = '"'
            return accept('"')

        elif key == "'":
            self.next_ignore = "'"
            return accept("'")

        elif key == "(":
            self.next_ignore = ")"
            return accept(")")
        else: return False


    #----------------------------------------------------------------------
    def keyPressEvent_autocompleter(self, event):

        #desplazar
        if event.key() in (QtCore.Qt.Key_Up, QtCore.Qt.Key_Down) and self.completer.isVisible():
            self.completer.setFocus()
            if event.key() == QtCore.Qt.Key_Up: self.completer.up()
            elif event.key() == QtCore.Qt.Key_Down: self.completer.down()
            return

        #insertar
        elif event.key() in (QtCore.Qt.Key_Enter, QtCore.Qt.Key_Enter-1, QtCore.Qt.Key_Tab) and self.completer.isVisible():
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

        key_text = event.text()
        if self.next_ignore == key_text:
            self.next_ignore = None
            return

        if event.key() in (
            #QtCore.Qt.Key_Enter,
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
        ttcc = self.autoInsert(event)
        if ttcc:
            self.setTextCursor(ttcc)
            return

        if event.key() == QtCore.Qt.Key_Tab:
            tc = self.textCursor()
            tc.insertText(" "*4)
            return


        if event.key() in [QtCore.Qt.Key_Enter, QtCore.Qt.Key_Enter-1]:
            tc = self.textCursor()
            pos = tc.position()
            tc.select(tc.LineUnderCursor)
            line = tc.selectedText()
            comment = ""
            if line.isspace() or line == "":
                len_s = len(line)
            else:
                normal = line.replace(" ", "")
                #if normal.startswith("//"):
                    #comment = "//"
                len_s = line.find(normal[0])

            tc.setPosition(pos)
            tc.insertText("\n"+" "*len_s+comment)
            return

        super(CustomTextEdit, self).keyPressEvent(event)

        self.show_autocomplete_if_conditions()


    #----------------------------------------------------------------------

    def smart_under_selection(self, tc):
        #word like: cdc|
        tc.movePosition(tc.WordLeft, tc.KeepAnchor)

        #word like: cdc.|
        if tc.selectedText().startswith("."): tc.movePosition(tc.WordLeft, tc.KeepAnchor)

        #word like: cdc.pri|
        tc.movePosition(tc.WordLeft, tc.KeepAnchor)
        if tc.selectedText().startswith("."): tc.movePosition(tc.WordLeft, tc.KeepAnchor)
        else: tc.movePosition(tc.WordRight, tc.KeepAnchor)


    #----------------------------------------------------------------------
    def show_autocomplete_if_conditions(self):

        tc = self.textCursor()
        self.smart_under_selection(tc)

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


    #----------------------------------------------------------------------
    def brace_match(self):

        return






