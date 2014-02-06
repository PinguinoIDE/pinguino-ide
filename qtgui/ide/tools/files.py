#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

from PySide import QtGui, QtCore

########################################################################
class Files(object):
    
    
    #----------------------------------------------------------------------
    @classmethod
    def update_path_files(self, path, listWidget, label, exclude=None):
        
        path = os.path.expanduser(path)
        paths = filter(lambda child: not child.startswith("."), os.listdir(path))
        parent = os.path.split(path)[0]
        
        listWidget.clear()
        
        dirs = []
        files = []
        for child in paths:
            if os.path.isfile(os.path.join(path, child)): files.append(os.path.join(path, child))
            elif os.path.isdir(os.path.join(path, child)): dirs.append(os.path.join(path, child))
            
        dirs.sort()
        files.sort()
        
        files = filter(lambda file:not file.endswith(".pyc"), files)
        files = filter(lambda file:not file.endswith(".hex"), files)
        if exclude: files = filter(lambda file:not file.endswith(exclude), files)
        
        
        icon_dir = QtGui.QIcon()
        icon_dir.addPixmap(QtGui.QPixmap(":/icons/icons/icon_dir.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        #icon_dir = QtGui.QIcon.fromTheme("folder")
        
        
        icon_file = QtGui.QIcon()
        icon_file.addPixmap(QtGui.QPixmap(":/icons/icons/icon_file.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        #icon_file = QtGui.QIcon.fromTheme("text-x-generic")
        
        icon_file_pde = QtGui.QIcon()
        icon_file_pde.addPixmap(QtGui.QPixmap(":/icons/icons/icon_file_pde.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        #icon_file_pde = QtGui.QIcon.fromTheme("text-x-generic")
        
        icon_file_py = QtGui.QIcon()
        icon_file_py.addPixmap(QtGui.QPixmap(":/icons/icons/icon_file_py.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        #icon_file_py = QtGui.QIcon.fromTheme("text-x-generic")
        
        
        item = QtGui.QListWidgetItem(listWidget)
        item.setIcon(icon_dir)
        setattr(item, "type_file", "dir")
        setattr(item, "path_file", parent)
        listWidget.item(0).setText("..")        
        
        index = 1
        for dir in dirs:
            name = os.path.split(dir)[1]
            item = QtGui.QListWidgetItem(listWidget)
            item.setIcon(icon_dir)
            setattr(item, "type_file", "dir")
            setattr(item, "path_file", dir)
            item.setToolTip(dir)
            listWidget.item(index).setText(name)
            index += 1
            
        for file in files:
            name = os.path.split(file)[1]
            item = QtGui.QListWidgetItem(listWidget)
            if name.endswith(".pde") or name.endswith(".gpde"): item.setIcon(icon_file_pde)
            elif name.endswith(".py"): item.setIcon(icon_file_py)
            else: item.setIcon(icon_file)
            setattr(item, "type_file", "file")
            setattr(item, "path_file", file)
            item.setToolTip(file)
            listWidget.item(index).setText(name)
            index += 1
        
                
        path_label = path
        if len(path_label) > 40: path_label = "..." + path_label[-40:]
        label.setText(path_label)
        label.setToolTip(path)
        

