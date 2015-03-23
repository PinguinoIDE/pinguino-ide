#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide import QtGui, QtCore

import os

from ..methods.decorators import Decorator
from ..methods.dialogs import Dialogs

from ...pinguino_api.tools import Debugger

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
else:
    #Python2
    from ConfigParser import RawConfigParser


########################################################################
class ProjectManager(object):
    """"""

    #----------------------------------------------------------------------
    def open_project(self):
        """"""


    #----------------------------------------------------------------------
    @Debugger.debug_method
    def add_existing_directory(self):
        """"""
        dir_project = Dialogs.set_open_dir(self)
        parent = self.add_new_tree(os.path.basename(dir_project), self.main.treeWidget_projects)
        self.generate_tree(dir_project, parent)
        self.add_dir_to_config(dir_project)


    #----------------------------------------------------------------------
    def add_current_file(self):
        """"""
        editor = self.get_tab().currentWidget()
        index = self.get_tab().indexOf(editor)
        filename = self.get_tab().tabText(index)
        self.add_file_to_config(editor.path)
        parent = self.main.treeWidget_projects
        self.add_new_file_item(filename, parent)


    #----------------------------------------------------------------------
    def generate_tree(self, path, parent):
        """"""
        lisdir = os.listdir(path)

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
            self.generate_tree(dir_, self.add_new_tree(dir_name, parent))

        for file_ in list_files:
            file_name = os.path.basename(file_)
            self.add_new_file_item(file_name, parent)


    #----------------------------------------------------------------------
    def add_new_file_item(self, filename, parent):
        """"""
        file_item = QtGui.QTreeWidgetItem(parent)

        file_item.setText(0, filename)

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

        file_item.setFlags(QtCore.Qt.ItemIsSelectable|QtCore.Qt.ItemIsEditable|QtCore.Qt.ItemIsDragEnabled|QtCore.Qt.ItemIsUserCheckable|QtCore.Qt.ItemIsEnabled)



    #----------------------------------------------------------------------
    def add_new_tree(self, name, parent):
        """"""
        tree = QtGui.QTreeWidgetItem(parent)
        tree.setText(0, name)
        icon_dir = QtGui.QIcon()
        icon_dir.addPixmap(QtGui.QPixmap(":/icons/icons/icon_dir.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        tree.setIcon(0, icon_dir)

        return tree


    #----------------------------------------------------------------------
    def set_project_name(self):
        """"""
        project_name = Dialogs.get_text(self, "Project name", default="untitled_project")
        self.main.treeWidget_projects.setHeaderLabel(project_name)
        self.main.treeWidget_projects.setHeaderLabel(project_name)


    #----------------------------------------------------------------------
    def get_project_name(self):
        """"""
        return self.main.treeWidget_projects.headerItem().text(0)



    #----------------------------------------------------------------------
    def new_project(self):
        """"""
        self.ConfigProject = RawConfigParser()
        project_name = self.set_project_name()

        self.main.tabWidget_tools.setCurrentIndex(1)
        add_dir = Dialogs.confirm_message(self, QtGui.QApplication.translate("Dialogs", "Do you want add an existing directory now?"))
        if add_dir: self.add_existing_directory()


    #----------------------------------------------------------------------
    def save_project(self):
        """"""
        save = Dialogs.set_save_file(self, self.get_project_name()+".ppde")
        if save[0]:
            path, filename = save
            self.ConfigProject.filename = path
            self.ConfigProject.write(open(self.ConfigProject.filename, "w"))


    #----------------------------------------------------------------------
    def add_dir_to_config(self, dirpath):
        """"""
        if not self.ConfigProject.has_section("Dirs"): self.ConfigProject.add_section("Dirs")
        options = self.ConfigProject.options("Dirs")

        index = 0
        while "dir_%d"%index in options:
            index += 1

        self.ConfigProject.set("Dirs", "dir_%d"%index, dirpath)

        if hasattr(self.ConfigProject, "filename"):
            self.ConfigProject.write(open(self.ConfigProject.filename, "w"))



    #----------------------------------------------------------------------
    def add_file_to_config(self, dirpath):
        """"""
        if not self.ConfigProject.has_section("Files"): self.ConfigProject.add_section("Files")
        options = self.ConfigProject.options("Files")


        index = 0
        while "file_%d"%index in options:
            index += 1

        self.ConfigProject.set("Files", "file_%d"%index, dirpath)
        if hasattr(self.ConfigProject, "filename"):
            self.ConfigProject.write(open(self.ConfigProject.filename, "w"))



