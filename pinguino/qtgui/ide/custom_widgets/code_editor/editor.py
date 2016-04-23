#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import re
import pickle

from PySide import QtGui, QtCore
from PySide.QtCore import QPoint

#from .autocompleter import PinguinoAutoCompleter
from .autocomplete_icons import CompleteIcons
from .pinguino_highlighter import Highlighter
from .syntax import Autocompleter, Snippet, Helpers
#from ..methods import constants as Constants
#from ..methods.decorators import Decorator


########################################################################
class CustomTextEdit(QtGui.QTextEdit):

    #----------------------------------------------------------------------
    def __init__(self, parent, linenumber, highlighter):

        super(CustomTextEdit, self).__init__(parent)

        self.setLineWrapMode(self.NoWrap)

        with open(os.path.join(os.getenv("PINGUINO_USER_PATH"), "reserved.pickle"), "rb") as file_reserved:
            self.helpers = pickle.load(file_reserved).get("helpers", {})

        self.cursorC = QtGui.QCursor()
        self.linenumber = linenumber

        #if autocompleter:
            #self.setau
            #self.completer = PinguinoAutoCompleter()
            #self.completer.text_edit = self
            #self.mousePressEvent = self.mouseAction
            #self.completer.setFont(self.font())
            #self.connect(self.completer, QtCore.SIGNAL("itemDoubleClicked(QListWidgetItem*)"), self.insertItem)
            #self.completer.keyPressEvent = self.keyPressEvent_autocompleter
            #self.completer.setFont(self.font())

            #icons = CompleteIcons()
            #self.completer.addItemsCompleter(Autocompleter["directive"], icons.iconDirectives)
            #self.completer.addItemsCompleter(Autocompleter["reserved"], icons.iconReserved)
            #self.completer.addItemsCompleter(Snippet.keys(), icons.iconSnippet)
            #self.last_w = ""
            #self.next_ignore = None


        if highlighter:
            Highlighter(self.document())

        self.setStyleSheet("""
        QTextEdit {
            background-color: #FFF;
            font-family: mono;
            font-weight: normal;
            font-size: 10pt;
            }
        """)


    #----------------------------------------------------------------------
    def set_autocompleter(self, autocompleter):
        """"""

        self.completer = autocompleter
        self.completer.text_edit = self
        self.mousePressEvent = self.mouseAction
        self.completer.setFont(self.font())
        self.connect(self.completer, QtCore.SIGNAL("itemDoubleClicked(QListWidgetItem*)"), self.insertItem)
        self.completer.keyPressEvent = self.keyPressEvent_autocompleter
        self.completer.setFont(self.font())

        icons = CompleteIcons()
        self.completer.addItemsCompleter(Autocompleter["directive"], icons.iconDirectives)
        self.completer.addItemsCompleter(Autocompleter["reserved"], icons.iconReserved)
        self.completer.addItemsCompleter(Snippet.keys(), icons.iconSnippet)
        self.last_w = ""
        self.next_ignore = None




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

        size = self.fontPointSize()
        if size == 0:
            size = 10

        if delta > 0: size = size + 1
        else: size = size - 1

        if size <= 1:
            size = 1

        cursor = self.textCursor()
        self.selectAll()
        self.setFontPointSize(size)

        font = self.linenumber.font()
        font.setPointSize(size)
        self.linenumber.setFont(font)

        self.linenumber.setStyleSheet("""
        font-family: mono;
        font-weight: normal;
        font-size: {}dpt;

        """.format(size))

        # cursor.clearSelection()
        self.setTextCursor(cursor)



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

        # if completion in Helpers.keys():

        self.temp_helpers = self.helpers.copy()
        self.temp_helpers.update(Helpers)
        self.temp_helpers.update(self.completer.local_functions)

        pos = tc.position()
        tc.movePosition(tc.EndOfLine, tc.KeepAnchor)
        at_right = tc.selectedText()
        tc.setPosition(pos, tc.MoveAnchor)

        if completion in self.temp_helpers.keys() and not at_right:
            pos = tc.position()

            # text_position = Snippet[completion].find("[!]")
            text_insert = self.temp_helpers[completion].replace("{{", "").replace("}}", "")
            position_in_line = tc.positionInBlock()

            start_position = self.temp_helpers[completion].find("{{")
            end_position = self.temp_helpers[completion].find("}}")

            tc.insertText(text_insert.replace("\n", "\n"+" "*position_in_line))
            tc.setPosition(pos + start_position)

            select = self.temp_helpers[completion][(start_position + 2):end_position]
            tc.beginEditBlock()
            self.moveCursor(tc.StartOfLine)
            self.find(select)
            tc.endEditBlock()


        elif completion in Snippet.keys():
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

        else:
            tc.insertText(completion)

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

        if not key_text:
            self.completer.hide()
            super(CustomTextEdit, self).keyPressEvent(event)
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
            QtCore.Qt.Key_Backspace,
            QtCore.Qt.Key_CapsLock,
            ) or event.modifiers() in (
            QtCore.Qt.ControlModifier,
            QtCore.Qt.AltModifier,
            ):  # this is so sad!
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
        """"""

        tc = self.textCursor()

        if self.get_format() in ["comment", "quotation"]:
            self.completer.hide()
            return

        self.smart_under_selection(tc)
        selected = tc.selectedText().split()

        if not selected:
            self.completer.hide()
            return


        if re.match('^[\w.]+$', selected[-1]) is None:
            self.completer.hide()
            return


        if selected: self.last_w = selected[-1]

        try:
            #Si no cumple con el mínimo de letras
            if len(self.last_w) < self.completer.spell:
                self.completer.hide()

            else:
                self.completer.popup(self.getPosPopup(), self.last_w)
                #self.completer.show()

        except UnicodeEncodeError:
            return  #capturas tildes y caracteres especiales


    #----------------------------------------------------------------------
    def brace_match(self):

        return


    #----------------------------------------------------------------------
    def get_format(self):

        contex_color = {"#7f0000": "quotation",
                        "#cc0000": "quotation",
                        "#007f00": "comment",
                        "#c81818": "comment",}

        tc = self.textCursor()
        pos = tc.positionInBlock()

        block = tc.block()
        layout = block.layout()
        formats = layout.additionalFormats()

        for format_ in formats:
            if pos >= format_.start and pos <= format_.start + format_.length:
                return contex_color.get(format_.format.foreground().color().name().lower(), None)






