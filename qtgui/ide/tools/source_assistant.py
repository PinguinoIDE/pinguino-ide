#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import pickle

from ..methods.decorators import Decorator

from PySide import QtCore


########################################################################
class SourceAssistant(object):

    #----------------------------------------------------------------------
    def __init__(self):
        """"""

        self.connect(self.main.label_assistant_library, QtCore.SIGNAL("linkActivated(QString)"), self.load_source_library)

        with open(os.path.join(os.getenv("PINGUINO_USER_PATH"), "reserved.pickle"), "rb") as file_reserved:
            self.helpers = pickle.load(file_reserved)["assistant"]

        self.update_assistant()


    #----------------------------------------------------------------------
    @Decorator.timer(1000)
    @Decorator.requiere_open_files()
    # @Decorator.requiere_text_mode()
    @Decorator.requiere_tools_tab("SourceAssistant")
    @Decorator.requiere_main_focus()
    def update_assistant(self):
        """"""
        editor = self.get_current_editor()
        tc = editor.text_edit.textCursor()
        tc.movePosition(tc.EndOfWord, tc.MoveAnchor)
        editor.text_edit.smart_under_selection(tc)
        selected = tc.selectedText().split()

        if not selected: return
        lib = selected[-1]


        if lib in self.helpers:


            self.main.label_assistant_name.setText(lib)
            self.main.label_assistant_args.setText(self.helpers[lib]["args"])
            self.main.label_assistant_return.setText(self.helpers[lib]["return"])

            library = """<html><head/><body><p><a href="{}"><span style=" text-decoration: none; color:#2980b9;">{}</span></a></p></body></html>"""
            self.main.label_assistant_library.setText(library.format(self.helpers[lib]["filename"], self.helpers[lib]["filename"]))

            self.main.plainTextEdit_assistant.clear()
            self.main.plainTextEdit_assistant.insertPlainText(self.helpers[lib]["content"])


    #----------------------------------------------------------------------
    def load_source_library(self, name):
        """"""

        core = os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "p8", "include", "pinguino", "core")
        libraries = os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "p8", "include", "pinguino", "libraries")

        if name in os.listdir(core): path = os.path.join(core, name)
        elif name in os.listdir(libraries): path = os.path.join(libraries, name)

        self.ide_open_file_from_path(filename=path, readonly=True)

