#! /usr/bin/python2
#-*- coding: utf-8 -*-

import functools
import time
import os
import logging
import traceback

from PySide import QtCore, QtGui

from .dialogs import Dialogs
from ..child_windows.submit_bug import SubmitBug

########################################################################
class Decorator(object):


    #----------------------------------------------------------------------
    @classmethod
    def files_tab_on_focus(cls):
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
    def requiere_open_files(cls):
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
    def timer(cls, time_):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                name = "timer_" + fn.__name__
                setattr(Pinguino, name, QtCore.QTimer())
                atr = getattr(Pinguino, name)
                QtCore.QObject.connect(atr, QtCore.SIGNAL("timeout()"), lambda :fn(Pinguino, *args, **kwargs))
                atr.start(time_)
                return name, atr
            return wrapped
        return actualdecorator

    #----------------------------------------------------------------------
    @classmethod
    def call_later(cls, time_=100):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                QtCore.QTimer.singleShot(time_, lambda :fn(Pinguino, *args, **kwargs))
            return wrapped
        return actualdecorator

    #----------------------------------------------------------------------
    @classmethod
    def requiere_browser_tab(cls, name):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                current_tab_name = Pinguino.main.tabWidget_browser.currentWidget().objectName()
                Pinguino.main.tabWidget_browser.accessibleName
                if current_tab_name == name:
                    return fn(Pinguino, *args, **kwargs)
                else:
                    #print(name + " not in focus.")
                    return None
            return wrapped
        return actualdecorator


    #----------------------------------------------------------------------
    @classmethod
    def requiere_tools_tab(cls, name):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                current_tab_name = Pinguino.main.tabWidget_tools.currentWidget().objectName()
                if current_tab_name == name:
                    return fn(Pinguino, *args, **kwargs)
                else:
                    #print(name + " not in focus.")
                    return None
            return wrapped
        return actualdecorator


    #----------------------------------------------------------------------
    @classmethod
    def requiere_text_mode(cls):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if not Pinguino.is_graphical() and not Pinguino.is_widget():


                    editor = Pinguino.get_tab().currentWidget()
                    if editor.objectName() == "PinguinoCodeEditor":
                        return fn(Pinguino, *args, **kwargs)
                    else: return None
                else: return None
            return wrapped
        return actualdecorator

    #----------------------------------------------------------------------
    @classmethod
    def requiere_graphical_mode(cls):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if Pinguino.is_graphical():
                    return fn(Pinguino, *args, **kwargs)
                else: return None
            return wrapped
        return actualdecorator

    #----------------------------------------------------------------------
    @classmethod
    def requiere_main_focus(cls):
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
    def requiere_line_edit_content(cls, line_edit):
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
    def requiere_file_saved(cls):
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
    def requiere_can_compile(cls):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if os.getenv("PINGUINO_CAN_COMPILE") == "True":
                    return fn(Pinguino, *args, **kwargs)
                else:
                    #print("Missing libraries and/or compiler")
                    Dialogs.warning_message(Pinguino, QtGui.QApplication.translate("Dialogs", "Impossible to compile this file, missing libraries and/or compiler."))
                    return None
            return wrapped
        return actualdecorator


    #----------------------------------------------------------------------
    @classmethod
    def update_toolbar(cls):
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
    def connect_features(cls):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if Pinguino.is_graphical():
                    return getattr(Pinguino.PinguinoKIT, fn.func_name)(Pinguino, *args, **kwargs)
                else:
                    return fn(Pinguino, *args, **kwargs)
            return wrapped
        return actualdecorator


    #----------------------------------------------------------------------
    @classmethod
    def if_autocomplete_is_enable(cls):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if Pinguino.main.actionAutocomplete.isChecked():
                    return fn(Pinguino, *args, **kwargs)
                #else: return None
            return wrapped
        return actualdecorator

    # #----------------------------------------------------------------------
    # @classmethod
    # def debug_time(cls):
        # def actualdecorator(fn):
            # @functools.wraps(fn)
            # def wrapped(Pinguino, *args, **kwargs):
                # inicio = time.time()
                # retorno = fn(Pinguino, *args, **kwargs)
                # fin = time.time()
                # Pinguino.write_log("debug: " + fn.__name__ + "\nTime: %.7fs\n" %(fin - inicio))
                # return retorno
            # return wrapped
        # return actualdecorator


    #----------------------------------------------------------------------
    @classmethod
    def clear_highlighted_lines(cls):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                Pinguino.clear_highlighted_lines()
                return fn(Pinguino, *args, **kwargs)
            return wrapped
        return actualdecorator


    ##----------------------------------------------------------------------
    #@classmethod
    #def thread(cls):
        #def actualdecorator(fn):
            #@functools.wraps(fn)
            #def wrapped(Pinguino, *args, **kwargs):
                #t = threading.Thread(target=fn, args=(Pinguino, ))
                #t.start()
                #retorno = t
                #return retorno
            #return wrapped
        #return actualdecorator

    ##----------------------------------------------------------------------
    #@classmethod
    #def call_delay(cls, key, delay):
        #def actualdecorator(fn):
            #@functools.wraps(fn)
            #def wrapped(Pinguino, *args, **kwargs):

                #last_time = getattr(Pinguino, "key_delay_"+key, False)

                #if last_time:
                    #if time.time() - last_time >= delay:
                        #print("tiempo")
                        #setattr(Pinguino, "key_delay_"+key, time.time())
                        #return fn(Pinguino, *args, **kwargs)
                    #else:
                        #print("no es tiempo")
                        #print(time.time())
                        #print(last_time)
                        #print(time.time() - last_time)

                #else:
                    #print("sin inicializar")
                    #setattr(Pinguino, "key_delay_"+key, time.time())
                    #return fn(Pinguino, *args, **kwargs)


            #return wrapped
        #return actualdecorator



    #----------------------------------------------------------------------
    @classmethod
    def debug_method(cls):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):

                # Pinguino.clear_highlighted_lines()
                try:
                    return fn(Pinguino, *args, **kwargs)
                except:
                    error = traceback.format_exc()
                    logging.debug(error)
                    cls.submmit_bug = SubmitBug(Pinguino, error)
                    cls.submmit_bug.show()
                    return

            return wrapped
        return actualdecorator





    #----------------------------------------------------------------------
    @classmethod
    def alert_tab(cls, widget_name):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if Pinguino.get_tab().count():

                    widget = getattr(Pinguino.main, widget_name)

                    tab = widget.parent().parent()
                    index = tab.indexOf(widget)
                    tabbar = tab.tabBar()
                    orig_c = tabbar.tabTextColor(0)

                    for i in range(0, 8, 2):
                        QtCore.QTimer.singleShot(300*i, lambda :tabbar.setTabTextColor(index, orig_c))
                        QtCore.QTimer.singleShot((300*i)+300, lambda :tabbar.setTabTextColor(index, QtCore.Qt.red))
                    QtCore.QTimer.singleShot(500*20, lambda :tabbar.setTabTextColor(index, orig_c))

                    return fn(Pinguino, *args, **kwargs)
                else:
                    #print("No files are open.")
                    return None
            return wrapped
        return actualdecorator

    #----------------------------------------------------------------------
    @classmethod
    def show_tab(cls, widget_name):
        def actualdecorator(fn):
            @functools.wraps(fn)
            def wrapped(Pinguino, *args, **kwargs):
                if Pinguino.get_tab().count():

                    widget = getattr(Pinguino.main, widget_name)

                    tab = widget.parent().parent()
                    index = tab.indexOf(widget)
                    tab.setCurrentIndex(index)

                    return fn(Pinguino, *args, **kwargs)
                else:
                    #print("No files are open.")
                    return None
            return wrapped
        return actualdecorator