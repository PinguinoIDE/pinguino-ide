#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import sys

from PySide import QtCore, QtGui

from .python_highlighter import Highlighter
from .python_shell import PythonShell

HEAD = os.getenv("PINGUINO_NAME") + " " + os.getenv("PINGUINO_VERSION") + "\n" + "Python " + sys.version + " on " + sys.platform
HELP = QtGui.QApplication.translate("PythonShell", "Commands available:") + ' "clear", "restart"'

START = ">>> "
NEW_LINE = "... "


########################################################################
class PinguinoTextEdit(QtGui.QPlainTextEdit):

    #----------------------------------------------------------------------
    def __init__(self, widget=None, shell=True):
        super(PinguinoTextEdit, self).__init__(widget)

        self.shell = PythonShell()

        self.setLineWrapMode(self.NoWrap)

        if shell:
            self.appendPlainText(HEAD)
            self.appendPlainText(HELP)
            self.appendPlainText(START)

            self.extra_args = {}

            self.historial = []
            self.index_historial = 0

            self.multiline_commands = []

        Highlighter(self.document(), extra=[START, NEW_LINE], python=shell)
        # self.connect(self, QtCore.SIGNAL("textChanged(QString)"), self.textChanged)
        self.setFrameShape(QtGui.QFrame.NoFrame)




    #----------------------------------------------------------------------
    def write(self, text, prefix=""):
        # if not self.checkbox_debug.isChecked(): return
        # return self.log_output(text[:-1])

        if prefix:
            text = text.replace("\n", "\n{} ".format(prefix))
            text = text.replace("\\n", "\\n{} ".format(prefix))
            text = "{} {}".format(prefix, text)
        else:
            text = text.replace("\n", "\n")
            text = text.replace("\\n", "\\n")


        if not (text.endswith("\n") or text.endswith("\n ")):
            text += "\n"

        # for i in range(100): print(i)

        self.moveCursor(QtGui.QTextCursor.EndOfLine)
        self.moveCursor(QtGui.QTextCursor.End)
        self.insertPlainText(text)
        self.moveCursor(QtGui.QTextCursor.End)

        self.repaint()


    #----------------------------------------------------------------------
    def keyPressEvent(self, event):

        self.set_last_line()

        if event.key() in (QtCore.Qt.Key_Enter, QtCore.Qt.Key_Enter-1):
            self.moveCursor(QtGui.QTextCursor.End)
            self.index_historial = 0
            super(PinguinoTextEdit, self).keyPressEvent(event)

            command = self.get_command()

            if self.multiline_commands:
                self.multiline_commands.append(command)
                #return

                if command.endswith("\n... \n"):
                    #mcommand = ";".join(self.multiline_commands)
                    command = command.replace("\n", ";")
                    command = command.replace(":;", ":").replace(":;", ":").replace(":;", ":")
                    command = command.replace(NEW_LINE, "")
                    command = command[:-2]
                    command += "\n"
                    if self.run_default_command(command):
                        self.appendPlainText(START)
                        self.multiline_commands = []
                        return
                    else:
                        self.multiline_commands = []
                        pass

                else:
                    self.insertPlainText(NEW_LINE)
                    return

            elif command.endswith(":\n"):
                self.multiline_commands.append(command)
                self.insertPlainText(NEW_LINE)
                return

            elif self.run_default_command(command):
                self.appendPlainText(START)
                return

            self.historial.append(command.replace("\n", ""))

            if not command.isspace():
                self.moveCursor(QtGui.QTextCursor.End)
                # self.insertPlainText(self.shell.run(command))
                self.shell.run(command, self)
            self.insertPlainText(START)
            self.moveCursor(QtGui.QTextCursor.End)

        elif event.key() == QtCore.Qt.Key_Backspace:
            if not self.get_command(): return
            else: super(PinguinoTextEdit, self).keyPressEvent(event)

        elif event.key() == QtCore.Qt.Key_Up:
            if len(self.historial) >= self.index_historial + 1:
                self.index_historial += 1
                self.moveCursor(QtGui.QTextCursor.End)

                tc = self.textCursor()
                tc.movePosition(tc.StartOfLine, tc.KeepAnchor)
                tc.removeSelectedText()
                tc.insertText(START)

                tc.insertText(self.historial[-self.index_historial])

        elif event.key() == QtCore.Qt.Key_Down:
            if len(self.historial) >= self.index_historial - 1:
                self.index_historial -= 1
                if self.index_historial <= 0:
                    self.index_historial += 1
                    return
                self.moveCursor(QtGui.QTextCursor.End)

                tc = self.textCursor()
                tc.movePosition(tc.StartOfLine, tc.KeepAnchor)
                tc.removeSelectedText()
                tc.insertText(START)

                tc.insertText(self.historial[-self.index_historial])

        elif event.key() == QtCore.Qt.Key_Left:
            if self.no_overwrite_start():
                super(PinguinoTextEdit, self).keyPressEvent(event)

        elif event.key() == QtCore.Qt.Key_Tab:
            tc = self.textCursor()
            tc.movePosition(tc.WordLeft, tc.KeepAnchor)
            word = tc.selectedText()
            object_ = None
            attr_ = ""

            tc.movePosition(tc.WordLeft, tc.KeepAnchor)
            n_word = tc.selectedText()

            if n_word == ".":
                tc.movePosition(tc.WordLeft, tc.KeepAnchor)
                object_ = tc.selectedText()

            if n_word.startswith("."):
                tc.movePosition(tc.WordLeft, tc.KeepAnchor)
                tc.movePosition(tc.WordLeft, tc.KeepAnchor)
                if tc.selectedText().startswith("."):
                    while tc.selectedText().startswith("."):
                        tc.movePosition(tc.WordLeft, tc.KeepAnchor)
                else:
                    tc.movePosition(tc.WordRight, tc.KeepAnchor)

                object_, attr_ = tc.selectedText().rsplit(".", 1)


            if n_word.endswith("."):
                tc.movePosition(tc.WordLeft, tc.KeepAnchor)
                if tc.selectedText().startswith("."):
                    while tc.selectedText().startswith("."):
                        tc.movePosition(tc.WordLeft, tc.KeepAnchor)
                else:
                    tc.movePosition(tc.WordRight, tc.KeepAnchor)

                object_, attr_ = tc.selectedText().rsplit(".", 1)

            if object_:
                objects_ = object_.split(".")
                s = self.shell.statement_module
                for ob in objects_:
                    s = getattr(s, ob)

                options = filter(lambda x:x.startswith(attr_), s.__dict__.keys())

            elif word:
                s = self.shell.statement_module
                options = filter(lambda x:x.startswith(word), s.__dict__.keys())

            options = list(filter(lambda x:not x.startswith("__"), options))

            common = os.path.commonprefix(options)
            if common and word != common: options = [common]

            if len(options) > 1:
                self.moveCursor(tc.StartOfLine)
                i = 0
                options = sorted(options)
                for option in options:
                    i += 1
                    if i == 3:
                        i = 0
                        self.insertPlainText("{}\n".format(option))
                    else: self.insertPlainText("{}".format(option).ljust(50," "))
                self.insertPlainText("\n\n")
                self.moveCursor(tc.End)

            elif len(options) == 1:
                self.moveCursor(tc.End)

                if object_:
                    tc.movePosition(tc.WordRight, tc.KeepAnchor)
                    while "." in tc.selectedText():
                        tc.movePosition(tc.WordRight, tc.KeepAnchor)
                    tc.removeSelectedText()

                elif word:
                    tc.movePosition(tc.WordRight, tc.KeepAnchor)
                    tc.removeSelectedText()

                self.insertPlainText(options[0])
                self.moveCursor(tc.End)

        elif event.key() == QtCore.Qt.Key_Home:
            super(PinguinoTextEdit, self).keyPressEvent(event)
            tc = self.textCursor()
            self.moveCursor(tc.WordRight, tc.MoveAnchor)

        elif event.key() in [QtCore.Qt.Key_PageUp, QtCore.Qt.Key_PageDown]:
            pass

        else:
            super(PinguinoTextEdit, self).keyPressEvent(event)


    #----------------------------------------------------------------------
    def get_command(self):
        plain = self.toPlainText()
        comand = plain[plain.rfind(START):]
        return comand[len(START):]


    #----------------------------------------------------------------------
    def wheelEvent(self, event):
        if event.modifiers() == QtCore.Qt.ControlModifier:
            self.step_font_size(event.delta())

        else: super(PinguinoTextEdit, self).wheelEvent(event)


    ##----------------------------------------------------------------------
    #def (self):
        #""""""


    #----------------------------------------------------------------------
    def contextMenuEvent(self, event):
        menu = QtGui.QMenu()

        if hasattr(self, "historial"):
            if self.historial:
                sub_menu = QtGui.QMenu(QtGui.QApplication.translate("PythonShell", "Last commands"))
                rhistorial = self.historial[:]

                rhistorial.reverse()

                rhistorial = filter(lambda cm: not cm.isspace(), rhistorial)
                rhistorial = filter(lambda cm: not cm == "", rhistorial)

                for command in rhistorial[:10]:
                    sub_menu.addAction(command, lambda :self.insertPlainText(command))
                menu.addMenu(sub_menu)
                menu.addSeparator()

        if self.isReadOnly():
            # menu.addAction(QtGui.QApplication.translate("PythonShell", "Clear"), self.command_clear)
            # menu.addAction(QtGui.QApplication.translate("PythonShell", "Restart"), self.command_restart)
            # menu.addSeparator()
            # menu.addAction(QtGui.QApplication.translate("PythonShell", "Cut"), self.cut, QtGui.QKeySequence.Cut)
            menu.addAction(QtGui.QApplication.translate("PythonShell", "Copy"), self.copy, QtGui.QKeySequence.Copy)
            # menu.addAction(QtGui.QApplication.translate("PythonShell", "Paste"), self.paste, QtGui.QKeySequence.Paste)
            menu.addAction(QtGui.QApplication.translate("PythonShell", "Select all"), self.selectAll, QtGui.QKeySequence.SelectAll)
            # menu.addSeparator()

        else:
            menu.addAction(QtGui.QApplication.translate("PythonShell", "Clear"), self.command_clear)
            menu.addAction(QtGui.QApplication.translate("PythonShell", "Restart"), self.command_restart)
            menu.addSeparator()
            menu.addAction(QtGui.QApplication.translate("PythonShell", "Cut"), self.cut, QtGui.QKeySequence.Cut)
            menu.addAction(QtGui.QApplication.translate("PythonShell", "Copy"), self.copy, QtGui.QKeySequence.Copy)
            menu.addAction(QtGui.QApplication.translate("PythonShell", "Paste"), self.paste, QtGui.QKeySequence.Paste)
            menu.addAction(QtGui.QApplication.translate("PythonShell", "Select all"), self.selectAll, QtGui.QKeySequence.SelectAll)
            # menu.addSeparator()
            #menu.addAction(self.main.actionComment_out_region)

        menu.setStyleSheet("""
        QMenu {
            font-family: inherit;
            font-weight: normal;
            }

        """)

        menu.exec_(event.globalPos())



    #----------------------------------------------------------------------
    def step_font_size(self, delta):
        font = self.font()
        size = font.pointSize()
        if delta > 0: size = size + 1
        else: size = size - 1

        self.setStyleSheet("""
        QPlainTextEdit {
            background-color: #333;
            color: #FFFFFF;
            font-family: mono;
            font-weight: normal;
            font-size: {}dpt;
        }
        """.format(size))


    #----------------------------------------------------------------------
    def set_extra_args(self, *args, **kwargs):
        for key in kwargs:
            setattr(self.shell.statement_module, key, kwargs[key])
        self.extra_args.update(kwargs)


    #----------------------------------------------------------------------
    def set_last_line(self):
        cursor = self.textCursor()
        position = cursor.position()
        plain = self.toPlainText()
        index = plain.rfind("\n")
        if position < (index + len(START) + 1): self.moveCursor(QtGui.QTextCursor.End)


    #----------------------------------------------------------------------
    def no_overwrite_start(self):
        """No move the cursor over >>>"""
        cursor = self.textCursor()
        position = cursor.position()
        plain = self.toPlainText()
        index = plain.rfind("\n")
        if position > index:
            return (position - index) > len(START) + 1
        #if position < index: self.moveCursor(QtGui.QTextCursor.End)


    #----------------------------------------------------------------------
    def run_default_command(self, command):
        try:
            command = command.replace("\n", "")
            run = getattr(self, "command_"+command, None)
            if run: run()
        except:
            return False

        return bool(run)


    #----------------------------------------------------------------------
    def command_clear(self):
        self.clear()
        self.appendPlainText(START)


    #----------------------------------------------------------------------
    def command_restart(self):
        self.shell.restart()
        self.clear()
        self.appendPlainText(HEAD)
        self.appendPlainText(HELP)
        self.set_extra_args(**self.extra_args)
        self.appendPlainText(START)
