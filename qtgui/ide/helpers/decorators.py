#!/usr/bin/env python
#-*- coding: utf-8 -*-

import functools

from PySide import QtCore

from .dialogs import Dialogs

########################################################################
class Decorator(object):
    """"""

    #----------------------------------------------------------------------
    @classmethod
    def files_tab_on_focus(self):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                Pinguino.main.actionSwitch_ide.setChecked(False)
                Pinguino.main.tabWidget_files.setVisible(True)
                Pinguino.main.tabWidget_graphical.setVisible(False)
                Pinguino.tab_changed()
                #Pinguino.main.frame_logo.setVisible(False)
                return fn(Pinguino, *args, **kwargs)
            return wrapped
        return actualdecorator

    #----------------------------------------------------------------------
    @classmethod
    def requiere_open_files(self):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if Pinguino.get_tab().count():
                    return fn(Pinguino, *args, **kwargs)
                else:
                    #print("No files are open.")
                    return None
            return wrapped
        return actualdecorator
    
    #----------------------------------------------------------------------
    @classmethod
    def timer(self, time):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                name = "timer_" + fn.__name__
                setattr(Pinguino, name, QtCore.QTimer())
                atr = getattr(Pinguino, name)
                QtCore.QObject.connect(atr, QtCore.SIGNAL("timeout()"), lambda :fn(Pinguino, *args, **kwargs))                
                atr.start(time)
                return name, atr
            return wrapped
        return actualdecorator
    
    #----------------------------------------------------------------------
    @classmethod
    def requiere_browser_tab(self, name):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                current_tab_name = Pinguino.main.tabWidget_browser.tabText(Pinguino.main.tabWidget_browser.currentIndex())
                if current_tab_name == name:
                    return fn(Pinguino, *args, **kwargs)
                else:
                    #print(name + " not in focus.")
                    return None
            return wrapped
        return actualdecorator
    
    
    #----------------------------------------------------------------------
    @classmethod
    def requiere_tools_tab(self, name):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                current_tab_name = Pinguino.main.tabWidget_tools.tabText(Pinguino.main.tabWidget_tools.currentIndex())
                if current_tab_name == name:
                    return fn(Pinguino, *args, **kwargs)
                else:
                    #print(name + " not in focus.")
                    return None
            return wrapped
        return actualdecorator
    
    
    #----------------------------------------------------------------------
    @classmethod
    def requiere_text_mode(self):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if not Pinguino.is_graphical() and not Pinguino.is_widget():
                    return fn(Pinguino, *args, **kwargs)
                else: return None
            return wrapped
        return actualdecorator

    
    #----------------------------------------------------------------------
    @classmethod
    def requiere_main_focus(self):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if Pinguino.isActiveWindow():
                    return fn(Pinguino, *args, **kwargs)
                else:
                    #print("Main inactive.")
                    return None
            return wrapped
        return actualdecorator
    
    #----------------------------------------------------------------------
    @classmethod
    def requiere_line_edit_content(self, line_edit):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if getattr(getattr(Pinguino, line_edit.split(".")[0]), line_edit.split(".")[1]).text():
                    return fn(Pinguino, *args, **kwargs)
                else:
                    return None
            return wrapped
        return actualdecorator
    
    #----------------------------------------------------------------------
    @classmethod
    def requiere_file_saved(self):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if Pinguino.is_graphical():
                    saved = Pinguino.PinguinoKIT.save_file(*args, **kwargs)
                    if saved: fn(Pinguino, *args, **kwargs)
                    else: Dialogs.save_before_compile(Pinguino)
                else:
                    saved = Pinguino.save_file(*args, **kwargs)
                    if saved: fn(Pinguino, *args, **kwargs)
                    else: Dialogs.save_before_compile(Pinguino)             
            return wrapped
        return actualdecorator
    
    
    #----------------------------------------------------------------------
    @classmethod
    def update_toolbar(self):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                count = Pinguino.get_tab().count()
                Pinguino.main.actionCompile.setEnabled(bool(count))
                Pinguino.main.actionClose_file.setEnabled(bool(count))
                Pinguino.main.actionSearch.setEnabled(bool(count))
                Pinguino.main.actionSearch_and_replace.setEnabled(bool(count))
                Pinguino.main.actionPaste.setEnabled(bool(count))
                Pinguino.main.actionUndo.setEnabled(bool(count))
                Pinguino.main.actionRedo.setEnabled(bool(count))
                
                if not count:
                    Pinguino.main.actionUpload.setEnabled(False)
                    
                if Pinguino.is_widget():
                    Pinguino.main.actionSearch.setEnabled(False)
                    Pinguino.main.actionSearch_and_replace.setEnabled(False)
                    Pinguino.main.actionCut.setEnabled(False)
                    Pinguino.main.actionCopy.setEnabled(False)
                    Pinguino.main.actionPaste.setEnabled(False)
                    Pinguino.main.actionUndo.setEnabled(False)
                    Pinguino.main.actionRedo.setEnabled(False)
                    Pinguino.main.actionCompile.setEnabled(False)                 
                    
                
                return fn(Pinguino, *args, **kwargs)
            return wrapped
        return actualdecorator
    
    
    #----------------------------------------------------------------------
    @classmethod
    def connect_features(self):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if Pinguino.is_graphical():
                    return getattr(Pinguino.PinguinoKIT, fn.func_name)(Pinguino, *args, **kwargs)
                else:
                    return fn(Pinguino, *args, **kwargs)
            return wrapped
        return actualdecorator
    
    