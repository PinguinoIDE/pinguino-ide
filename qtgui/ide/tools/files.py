#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
from ..methods.dialogs import Dialogs
from ..methods.decorators import Decorator

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

        if not type(path) is list: path = [path]

        flags = QtCore.Qt.ItemIsSelectable|QtCore.Qt.ItemIsDragEnabled|QtCore.Qt.ItemIsUserCheckable|QtCore.Qt.ItemIsEnabled
        self.main.treeWidget_explorer.clear()
        for dir_project in path:
            if os.path.exists(dir_project):
                if os.listdir(dir_project):
                    parent = self.add_new_tree(os.path.basename(dir_project), self.main.treeWidget_explorer, dir_project, flags)
                    self.generate_tree(dir_project, parent, levels=1, flags=flags)
                    parent.setExpanded(True)


    #----------------------------------------------------------------------
    def generate_tree(self, path, parent, levels=0, currentlevel=1, flags=None, to_ignore=[], inherits_status=dict()):

        if currentlevel > levels:
            if os.listdir(path):
                self.add_new_file_item("empty", parent, "empty", flags=None)
            return

        if not os.path.exists(path):
            return

        lisdir = os.listdir(path)

        # Filters
        lisdir = filter(lambda f:not f.startswith("."), lisdir)
        lisdir = filter(lambda f:not f.endswith(".ppde"), lisdir)
        lisdir = filter(lambda f:not f.endswith(".hex"), lisdir)
        lisdir = filter(lambda f:not f.endswith("~"), lisdir)

        list_dirs = []
        list_files = []

        for item in lisdir:
            fullpath = os.path.join(path, item)

            if fullpath in to_ignore: continue
            # print path in to_ignore, path, to_ignore

            if os.path.isdir(fullpath):
                list_dirs.append(fullpath)
            elif os.path.isfile(fullpath):
                list_files.append(fullpath)

        list_dirs.sort()
        list_files.sort()

        files_from_tree = []
        for dir_ in list_dirs:
            dir_name = os.path.basename(dir_)
            ft = self.generate_tree(dir_, self.add_new_tree(dir_name, parent, dir_, flags), currentlevel=currentlevel+1, levels=levels, to_ignore=to_ignore, inherits_status=inherits_status)
            if ft: files_from_tree.extend(ft)

        for file_ in list_files:
            file_name = os.path.basename(file_)
            if inherits_status and type(inherits_status) == dict:
                if file_ in inherits_status: check = inherits_status[file_]
                else: check = False
            elif type(inherits_status) == bool:
                check = inherits_status
            else:
                check = None

            if file_name.endswith(".pde") or file_name.endswith(".h") or file_name.endswith(".c") or file_name.endswith(".cpp"):
                self.add_new_file_item(file_name, parent, file_, flags, check)
            else:
                self.add_new_file_item(file_name, parent, file_, flags, None)

        list_files.extend(files_from_tree)
        list_files.sort()

        return list_files


    #----------------------------------------------------------------------
    def add_new_file_item(self, filename, parent, fullpath, flags=None, check=None):
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

        if not check is None:
            if check:
                file_item.setCheckState(0, QtCore.Qt.Checked)
            else:
                file_item.setCheckState(0, QtCore.Qt.Unchecked)



    #----------------------------------------------------------------------
    def add_new_tree(self, name, parent, fullpath, flags=None):
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
    def expand_tree(self, item):
        """"""
        dir_name = item.text(0)
        dir_ = item.path
        parent = item

        if item.child(0) and item.child(0).path == "empty":
            self.generate_tree(dir_, parent, levels=1)
            item.removeChild(item.child(0))


    #----------------------------------------------------------------------
    def open_from_tree(self, event, int_):

        if os.path.isfile(event.path):
            self.ide_open_file_from_path(filename=event.path)


    #----------------------------------------------------------------------
    def jump_dir_files(self, list_widget_item):
        if getattr(list_widget_item, "type_file") == "dir":
            self.update_path_files(getattr(list_widget_item, "path_file"))
        if getattr(list_widget_item, "type_file") == "file":
            self.ide_open_file_from_path(filename=getattr(list_widget_item, "path_file"))


    #----------------------------------------------------------------------
    def change_dir_files(self, index):
        to_dir = ["Examples", "Dir", "Home", "Other"][index]

        if to_dir == "Examples":
            self.update_path_files([os.path.join(os.getenv("PINGUINO_USER_PATH"), "examples"),
                                    os.path.join(os.getenv("PINGUINO_USER_PATH"), "graphical_examples"),
                                    os.path.join(os.getenv("PINGUINO_USER_PATH"), "libraries", "library_examples"),
                                    ])

        elif to_dir == "Home":
            self.update_path_files(QtCore.QDir.home().path())

        elif to_dir == "Dir":
            editor = self.get_current_editor()
            dir_ = getattr(editor, "path", None)
            if dir_: self.update_path_files(os.path.split(dir_)[0])

        # elif to_dir == "Third":
            # path = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "examples")
            # if os.path.exists(path): self.update_path_files(path)

        elif to_dir == "Other":
            open_dir = Dialogs.set_open_dir(self)
            if open_dir:
                self.update_path_files(open_dir)

