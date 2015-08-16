#! /usr/bin/python2
#-*- coding: utf-8 -*-

from PySide import QtGui, QtCore

import os
import shutil
import webbrowser
# import logging
# from datetime import datetime
from zipfile import ZipFile

from ...pinguino_core.pinguino_config import PinguinoConfig

from ..commons.repositories import Repository, ErrorModules

# from ..methods.decorators import Decorator
from ..methods.dialogs import Dialogs
# from ..methods.parser import PinguinoParser

from ..commons.widgets_features import PrettyFeatures


# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
else:
    #Python2
    from ConfigParser import RawConfigParser



########################################################################
class LibraryManager(object):
    """"""

    #----------------------------------------------------------------------
    def __init__(self):
        """"""

        if not os.path.exists(os.getenv("PINGUINO_USERLIBS_PATH")):
            os.mkdir(os.getenv("PINGUINO_USERLIBS_PATH"))

        if not os.path.exists(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr")):
            os.mkdir(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr"))

        if not os.path.exists(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "library_examples")):
            os.mkdir(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "library_examples"))

        if not os.path.exists(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "blocks")):
            os.mkdir(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "blocks"))

        self.connect(self.main.pushButton_libraries_install_from_zip, QtCore.SIGNAL("clicked()"), self.install_from_zip)
        self.connect(self.main.pushButton_libraries_install, QtCore.SIGNAL("clicked()"), self.install_from_repository)

        self.connect(self.main.tableWidget_user_libraries, QtCore.SIGNAL("clicked(QModelIndex)"), self.open_link)

        PrettyFeatures.LineEdit_default_text(self.main.lineEdit_libraries_source, "e.g. git@github.com:repo/pinguinolib-library.git")

        self.update_libraries_view()

        self.main.radioButton_library_git.setEnabled(ErrorModules["gitpython"])
        self.main.radioButton_library_mercurial.setEnabled(ErrorModules["hgapi"])

        self.main.pushButton_libraries_install.setEnabled(ErrorModules["gitpython"] or ErrorModules["hgapi"])
        self.main.lineEdit_libraries_source.setEnabled(ErrorModules["gitpython"] or ErrorModules["hgapi"])


    #----------------------------------------------------------------------
    def install_from_zip(self):
        """"""
        open_files = QtGui.QFileDialog.getOpenFileNames(self,
                os.getenv("PINGUINO_NAME")+QtGui.QApplication.translate("Dialogs", " - Open"),
                QtCore.QDir.home().path(),
                QtGui.QApplication.translate("Dialogs", "Zip Files (*.zip)"))

        if not open_files[0]: return
        libraries = open_files[0]

        for lib in libraries:
            name = os.path.split(lib)[1]
            name = os.path.splitext(name)[0]

            path_dirlib = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr")
            # if os.path.exists(os.path.join(path_dirlib, name)):
                # Dialogs.error_message(self, QtGui.QApplication.translate("Dialogs", "Library duplicated (name conflict)."))
                # continue

            libZip = ZipFile(lib)
            libZip.extractall(path_dirlib)

            # enb = open(os.path.join(path_dirlib, name, ".enable"), "w")
            # enb.close()

            pinguino = os.path.join(path_dirlib, name, "PINGUINO")
            parser = RawConfigParser()
            parser.readfp(open(pinguino, "r"))
            info = dict(parser.items("PINGUINO"))

            new_name = "{name}".format(**info)
            self.remove_lib(new_name)

            new_dir_path = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", new_name)
            os.rename(os.path.join(path_dirlib, name), new_dir_path)

            self.post_install(new_name)

        self.update_libraries_view()


    #----------------------------------------------------------------------
    def install_from_repository(self):
        """"""
        repo_path = self.main.lineEdit_libraries_source.text()
        dir_path = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", "temp_lib_name")
        repo_type = self.get_repo_type()
        library_cv = Repository(repo_path, dir_path, repo_type)
        library_cv.install_library()

        # enb = open(os.path.join(dir_path, ".enable"), "w")
        # enb.close()

        pinguino = os.path.join(dir_path, "PINGUINO")
        parser = RawConfigParser()
        parser.readfp(open(pinguino, "r"))
        info = dict(parser.items("PINGUINO"))

        new_name = "{name}".format(**info)
        self.remove_lib(new_name)

        new_dir_path = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", new_name)
        os.rename(dir_path, new_dir_path)

        self.post_install(new_name)
        self.update_libraries_view()


    #----------------------------------------------------------------------
    def get_repo_type(self):
        """"""
        if self.main.radioButton_library_git.isChecked():
            return "git"
        elif self.main.radioButton_library_mercurial.isChecked():
            return "hg"


    #----------------------------------------------------------------------
    def post_install(self, lib_name):
        """"""
        lista = []

        path_examples = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", lib_name, "examples")
        dest_examples = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "library_examples", lib_name)
        lista.append([path_examples, dest_examples])

        path_blocks = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", lib_name, "blocks")
        dest_blocks = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "blocks", lib_name)
        lista.append([path_blocks, dest_blocks])

        for src, dest in lista:
            if os.path.exists(src):
                if os.path.exists(dest): shutil.rmtree(dest)
                shutil.copytree(src, dest)

        # self.pinguinoAPI.force_reload_libs()
        # self.update_reserved_words()
        # delattr(self, "assistant")
        self.refresh_libraries()



    #----------------------------------------------------------------------
    def get_all_libraries_paths(self):
        """"""
        paths = []
        usr = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr")
        return map(lambda lib:os.path.join(usr, lib), os.listdir(usr))


    # #----------------------------------------------------------------------
    # def get_enable_libraries(self):
        # """"""
        # enable = []
        # all_libs = self.get_all_libraries_names()
        # for lib in all_libs:
            # if not os.path.exists(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", lib, ".disable")):
                # enable.append(lib)
        # return enable


    # #----------------------------------------------------------------------
    # def get_p8_libraries(self):
        # """"""
        # p8_libs = []
        # for lib in self.get_all_libraries_names():
            # p8 = os.path.exists(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", lib, "p8"))
            # if os.path.exists(p8):
                # p8_libs.append(p8)

        # return p8_libs

    # #----------------------------------------------------------------------
    # def get_p32_libraries(self):
        # """"""
        # p32_libs = []
        # for lib in self.get_all_libraries_names():
            # p32 = os.path.exists(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", lib, "p32"))
            # if os.path.exists(p32):
                # p8_libs.append(p32)

        # return p32_libs


    # #----------------------------------------------------------------------
    # def get_pdl_libraries(self):
        # """"""
        # pdl_libs = []
        # for lib in self.get_all_libraries_names():
            # pdls = os.listdir(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", lib, "pdl"))
            # for pdl in pdls:
                # if pdl.endswith(".pdl"):
                    # pdl_libs.append(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", lib, "pdl", pdl))

        # return pdl_libs


    # #----------------------------------------------------------------------
    # def get_pdl32_libraries(self):
        # """"""
        # pdl32_libs = []
        # for lib in self.get_all_libraries_names():
            # pdls = os.listdir(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", lib, "pdl"))
            # for pdl in pdls:
                # if pdl.endswith(".pdl32"):
                    # pdl32_libs.append(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", lib, "pdl", pdl))

        # return pdl32_libs


    #----------------------------------------------------------------------
    def get_library_info(self):
        """"""
        sources = []
        for lib in self.get_all_libraries_paths():
            pinguino = os.path.join(lib, "PINGUINO")
            # enable = os.path.exists(os.path.join(lib, ".disable"))
            parser = RawConfigParser()
            parser.readfp(open(pinguino, "r"))
            info = dict(parser.items("PINGUINO"))
            # info["disable"] = enable
            sources.append(info)

        return sources


    #----------------------------------------------------------------------
    def update_libraries_view(self):
        # self.ConfigLibs.save_config()
        # self.ConfigLibs.load_config()
        sources = self.get_library_info()

        rm_buttons = []
        index = 0
        self.main.tableWidget_user_libraries.setRowCount(0)

        hh = self.main.tableWidget_user_libraries.horizontalHeader()
        hh.resizeSection(0, 250)

        for lib in sources:

            name = lib["name"]
            arch = lib["arch"]
            author = lib["author"]
            description = lib["description"]
            # disable = lib["disable"]
            version = lib.get("version", "0.0")
            repository = lib["repository"]
            url = lib["url"]

            #if not installed: continue
            self.main.tableWidget_user_libraries.setRowCount(self.main.tableWidget_user_libraries.rowCount()+1)

            #name
            checkable = QtGui.QTableWidgetItem()
            # checkable.setFlags(QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsUserCheckable)
            # if disable: checkable.setCheckState(QtCore.Qt.Unchecked)
            # else: checkable.setCheckState(QtCore.Qt.Checked)
            checkable.setToolTip(description)
            # setattr(checkable, "library", "{}-{}".format(name, version))
            checkable.setTextAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignVCenter)
            self.main.tableWidget_user_libraries.setItem(index, 0, checkable)

            #arch
            arch_ = QtGui.QTableWidgetItem()
            arch_.setFlags(QtCore.Qt.ItemIsEnabled)
            arch_.setTextAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignVCenter)
            self.main.tableWidget_user_libraries.setItem(index, 1, arch_)

            #author
            auth = QtGui.QTableWidgetItem()
            auth.setFlags(QtCore.Qt.ItemIsEnabled)
            auth.setTextAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignVCenter)
            self.main.tableWidget_user_libraries.setItem(index, 2, auth)

            #link
            if url:
                web_page = QtGui.QTableWidgetItem()
                web_page.setFlags(QtCore.Qt.ItemIsEnabled)
                brush = QtGui.QBrush(QtGui.QColor(0, 0, 255))
                brush.setStyle(QtCore.Qt.NoBrush)
                web_page.setForeground(brush)
                font = web_page.font()
                font.setUnderline(True)
                web_page.setFont(font)
                web_page.setTextAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignVCenter)
                web_page.setToolTip(url)
                setattr(web_page, "url", url)
                self.main.tableWidget_user_libraries.setItem(index, 3, web_page)

            # #remove
            # rm = QtGui.QTableWidgetItem()
            # rm.setFlags(QtCore.Qt.ItemIsEnabled)
            # rm.setTextAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignVCenter)
            # self.main.tableWidget_user_libraries.setItem(index, 4, rm)


            self.main.tableWidget_user_libraries.item(index, 0).setText("{} {}".format(name, version))
            self.main.tableWidget_user_libraries.item(index, 1).setText(arch)
            self.main.tableWidget_user_libraries.item(index, 2).setText(author)
            if url:
                self.main.tableWidget_user_libraries.item(index, 3).setText("link")

            #remove
            rm_buttons.append(QtGui.QPushButton())
            rm_buttons[-1].setText("Remove")
            self.connect(rm_buttons[-1], QtCore.SIGNAL("clicked()"), self.dummy_event(name))
            self.main.tableWidget_user_libraries.setCellWidget(index, 4, rm_buttons[-1])

            index += 1


    #----------------------------------------------------------------------
    def dummy_event(self, lib):
        """"""
        def button():
            self.remove_lib(lib)
        return button


    #----------------------------------------------------------------------
    def open_link(self, model_index):
        """"""

        # column = model_index.column()
        # if column == 0:
            # self.update_eneables()

        item = self.main.tableWidget_user_libraries.itemFromIndex(model_index)
        url = getattr(item, "url", None)
        if not url: return

        reply = Dialogs.confirm_message(self, QtGui.QApplication.translate("Dialogs", "Do you want open this URL in a new tab?")+"\n"+url)
        if reply: webbrowser.open_new_tab(url)


    # #----------------------------------------------------------------------
    # def update_eneables(self):
        # """"""
        # for index in range(self.main.tableWidget_user_libraries.rowCount()):
            # item = self.main.tableWidget_user_libraries.item(index, 0)
            # if item.checkState() == QtCore.Qt.CheckState.Checked:
                # self.enable_lib(item.library)
            # else:
                # self.disable_lib(item.library)


    # #----------------------------------------------------------------------
    # def enable_lib(self, library):
        # """"""
        # path_enable = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", library, ".disable")
        # if os.path.exists(path_enable):
            # os.remove(path_enable)


    # #----------------------------------------------------------------------
    # def disable_lib(self, library):
        # """"""
        # path_enable = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", library, ".disable")
        # if not os.path.exists(path_enable):
            # enb = open(path_enable, "w")
            # enb.close()

    #----------------------------------------------------------------------
    def remove_lib(self, library):
        """"""
        if library in os.listdir(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr")):
            reply = Dialogs.confirm_message(self, QtGui.QApplication.translate("Dialogs", "Do you want remove this library?")+"\n"+library)
            for dir_ in ["usr", "library_examples", "blocks"]:
                path = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), dir_, library)
                if os.path.exists(path): shutil.rmtree(path)

            self.update_libraries_view()
            self.refresh_libraries()


    #----------------------------------------------------------------------
    def refresh_libraries(self):
        """"""
        self.pinguinoAPI.force_reload_libs()
        PinguinoConfig.update_user_libs(self.pinguinoAPI)
        self.update_reserved_words()
        if hasattr(self, "assistant"): delattr(self, "assistant")
