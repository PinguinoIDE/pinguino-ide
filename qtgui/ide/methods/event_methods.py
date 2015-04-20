#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os
import codecs
import webbrowser
import shutil
import logging

from math import ceil

from PySide import QtCore, QtGui

from ..custom_widgets import PinguinoCodeEditor
#from ..methods.syntax import Snippet
from ..methods.dialogs import Dialogs
from ..methods.decorators import Decorator
#from ..methods import constants as Constants

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from ..commons.intel_hex3 import IntelHex
else:
    #Python2
    from ..commons.intel_hex import IntelHex

from ..child_windows.about import About
from ..child_windows.libraries import LibManager
from ..child_windows.paths import Paths
from ..child_windows.hex_viewer import HexViewer
from ..child_windows.insert_block_dialog import InsertBlock
from ..child_windows.environ_viewer import EnvironViewer
from ..child_windows.submit_bug import SubmitBug
from ..child_windows.patches import Patches


########################################################################
class EventMethods(object):

    # Menu File

    #----------------------------------------------------------------------
    @Decorator.connect_features()
    def new_file(self, *args, **kwargs):
        path = kwargs.get("filename", self.__get_name__())
        filename = os.path.split(path)[1]
        editor = PinguinoCodeEditor()
        self.main.tabWidget_files.addTab(editor, filename)
        #editor.text_edit.insertPlainText(Snippet["file {snippet}"][1].replace("\t", ""))
        #editor.text_edit.insertPlainText("\n")
        #editor.text_edit.insertPlainText(Snippet["Bare minimum {snippet}"][1].replace("\t", ""))

        tc = editor.text_edit.textCursor()
        editor.text_edit.insert("file {snippet}")
        tc.movePosition(tc.End)
        tc.insertText("\n\n")
        editor.text_edit.setTextCursor(tc)
        editor.text_edit.insert("Bare minimum {snippet}")

        self.main.tabWidget_files.setCurrentWidget(editor)
        editor.text_edit.textChanged.connect(self.__text_changed__)
        editor.text_edit.undoAvailable.connect(self.__text_can_undo__)
        editor.text_edit.redoAvailable.connect(self.__text_can_redo__)
        editor.text_edit.copyAvailable.connect(self.__text_can_copy__)
        editor.text_edit.dropEvent = self.__drop__
        editor.text_edit.keyPressEvent = self.__key_press__
        editor.text_edit.contextMenuEvent = self.file_edit_context_menu
        editor.text_edit.setAcceptRichText(False)
        self.main.tabWidget_files.setTabText(self.main.tabWidget_files.currentIndex(), filename[:-1])
        editor.text_edit.setFocus()


    #----------------------------------------------------------------------
    def open_files(self):

        editor = self.main.tabWidget_files.currentWidget()
        path = getattr(editor, "path", None)
        if path: path = os.path.dirname(path)
        else: path = QtCore.QDir.home().path()
        filenames = Dialogs.set_open_file(self, path)

        for filename in filenames:
            if self.__check_duplicate_file__(filename): continue

            self.update_recents(filename)
            if filename.endswith(".gpde"):
                self.switch_ide_mode(True)
                self.PinguinoKIT.open_files(filename=filename)
                return
            elif filename.endswith(".pde"):
                self.switch_ide_mode(False)

            self.new_file(os.path.split(filename)[1])
            editor = self.main.tabWidget_files.currentWidget()
            pde_file = codecs.open(filename, "r", "utf-8")
            content = "".join(pde_file.readlines())
            pde_file.close()
            editor.text_edit.setPlainText(content)
            setattr(editor, "path", filename)
            setattr(editor, "last_saved", content)
            self.main.tabWidget_files.setTabToolTip(self.main.tabWidget_files.currentIndex(), filename)
            self.main.tabWidget_files.setTabText(self.main.tabWidget_files.currentIndex(), os.path.split(filename)[1])
            #self.update_recents(filename)
            self.check_backup_file(editor=editor)

        self.tab_changed()


    #----------------------------------------------------------------------
    @Decorator.connect_features()
    def save_file(self, *args, **kwargs):

        editor = kwargs.get("editor", None)
        if not editor: editor = self.get_tab().currentWidget()
        index = self.get_tab().indexOf(editor)
        filename = self.main.tabWidget_files.tabText(index)
        save_path = getattr(editor, "path", None)

        if not save_path:
            save_path, filename = Dialogs.set_save_file(self, filename)
            if not save_path: return False
            setattr(editor, "path", save_path)
            self.main.tabWidget_files.setTabText(index, filename)
            self.main.tabWidget_files.setTabToolTip(index, save_path)
            self.setWindowTitle(os.getenv("PINGUINO_FULLNAME")+" - "+save_path)

            self.update_recents(save_path)

        self.__save_file__(editor=editor)
        return True


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def close_file(self, *args, **kwargs):
        editor = kwargs.get("editor", None)
        if not editor: editor = self.get_tab().currentWidget()
        index = self.get_tab().indexOf(editor)
        filename = self.get_tab().tabText(index)
        save_path = getattr(editor, "path", None)

        if not save_path and filename.endswith("*"):
            reply = Dialogs.set_no_saved_file(self, filename)

            if reply == True:
                save_path, filename = Dialogs.set_save_file(self, filename)
                if not save_path: return
                setattr(editor, "path", save_path)
                self.__save_file__(editor=editor)

            elif reply == None: return

        elif filename.endswith("*"):
            reply = Dialogs.set_no_saved_file(self, filename)
            #print reply
            if reply == True: self.__save_file__(editor=editor)
            elif reply == None: return

        self.get_tab().removeTab(index)


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.connect_features()
    def save_as(self, *args, **kwargs):
        editor = kwargs.get("editor", None)
        if not editor: editor = self.get_tab().currentWidget()
        index = self.get_tab().indexOf(editor)
        #editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        filename = self.main.tabWidget_files.tabText(index)
        save_path = getattr(editor, "path", None)
        if save_path is None: save_path = filename

        save_path, filename = Dialogs.set_save_file(self, save_path)
        if not save_path: return False
        setattr(editor, "path", save_path)
        self.main.tabWidget_files.setTabText(index, filename)
        self.main.tabWidget_files.setTabToolTip(index, save_path)
        self.setWindowTitle(os.getenv("PINGUINO_FULLNAME")+" - "+save_path)

        self.__save_file__(editor=editor)
        return True


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def save_all(self):
        tab = self.get_tab()
        for index in range(tab.count()):
            self.save_file(editor=tab.widget(index))


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def close_all(self):
        tab = self.get_tab()
        widgets = map(tab.widget, range(tab.count()))
        for widget in widgets:
            self.close_file(editor=widget)

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def close_others(self):
        tab = self.get_tab()
        current = tab.currentWidget()
        widgets = map(tab.widget, range(tab.count()))
        for widget in widgets:
            if widget == current: continue
            self.close_file(editor=widget)

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def print_file(self):
        #Bug: no print to file, this is PySide bug.
        editor = self.get_tab().currentWidget()
        filename = self.get_tab().tabText(self.get_tab().currentIndex()).replace(".pde", ".pdf")
        QPrinter = QtGui.QPrinter
        printer = QPrinter(QPrinter.HighResolution)
        printer.setPageSize(QPrinter.Letter)
        printer.setOutputFileName(filename)
        printer.setDocName(filename)
        printer.setPageOrder(QPrinter.FirstPageFirst)
        printer.setOutputFormat(QPrinter.PdfFormat)
        preview = QtGui.QPrintDialog(printer)
        preview.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)
        if preview.exec_():
            document = editor.text_edit.document()
            document.print_(printer)


    #----------------------------------------------------------------------
    def __close_ide__(self, *args, **kwargs):

        self.configIDE.set("Main", "size", self.size().toTuple())
        self.configIDE.set("Main", "position", self.pos().toTuple())
        self.configIDE.set("Main", "maximized", self.isMaximized())
        #self.configIDE.set("Main", "terminal_height", self.main.dockWidget_output.height())

        # side = self.dockWidgetArea(self.main.dockWidget_right)
        # self.configIDE.set("Main", "dock_tools", side.name.decode())

        # side = self.dockWidgetArea(self.main.dockWidget_blocks)
        # self.configIDE.set("Main", "dock_blocks", side.name.decode())

        # side = self.dockWidgetArea(self.main.dockWidget_output)
        # self.configIDE.set("Main", "dock_shell", side.name.decode())

        self.configIDE.set("Main", "menubar", self.main.menubar.isVisible())

        count = 1
        self.configIDE.clear_recents()
        for file_ in self.recent_files:
            self.configIDE.set("Recents", "recent_"+str(count), file_)
            count += 1

        count = 1
        self.configIDE.clear_recents_open()
        for file_ in self.get_all_open_files():
            self.configIDE.set("Recents", "open_"+str(count), file_)
            count += 1

        self.configIDE.set("Features", "graphical", self.is_graphical())

        # self.configIDE.set("Features", "debug_in_output", self.main.checkBox_output_debug.isChecked())
        # self.configIDE.set("Features", "out_in_output", self.main.checkBox_output_messages.isChecked())

        self.configIDE.save_config()

        self.close()


    # Menu Edit

    #----------------------------------------------------------------------
    def undo(self):
        editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        editor.text_edit.undo()


    #----------------------------------------------------------------------
    def redo(self):
        editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        editor.text_edit.redo()


    #----------------------------------------------------------------------
    @Decorator.requiere_text_mode()
    @Decorator.requiere_open_files()
    def cut(self):
        editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        editor.text_edit.cut()


    #----------------------------------------------------------------------
    @Decorator.requiere_text_mode()
    @Decorator.requiere_open_files()
    def copy(self):
        editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        editor.text_edit.copy()


    #----------------------------------------------------------------------
    @Decorator.requiere_text_mode()
    @Decorator.requiere_open_files()
    def paste(self):
        editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        editor.text_edit.paste()

    #----------------------------------------------------------------------
    @Decorator.requiere_text_mode()
    @Decorator.requiere_open_files()
    def delete(self):
        editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        tc = editor.text_edit.textCursor()
        if tc.selectedText(): tc.removeSelectedText()

    #----------------------------------------------------------------------
    @Decorator.requiere_text_mode()
    @Decorator.requiere_open_files()
    def select_all(self):
        editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        editor.text_edit.selectAll()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def set_tab_search(self, mode):

        self.main.tabWidget_tools.setCurrentIndex(self.TAB_SEARCH)

        self.main.lineEdit_search.setFocus()
        editor = self.main.tabWidget_files.currentWidget()
        cursor = editor.text_edit.textCursor()
        self.main.lineEdit_search.setText(cursor.selectedText())

        replace = (mode == "replace")
        self.main.lineEdit_replace.setVisible(replace)
        self.main.label_replace.setVisible(replace)
        self.main.pushButton_replace.setVisible(replace)
        self.main.pushButton_replace_all.setVisible(replace)

    # Menu Source

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def commentregion(self):
        editor = self.main.tabWidget_files.currentWidget()
        comment_wildcard = "// "

        #cursor is a COPY all changes do not affect the QPlainTextEdit's cursor!!!
        cursor = editor.text_edit.textCursor()

        text = cursor.selectedText()

        if text == "":  #no selected, single line
            start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
            startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()
            endPosition = editor.text_edit.document().findBlockByLineNumber(start+1).position() - 1

            cursor.setPosition(startPosition)
            cursor.setPosition(endPosition, QtGui.QTextCursor.KeepAnchor)
            editor.text_edit.setTextCursor(cursor)

        else:
            start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
            startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()


            end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()

            endPosition = editor.text_edit.document().findBlockByLineNumber(end+1).position() - 1

            cursor.setPosition(startPosition)
            cursor.setPosition(endPosition, QtGui.QTextCursor.KeepAnchor)
            editor.text_edit.setTextCursor(cursor)


        cursor = editor.text_edit.textCursor()

        start_ = cursor.selectionStart()
        end_ = cursor.selectionEnd()

        #selectionEnd = cursor.selectionEnd()
        start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
        end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()
        startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()

        #init=(start, end)
        #Start a undo block
        cursor.beginEditBlock()

        #Move the COPY cursor
        cursor.setPosition(startPosition)
        #Move the QPlainTextEdit Cursor where the COPY cursor IS!
        editor.text_edit.setTextCursor(cursor)
        editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)

        for i in comment_wildcard:
            editor.text_edit.moveCursor(QtGui.QTextCursor.Right, QtGui.QTextCursor.KeepAnchor)

        start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()

        editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)
        s = editor.text_edit.cursor()
        s.pos()
        for i in xrange(start, end + 1):
            editor.text_edit.textCursor().insertText(comment_wildcard)
            #cursor.insertText(comment_wildcard)
            editor.text_edit.moveCursor(QtGui.QTextCursor.Down)
            editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)

        editor.text_edit.moveCursor(QtGui.QTextCursor.EndOfLine)

        end_ += (end + 1 - start) * len(comment_wildcard)
        cursor.setPosition(start_)
        cursor.setPosition(end_, QtGui.QTextCursor.KeepAnchor)
        editor.text_edit.setTextCursor(cursor)

        #End a undo block
        cursor.endEditBlock()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def uncommentregion(self):

        editor = self.main.tabWidget_files.currentWidget()
        comment_wildcard = "// "

        #cursor is a COPY all changes do not affect the QPlainTextEdit's cursor!!!
        cursor = editor.text_edit.textCursor()

        start_ = cursor.selectionStart()
        end_ = cursor.selectionEnd()

        start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
        end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()
        startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()

        #Start a undo block
        cursor.beginEditBlock()

        #Move the COPY cursor
        cursor.setPosition(startPosition)
        #Move the QPlainTextEdit Cursor where the COPY cursor IS!
        editor.text_edit.setTextCursor(cursor)
        editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)
        for i in xrange(start, end + 1):

            for i in comment_wildcard:
                editor.text_edit.moveCursor(QtGui.QTextCursor.Right, QtGui.QTextCursor.KeepAnchor)

            text = editor.text_edit.textCursor().selectedText()
            if text == comment_wildcard:
                editor.text_edit.textCursor().removeSelectedText()
            elif u'\u2029' in text:
                #\u2029 is the unicode char for \n
                #if there is a newline, rollback the selection made above.
                editor.text_edit.moveCursor(QtGui.QTextCursor.Left, QtGui.QTextCursor.KeepAnchor)

            editor.text_edit.moveCursor(QtGui.QTextCursor.Down)
            editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)

        end_ -= (end + 1 - start) * len(comment_wildcard)
        cursor.setPosition(start_)
        cursor.setPosition(end_, QtGui.QTextCursor.KeepAnchor)
        editor.text_edit.setTextCursor(cursor)

        #End a undo block
        cursor.endEditBlock()



    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def comment_uncomment(self):
        editor, cursor, prevCursor, selected, firstLine = self.select_block_edit()

        if firstLine != False:
            if firstLine.startswith("//"): self.uncommentregion()
            else: self.commentregion()

        if not selected:
            cursor.clearSelection()
            editor.text_edit.setTextCursor(prevCursor)


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def indentregion(self):
        editor, cursor, prevCursor, selected, firstLine = self.select_block_edit()

        if firstLine != False:

            editor = self.main.tabWidget_files.currentWidget()
            comment_wildcard = " " * 4

            #cursor is a COPY all changes do not affect the QPlainTextEdit's cursor!!!
            cursor = editor.text_edit.textCursor()

            text = cursor.selectedText()

            if text == "":  #no selected, single line
                start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
                startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()
                endPosition = editor.text_edit.document().findBlockByLineNumber(start+1).position() - 1

                cursor.setPosition(startPosition)
                cursor.setPosition(endPosition, QtGui.QTextCursor.KeepAnchor)
                editor.text_edit.setTextCursor(cursor)

            else:
                start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
                startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()


                end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()

                endPosition = editor.text_edit.document().findBlockByLineNumber(end+1).position() - 1

                cursor.setPosition(startPosition)
                cursor.setPosition(endPosition, QtGui.QTextCursor.KeepAnchor)
                editor.text_edit.setTextCursor(cursor)


            cursor = editor.text_edit.textCursor()

            start_ = cursor.selectionStart()
            end_ = cursor.selectionEnd()

            #selectionEnd = cursor.selectionEnd()
            start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
            end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()
            startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()

            #init=(start, end)
            #Start a undo block
            cursor.beginEditBlock()

            #Move the COPY cursor
            cursor.setPosition(startPosition)
            #Move the QPlainTextEdit Cursor where the COPY cursor IS!
            editor.text_edit.setTextCursor(cursor)
            editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)

            for i in comment_wildcard:
                editor.text_edit.moveCursor(QtGui.QTextCursor.Right, QtGui.QTextCursor.KeepAnchor)

            start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()

            editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)
            s = editor.text_edit.cursor()
            s.pos()
            for i in xrange(start, end + 1):
                editor.text_edit.textCursor().insertText(comment_wildcard)
                #cursor.insertText(comment_wildcard)
                editor.text_edit.moveCursor(QtGui.QTextCursor.Down)
                editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)

            editor.text_edit.moveCursor(QtGui.QTextCursor.EndOfLine)

            end_ += (end + 1 - start) * len(comment_wildcard)
            cursor.setPosition(start_)
            cursor.setPosition(end_, QtGui.QTextCursor.KeepAnchor)
            editor.text_edit.setTextCursor(cursor)

            #End a undo block
            cursor.endEditBlock()


        if not selected:
            cursor.clearSelection()
            editor.text_edit.setTextCursor(prevCursor)



    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def dedentregion(self):
        editor, cursor, prevCursor, selected, firstLine = self.select_block_edit()

        if firstLine != False and firstLine.startswith(" "*4):

            editor = self.main.tabWidget_files.currentWidget()
            comment_wildcard = " " * 4

            #cursor is a COPY all changes do not affect the QPlainTextEdit's cursor!!!
            cursor = editor.text_edit.textCursor()

            start_ = cursor.selectionStart()
            end_ = cursor.selectionEnd()

            start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
            end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()
            startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()

            #Start a undo block
            cursor.beginEditBlock()

            #Move the COPY cursor
            cursor.setPosition(startPosition)
            #Move the QPlainTextEdit Cursor where the COPY cursor IS!
            editor.text_edit.setTextCursor(cursor)
            editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)
            for i in xrange(start, end + 1):

                for i in comment_wildcard:
                    editor.text_edit.moveCursor(QtGui.QTextCursor.Right, QtGui.QTextCursor.KeepAnchor)

                text = editor.text_edit.textCursor().selectedText()
                if text == comment_wildcard:
                    editor.text_edit.textCursor().removeSelectedText()
                elif u'\u2029' in text:
                    #\u2029 is the unicode char for \n
                    #if there is a newline, rollback the selection made above.
                    editor.text_edit.moveCursor(QtGui.QTextCursor.Left, QtGui.QTextCursor.KeepAnchor)

                editor.text_edit.moveCursor(QtGui.QTextCursor.Down)
                editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)

            end_ -= (end + 1 - start) * len(comment_wildcard)
            cursor.setPosition(start_)
            cursor.setPosition(end_, QtGui.QTextCursor.KeepAnchor)
            editor.text_edit.setTextCursor(cursor)

            #End a undo block
            cursor.endEditBlock()

        if not selected:
            cursor.clearSelection()
            editor.text_edit.setTextCursor(prevCursor)


    # Pinguino

    #----------------------------------------------------------------------
    def __show_libmanager__(self):
        self.frame_stdout = LibManager(self)
        self.frame_stdout.show()


    #----------------------------------------------------------------------
    def __config_paths__(self):
        self.frame_paths = Paths(self)
        self.frame_paths.show()


    #----------------------------------------------------------------------
    @Decorator.show_tab("tab_boardconfig")
    def __show_board_config__(self):
        pass


    #----------------------------------------------------------------------
    def __show_submit_bug__(self):
        self.submit_bug = SubmitBug(self)
        self.submit_bug.show()


    #----------------------------------------------------------------------
    def __show_patches__(self):
        self.patches = Patches(self)

        if not self.patches.get_patches():
            Dialogs.info_message(self, "There are no new updates available.\n %s is up to date" % os.getenv("PINGUINO_FULLNAME"))
            self.patches.close()
        else:
            self.patches.show()


    #----------------------------------------------------------------------
    def __show_hex_code__(self):
        if getattr(self.get_tab().currentWidget(), "path", False):
            hex_filename = self.get_tab().currentWidget().path.replace(".gpde", ".pde").replace(".pde", ".hex")
        else:
            Dialogs.error_message(self, QtGui.QApplication.translate("Dialogs", "You must compile before."))
            return
        if os.path.isfile(hex_filename):

            hex_obj = IntelHex(open(hex_filename, "r"))
            hex_dict = hex_obj.todict()
            rows = int(ceil(max(hex_dict.keys()) / float(0x18)))

            if rows < 1e3:
                self.frame_hex_viewer = HexViewer(self, hex_obj, hex_filename)
                self.frame_hex_viewer.show()
            else:
                file_ = codecs.open(hex_filename, "r", "utf-8")
                content = file_.readlines()
                file_.close
                self.frame_hex_plain = PlainOut(hex_filename, "".join(content), highlight=True)
                self.frame_hex_plain.show()

        else:
            Dialogs.error_message(self, QtGui.QApplication.translate("Dialogs", "You must compile before."))


    #----------------------------------------------------------------------
    @Decorator.show_tab("tab_stdout")
    def __show_stdout__(self):
        pass
        # self.frame_stdout = PlainOut("Stdout")
        # self.frame_stdout.show()


    #----------------------------------------------------------------------
    def __show_environ__(self, debug):
        self.frame_environ = EnvironViewer(self, debug)
        self.frame_environ.show()


    #----------------------------------------------------------------------
    def __show_main_c__(self):

        source = os.path.join(os.getenv("PINGUINO_USER_PATH"), "source")
        board = self.configIDE.config("Board", "arch", 8)
        if board == 32: extra = "32"
        else: extra = ""
        filename = os.path.join(source, "main%s.c"%extra)
        self.open_file_from_path(filename=filename, readonly=True)


    #----------------------------------------------------------------------
    def __show_define_h__(self):

        filename = os.path.join(os.getenv("PINGUINO_USER_PATH"), "source", "define.h")
        self.open_file_from_path(filename=filename, readonly=True)

    #----------------------------------------------------------------------
    def __show_user_c__(self):

        filename = os.path.join(os.getenv("PINGUINO_USER_PATH"), "source", "user.c")
        self.open_file_from_path(filename=filename, readonly=True)


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_file_saved()
    @Decorator.requiere_can_compile()
    def pinguino_compile(self, dialog_upload=True):

        filename = self.get_tab().currentWidget().path

        if not self.is_graphical():
            compile_code = lambda :self.pinguinoAPI.compile_file(filename)

        else:
            compile_code = lambda :self.pinguinoAPI.compile_string(self.PinguinoKIT.get_pinguino_source_code())


        # self.set_board()
        # reply = Dialogs.confirm_board(self)

        # if reply == False:
            # self.__show_board_config__()
            # return False
        # elif reply == None:
            # return False

        self.write_log(QtGui.QApplication.translate("Frame", "Compiling: %s")%filename)
        self.write_log(self.get_description_board())
        # self.write_log("")

        compile_code()
        self.update_stdout()
        self.post_compile(dialog_upload)

    #----------------------------------------------------------------------
    def post_compile(self, dialog_upload=True):

        self.main.actionUpload.setEnabled(self.pinguinoAPI.compiled())
        if not self.pinguinoAPI.compiled():

            errors_preprocess = self.pinguinoAPI.get_errors_preprocess()
            if errors_preprocess:
                for error in errors_preprocess["preprocess"]:
                    self.write_log("ERROR: "+error)

            errors_c = self.pinguinoAPI.get_errors_compiling_c()
            if errors_c:
                self.write_log("ERROR: "+errors_c["complete_message"])
                line_errors = errors_c["line_numbers"]
                for line_error in line_errors:
                    self.highligh_line(line_error, "#ff7f7f")

            errors_asm = self.pinguinoAPI.get_errors_compiling_asm()
            if errors_asm:
                for error in errors_asm["error_symbols"]:
                    self.write_log("ERROR: "+error)

            errors_linking = self.pinguinoAPI.get_errors_linking()
            if errors_linking:
                for error in errors_linking["linking"]:
                    self.write_log("ERROR: "+error)

                line_errors_l = errors_linking["line_numbers"]
                for line_error in line_errors_l:
                    self.highligh_line(line_error, "#ff7f7f")


            if errors_asm or errors_c:
                Dialogs.error_while_compiling(self)
                self.__show_stdout__()
            elif errors_linking:
                Dialogs.error_while_linking(self)
                self.__show_stdout__()
            elif errors_preprocess:
                Dialogs.error_while_preprocess(self)
                self.__show_stdout__()

            else:
                Dialogs.error_while_unknow(self)
                self.__show_stdout__()

        else:
            result = self.pinguinoAPI.get_result()
            self.write_log(QtGui.QApplication.translate("Frame", "compilation done"))
            self.write_log(result["code_size"])
            self.write_log(QtGui.QApplication.translate("Frame", "%s seconds process time")%result["time"])

            if dialog_upload:
                if Dialogs.compilation_done(self):
                    self.pinguino_upload()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def pinguino_upload(self):
        uploaded, result = self.pinguinoAPI.upload()
        self.write_log(result)
        if uploaded:
            Dialogs.upload_done(self)
        elif Dialogs.upload_fail(self, result):
            self.pinguino_upload()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_file_saved()
    def pinguino_compile_and_upload(self):
        self.pinguino_compile(dialog_upload=False)
        if self.pinguinoAPI.compiled():
            self.pinguino_upload()


    #----------------------------------------------------------------------
    def pinguino_upload_hex(self):

        Dialogs.warning_message(self, "Be careful with this feature, ensure that .hex file it is correct.")

        path = QtCore.QDir.home().path()
        filename = Dialogs.set_open_hex(self, path)

        if not filename: return

        self.set_board()
        reply = Dialogs.confirm_board(self)

        if reply == False:
            self.__show_board_config__()
            return False
        elif reply == None:
            return False

        board = self.pinguinoAPI.get_board()
        reply = Dialogs.confirm_message(self, "Do you want upload '%s' to %s"%(filename, board.name))

        if reply:
            self.pinguinoAPI.__hex_file__ = filename
            self.pinguino_upload()







    # Graphical

    #----------------------------------------------------------------------
    def __show_pinguino_code__(self):
        name = getattr(self.get_tab().currentWidget(), "path", "")
        if name: name = " - " + name
        self.frame_pinguino_code = PlainOut(QtGui.QApplication.translate("Dialogs", "Pinguino code"))
        self.frame_pinguino_code.show_text(self.PinguinoKIT.get_pinguino_source_code(), pde=True)
        self.frame_pinguino_code.show()

    #----------------------------------------------------------------------
    def __export_pinguino_code__(self):
        area = self.PinguinoKIT.get_work_area()
        area.export_code_to_pinguino_editor()

    #----------------------------------------------------------------------
    @Decorator.requiere_graphical_mode()
    @Decorator.requiere_open_files()
    def __insert_block__(self):
        self.frame_insert_block = InsertBlock(self.PinguinoKIT)
        self.frame_insert_block.show()



    # Options

    #----------------------------------------------------------------------
    def switch_autocomplete(self):
        enable = self.main.actionAutocomplete.isChecked()
        self.configIDE.set("Features", "autocomplete", enable)
        self.configIDE.save_config()


    #----------------------------------------------------------------------
    def switch_color_theme(self, pinguino_color=True):
        default_pallete = ["toolBar_edit", "toolBar_files", "toolBar_search_replace",
                           "toolBar_undo_redo", "toolBar_pinguino", "toolBar_pinguino",
                           "toolBar_graphical", "toolBar_switch", "toolBar_system",
                           "menubar", "statusBar"]

        pinguino_pallete = ["dockWidget_bottom", "dockWidget_right"]

        if pinguino_color:
            self.PinguinoPallete.set_background_pinguino(self.main.centralwidget.parent())
            for element in pinguino_pallete:
                self.PinguinoPallete.set_background_pinguino(getattr(self.main, element))
            for element in default_pallete:
                self.PinguinoPallete.set_default_palette(getattr(self.main, element))
            self.main.label_logo.setPixmap(QtGui.QPixmap(":/logo/art/banner.png"))
        else:
            self.PinguinoPallete.set_default_palette(self.main.centralwidget.parent())
            for element in default_pallete + pinguino_pallete:
                self.PinguinoPallete.set_default_palette(getattr(self.main, element))
            self.main.label_logo.setPixmap(QtGui.QPixmap(":/logo/art/banner_blue.png"))

        self.configIDE.set("Main", "color_theme", pinguino_color)
        self.main.actionColor_theme.setChecked(pinguino_color)


    #----------------------------------------------------------------------
    def switch_confirm_board(self, event=None):
        enable = self.main.actionConfirm_board.isChecked()
        self.configIDE.set("Features", "confirm_board", enable)
        self.configIDE.save_config()


    # Help

    #----------------------------------------------------------------------
    def open_web_site(self, url):
        webbrowser.open_new_tab(url)


    #----------------------------------------------------------------------
    def __show_about__(self):
        self.frame_about = About()
        self.frame_about.show()

    # Tools Files



    # Tools Source


    # Tools Search
    # see search_replace.py


    # Widgets

    #----------------------------------------------------------------------
    @Decorator.update_toolbar()
    @Decorator.connect_features()
    def tab_changed(self, *args, **kwargs):
        self.main.tabWidget_files.setVisible(self.main.tabWidget_files.count() > 0)
        self.main.frame_logo.setVisible(not self.main.tabWidget_files.count() > 0)
        self.main.actionClose_file.setEnabled(self.main.tabWidget_files.count() > 0)

        editor = self.main.tabWidget_files.currentWidget()
        if getattr(editor, "path", None): self.setWindowTitle(os.getenv("PINGUINO_FULLNAME")+" - "+editor.path)
        else: self.setWindowTitle(os.getenv("PINGUINO_FULLNAME"))

        index = self.main.tabWidget_files.currentIndex()
        filename = self.main.tabWidget_files.tabText(index)
        if filename.endswith("*"): self.main.actionSave_file.setEnabled(True)
        else: self.main.actionSave_file.setDisabled(True)

        # self.__update_current_dir_on_files__()


    #----------------------------------------------------------------------
    def tab_close(self, index):
        editor = self.get_tab().widget(index)
        self.close_file(editor=editor)


    # Graphical Tool Bar

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def save_screen_image(self):
        editor = self.get_tab().currentWidget()
        scroll_area = editor.scroll_area
        image = QtGui.QPixmap.grabWidget(scroll_area,
                                         QtCore.QRect(0, 0,
                                                      scroll_area.width()-13,
                                                      scroll_area.height()-13))

        filename = self.get_tab().tabText(self.get_tab().currentIndex())
        filename = os.path.splitext(filename)[0] + ".png"
        filename = Dialogs.set_save_image(self, filename)
        if filename: image.save(filename, "png")


    #----------------------------------------------------------------------
    def dialog_rename_file(self):
        """"""
        editor = self.get_tab().currentWidget()
        new_name = Dialogs.get_text(self, "Rename file", os.path.basename(editor.path))
        self.rename_file(editor, new_name)




    #----------------------------------------------------------------------
    def rename_file(self, editor, new_name):
        """"""
        filename = os.path.basename(editor.path)
        logging.debug("Renamed {} for {}".format(filename, new_name) )



    #----------------------------------------------------------------------
    def switch_ide_mode(self, graphical):
        self.main.actionSwitch_ide.setChecked(graphical)
        self.main.tabWidget_graphical.setVisible(graphical and self.main.tabWidget_graphical.count() > 0)
        self.main.tabWidget_files.setVisible(not graphical and self.main.tabWidget_files.count() > 0)


        menu = self.toolbutton_menutoolbar.menu()
        if graphical:
            self.update_actions_for_graphical()
            menu.insertMenu(self.main.menuHelp.menuAction(), self.main.menuGraphical)
        else:
            self.update_actions_for_text()
            menu.removeAction(self.main.menuGraphical.menuAction())

        self.tab_changed()



    # Events


    #----------------------------------------------------------------------
    def __key_press__(self, event):
        editor = self.main.tabWidget_files.currentWidget()
        if self.is_autocomplete_enable():
            editor.text_edit.__keyPressEvent__(event)
        else:
            editor.text_edit.force_keyPressEvent(event)


    #----------------------------------------------------------------------
    def __drop__(self, event):
        mine = event.mimeData()
        if mine.hasUrls():
            for path in mine.urls():
                self.open_file_from_path(filename=path.path())


    #----------------------------------------------------------------------
    def tab_files_context_menu(self, event):
        menu = QtGui.QMenu()

        menu.addAction("Rename", self.dialog_rename_file)
        menu.addAction(self.main.actionSave_file)
        menu.addAction(self.main.actionSave_as)
        menu.addAction(self.main.actionSave_all)
        menu.addSeparator()
        menu.addAction(self.main.actionClose_file)
        menu.addAction(self.main.actionClose_all)
        menu.addAction(self.main.actionClose_others)

        menu.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)

        menu.exec_(event.globalPos())


    #----------------------------------------------------------------------
    def file_edit_context_menu(self, event):
        menu = QtGui.QMenu()

        editor = self.main.tabWidget_files.currentWidget()
        filename = getattr(editor, "path", False)
        if filename and (filename.startswith(os.path.join(os.getenv("PINGUINO_USER_PATH"), "examples")) or filename.startswith(os.path.join(os.getenv("PINGUINO_USER_PATH"), "graphical_examples"))):
            menu.addAction(QtGui.QApplication.translate("Frame", "Restore example"), self.restore_example)
            menu.addSeparator()

        menu.addAction(self.main.actionUndo)
        menu.addAction(self.main.actionRedo)
        menu.addSeparator()
        menu.addAction(self.main.actionCut)
        menu.addAction(self.main.actionCopy)
        menu.addAction(self.main.actionPaste)
        menu.addAction(self.main.actionDelete)
        menu.addSeparator()
        menu.addAction(self.main.actionSelect_all)
        menu.addSeparator()
        menu.addAction(self.main.actionComment_out_region)
        menu.addAction(self.main.actionComment_Uncomment_region)
        menu.addSeparator()
        menu.addAction(self.main.actionIndent)
        menu.addAction(self.main.actionDedent)
        menu.addSeparator()
        menu.addAction(self.main.actionCompile)
        menu.addAction(self.main.actionUpload)
        menu.addAction(self.main.actionIf_Compile_then_Upload)
        menu.addSeparator()
        menu.addAction(self.main.actionWiki_docs)
        menu.addAction(self.main.actionLibrary_manager)
        menu.addAction(self.main.actionHex_code)
        menu.addAction(self.main.actionSet_paths)
        menu.addAction(self.main.actionStdout)
        menu.addSeparator()
        menu.addAction(self.main.actionAutocomplete)

        menu.setStyleSheet("""
        QMenu {
            font-family: inherit;
            font-weight: normal;
            }

        """)

        menu.exec_(event.globalPos())


    #----------------------------------------------------------------------
    def restore_example(self):
        editor = self.main.tabWidget_files.currentWidget()
        filename = getattr(editor, "path", False)
        filename_install = filename.replace(os.getenv("PINGUINO_USER_PATH"), os.getenv("PINGUINO_INSTALL_PATH"))
        shutil.copyfile(filename_install, filename)
        self.reload_file()


    #----------------------------------------------------------------------
    def update_mode_output(self, visible):
        if self.is_graphical():
            self.configIDE.set("Features", "terminal_on_graphical", visible)
        else:
            self.configIDE.set("Features", "terminal_on_text", visible)


    # #----------------------------------------------------------------------
    # def toggle_pythonshell(self, visible):
        # self.main.dockWidget_output.setVisible(visible)
        # self.update_mode_output(visible)
        # #self.configIDE.config("Features", "terminal_on_text", visible)


    # #----------------------------------------------------------------------
    # def update_tab_position(self, tab, area):

        # if area.name == "RightDockWidgetArea":
            # tab.setTabPosition(QtGui.QTabWidget.West)
        # elif area.name == "LeftDockWidgetArea":
            # tab.setTabPosition(QtGui.QTabWidget.East)
