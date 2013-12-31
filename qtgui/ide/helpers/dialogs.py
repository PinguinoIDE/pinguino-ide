#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

from PySide import QtGui, QtCore

from .constants import NAME

########################################################################
class Dialogs(object):
    """"""

    #----------------------------------------------------------------------
    @classmethod
    def set_save_file(self, parent, filename):
        if filename.endswith("*"): filename = filename[:-1]
        ext = os.path.splitext(os.path.split(filename)[1])[1]
        save_filename = QtGui.QFileDialog.getSaveFileName(parent,
                NAME+" - Save",
                os.path.join(QtCore.QDir.home().path(), filename),
                "Pinguino files (*%s);;All Files (*)"%ext)
        if save_filename: return save_filename[0], os.path.split(save_filename[0])[1]
        else: return None
        
    #----------------------------------------------------------------------
    @classmethod
    def set_open_file(self, parent):
        open_files = QtGui.QFileDialog.getOpenFileNames(parent,
                NAME+" - Open",
                QtCore.QDir.home().path(), 
                "Pinguino Files (*.pde *.gpde);;All Files (*)")
        if open_files: return open_files[0]
        else: return None
            

    #----------------------------------------------------------------------
    @classmethod
    def set_no_saved_file(self, parent, filename):
        """"""
        options = QtGui.QMessageBox.Yes | QtGui.QMessageBox.Discard | QtGui.QMessageBox.Cancel
        reply = QtGui.QMessageBox.question(parent,
                NAME+" - Save?",
                "this file has not been saved,\nWould you like to do?\n\n"+filename,
                options)
        
        if reply == QtGui.QMessageBox.Yes: return True
        elif reply == QtGui.QMessageBox.Discard: return False
        else: return None
        
        
    #----------------------------------------------------------------------
    @classmethod
    def set_open_dir(self, parent):
        """"""
        open_dir = QtGui.QFileDialog.getExistingDirectory(parent,
                NAME+" - Open directory",
                QtCore.QDir.home().path())
        
        if open_dir: return open_dir
        else: return None
        
    #----------------------------------------------------------------------
    @classmethod
    def file_duplicated(self, parent, filename):
        """"""
        QtGui.QMessageBox.information(parent,
                NAME+" - File already open.",
                "This file is already open in other tab.\n%s"%filename)
        return True
    
    #----------------------------------------------------------------------
    @classmethod
    def error_message(self, parent, message):
        """"""
        QtGui.QMessageBox.warning(parent,
                NAME+" - Error",
                message)
        return True
        
    #----------------------------------------------------------------------
    @classmethod
    def confirm_message(self, parent, title, message):
        """"""
        options = QtGui.QMessageBox.Yes | QtGui.QMessageBox.Cancel
        reply = QtGui.QMessageBox.question(parent,
                NAME+" - "+title,
                message,
                options)
        
        if reply == QtGui.QMessageBox.Yes: return True
        elif reply == QtGui.QMessageBox.Discard: return False
        else: return None
                
        
    #----------------------------------------------------------------------
    @classmethod
    def info_message(self, parent, message):
        """"""
        QtGui.QMessageBox.information(parent,
                NAME+" - Info",
                message)
        return True
    
    #----------------------------------------------------------------------
    @classmethod
    def info_board(self, parent):
        """"""
        board_config = parent.get_description_board()
        
        msg_box = QtGui.QMessageBox()
        msg_box.setIcon(QtGui.QMessageBox.Question)
        msg_box.setWindowTitle(NAME+" - Board info")
        msg_box.setText(board_config+"\n\nUse this board config?")
        
        accept = QtGui.QPushButton()
        accept.setText("Ok")
        change = QtGui.QPushButton()
        change.setText("Change board")
        
        msg_box.addButton(accept, QtGui.QMessageBox.AcceptRole)        
        msg_box.addButton(change, QtGui.QMessageBox.ApplyRole)
        
        
        msg_box.setDefaultButton(accept)
        
        reply = msg_box.exec_()    
        if reply == 1: return True
        elif reply == 0: return False
        
    #----------------------------------------------------------------------
    @classmethod
    def save_before_compile(self, parent):
        """"""
        QtGui.QMessageBox.information(parent,
                NAME+" - must save file.",
                "You must save the file before compiling.")
        return True
    
    #----------------------------------------------------------------------
    @classmethod
    def confirm_board(self, parent):
        """"""
        parent.set_board()
        board_config = parent.get_description_board()
        
        msg_box = QtGui.QMessageBox()
        msg_box.setIcon(QtGui.QMessageBox.Question)
        msg_box.setWindowTitle(NAME+" - Config board")
        msg_box.setText(board_config+"\n\nUse this board config?")
        
        cancel = QtGui.QPushButton()
        cancel.setText("Cancel")
        
        change = QtGui.QPushButton()
        change.setText("Change board")
        
        compile_ = QtGui.QPushButton()
        compile_.setText("Compile")
        
        msg_box.addButton(cancel, QtGui.QMessageBox.RejectRole)
        msg_box.addButton(change, QtGui.QMessageBox.NoRole)
        msg_box.addButton(compile_, QtGui.QMessageBox.YesRole)
        
        msg_box.setDefaultButton(compile_)
        
        reply = msg_box.exec_()    
        
        if reply == 2: return True
        elif reply == 1: return False
        else: return None
        
        
    #----------------------------------------------------------------------
    @classmethod
    def error_while_compiling(self, parent):
        """"""
        msg_box = QtGui.QMessageBox()
        msg_box.setIcon(QtGui.QMessageBox.Warning)
        msg_box.setWindowTitle(NAME+" - Error")
        msg_box.setText("Error while compiling.")
        
        stdout = QtGui.QPushButton()
        stdout.setText("View stdout")
        
        ok = QtGui.QPushButton()
        ok.setText("Ok")
        
        msg_box.addButton(stdout, QtGui.QMessageBox.RejectRole)
        msg_box.addButton(ok, QtGui.QMessageBox.NoRole)
        
        msg_box.setDefaultButton(ok)
        
        reply = msg_box.exec_()    
        
        if reply == 0: return True
        elif reply == 1: return False
    
    #----------------------------------------------------------------------
    @classmethod
    def error_while_linking(self, parent):
        """"""
        msg_box = QtGui.QMessageBox()
        msg_box.setIcon(QtGui.QMessageBox.Warning)
        msg_box.setWindowTitle(NAME+" - Error")
        msg_box.setText("Error while linking.")
        
        stdout = QtGui.QPushButton()
        stdout.setText("View stdout")
        
        ok = QtGui.QPushButton()
        ok.setText("Ok")
        
        msg_box.addButton(stdout, QtGui.QMessageBox.RejectRole)
        msg_box.addButton(ok, QtGui.QMessageBox.NoRole)
        
        msg_box.setDefaultButton(ok)
        
        reply = msg_box.exec_()    
        
        if reply == 0: return True
        elif reply == 1: return False
        
    
    #----------------------------------------------------------------------
    @classmethod
    def error_while_preprocess(self, parent):
        """"""
        msg_box = QtGui.QMessageBox()
        msg_box.setIcon(QtGui.QMessageBox.Warning)
        msg_box.setWindowTitle(NAME+" - Error")
        msg_box.setText("Error while preprocess.")
        
        stdout = QtGui.QPushButton()
        stdout.setText("View stdout")
        
        ok = QtGui.QPushButton()
        ok.setText("Ok")
        
        msg_box.addButton(stdout, QtGui.QMessageBox.RejectRole)
        msg_box.addButton(ok, QtGui.QMessageBox.NoRole)
        
        msg_box.setDefaultButton(ok)
        
        reply = msg_box.exec_()    
        
        if reply == 0: return True
        elif reply == 1: return False
        
        
    
    #----------------------------------------------------------------------
    @classmethod
    def error_while_unknow(self, parent):
        """"""
        msg_box = QtGui.QMessageBox()
        msg_box.setIcon(QtGui.QMessageBox.Warning)
        msg_box.setWindowTitle(NAME+" - Error")
        msg_box.setText("Unknow error.")
        
        stdout = QtGui.QPushButton()
        stdout.setText("View stdout")
        
        ok = QtGui.QPushButton()
        ok.setText("Ok")
        
        msg_box.addButton(stdout, QtGui.QMessageBox.RejectRole)
        msg_box.addButton(ok, QtGui.QMessageBox.NoRole)
        
        msg_box.setDefaultButton(ok)
        
        reply = msg_box.exec_()    
        
        if reply == 0: return True
        elif reply == 1: return False
                
                
        
    #----------------------------------------------------------------------
    @classmethod
    def compilation_done(self, parent):
        """"""
        msg_box = QtGui.QMessageBox()
        msg_box.setIcon(QtGui.QMessageBox.Information)
        msg_box.setWindowTitle(NAME+" - Compiled")
        msg_box.setText("Compilation done!")
        
        upload = QtGui.QPushButton()
        upload.setText("Upload now!")
        
        ok = QtGui.QPushButton()
        ok.setText("Ok")
        
        msg_box.addButton(upload, QtGui.QMessageBox.AcceptRole)
        msg_box.addButton(ok, QtGui.QMessageBox.NoRole)
        
        msg_box.setDefaultButton(ok)
        
        reply = msg_box.exec_()    
        
        if reply == 0: return True
        elif reply == 1: return False
        
    #----------------------------------------------------------------------
    @classmethod
    def upload_done(self, parent):
        """"""
        QtGui.QMessageBox.information(parent,
                NAME+" - Upload done",
                "File sucessfully uploaded to pinguino.")
        return True
    
        
    #----------------------------------------------------------------------
    @classmethod
    def upload_fail(self, parent, message):
        """"""
        msg_box = QtGui.QMessageBox()
        msg_box.setIcon(QtGui.QMessageBox.Information)
        msg_box.setWindowTitle(NAME+" - Upload fail")
        msg_box.setText(message)
        
        upload = QtGui.QPushButton()
        upload.setText("Try again!!")
        
        ok = QtGui.QPushButton()
        ok.setText("Cancel")
        
        msg_box.addButton(upload, QtGui.QMessageBox.AcceptRole)
        msg_box.addButton(ok, QtGui.QMessageBox.NoRole)
        
        msg_box.setDefaultButton(upload)
        
        reply = msg_box.exec_()    
        
        if reply == 0: return True
        elif reply == 1: return False
        
        
    #----------------------------------------------------------------------
    @classmethod
    def set_save_image(self, parent, filename):
        """"""
        file_name = QtGui.QFileDialog.getSaveFileName(parent,
                NAME+" - Save image",
                filename,
                "Png files (*.png);;All Files (*)")
        
        if file_name: return file_name[0]
        else: return None