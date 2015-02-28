#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import sys

from PySide import QtCore, QtGui

from .python_highlighter import Highlighter
from ..methods.python_shell import PythonShell

HEAD = os.getenv("PINGUINO_NAME") + " " + os.getenv("PINGUINO_VERSION") + "\n" + "Python " + sys.version + " on " + sys.platform
HELP = QtGui.QApplication.translate("PythonShell", "Commands available:") + ' "clear", "restart"\n'

START = ">>> "
NEW_LINE = "... "


########################################################################
class PinguinoTerminal(QtGui.QPlainTextEdit):

    #----------------------------------------------------------------------
    def __init__(self, widget, checkbox):
        super(PinguinoTerminal, self).__init__(widget)

        self.appendPlainText(HEAD)
        self.appendPlainText(HELP)
        self.appendPlainText(START)

        self.extra_args = {}
        self.shell = PythonShell()

        self.historial = []
        self.index_historial = 0

        self.multiline_commands = []

        Highlighter(self.document(), [START, NEW_LINE])

        self.connect(self, QtCore.SIGNAL("textChanged(QString)"), self.textChanged)

        self.checkbox = checkbox

        self.setFrameShape(QtGui.QFrame.NoFrame)



    #----------------------------------------------------------------------
    def log_output(self, text):
        if text:
            self.moveCursor(QtGui.QTextCursor.End)
            self.insertPlainText(self.shell.run('print("""%s""")'%text.replace('"', "'")))
            self.insertPlainText(START)
            self.moveCursor(QtGui.QTextCursor.End)

        self.repaint()


    #----------------------------------------------------------------------
    def write(self, text):
        if not self.checkbox.isChecked() : return
        text = text[:-1]
        text = text.replace("\n", "\n[DEBUG] ")
        text = text.replace("\\n", "\\n[DEBUG] ")
        if text:
            self.moveCursor(QtGui.QTextCursor.StartOfLine)
            self.insertPlainText(self.shell.run('print("""%s""")'%text.replace('"', "'")))
            #self.insertPlainText(START)
            self.moveCursor(QtGui.QTextCursor.End)

        self.repaint()


    #----------------------------------------------------------------------
    def keyPressEvent(self, event):

        self.set_last_line()

        if event.key() in (QtCore.Qt.Key_Enter, QtCore.Qt.Key_Enter-1):
            self.moveCursor(QtGui.QTextCursor.End)
            self.index_historial = 0
            super(PinguinoTerminal, self).keyPressEvent(event)

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
                self.insertPlainText(self.shell.run(command))
            self.insertPlainText(START)
            self.moveCursor(QtGui.QTextCursor.End)

        elif event.key() == QtCore.Qt.Key_Backspace:
            if not self.get_command(): return
            else: super(PinguinoTerminal, self).keyPressEvent(event)

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
                super(PinguinoTerminal, self).keyPressEvent(event)

        else:
            super(PinguinoTerminal, self).keyPressEvent(event)


    #----------------------------------------------------------------------
    def get_command(self):
        plain = self.toPlainText()
        comand = plain[plain.rfind(START):]
        return comand[len(START):]


    #----------------------------------------------------------------------
    def wheelEvent(self, event):
        if event.modifiers() == QtCore.Qt.ControlModifier:
            self.step_font_size(event.delta())

        else: super(PinguinoTerminal, self).wheelEvent(event)


    ##----------------------------------------------------------------------
    #def (self):
        #""""""


    #----------------------------------------------------------------------
    def contextMenuEvent(self, event):
        menu = QtGui.QMenu()

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


        menu.addAction(QtGui.QApplication.translate("PythonShell", "Clear"), self.command_clear)
        menu.addAction(QtGui.QApplication.translate("PythonShell", "Restart"), self.command_restart)
        menu.addSeparator()
        menu.addAction(QtGui.QApplication.translate("PythonShell", "Cut"), self.cut, QtGui.QKeySequence.Cut)
        menu.addAction(QtGui.QApplication.translate("PythonShell", "Copy"), self.copy, QtGui.QKeySequence.Copy)
        menu.addAction(QtGui.QApplication.translate("PythonShell", "Paste"), self.paste, QtGui.QKeySequence.Paste)
        menu.addAction(QtGui.QApplication.translate("PythonShell", "Select all"), self.selectAll, QtGui.QKeySequence.SelectAll)
        menu.addSeparator()
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
            font-size: %dpt;
        }
        """%size)


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
