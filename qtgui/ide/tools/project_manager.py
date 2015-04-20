#! /usr/bin/python2
#-*- coding: utf-8 -*-

from PySide import QtGui, QtCore

import os
import logging

from ..methods.decorators import Decorator
from ..methods.dialogs import Dialogs


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
    def __init__(self):
        """"""
        self.connect(self.main.actionAdd_existing_directory, QtCore.SIGNAL("triggered()"), self.add_existing_directory)
        self.connect(self.main.actionNew_project, QtCore.SIGNAL("triggered()"), self.new_project)
        self.connect(self.main.pushButton_newproject, QtCore.SIGNAL("clicked()"), self.new_project)
        self.connect(self.main.actionSave_project, QtCore.SIGNAL("triggered()"), self.save_project)
        self.connect(self.main.actionAdd_current_file, QtCore.SIGNAL("triggered()"), self.add_current_file)

        self.connect(self.main.treeWidget_projects, QtCore.SIGNAL("itemExpanded(QTreeWidgetItem*)"), self.expand_tree)
        self.connect(self.main.treeWidget_projects, QtCore.SIGNAL("itemClicked(QTreeWidgetItem*,int)"), self.open_from_tree)
        # self.connect(self.main.treeWidget_projects, QtCore.SIGNAL("itemDoubleClicked(QTreeWidgetItem*,int)"), self.open_from_tree)



    #----------------------------------------------------------------------
    def open_project(self):
        """"""

        # self.update_project_status(project)


    #----------------------------------------------------------------------
    def add_existing_directory(self):
        """"""
        dir_project = Dialogs.set_open_dir(self)
        flags = QtCore.Qt.ItemIsSelectable|QtCore.Qt.ItemIsEditable|QtCore.Qt.ItemIsDragEnabled|QtCore.Qt.ItemIsUserCheckable|QtCore.Qt.ItemIsEnabled
        parent = self.add_new_tree(os.path.basename(dir_project), self.main.treeWidget_projects, dir_project, flags)
        self.generate_tree(dir_project, parent, levels=50, flags=flags)
        parent.setExpanded(True)
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
    def set_project_name(self):
        """"""
        project_name = Dialogs.get_text(self, "Project name", default="untitled_project")
        self.main.treeWidget_projects.setHeaderLabel(project_name)
        return project_name
        # self.main.treeWidget_projects.setHeaderLabel(project_name)


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
        self.update_project_status(project_name)
        self.project_saved = False



    #----------------------------------------------------------------------
    def save_project(self, silent=False):
        """"""
        if self.project_saved:
            self.ConfigProject.write(open(self.ConfigProject.filename, "w"))

        elif not silent:
            save = Dialogs.set_save_file(self, self.get_project_name()+".ppde")
            if save[0]:
                path, filename = save
                self.ConfigProject.filename = path
                self.ConfigProject.write(open(self.ConfigProject.filename, "w"))
                self.project_saved = True


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


    #----------------------------------------------------------------------
    def update_project_status(self, project="reload"):
        """"""
        if project == "reload": pass
        elif project: os.environ["PINGUINO_PROJECT"] = project
        else: os.environ["PINGUINO_PROJECT"] = ""

        self.main.treeWidget_projects.setVisible(bool(project))
        self.main.pushButton_newproject.setVisible(not bool(project))

        self.main.treeWidget_projects.contextMenuEvent = self.project_context_menu


    #----------------------------------------------------------------------
    def get_current_item(self):
        """"""
        return self.main.treeWidget_projects.currentItem()




    #----------------------------------------------------------------------
    def project_context_menu(self, event):
        menu = QtGui.QMenu()

        if os.path.isfile(self.get_current_item().path):
            menu.addAction("Set as main upload file", self.set_as_main_upload)

        menu.addSeparator()

        menu.setStyleSheet("""
        QMenu {
            font-family: inherit;
            font-weight: normal;
            }

        """)

        menu.exec_(event.globalPos())


    #----------------------------------------------------------------------
    def set_as_main_upload(self):
        """"""
        item = self.get_current_item()
        item.setForeground(0, QtGui.QColor("#FF0000"))

        if not self.ConfigProject.has_section("Settings"): self.ConfigProject.add_section("Settings")
        self.ConfigProject.set("Settings", "main_upload", item.path)
        self.save_project(True)