#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

from PySide import QtGui, QtCore

#from setup import os.getenv("NAME")

########################################################################
class Dialogs(object):

    #----------------------------------------------------------------------
    @classmethod
    def set_save_file(cls, parent, filename):
        if filename.endswith("*"): filename = filename[:-1]
        ext = os.path.splitext(os.path.split(filename)[1])[1]
        save_filename = QtGui.QFileDialog.getSaveFileName(parent,
                os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Save"),
                os.path.join(QtCore.QDir.home().path(), filename),
                QtGui.QApplication.translate("Dialogs", "Pinguino files (*%s);;All Files (*)")%ext)
        if save_filename: return save_filename[0], os.path.split(save_filename[0])[1]
        else: return None, None

    #----------------------------------------------------------------------
    @classmethod
    def set_open_file(cls, parent, path):
        open_files = QtGui.QFileDialog.getOpenFileNames(parent,
                os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Open"),
                #QtCore.QDir.home().path(),
                path,
                QtGui.QApplication.translate("Dialogs", "Pinguino Files (*.pde *.gpde);;All Files (*)"))
        if open_files: return open_files[0]
        else: return None


    #----------------------------------------------------------------------
    @classmethod
    def set_no_saved_file(cls, parent, filename):

        options = QtGui.QMessageBox.Yes | QtGui.QMessageBox.Discard | QtGui.QMessageBox.Cancel
        reply = QtGui.QMessageBox.question(parent,
                os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Save"),
                QtGui.QApplication.translate("Dialogs", "This file has not been saved,\nWould you like to do?\n\n")+filename[:-1],
                options)

        if reply == QtGui.QMessageBox.Yes: return True
        elif reply == QtGui.QMessageBox.Discard: return False
        else: return None


    #----------------------------------------------------------------------
    @classmethod
    def set_open_dir(cls, parent):

        open_dir = QtGui.QFileDialog.getExistingDirectory(parent,
                os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Open directory"),
                QtCore.QDir.home().path())

        if open_dir: return open_dir
        else: return None

    #----------------------------------------------------------------------
    @classmethod
    def file_duplicated(cls, parent, filename):

        QtGui.QMessageBox.information(parent,
                os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - File already open"),
                QtGui.QApplication.translate("Dialogs", "This file is already open in other tab.\n%s")%filename)
        return True

    #----------------------------------------------------------------------
    @classmethod
    def error_message(cls, parent, message):

        QtGui.QMessageBox.warning(parent,
                os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Error"),
                message)
        return True

    #----------------------------------------------------------------------
    @classmethod
    def confirm_message(cls, parent, message):

        options = QtGui.QMessageBox.Yes | QtGui.QMessageBox.Cancel
        reply = QtGui.QMessageBox.question(parent,
                os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Confirmation"),
                message,
                options)

        if reply == QtGui.QMessageBox.Yes: return True
        elif reply == QtGui.QMessageBox.Discard: return False
        else: return None


    #----------------------------------------------------------------------
    @classmethod
    def info_message(cls, parent, message):

        QtGui.QMessageBox.information(parent,
                os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Information"),
                message)
        return True

    #----------------------------------------------------------------------
    @classmethod
    def warning_message(cls, parent, message):

        QtGui.QMessageBox.warning(parent,
                                  os.getenv("NAME") + QtGui.QApplication.translate("Dialogs", " - Warning"),
                                  message)

        return True

    #----------------------------------------------------------------------
    @classmethod
    def save_before_compile(cls, parent):

        QtGui.QMessageBox.information(parent,
                os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Save file first."),
                QtGui.QApplication.translate("Dialogs", "You must save the file before compiling."))
        return True

    #----------------------------------------------------------------------
    @classmethod
    def confirm_board(cls, parent):

        parent.set_board()

        confirm = parent.configIDE.config("Features", "confirm_board", True)
        if not confirm: return True

        board_config = parent.get_description_board()

        msg_box = QtGui.QMessageBox()
        msg_box.setIcon(QtGui.QMessageBox.Question)
        msg_box.setWindowTitle(os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Config board"))
        msg_box.setText(board_config+QtGui.QApplication.translate("Dialogs", "\nUse this board config?\n"))

        cancel = QtGui.QPushButton()
        cancel.setText(QtGui.QApplication.translate("Dialogs", "Cancel"))

        change = QtGui.QPushButton()
        change.setText(QtGui.QApplication.translate("Dialogs", "Change board"))

        compile_ = QtGui.QPushButton()
        compile_.setText(QtGui.QApplication.translate("Dialogs", "Compile"))

        checkbox = QtGui.QCheckBox()
        checkbox.setText(QtGui.QApplication.translate("Dialogs", "Don't ask again"))
        layout = msg_box.layout()

        checkbox.setStyleSheet("""
        font-family: ubuntu regular;
        font-weight: bold;

        """)
        checkbox.setChecked(False)
        layout.addWidget(checkbox, 1, 0, 1, 2)

        msg_box.addButton(cancel, QtGui.QMessageBox.RejectRole)
        msg_box.addButton(change, QtGui.QMessageBox.NoRole)
        msg_box.addButton(compile_, QtGui.QMessageBox.YesRole)

        msg_box.setDefaultButton(compile_)

        msg_box.setStyleSheet("""
        font-family: ubuntu regular;
        font-weight: normal;

        """)

        reply = msg_box.exec_()

        show_again = checkbox.isChecked()
        parent.configIDE.set("Features", "confirm_board", not show_again)
        parent.main.actionConfirm_board.setChecked(not show_again)
        parent.configIDE.save_config()
        parent.configIDE.load_config()

        if reply == 2: return True
        elif reply == 1: return False
        else: return None


    #----------------------------------------------------------------------
    @classmethod
    def error_while_compiling(cls, parent):

        msg_box = QtGui.QMessageBox(parent)
        msg_box.setIcon(QtGui.QMessageBox.Warning)
        msg_box.setWindowTitle(os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Error"))
        msg_box.setText(QtGui.QApplication.translate("Dialogs", "Error while compiling."))

        stdout = QtGui.QPushButton()
        stdout.setText(QtGui.QApplication.translate("Dialogs", "View stdout"))

        ok = QtGui.QPushButton()
        ok.setText(QtGui.QApplication.translate("Dialogs", "Ok"))

        msg_box.addButton(stdout, QtGui.QMessageBox.RejectRole)
        msg_box.addButton(ok, QtGui.QMessageBox.NoRole)

        msg_box.setDefaultButton(ok)

        msg_box.setStyleSheet("""
        font-family: ubuntu regular;
        font-weight: normal;

        """)

        reply = msg_box.exec_()

        if reply == 0: return True
        elif reply == 1: return False

    #----------------------------------------------------------------------
    @classmethod
    def error_while_linking(cls, parent):

        msg_box = QtGui.QMessageBox(parent)
        msg_box.setIcon(QtGui.QMessageBox.Warning)
        msg_box.setWindowTitle(os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Error"))
        msg_box.setText(QtGui.QApplication.translate("Dialogs", "Error while linking."))

        stdout = QtGui.QPushButton()
        stdout.setText(QtGui.QApplication.translate("Dialogs", "View stdout"))

        ok = QtGui.QPushButton()
        ok.setText(QtGui.QApplication.translate("Dialogs", "Ok"))

        msg_box.addButton(stdout, QtGui.QMessageBox.RejectRole)
        msg_box.addButton(ok, QtGui.QMessageBox.NoRole)

        msg_box.setDefaultButton(ok)

        msg_box.setStyleSheet("""
        font-family: ubuntu regular;
        font-weight: normal;

        """)

        reply = msg_box.exec_()

        if reply == 0: return True
        elif reply == 1: return False



    #----------------------------------------------------------------------
    @classmethod
    def overwrite_file(cls, parent, filename, exist):

        msg_box = QtGui.QMessageBox(parent)
        msg_box.setIcon(QtGui.QMessageBox.Warning)
        msg_box.setWindowTitle(os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Error"))
        if exist:
            msg_box.setText(QtGui.QApplication.translate("Dialogs", "This file has changed on disk.")+"\n"+filename)
        else:
            msg_box.setText(QtGui.QApplication.translate("Dialogs", "This file has deleted/moved on disk.")+"\n"+filename)

        stdout = QtGui.QPushButton()
        stdout.setText(QtGui.QApplication.translate("Dialogs", "Overwrite"))

        if exist:
            ok = QtGui.QPushButton()
            ok.setText(QtGui.QApplication.translate("Dialogs", "Reload"))

        msg_box.addButton(stdout, QtGui.QMessageBox.RejectRole)
        if exist: msg_box.addButton(ok, QtGui.QMessageBox.NoRole)

        if exist: msg_box.setDefaultButton(ok)
        else: msg_box.setDefaultButton(stdout)

        msg_box.setStyleSheet("""
        font-family: ubuntu regular;
        font-weight: normal;

        """)

        reply = msg_box.exec_()

        if reply == 0: return True
        elif reply == 1: return False


    #----------------------------------------------------------------------
    @classmethod
    def error_while_preprocess(cls, parent):

        msg_box = QtGui.QMessageBox(parent)
        msg_box.setIcon(QtGui.QMessageBox.Warning)
        msg_box.setWindowTitle(os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Error"))
        msg_box.setText(QtGui.QApplication.translate("Dialogs", "Error while preprocess."))

        stdout = QtGui.QPushButton()
        stdout.setText(QtGui.QApplication.translate("Dialogs", "View stdout"))

        ok = QtGui.QPushButton()
        ok.setText(QtGui.QApplication.translate("Dialogs", "Ok"))

        msg_box.addButton(stdout, QtGui.QMessageBox.RejectRole)
        msg_box.addButton(ok, QtGui.QMessageBox.NoRole)

        msg_box.setDefaultButton(ok)

        msg_box.setStyleSheet("""
        font-family: ubuntu regular;
        font-weight: normal;

        """)

        reply = msg_box.exec_()

        if reply == 0: return True
        elif reply == 1: return False



    #----------------------------------------------------------------------
    @classmethod
    def error_while_unknow(cls, parent):

        msg_box = QtGui.QMessageBox(parent)
        msg_box.setIcon(QtGui.QMessageBox.Warning)
        msg_box.setWindowTitle(os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Error"))
        msg_box.setText(QtGui.QApplication.translate("Dialogs", "Unknow error."))

        stdout = QtGui.QPushButton()
        stdout.setText(QtGui.QApplication.translate("Dialogs", "View stdout"))

        ok = QtGui.QPushButton()
        ok.setText(QtGui.QApplication.translate("Dialogs", "Ok"))

        msg_box.addButton(stdout, QtGui.QMessageBox.RejectRole)
        msg_box.addButton(ok, QtGui.QMessageBox.NoRole)

        msg_box.setDefaultButton(ok)

        msg_box.setStyleSheet("""
        font-family: ubuntu regular;
        font-weight: normal;

        """)

        reply = msg_box.exec_()

        if reply == 0: return True
        elif reply == 1: return False



    #----------------------------------------------------------------------
    @classmethod
    def compilation_done(cls, parent):

        msg_box = QtGui.QMessageBox(parent)
        msg_box.setIcon(QtGui.QMessageBox.Information)
        msg_box.setWindowTitle(os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Compiled"))
        msg_box.setText(QtGui.QApplication.translate("Dialogs", "Compilation done!"))

        upload = QtGui.QPushButton()
        upload.setText(QtGui.QApplication.translate("Dialogs", "Upload now!"))

        ok = QtGui.QPushButton()
        ok.setText(QtGui.QApplication.translate("Dialogs", "Ok"))

        msg_box.addButton(upload, QtGui.QMessageBox.AcceptRole)
        msg_box.addButton(ok, QtGui.QMessageBox.NoRole)

        msg_box.setDefaultButton(ok)

        msg_box.setStyleSheet("""
        font-family: ubuntu regular;
        font-weight: normal;

        """)

        reply = msg_box.exec_()

        if reply == 0: return True
        elif reply == 1: return False

    #----------------------------------------------------------------------
    @classmethod
    def upload_done(cls, parent):

        QtGui.QMessageBox.information(parent,
                os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Upload done"),
                QtGui.QApplication.translate("Dialogs", "File sucessfully uploaded to pinguino."))
        return True


    #----------------------------------------------------------------------
    @classmethod
    def upload_fail(cls, parent, message):

        msg_box = QtGui.QMessageBox(parent)
        msg_box.setIcon(QtGui.QMessageBox.Information)
        msg_box.setWindowTitle(os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Upload fail"))
        msg_box.setText(message)

        upload = QtGui.QPushButton()
        upload.setText(QtGui.QApplication.translate("Dialogs", "Try again!!"))

        ok = QtGui.QPushButton()
        ok.setText(QtGui.QApplication.translate("Dialogs", "Cancel"))

        msg_box.addButton(upload, QtGui.QMessageBox.AcceptRole)
        msg_box.addButton(ok, QtGui.QMessageBox.NoRole)

        msg_box.setDefaultButton(upload)

        msg_box.setStyleSheet("""
        font-family: ubuntu regular;
        font-weight: normal;

        """)

        reply = msg_box.exec_()

        if reply == 0: return True
        elif reply == 1: return False


    #----------------------------------------------------------------------
    @classmethod
    def set_save_image(cls, parent, filename):

        file_name = QtGui.QFileDialog.getSaveFileName(parent,
                os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Save image"),
                filename,
                QtGui.QApplication.translate("Dialogs", "Png files (*.png);;All Files (*)"))

        if file_name: return file_name[0]
        else: return None
