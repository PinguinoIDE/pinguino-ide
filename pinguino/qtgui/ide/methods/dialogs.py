#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

from PySide2 import QtGui, QtCore, QtWidgets


########################################################################
class Dialogs(object):

    #----------------------------------------------------------------------
    @classmethod
    def set_save_file(cls, parent, filename, dirpath=None):

        if dirpath is None: dirpath = parent.get_default_dir()
        if filename.endswith("*"): filename = filename[:-1]
        ext = os.path.splitext(os.path.split(filename)[1])[1]
        save_filename = QtGui.QFileDialog.getSaveFileName(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Save"),
                os.path.join(dirpath, filename),
                QtWidgets.QApplication.translate("Dialogs", "Pinguino files (*{});;All Files (*)".format(ext)))
        if save_filename: return save_filename[0], os.path.split(save_filename[0])[1]
        else: return None, None


    #----------------------------------------------------------------------
    @classmethod
    def set_open_file(cls, parent, exts="*.pde *.gpde *.ppde", dirpath=None):

        if dirpath is None:dirpath = parent.get_default_dir()
        open_file = QtGui.QFileDialog.getOpenFileName(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Open"),
                dirpath,
                QtWidgets.QApplication.translate("Dialogs", "Pinguino Files ({});;All Files (*)".format(exts)))
        if open_file[0]:
            return open_file[0]
        else: return None

    #----------------------------------------------------------------------
    @classmethod
    def set_open_files(cls, parent, exts="*.pde *.gpde *.ppde", dirpath=None):

        if dirpath is None:dirpath = parent.get_default_dir()
        open_files = QtGui.QFileDialog.getOpenFileNames(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Open"),
                dirpath,
                QtWidgets.QApplication.translate("Dialogs", "Pinguino Files ({});;All Files (*)".format(exts)))
        if open_files[0]:
            return open_files[0]
        else: return None


    #----------------------------------------------------------------------
    @classmethod
    def set_open_hex(cls, parent, dirpath=None):

        if dirpath is None:dirpath = parent.get_default_dir()
        open_file = QtGui.QFileDialog.getOpenFileName(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Select"),
                dirpath,
                QtWidgets.QApplication.translate("Dialogs", "Hex Files (*.hex);;All Files (*)"))
        if open_file: return open_file[0]
        else: return None


    #----------------------------------------------------------------------
    @classmethod
    def set_no_saved_file(cls, parent, filename):

        options = QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.Discard | QtWidgets.QMessageBox.Cancel
        reply = QtWidgets.QMessageBox.question(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Save"),
                QtWidgets.QApplication.translate("Dialogs", "This file has not been saved,\nWould you like to do?\n\n")+filename[:-1],
                options)

        if reply == QtWidgets.QMessageBox.Yes: return True
        elif reply == QtWidgets.QMessageBox.Discard: return False
        else: return None


    #----------------------------------------------------------------------
    @classmethod
    def set_open_dir(cls, parent, dirpath=None):

        if dirpath is None:dirpath = parent.get_default_dir()
        open_dir = QtGui.QFileDialog.getExistingDirectory(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Open directory"),
                dirpath)

        if open_dir: return open_dir
        else: return None


    #----------------------------------------------------------------------
    @classmethod
    def file_duplicated(cls, parent, filename):

        QtWidgets.QMessageBox.information(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - File already open"),
                QtWidgets.QApplication.translate("Dialogs", "This file is already open in other tab.\n{}".format(filename)))
        return True


    #----------------------------------------------------------------------
    @classmethod
    def error_message(cls, parent, message):

        QtWidgets.QMessageBox.warning(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Error"),
                message)
        return True


    #----------------------------------------------------------------------
    @classmethod
    def confirm_message(cls, parent, message):

        options = QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.Cancel
        reply = QtWidgets.QMessageBox.question(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Confirmation"),
                message,
                options)

        if reply == QtWidgets.QMessageBox.Yes: return True
        elif reply == QtWidgets.QMessageBox.Discard: return False
        else: return None


    #----------------------------------------------------------------------
    @classmethod
    def info_message(cls, parent, message):

        QtWidgets.QMessageBox.information(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Information"),
                message)
        return True


    #----------------------------------------------------------------------
    @classmethod
    def warning_message(cls, parent, message):

        QtWidgets.QMessageBox.warning(parent,
                                  os.getenv("PINGUINO_NAME") + QtWidgets.QApplication.translate("Dialogs", " - Warning"),
                                  message)

        return True


    #----------------------------------------------------------------------
    @classmethod
    def save_before_compile(cls, parent):

        QtWidgets.QMessageBox.information(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Save file first."),
                QtWidgets.QApplication.translate("Dialogs", "You must save the file before compiling."))
        return True


    #----------------------------------------------------------------------
    @classmethod
    def add_files_to_project(cls, parent):

        QtWidgets.QMessageBox.information(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Save file first."),
                QtWidgets.QApplication.translate("Dialogs", "You must add files to project before compiling."))
        return True


    #----------------------------------------------------------------------
    @classmethod
    def overwrite_file(cls, parent, filename, exist):

        msg_box = QtWidgets.QMessageBox(parent)
        msg_box.setIcon(QtWidgets.QMessageBox.Warning)
        msg_box.setWindowTitle(os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Error"))
        if exist:
            msg_box.setText(QtWidgets.QApplication.translate("Dialogs", "This file has changed on disk.")+"\n"+filename)
        else:
            msg_box.setText(QtWidgets.QApplication.translate("Dialogs", "This file has deleted/moved on disk.")+"\n"+filename)

        stdout = QtWidgets.QPushButton()
        stdout.setText(QtWidgets.QApplication.translate("Dialogs", "Overwrite"))

        if exist:
            ok = QtWidgets.QPushButton()
            ok.setText(QtWidgets.QApplication.translate("Dialogs", "Reload"))

        msg_box.addButton(stdout, QtWidgets.QMessageBox.RejectRole)
        if exist: msg_box.addButton(ok, QtWidgets.QMessageBox.NoRole)

        if exist: msg_box.setDefaultButton(ok)
        else: msg_box.setDefaultButton(stdout)

        msg_box.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)

        reply = msg_box.exec_()


    #----------------------------------------------------------------------
    @classmethod
    def compilation_done(cls, parent):

        msg_box = QtWidgets.QMessageBox(parent)
        msg_box.setIcon(QtWidgets.QMessageBox.Information)
        msg_box.setWindowTitle(os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Compiled"))
        msg_box.setText(QtWidgets.QApplication.translate("Dialogs", "Compilation done!"))

        upload = QtWidgets.QPushButton()
        upload.setText(QtWidgets.QApplication.translate("Dialogs", "Upload now!"))

        ok = QtWidgets.QPushButton()
        ok.setText(QtWidgets.QApplication.translate("Dialogs", "Ok"))

        msg_box.addButton(upload, QtWidgets.QMessageBox.AcceptRole)
        msg_box.addButton(ok, QtWidgets.QMessageBox.NoRole)

        msg_box.setDefaultButton(ok)

        msg_box.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)

        reply = msg_box.exec_()

        if reply == 0: return True
        elif reply == 1: return False


    #----------------------------------------------------------------------
    @classmethod
    def upload_done(cls, parent):

        QtWidgets.QMessageBox.information(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Upload done"),
                QtWidgets.QApplication.translate("Dialogs", "File sucessfully uploaded to Pinguino."))
        return True


    #----------------------------------------------------------------------
    @classmethod
    def upload_fail(cls, parent, message):

        msg_box = QtWidgets.QMessageBox(parent)
        msg_box.setIcon(QtWidgets.QMessageBox.Information)
        msg_box.setWindowTitle(os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Upload fail"))
        msg_box.setText(message)

        upload = QtWidgets.QPushButton()
        upload.setText(QtWidgets.QApplication.translate("Dialogs", "Try again!"))

        ok = QtWidgets.QPushButton()
        ok.setText(QtWidgets.QApplication.translate("Dialogs", "Cancel"))

        msg_box.addButton(upload, QtWidgets.QMessageBox.AcceptRole)
        msg_box.addButton(ok, QtWidgets.QMessageBox.NoRole)

        msg_box.setDefaultButton(upload)

        msg_box.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)

        reply = msg_box.exec_()

        if reply == 0: return True
        elif reply == 1: return False


    #----------------------------------------------------------------------
    @classmethod
    def set_save_image(cls, parent, filename, dirpath=None):

        if dirpath is None:dirpath = parent.get_default_dir()
        file_name = QtGui.QFileDialog.getSaveFileName(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Save image"),
                os.path.join(dirpath, filename),
                QtWidgets.QApplication.translate("Dialogs", "Png files (*.png);;All Files (*)"))

        if file_name: return file_name[0]
        else: return None


    #----------------------------------------------------------------------
    @classmethod
    def get_text(self, parent, name, default=""):

        text, ok = QtGui.QInputDialog.getText(parent,
                os.getenv("PINGUINO_NAME")+QtWidgets.QApplication.translate("Dialogs", " - Input text"),
                name+":", QtWidgets.QLineEdit.Normal,
                default)
        if ok and text != '': return text
        else: return False
