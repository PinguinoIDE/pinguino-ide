#!/usr/bin/env python
#-*- coding: utf-8 -*-

import codecs
import os
import re
from ConfigParser import RawConfigParser

from PySide import QtGui, QtCore

from .metodos import Metodos
from .work_area import WorkArea
from .constant import FONTS_DIR, FORMAT, INTRO_CODE, TAB_NAME
from ..bloques.color import setColor
from ...frames.grafical_widget import Ui_Form_graphical
from ...ide.helpers.dialogs import Dialogs

########################################################################
class GraphicalIDE(Metodos):
    
    #----------------------------------------------------------------------
    def __init__(self, ide):
        """"""
        self.main = ide.main
        self.ide = ide
        
        self.load_fonts()
        
        self.colorTheme = "['3a7fff', 'ffaa00', 'a861ff', 'bca6ff', '8ebbff', '8ebbff']"
        
        self.colorTheme = self.colorTheme.replace("[", "").replace("]", "").replace("'", "").replace(" ", "")
        self.colorTheme = self.colorTheme.split(",")

        self.colorEspecific = {"linear": self.colorTheme[0],
                               "function": self.colorTheme[1],
                               "nested": self.colorTheme[2],
                               "insidebool": self.colorTheme[3],
                               "inside": self.colorTheme[4],
                               "frame": self.colorTheme[5],}
        
        for key in self.colorEspecific.keys():
            setColor(self.colorEspecific[key], key)
            
        self.colorEspecific["output"] = self.colorTheme[4]
        self.colorEspecific["output-bool"] = self.colorTheme[3]
        self.colorEspecific["frame-edit"] = self.colorTheme[0]
        self.colorEspecific["frame-plot"] = self.colorTheme[0]


        self.highlightedRect = QtCore.QRect()
        self.inPlace = 0
        self.posY = 20
        self.openID = 1
        
        self.fileName = None
        
        self.listBlocks = []
        #self.listSpaces = 0
        
        self.setupBloques()
        
        self.main.tabWidget_graphical.setCurrentIndex(0)
        self.main.tabWidget_graphical.setMaximumSize(10000, 10e6)
       
        for index in range(self.main.tabWidget_blocks.count()):
            if self.main.tabWidget_blocks.tabText(index) == "Pinguino":
                self.main.tabWidget_blocks.setCurrentIndex(index)
                break
        
    
    #----------------------------------------------------------------------
    def new_file(self, *args, **kwargs):
        """"""
        path = kwargs.get("filename", self.ide.__get_name__(ext=".gpde"))
        filename = os.path.split(path)[1]        
        
        editor = QtGui.QWidget()
        
        widget = Ui_Form_graphical()
        widget.setupUi(editor)
        widget.widget.setMinimumSize(QtCore.QSize(2000, 2000))
        
        main_widget = widget.widget
        
        scroll_area = widget.scrollArea
        
        editor.scroll_area = widget.scrollArea
        
        area = WorkArea(parent=main_widget,
                        scroll=scroll_area,
                        widget=widget, 
                        frame=self, 
                        ide=self.ide)

        editor.graphical_area = area
        
        self.main.tabWidget_graphical.addTab(editor, filename)
              
        self.main.tabWidget_graphical.setCurrentWidget(editor) 
        index = self.main.tabWidget_graphical.currentIndex()  
        self.main.tabWidget_graphical.setTabText(index, filename[:-1])
        
    
    
    #----------------------------------------------------------------------
    def tab_changed(self, *args, **kwargs):
        """"""
        self.main.tabWidget_graphical.setVisible(self.main.tabWidget_graphical.count() > 0)
        self.main.frame_logo.setVisible(not self.main.tabWidget_graphical.count() > 0)
        self.main.actionClose_file.setEnabled(self.main.tabWidget_graphical.count() > 0)
            
        index = self.main.tabWidget_graphical.currentIndex()
        filename = self.main.tabWidget_graphical.tabText(index)
        if filename.endswith("*"): self.main.actionSave_file.setEnabled(True)
        else: self.main.actionSave_file.setDisabled(True)
        
        self.ide.__update_current_dir_on_files__()


    #----------------------------------------------------------------------
    def save_file(self, *args, **kwargs):
        """"""
        editor = kwargs.get("editor", None)
        if not editor: editor = self.ide.get_tab().currentWidget()
        index = self.ide.get_tab().indexOf(editor)
        #editor = self.main.tabWidget_graphical.currentWidget()
        #index = self.main.tabWidget_graphical.currentIndex()
        filename = self.main.tabWidget_graphical.tabText(index)
        save_path = getattr(editor, "path", None)
        
        if not save_path:
            save_path, filename = Dialogs.set_save_file(self.ide, filename)
            if not save_path: return False
            setattr(editor, "path", save_path)
            self.main.tabWidget_graphical.setTabText(index, filename)
            self.main.tabWidget_graphical.setTabToolTip(index, save_path) 
            self.ide.setWindowTitle(TAB_NAME+" - "+save_path)        
        
        self.__save_file__(editor=editor)
        return True
    

    #----------------------------------------------------------------------
    def save_as(self, *args, **kwargs):
        """"""
        editor = kwargs.get("editor", None)
        if not editor: editor = self.ide.get_tab().currentWidget()
        index = self.ide.get_tab().indexOf(editor)
        #editor = self.main.tabWidget_graphical.currentWidget()
        #index = self.main.tabWidget_graphical.currentIndex()
        filename = self.main.tabWidget_graphical.tabText(index)
        save_path = getattr(editor, "path", None)
        
        save_path, filename = Dialogs.set_save_file(self.ide, filename)
        if not save_path: return False
        setattr(editor, "path", save_path)
        self.main.tabWidget_graphical.setTabText(index, filename)
        self.main.tabWidget_graphical.setTabToolTip(index, save_path) 
        self.ide.setWindowTitle(TAB_NAME+" - "+save_path)        
    
        self.__save_file__(editor=editor)
        return True
        
    
    
    #----------------------------------------------------------------------
    def get_pinguino_source_code(self):
        """"""
        editor = self.main.tabWidget_graphical.currentWidget()
        editor.graphical_area.updateCode()
        return editor.graphical_area.PinguinoCode
                
                
    #----------------------------------------------------------------------
    def __save_file__(self, *args, **kwargs):
        """"""
        editor = kwargs.get("editor", self.ide.get_tab())
        content = self.get_gpde()
        self.save_raw_parser(content, editor.path)
        self.ide.__text_saved__()
        
        
    #----------------------------------------------------------------------
    def save_raw_parser(self, content, filename):
        """"""
        file_parser = RawConfigParser()
        count = 0
        for block in content:
            count += 1
            name_section = "Block-%d"%count
            file_parser.add_section(name_section)
            for key in block.keys():
                file_parser.set(name_section, key, block[key])
        file_parser.write(file(filename, "w"))
        
        
    #----------------------------------------------------------------------
    def read_raw_parser(self, filename):
        """"""
        blocks_set = []
        
        file_parser = RawConfigParser()
        file_parser.readfp(file(filename, "r"))
        
        sections = file_parser.sections()
        for section in sections:
            options = file_parser.options(section)
            block = {}
            for option in options:
                file_parser.get(section, option)
                value = file_parser.get(section, option)
                if value[0] in ["[", "("]:
                    block[option] = eval(file_parser.get(section, option))
                else:
                    block[option] = file_parser.get(section, option)
                    
        
            blocks_set.append(block)
        return blocks_set
        

        
        
    #----------------------------------------------------------------------
    def get_gpde(self):
        """"""
        editor = self.main.tabWidget_graphical.currentWidget()
        blocks = editor.graphical_area.get_project_blocks()
        
        to_save = []
        for block in blocks:
            to_save.append({
                "basename": block.metadata.basename,
                "position": (block.metadata.pos_.x(), block.metadata.pos_.y()),
                "constructor": block.metadata.get_contructor(),
                "name": block.metadata.name,
                "self_id": self.serialize_widgets([block])[0],
                "to": self.serialize_widgets(block.metadata.to),
                "from": self.serialize_widgets(block.metadata.from_),
                "inside": self.serialize_widgets(block.metadata.inside),
                "nested": self.serialize_widgets(block.metadata.nested),
                })
            
        return to_save
    
    #----------------------------------------------------------------------
    def serialize_widgets(self, widgets):
        """"""
        ser = []
        for widget in widgets:
            name = widget.__str__()
            id_ = re.findall("[.]*0x[\dabcdef]*[.]*", name)[0]
            ser.append(id_)
        return ser
        
    
    
    #----------------------------------------------------------------------
    def open_files(self, *args, **kwargs):
        """"""
        filenames = Dialogs.set_open_file(self.ide, ".gpde")
        for filename in filenames:
            if self.ide.__check_duplicate_file__(filename): continue
            self.new_file(filename)
            editor = self.main.tabWidget_graphical.currentWidget()
            set_blocks = self.read_raw_parser(filename)
            self.load_blocks(set_blocks)
            self.main.tabWidget_graphical.setTabToolTip(self.main.tabWidget_graphical.currentIndex(), filename)
            self.main.tabWidget_graphical.setTabText(self.main.tabWidget_graphical.currentIndex(), os.path.split(filename)[1])       
    
            
        self.ide.tab_changed()
        
        
    #----------------------------------------------------------------------
    def load_blocks(self, set_blocks):
        """"""
        editor = self.main.tabWidget_graphical.currentWidget()
        
        toFitInside = []
        restaurPos = []
        all_icons = []        

        for block in set_blocks:
            name = block["name"]
            args = ["", ""] + block["constructor"]
            pos = QtCore.QPoint(*block["position"])
            basename = block["basename"]
            ID = block["self_id"]
            
            newIcon, pos2 = editor.graphical_area.new_ploq(name, args, pos, basename, ID)
            newIcon.metadata.self_id = ID
            
            newIcon.move(*block["position"])
            newIcon.setCursor(QtGui.QCursor(QtCore.Qt.OpenHandCursor)) 
            
            newIcon.metadata.to = block["to"]
            newIcon.metadata.from_ = block["from"]
            newIcon.metadata.inside = block["inside"]
            newIcon.metadata.nested = block["nested"]
            newIcon.move(pos)
                
        self.replace_id_by_widgets()
        
        for block in editor.graphical_area.get_project_blocks():
            if len(block.metadata.inside) > 0:
                toFitInside.append([block, block.metadata.inside])        
                
        for parent, toFit in toFitInside:
            for ins in toFit:
                wdg = self.get_widget_from_id(ins)
                wdg.metadata.add_parent([parent, wdg.metadata.widget], force=True)
                
    
    #----------------------------------------------------------------------
    def replace_id_by_widgets(self):
        """"""
        editor = self.main.tabWidget_graphical.currentWidget()
        for block in editor.graphical_area.get_project_blocks():
            block.metadata.to = map(self.get_widget_from_id, block.metadata.to)
            block.metadata.from_ = map(self.get_widget_from_id, block.metadata.from_)
            block.metadata.inside = map(self.get_widget_from_id, block.metadata.inside)
            block.metadata.nested = map(self.get_widget_from_id, block.metadata.nested)

    #----------------------------------------------------------------------
    def get_widget_from_id(self, id_):
        """"""
        if type(id_) != type(""): return id_
        editor = self.main.tabWidget_graphical.currentWidget()
        blocks = editor.graphical_area.get_project_blocks()
        
        for block in blocks:
            if block.metadata.self_id == id_:
                return block
        
        
    #----------------------------------------------------------------------
    def load_fonts(self):
        for dir_font in os.listdir(FONTS_DIR):
            for ttf in filter(lambda file:file.endswith(".ttf") or file.endswith(".otf"), os.listdir(os.path.join(FONTS_DIR, dir_font))):
                QtGui.QFontDatabase.addApplicationFont(os.path.join(FONTS_DIR, dir_font, ttf))
        
        
    #----------------------------------------------------------------------
    def open_file_from_path(self, *args, **kwargs):
        """"""
        filename = kwargs["filename"]

        if self.ide.__check_duplicate_file__(filename): return
        self.new_file(filename)
        editor = self.main.tabWidget_graphical.currentWidget()
        set_blocks = self.read_raw_parser(filename)
        self.load_blocks(set_blocks)
        
        editor = self.main.tabWidget_graphical.currentWidget()
        #index = self.main.tabWidget_graphical.currentIndex()   
        #self.main.tabWidget_graphical.setTabText(index, os.path.split(filename)[1])    
        #self.main.tabWidget_graphical.setTabToolTip(index, filename)
        editor.path = filename
        self.main.tabWidget_graphical.setTabToolTip(self.main.tabWidget_graphical.currentIndex(), filename)
        self.main.tabWidget_graphical.setTabText(self.main.tabWidget_graphical.currentIndex(), os.path.split(filename)[1])  
        
            
        self.ide.tab_changed()
        
    #----------------------------------------------------------------------
    def save_as_pde(self):
        """"""
        editor = self.main.tabWidget_graphical.currentWidget()
        filename = editor.path.replace(".gpde", ".pde")
        file_pde = file(filename, "w")
        file_pde.write(self.get_pinguino_source_code())
        file_pde.close()
        
        