#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide import QtGui, QtCore

import os

from ..methods.decorators import Decorator
from ..methods.dialogs import Dialogs

from ...pinguino_api.tools import Debugger

import logging


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
        self.generate_tree(dir_project)
        pass



    #----------------------------------------------------------------------
    def generate_tree(self, startpath):
        """"""
        parent = self.main.treeWidget_projects
        # widget.itemDoubleClicked = self.itemClicked


        # widget.itemClicked.im_func = self.itemClicked
        # widget.itemDoubleClicked.im_func = self.itemClicked

        # self.connect(self.main.treeWidget_projects, QtCore.SIGNAL("itemClicked(QTreeWidgetItem*,int)"), self.itemClicked)
        # self.connect(self.main.treeWidget_projects, QtCore.SIGNAL("itemDoubleClicked(QTreeWidgetItem*,int)"), self.itemClicked)


        # widget.itemActivated.connect(self.itemClicked)
        # widget.itemEntered.connect(self.itemClicked)
        # widget.ite.connect(self.itemClicked)

        for root, dirs, files in os.walk(startpath):
            level = root.replace(startpath, '').count(os.sep)
            # indent = ' ' * 4 * (level)
            # print('{}{}/'.format(indent, os.path.basename(root)))
            parent = self.add_new_tree(os.path.basename(root), parent)
            # subindent = ' ' * 4 * (level + 1)
            for filename in files:
                self.add_new_file_item(filename, parent)



    #----------------------------------------------------------------------
    def add_new_file_item(self, filename, parent):
        """"""
        file_item = QtGui.QTreeWidgetItem(parent)

        file_item.setText(0, filename)

        if filename.endswith(".pde"):
            preproces_as = "define_" + filename.replace(".pde", ".h")
            compile_as = filename.replace(".pde", ".c")

            file_item.setText(1, preproces_as)
            file_item.setCheckState(1, QtCore.Qt.Unchecked)

            file_item.setText(2, compile_as)
            file_item.setCheckState(2, QtCore.Qt.Unchecked)


        elif filename.endswith(".h"):
            # preproces_as = ""
            compile_as = "define.h"

            # file_item.setText(1, preproces_as)
            # file_item.setCheckState(1, QtCore.Qt.Unchecked)

            file_item.setText(2, compile_as)
            file_item.setCheckState(2, QtCore.Qt.Unchecked)


        elif filename.endswith(".c"):
            # preproces_as = ""
            compile_as = "user.c"

            # file_item.setText(1, preproces_as)
            # file_item.setCheckState(1, QtCore.Qt.Unchecked)

            file_item.setText(2, compile_as)
            file_item.setCheckState(2, QtCore.Qt.Unchecked)




        file_item.setFlags(QtCore.Qt.ItemIsSelectable|QtCore.Qt.ItemIsEditable|QtCore.Qt.ItemIsDragEnabled|QtCore.Qt.ItemIsUserCheckable|QtCore.Qt.ItemIsEnabled)



    #----------------------------------------------------------------------
    def add_new_tree(self, name, parent):
        """"""
        tree = QtGui.QTreeWidgetItem(parent)
        tree.setText(0, name)

        return tree



