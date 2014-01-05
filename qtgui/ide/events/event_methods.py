#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import codecs
import webbrowser

from PySide import QtCore, QtGui

from ..code_editor.pinguino_code_editor import PinguinoCodeEditor
from ..helpers.syntax import Snippet
from ..helpers.dialogs import Dialogs
from ..helpers.decorators import Decorator
from ..helpers import constants as Constants
#from ..tools.code_navigator import CodeNavigator
from ..tools.files import Files
from ..tools.search_replace import SearchReplace
from ..child_windows.about import About
from ..child_windows.board_config import BoardConfig
from ..child_windows.stdout import Stdout
from ..child_windows.libraries import LibManager
from ..child_windows.hex_viewer import HexViewer
from ..widgets.wiki_widget import WikiDock


########################################################################
class EventMethods(SearchReplace):
    """"""
    
    #----------------------------------------------------------------------
    @Decorator.connect_features()
    def new_file(self, *args, **kwargs):
        """"""
        path = kwargs.get("filename", self.__get_name__())
        filename = os.path.split(path)[1]         

        editor = PinguinoCodeEditor()
        self.main.tabWidget_files.addTab(editor, filename)
        editor.text_edit.insertPlainText(Snippet["file {snippet}"][1].replace("\t", ""))
        editor.text_edit.insertPlainText("\n")
        editor.text_edit.insertPlainText(Snippet["Bare minimum {snippet}"][1].replace("\t", ""))
        self.main.tabWidget_files.setCurrentWidget(editor)
        editor.text_edit.textChanged.connect(self.__text_changed__)
        editor.text_edit.undoAvailable.connect(self.__text_can_undo__)
        editor.text_edit.redoAvailable.connect(self.__text_can_redo__)
        editor.text_edit.copyAvailable.connect(self.__text_can_copy__)
        editor.text_edit.dropEvent = self.__drop__
        editor.text_edit.keyPressEvent = self.__key_pess__
        
        self.main.tabWidget_files.setTabText(self.main.tabWidget_files.currentIndex(), filename[:-1])
        
        
    #----------------------------------------------------------------------
    def __key_pess__(self, event):
        """"""
        editor = self.main.tabWidget_files.currentWidget()
        if self.is_autocomplete_enable():
            editor.text_edit.__keyPressEvent__(event)
        else:
            editor.text_edit.force_keyPressEvent(event)
        
        
    #----------------------------------------------------------------------
    #@Decorator.connect_features()
    def open_files(self):
        """"""
        filenames = Dialogs.set_open_file(self)
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
            #pde_file = file(filename, mode="r")
            pde_file = codecs.open(filename, "r", "utf-8")
            content = "".join(pde_file.readlines())
            pde_file.close()
            editor.text_edit.setPlainText(content)
            #self.get_tab()
            setattr(editor, "path", filename)
            self.main.tabWidget_files.setTabToolTip(self.main.tabWidget_files.currentIndex(), filename)
            self.main.tabWidget_files.setTabText(self.main.tabWidget_files.currentIndex(), os.path.split(filename)[1]) 
            self.update_recents(filename)
            
        self.tab_changed()
        
    #----------------------------------------------------------------------
    #@Decorator.connect_features()
    def open_file_from_path(self, *args, **kwargs):
        """"""
        filename = kwargs["filename"]
        if self.__check_duplicate_file__(filename): return

        self.update_recents(filename)

        if filename.endswith(".gpde"):
            self.switch_ide_mode(True)
            self.PinguinoKIT.open_file_from_path(filename=filename)
            return
        elif filename.endswith(".pde"):
            self.switch_ide_mode(False)
        
        self.new_file(filename=filename)
        editor = self.main.tabWidget_files.currentWidget()
        #pde_file = file(path, mode="r")
        pde_file = codecs.open(filename, "r", "utf-8")
        content = "".join(pde_file.readlines())
        pde_file.close()
        editor.text_edit.setPlainText(content)
        setattr(editor, "path", filename)
        self.main.tabWidget_files.setTabToolTip(self.main.tabWidget_files.currentIndex(), filename)
        self.main.tabWidget_files.setTabText(self.main.tabWidget_files.currentIndex(), os.path.split(filename)[1])       
        self.tab_changed()
        
        
    #----------------------------------------------------------------------
    @Decorator.call_later()
    def open_last_files(self):
        """"""
        opens = self.configIDE.get_recents_open()
        for file_ in opens:
            if os.path.exists(file_):
                self.open_file_from_path(filename=file_)
            
        self.main.actionSwitch_ide.setChecked(self.configIDE.config("Features", "graphical", False))
        self.switch_ide_mode(self.configIDE.config("Features", "graphical", False))        
        
        
        
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def comment_uncomment(self):
        """"""
        editor = self.main.tabWidget_files.currentWidget()
        cursor = editor.text_edit.textCursor()
        #prevCursor = editor.text_edit.textCursor()
        
        text = cursor.selectedText()
        lines = text.split(u'\u2029')
        firstLine = False
        for line in lines:
            if not str(line).isspace() and not str(line)=="":
                firstLine = line
                break

        if firstLine != False:
            if firstLine.startswith("//"): self.uncommentregion()
            else: self.commentregion()
            
            
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def commentregion(self):
        """"""     

        editor = self.main.tabWidget_files.currentWidget()
        comment_wildcard = "// "
        
        #cursor is a COPY all changes do not affect the QPlainTextEdit's cursor!!!
        cursor = editor.text_edit.textCursor()
        
        start_ = cursor.selectionStart()
        end_ = cursor.selectionEnd()        
                
        selectionEnd = cursor.selectionEnd()
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
        #editor.tex
        
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
        
        
        end_ += (end + 1 - start) * 3
        cursor.setPosition(start_)
        cursor.setPosition(end_, QtGui.QTextCursor.KeepAnchor)
        editor.text_edit.setTextCursor(cursor)        
        
        
        
        #End a undo block
        cursor.endEditBlock()
        
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def uncommentregion(self):
        """"""
        
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
            
        end_ -= (end + 1 - start) * 3
        cursor.setPosition(start_)
        cursor.setPosition(end_, QtGui.QTextCursor.KeepAnchor)
        editor.text_edit.setTextCursor(cursor)              
    
        #End a undo block
        cursor.endEditBlock()         
        

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def close_file(self, *args, **kwargs):
        """"""
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
                self.__save_file__(editor)
                
            elif reply == None: return
            
            
        elif filename.endswith("*"):
            reply = Dialogs.set_no_saved_file(self, filename)
            #print reply
            if reply == True: self.__save_file__(editor)
            elif reply == None: return            
            
        
        self.get_tab().removeTab(index)
                
            
            
    #----------------------------------------------------------------------
    @Decorator.connect_features()
    def save_file(self, *args, **kwargs):
        """"""
        editor = kwargs.get("editor", None)
        if not editor: editor = self.get_tab().currentWidget()
        index = self.get_tab().indexOf(editor)
        #editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        filename = self.main.tabWidget_files.tabText(index)
        save_path = getattr(editor, "path", None)
        
        if not save_path:
            save_path, filename = Dialogs.set_save_file(self, filename)
            if not save_path: return False
            setattr(editor, "path", save_path)
            self.main.tabWidget_files.setTabText(index, filename)
            self.main.tabWidget_files.setTabToolTip(index, save_path)    
            self.setWindowTitle(Constants.TAB_NAME+" - "+save_path)
            
            self.update_recents(save_path)            
        
        self.__save_file__(editor=editor)
        return True
        
        
            
            
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.connect_features()
    def save_as(self, *args, **kwargs):
        """"""
        editor = kwargs.get("editor", None)
        if not editor: editor = self.get_tab().currentWidget()
        index = self.get_tab().indexOf(editor)
        #editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        filename = self.main.tabWidget_files.tabText(index)
        save_path = getattr(editor, "path", None)
        
        save_path, filename = Dialogs.set_save_file(self, filename)
        if not save_path: return False
        setattr(editor, "path", save_path)
        self.main.tabWidget_files.setTabText(index, filename)
        self.main.tabWidget_files.setTabToolTip(index, save_path) 
        self.setWindowTitle(Constants.TAB_NAME+" - "+save_path)
        
        self.__save_file__(editor=editor)
        return True
        
    #----------------------------------------------------------------------
    def undo(self):
        """"""
        editor = self.main.tabWidget_files.currentWidget()
        index = self.main.tabWidget_files.currentIndex()
        editor.text_edit.undo()
        
    #----------------------------------------------------------------------
    def redo(self):
        """"""
        editor = self.main.tabWidget_files.currentWidget()
        index = self.main.tabWidget_files.currentIndex()
        editor.text_edit.redo()
        
    #----------------------------------------------------------------------
    def cut(self):
        """"""
        editor = self.main.tabWidget_files.currentWidget()
        index = self.main.tabWidget_files.currentIndex()
        editor.text_edit.cut()
        
    #----------------------------------------------------------------------
    def copy(self):
        """"""
        editor = self.main.tabWidget_files.currentWidget()
        index = self.main.tabWidget_files.currentIndex()
        editor.text_edit.copy()
        
    #----------------------------------------------------------------------
    def paste(self):
        """"""
        editor = self.main.tabWidget_files.currentWidget()
        index = self.main.tabWidget_files.currentIndex()
        editor.text_edit.paste()
        
        
    #----------------------------------------------------------------------
    @Decorator.update_toolbar()
    @Decorator.connect_features()
    def tab_changed(self, *args, **kwargs):
        """"""
        self.main.tabWidget_files.setVisible(self.main.tabWidget_files.count() > 0)
        self.main.frame_logo.setVisible(not self.main.tabWidget_files.count() > 0)
        self.main.actionClose_file.setEnabled(self.main.tabWidget_files.count() > 0)
            
        editor = self.main.tabWidget_files.currentWidget()
        if getattr(editor, "path", None): self.setWindowTitle(Constants.TAB_NAME+" - "+editor.path)
        else: self.setWindowTitle(Constants.TAB_NAME)
        
        index = self.main.tabWidget_files.currentIndex()
        filename = self.main.tabWidget_files.tabText(index)
        if filename.endswith("*"): self.main.actionSave_file.setEnabled(True)
        else: self.main.actionSave_file.setDisabled(True)
        
        #self.update_tool_bar(editor)
        self.__update_current_dir_on_files__()
        #self.__update_menu_view_hex_code__()
        
        
    #----------------------------------------------------------------------
    def tab_close(self, index):
        """"""
        editor = self.get_tab().widget(index)
        self.close_file(editor=editor)
        
    #----------------------------------------------------------------------
    def jump_to_line(self, line):
        """"""
        self.highligh_line(line,  "#DBFFE3")
        
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def highligh_line(self, line=None, color="#ff0000", text_cursor=None):
        """"""
        editor = self.main.tabWidget_files.currentWidget()
        
        if line:
            content = editor.text_edit.toPlainText()
            content = content.split("\n")[:line]
            position = len("\n".join(content))
            text_cur = editor.text_edit.textCursor()
            text_cur.setPosition(position)
            text_cur.clearSelection()
            editor.text_edit.setTextCursor(text_cur)
        else:
            text_cur = editor.text_edit.textCursor()
            text_doc = editor.text_edit.document()
            text_cur.clearSelection()
            editor.text_edit.setDocument(text_doc)
            editor.text_edit.setTextCursor(text_cur)            
            
        
        selection = QtGui.QTextEdit.ExtraSelection()
        selection.format.setBackground(QtGui.QColor(color))
        selection.format.setProperty(QtGui.QTextFormat.FullWidthSelection, True)
        selection.cursor = editor.text_edit.textCursor()
        editor.text_edit.setExtraSelections([selection])
        selection.cursor.clearSelection()
        
        if text_cursor: editor.text_edit.setTextCursor(text_cursor)
        
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def clear_highlighs(self):
        """"""
        editor = self.main.tabWidget_files.currentWidget()
        editor.text_edit.setExtraSelections([])
        
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def set_tab_search(self):
        """"""
        self.main.tabWidget_tools.setCurrentIndex(2)
        self.main.lineEdit_search.setFocus()
        
        
    #----------------------------------------------------------------------
    def jump_function(self, model_index):
        """"""
        column = model_index.column()
        item = self.main.tableWidget_functions.itemFromIndex(model_index).text()
        if column == 2:
            line = item[:item.find("-")]
            self.jump_to_line(int(line))

    #----------------------------------------------------------------------
    def jump_function_header(self, row):
        """"""
        item = self.main.tableWidget_functions.item(row, 2).text()
        line = item[:item.find("-")]
        self.jump_to_line(int(line))

    #----------------------------------------------------------------------
    def jump_variable(self, model_index):
        """"""
        column = model_index.column()
        item = self.main.tableWidget_variables.itemFromIndex(model_index).text()
        if column == 1:
            line = item
            self.jump_to_line(int(line))
            
    #----------------------------------------------------------------------
    def jump_variable_header(self, row):
        """"""
        item = self.main.tableWidget_variables.item(row, 1).text()
        line = item
        self.jump_to_line(int(line))

    #----------------------------------------------------------------------
    def jump_directive(self, model_index):
        """"""
        column = model_index.column()
        item = self.main.tableWidget_directives.itemFromIndex(model_index).text()
        if column == 2:
            line = item
            self.jump_to_line(int(line))

    #----------------------------------------------------------------------
    def jump_directive_header(self, row):
        """"""
        item = self.main.tableWidget_directives.item(row, 2).text()
        line = item
        self.jump_to_line(int(line))
                    
    #----------------------------------------------------------------------
    def change_dir_files(self, to_dir):
        """"""
        if to_dir == "Examples":
            self.__update_path_files__(Constants.PINGUINO_EXAMPLES_DIR)
            
        elif to_dir == "Home":
            self.__update_path_files__(Constants.HOME_DIR)
            
        elif to_dir == "Current file dir":
            editor = self.main.tabWidget_files.currentWidget()
            dir_ = getattr(editor, "path", None)
            if dir_: self.__update_path_files__(os.path.split(dir_)[0])
            
        elif to_dir == "Other...":
            open_dir = Dialogs.set_open_dir(self)
            if open_dir:
                self.__update_path_files__(open_dir)
                
                    
    #----------------------------------------------------------------------
    def change_dir_filesg(self, to_dir):
        """"""
        if to_dir == "Examples":
            self.__update_graphical_path_files__(Constants.PINGUINOG_EXAMPLES_DIR)
            
        elif to_dir == "Home":
            self.__update_graphical_path_files__(Constants.HOME_DIR)
            
        elif to_dir == "Current file dir":
            editor = self.main.tabWidget_files.currentWidget()
            dir_ = getattr(editor, "path", None)
            if dir_: self.__update_graphical_path_files__(os.path.split(dir_)[0])
            
        elif to_dir == "Other...":
            open_dir = Dialogs.set_open_dir(self)
            if open_dir:
                self.__update_graphical_path_files__(open_dir)                
                
        
    #----------------------------------------------------------------------
    def jump_dir_files(self, list_widget_item):
        """"""
        if getattr(list_widget_item, "type_file") == "dir":
            self.__update_path_files__(getattr(list_widget_item, "path_file"))
        if getattr(list_widget_item, "type_file") == "file":
            if getattr(list_widget_item, "path_file").endswith(".pde"):
                self.open_file_from_path(filename=getattr(list_widget_item, "path_file"))
        
    #----------------------------------------------------------------------
    def jump_dir_filesg(self, list_widget_item):
        """"""
        if getattr(list_widget_item, "type_file") == "dir":
            self.__update_graphical_path_files__(getattr(list_widget_item, "path_file"))
        if getattr(list_widget_item, "type_file") == "file":
            if getattr(list_widget_item, "path_file").endswith(".gpde"):
                self.open_file_from_path(filename=getattr(list_widget_item, "path_file"))
                
    #----------------------------------------------------------------------
    def get_tab(self):
        """"""
        if self.main.actionSwitch_ide.isChecked(): return self.main.tabWidget_graphical
        else: return self.main.tabWidget_files
            
    #----------------------------------------------------------------------
    def __update_path_files__(self, path):
        """"""
        Files.update_path_files(path, self.main.listWidget_files, self.main.label_path)
        
    #----------------------------------------------------------------------
    def __update_graphical_path_files__(self, path):
        """"""
        Files.update_path_files(path, self.main.listWidget_filesg, self.main.label_pathg)
        
        
    #----------------------------------------------------------------------
    def __update_current_dir_on_files__(self):
        tab = self.get_tab()
        if tab == self.main.tabWidget_files:
            if self.main.comboBox_files.currentText() == "Current file dir":
                editor = tab.currentWidget()
                dir_ = getattr(editor, "path", None)
                if dir_: self.__update_path_files__(os.path.split(dir_)[0])
                
        else:
            if self.main.comboBox_filesg.currentText() == "Current file dir":
                editor = tab.currentWidget()
                dir_ = getattr(editor, "path", None)
                if dir_: self.__update_graphical_path_files__(os.path.split(dir_)[0])
            
    #----------------------------------------------------------------------
    @Decorator.connect_features()
    def __save_file__(self, *args, **kwargs):
        """"""
        editor = kwargs.get("editor", self.get_tab())
        content = editor.text_edit.toPlainText()
        #pde_file = file(editor.path, mode="w")
        pde_file = codecs.open(editor.path, "w", "utf-8")
        pde_file.write(content)
        pde_file.close()
        self.__text_saved__()
        
    #----------------------------------------------------------------------
    def __get_name__(self, ext=".pde"):
        """"""
        index = 1
        name = "untitled-%d" % index + ext
        #filenames = [self.main.tabWidget_files.tabText(i) for i in range(self.main.tabWidget_files.count())]
        filenames = [self.get_tab().tabText(i) for i in range(self.get_tab().count())]
        while name in filenames or name + "*" in filenames:
            index += 1
            name = "untitled-%d" % index + ext
        return name + "*"
    
    #----------------------------------------------------------------------
    def __text_changed__(self, *args, **kwargs):
        """"""
        index = self.main.tabWidget_files.currentIndex()
        filename = self.main.tabWidget_files.tabText(index)
        if not filename.endswith("*"):
            self.main.tabWidget_files.setTabText(index, filename+"*")
            self.main.actionSave_file.setEnabled(True)
        self.clear_highlighs()
            
    
    #----------------------------------------------------------------------
    def __text_saved__(self, *args, **kwargs):
        """"""
        index = self.get_tab().currentIndex()
        filename = self.get_tab().tabText(index)
        if filename.endswith("*"):
            self.get_tab().setTabText(index, filename[:-1])
        self.main.actionSave_file.setEnabled(False)
        
    #----------------------------------------------------------------------
    def __text_can_undo__(self, *args, **kwargs):
        """"""
        state = not self.main.actionUndo.isEnabled()
        self.main.actionUndo.setEnabled(state)
        editor = self.main.tabWidget_files.currentWidget()
        editor.tool_bar_state["undo"] = state
        
    #----------------------------------------------------------------------
    def __text_can_redo__(self, *args, **kwargs):
        """"""
        state = not self.main.actionRedo.isEnabled()
        self.main.actionRedo.setEnabled(state)
        editor = self.main.tabWidget_files.currentWidget()
        editor.tool_bar_state["redo"] = state
        
    #----------------------------------------------------------------------
    def __text_can_copy__(self, *args, **kwargs):
        """"""
        state = not self.main.actionCopy.isEnabled()
        self.main.actionCopy.setEnabled(state)
        self.main.actionCut.setEnabled(state)
        editor = self.main.tabWidget_files.currentWidget()
        editor.tool_bar_state["copy"] = state
        
    #----------------------------------------------------------------------
    def __check_duplicate_file__(self, filename):
        """"""
        filenames = [getattr(self.get_tab().widget(i), "path", None) for i in range(self.get_tab().count())]
        if filename in filenames:
            Dialogs.file_duplicated(self, filename)
            self.get_tab().setCurrentIndex(filenames.index(filename))
            return True
        return False
    
    
    #----------------------------------------------------------------------
    def __show_about__(self):
        """"""
        self.frame_about = About(self)
        self.frame_about.show()
        
    #----------------------------------------------------------------------
    def __show_board_config__(self):
        """"""
        self.frame_board = BoardConfig(self)
        self.frame_board.show()
        
    #----------------------------------------------------------------------
    def __show_stdout__(self):
        """"""
        self.frame_stdout = Stdout(self, "Stdout")
        self.frame_stdout.show()
        
    #----------------------------------------------------------------------
    def __show_libmanager__(self):
        """"""
        self.frame_stdout = LibManager(self)
        self.frame_stdout.show()
        
    #----------------------------------------------------------------------
    def __show_pinguino_code__(self):
        """"""
        name = getattr(self.get_tab().currentWidget(), "path", "")
        if name: name = " - " + name
        self.frame_pinguino_code = Stdout(self, "Pinguino code")
        self.frame_pinguino_code.show_text(self.PinguinoKIT.get_pinguino_source_code(), pde=True)
        self.frame_pinguino_code.show()
        
    #----------------------------------------------------------------------
    def __show_hex_code__(self):
        """"""
        if getattr(self.get_tab().currentWidget(), "path", False):
            hex_filename = self.get_tab().currentWidget().path.replace(".gpde", ".pde").replace(".pde", ".hex")
        else:
            Dialogs.error_message(self, "You must compile before.")
            return

        if os.path.isfile(hex_filename):
            self.frame_hex_viewer = HexViewer(self, hex_filename)
            self.frame_hex_viewer.show()            
            
        else:
            Dialogs.error_message(self, "You must compile before.")
                    
                
                
    #----------------------------------------------------------------------
    def __show_current_board_info__(self):
        """"""
        if Dialogs.info_board(self):
            self.__show_board_config__()
            
    #----------------------------------------------------------------------
    def show_wiki_docs(self):
        """"""
        self.frame_wiki_dock = WikiDock()
        self.frame_wiki_dock.show()
        
        
    ##----------------------------------------------------------------------
    #@Decorator.files_tab_on_focus()
    #def set_url_wiki_docs(self, url):
        #""""""
        #wiki_widget = WikiWidget(self)
        #setattr(wiki_widget, "is_widget", True)
        #title = wiki_widget.replace_with_url(url)
        #self.main.tabWidget_files.addTab(wiki_widget, "Docs - " + title)
        #self.main.tabWidget_files.setCurrentIndex(self.main.tabWidget_files.count()-1)

        
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_file_saved()
    def pinguino_compile(self):
        """"""
        if not self.is_graphical():
            filename = self.get_tab().currentWidget().path
        else:
            filename = self.PinguinoKIT.save_as_pde()
            filename = self.get_tab().currentWidget().path.replace(".gpde", ".pde")
            
            
        self.set_board()
        reply = Dialogs.confirm_board(self)
    
        if reply == False:
            self.__show_board_config__()
            return
        elif reply == None: return
            
        
        #self.clear_output_ide()
        self.output_ide("compilling: %s"%filename)
        self.output_ide(self.get_description_board())
        
        self.pinguinoAPI.compile_file(filename)
        
        self.main.actionUpload.setEnabled(self.pinguinoAPI.compiled())
        if not self.pinguinoAPI.compiled():
            
            errors_preprocess = self.pinguinoAPI.get_errors_preprocess()
            if errors_preprocess:
                for error in errors_preprocess["preprocess"]:
                    self.output_ide(error)
            
            errors_c = self.pinguinoAPI.get_errors_compiling_c()
            if errors_c:
                self.output_ide(errors_c["complete_message"])
                line_errors = errors_c["line_numbers"]
                for line_error in line_errors:
                    self.highligh_line(line_error, "#ff7f7f")
            
            errors_asm = self.pinguinoAPI.get_errors_compiling_asm()
            if errors_asm:
                for error in errors_asm["error_symbols"]:
                    self.output_ide(error)
            
            errors_linking = self.pinguinoAPI.get_errors_linking()
            if errors_linking:
                for error in errors_linking["linking"]:
                    self.output_ide(error)
                    
            if errors_asm or errors_c:
                if Dialogs.error_while_compiling(self): self.__show_stdout__()
            elif errors_linking:
                if Dialogs.error_while_linking(self): self.__show_stdout__()
            elif errors_preprocess:
                if Dialogs.error_while_preprocess(self): self.__show_stdout__()
                
                
            else:
                if Dialogs.error_while_unknow(self): self.__show_stdout__()                
                
                
        else:
            result = self.pinguinoAPI.get_result()
            self.output_ide("compilation done")
            self.output_ide(result["code_size"])
            self.output_ide("%s seconds process time"%result["time"])
            
            if Dialogs.compilation_done(self): self.pinguino_upload()
            
        if self.is_graphical(): os.remove(filename)
        
        self.main.plainTextEdit_output.appendPlainText(">>> ")
            
        
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def pinguino_upload(self):
        """"""
        uploaded, result = self.pinguinoAPI.upload()
        self.output_ide(result)
        
        if uploaded: Dialogs.upload_done(self)
        else:
            if Dialogs.upload_fail(self, result): self.pinguino_upload()
        
    #----------------------------------------------------------------------
    def pinguino_use_local_compilers(self):
        """"""
        Dialogs.info_message(self, "No implemented yet.")
        #self.pinguinoAPI.set_os_variables(local_compilers=True)
        self.main.actionUse_local_compilers.setChecked(True)
        self.main.actionUse_system_compilers.setChecked(False)
        
    #----------------------------------------------------------------------
    def pinguino_use_system_compilers(self):
        """"""
        Dialogs.info_message(self, "No implemented yet.")
        ##self.pinguinoAPI.set_os_variables(local_compilers=False)
        #self.main.actionUse_local_compilers.setChecked(False)
        #self.main.actionUse_system_compilers.setChecked(True)
        self.main.actionUse_local_compilers.setChecked(True)
        self.main.actionUse_system_compilers.setChecked(False)
        
                        
    #----------------------------------------------------------------------
    def __drop__(self, event):
        mine = event.mimeData()
        if mine.hasUrls():
            for path in mine.urls():
                self.open_file_from_path(path.path())
                
                #[self.chargepde(url.path()) for url in mine.urls()]
                
                
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def save_screen_image(self):
        """"""
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
    def __close_ide__(self, *args, **kwargs):
        """"""
        size = self.size()
        self.configIDE.set("Main", "size", size.toTuple())
        
        pos = self.pos()
        self.configIDE.set("Main", "position", pos.toTuple())
        
        self.configIDE.set("Main", "maximized", self.isMaximized())
        
        count = 1
        for file_ in self.recent_files:
            self.configIDE.set("Recents", "rencent_"+str(count), file_)
            count += 1
            
        count = 1
        for file_ in self.get_all_open_files():
            self.configIDE.set("Recents", "open_"+str(count), file_)
            count += 1
            
        self.configIDE.set("Features", "graphical", self.is_graphical())
            
            
        self.configIDE.save_config()
        
        self.close()
        

    
        
    #----------------------------------------------------------------------
    def load_main_config(self):
        """"""
        if self.configIDE.config("Main", "maximized", True):
            self.showMaximized()
        
        else:
            pos = self.configIDE.config("Main", "position", "(0, 0)")
            self.move(*eval(pos))
            
            size = self.configIDE.config("Main", "size", "(1050, 550)")
            self.resize(*eval(size))
        
        
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def save_all(self):
        """"""
        tab = self.get_tab()
        for index in range(tab.count()):
            self.save_file(editor=tab.widget(index))
        
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def close_all(self):
        """"""
        tab = self.get_tab()
        widgets = map(lambda index:tab.widget(index), range(tab.count()))
        for widget in widgets:
            self.close_file(editor=widget)
            
    
    #----------------------------------------------------------------------
    def get_all_open_files(self):
        """"""
        opens = []
        tab = self.main.tabWidget_files
        widgets = map(lambda index:tab.widget(index), range(tab.count()))
        for widget in widgets:
            path = getattr(widget, "path", False)
            if path: opens.append(path)
        tab = self.main.tabWidget_graphical
        widgets = map(lambda index:tab.widget(index), range(tab.count()))
        for widget in widgets:
            path = getattr(widget, "path", False)
            if path: opens.append(path)
        return opens
        
    #----------------------------------------------------------------------
    def open_web_site(self, url):
        """"""
        webbrowser.open_new_tab(url)
        
        
    #----------------------------------------------------------------------
    def update_recents(self, filename):
        """"""
        if filename in self.recent_files:
            self.recent_files.remove(filename)
        self.recent_files.insert(0, filename)
        self.recent_files = self.recent_files[:10]
        
        self.update_recents_menu()
        
        
    #----------------------------------------------------------------------
    def update_recents_menu(self):
        """"""
        
        self.main.menuRecents.clear()
        for file_ in self.recent_files:
            """"""
            action = QtGui.QAction(self)
            
            filename = os.path.split(file_)[1]
            
            len_ = 40
            if len(file_) > len_:
                file_path_1 = file_[:len_/2]
                file_path_2 = file_[-len_/2:]
                file_path = file_path_1 + "..." + file_path_2
            else: file_path = file_
            
            if os.path.isfile(file_path):
                action.setText(filename+" ("+file_path+")")
                self.connect(action, QtCore.SIGNAL("triggered()"), self.menu_recent_event(file_))
                action.ActionEvent = self.menu_recent_event
                
                self.main.menuRecents.addAction(action)
        
        
    #----------------------------------------------------------------------
    def menu_recent_event(self, file_):
        """"""
        def menu():
            """"""
            self.open_file_from_path(filename=file_)
        return menu
            
        
    #----------------------------------------------------------------------
    def switch_color_theme(self, pinguino_color=True):
        """"""
        default_pallete = ["toolBar_edit", "toolBar_files", "toolBar_search_replace",
                           "toolBar_undo_redo", "toolBar_pinguino", "toolBar_pinguino",
                           "toolBar_graphical", "toolBar_switch", "statusBar"]
        
        pinguino_pallete = ["dockWidget_output", "dockWidget_tools", "dockWidget_blocks"]
        
        if pinguino_color:
            for element in pinguino_pallete:
                self.PinguinoPallete.set_background_pinguino(getattr(self.main, element))
            for element in default_pallete:
                self.PinguinoPallete.set_default_palette(getattr(self.main, element))
            self.PinguinoPallete.set_background_pinguino(self.main.centralwidget.parent())
            self.main.label_logo.setPixmap(QtGui.QPixmap(":/logo/art/banner.png"))
        else:
            for element in default_pallete + pinguino_pallete:
                self.PinguinoPallete.set_default_palette(getattr(self.main, element))
            self.PinguinoPallete.set_default_palette(self.main.centralwidget.parent())
            self.main.label_logo.setPixmap(QtGui.QPixmap(":/logo/art/banner_blue.png"))
        
        self.configIDE.set("Main", "color_theme", pinguino_color)
        self.main.actionColor_theme.setChecked(pinguino_color)