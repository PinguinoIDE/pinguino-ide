#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os
from ..methods.dialogs import Dialogs

from PySide import QtGui, QtCore

########################################################################
class Files(object):

    #----------------------------------------------------------------------
    def __init__(self):
        """"""
        self.connect(self.main.comboBox_files, QtCore.SIGNAL("currentIndexChanged(int)"), self.change_dir_files)
        self.connect(self.main.treeWidget_explorer, QtCore.SIGNAL("itemExpanded(QTreeWidgetItem*)"), self.expand_tree)
        self.connect(self.main.treeWidget_explorer, QtCore.SIGNAL("itemDoubleClicked(QTreeWidgetItem*,int)"), self.open_from_tree)


    #----------------------------------------------------------------------
    def update_path_files(self, path):

        dir_project = path
        flags = QtCore.Qt.ItemIsSelectable|QtCore.Qt.ItemIsDragEnabled|QtCore.Qt.ItemIsUserCheckable|QtCore.Qt.ItemIsEnabled
        self.main.treeWidget_explorer.clear()
        parent = self.add_new_tree(os.path.basename(dir_project), self.main.treeWidget_explorer, dir_project, flags)
        self.generate_tree(dir_project, parent, levels=1, flags=flags)
        parent.setExpanded(True)


    #----------------------------------------------------------------------
    def generate_tree(self, path, parent, levels=0, currentlevel=1, flags=None):

        if currentlevel > levels:
            if os.listdir(path):
                self.add_new_file_item("empty", parent, "empty", flags=None)
            return

        lisdir = os.listdir(path)

        # Filters

        lisdir = filter(lambda f:not f.startswith("."), lisdir)
        lisdir = filter(lambda f:not f.endswith(".ppde"), lisdir)

        list_dirs = []
        list_files = []

        for item in lisdir:
            fullpath = os.path.join(path, item)
            if os.path.isdir(fullpath):
                list_dirs.append(fullpath)
            elif os.path.isfile(fullpath):
                list_files.append(fullpath)

        list_dirs.sort()
        list_files.sort()

        for dir_ in list_dirs:
            dir_name = os.path.basename(dir_)
            self.generate_tree(dir_, self.add_new_tree(dir_name, parent, dir_, flags), currentlevel=currentlevel+1, levels=levels)

        for file_ in list_files:
            file_name = os.path.basename(file_)
            self.add_new_file_item(file_name, parent, file_, flags)


    #----------------------------------------------------------------------
    def add_new_file_item(self, filename, parent, fullpath, flags):
        """"""
        file_item = QtGui.QTreeWidgetItem(parent)

        file_item.setText(0, filename)
        file_item.path = fullpath

        icon_file = QtGui.QIcon()
        icon_file.addPixmap(QtGui.QPixmap(":/icons/icons/icon_file.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        #icon_file = QtGui.QIcon.fromTheme("text-x-generic")

        icon_file_pde = QtGui.QIcon()
        icon_file_pde.addPixmap(QtGui.QPixmap(":/icons/icons/icon_file_pde.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        #icon_file_pde = QtGui.QIcon.fromTheme("text-x-generic")

        icon_file_py = QtGui.QIcon()
        icon_file_py.addPixmap(QtGui.QPixmap(":/icons/icons/icon_file_py.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        #icon_file_py = QtGui.QIcon.fromTheme("text-x-generic")

        if filename.endswith("py"): file_item.setIcon(0, icon_file_py)
        elif filename.endswith("pde"): file_item.setIcon(0, icon_file_pde)
        else: file_item.setIcon(0, icon_file)

        if flags:
            file_item.setFlags(flags)



    #----------------------------------------------------------------------
    def add_new_tree(self, name, parent, fullpath, flags):
        """"""
        tree = QtGui.QTreeWidgetItem(parent)
        tree.setText(0, name)
        tree.path = fullpath
        icon_dir = QtGui.QIcon()
        icon_dir.addPixmap(QtGui.QPixmap(":/icons/icons/icon_dir.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        tree.setIcon(0, icon_dir)

        if flags:
            tree.setFlags(flags)

        return tree


    #----------------------------------------------------------------------
    def expand_tree(self, event):
        """"""
        dir_name = event.text(0)
        dir_ = event.path
        parent = event

        if event.child(0).path == "empty":
            self.generate_tree(dir_, parent, levels=1)
            event.removeChild(event.child(0))


    #----------------------------------------------------------------------
    def open_from_tree(self, event, int_):

        if os.path.isfile(event.path):
            self.open_file_from_path(filename=event.path)


    #----------------------------------------------------------------------
    def jump_dir_files(self, list_widget_item):
        if getattr(list_widget_item, "type_file") == "dir":
            self.update_path_files(getattr(list_widget_item, "path_file"))
        if getattr(list_widget_item, "type_file") == "file":
            self.open_file_from_path(filename=getattr(list_widget_item, "path_file"))


    #----------------------------------------------------------------------
    def change_dir_files(self, index):
        to_dir = ["Examples", "Dir", "Home", "Other"][index]

        if to_dir == "Examples":
            self.update_path_files(os.path.join(os.getenv("PINGUINO_USER_PATH"), "examples"))

        elif to_dir == "Home":
            self.update_path_files(QtCore.QDir.home().path())

        elif to_dir == "Dir":
            editor = self.main.tabWidget_files.currentWidget()
            dir_ = getattr(editor, "path", None)
            if dir_: self.update_path_files(os.path.split(dir_)[0])

        elif to_dir == "Third":
            path = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "examples")
            if os.path.exists(path): self.update_path_files(path)

        elif to_dir == "Other":
            open_dir = Dialogs.set_open_dir(self)
            if open_dir:
                self.update_path_files(open_dir)

