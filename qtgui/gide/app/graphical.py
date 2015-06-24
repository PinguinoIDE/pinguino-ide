#! /usr/bin/python2
#-*- coding: utf-8 -*-

import codecs
import os
import re
import sys

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
    from io import StringIO
else:
    #Python2
    from ConfigParser import RawConfigParser
    from cStringIO import StringIO

import logging

from PySide import QtGui, QtCore

# from .code2blocks import Code2Blocks
from .blocks import Blocks
from .work_area import WorkArea
from .tool_area import ToolArea
#from .constant import os.getenv("PINGUINO_NAME")
#from ..bloques.color import setColor
from ..py_bloques.get_blocks import all_sets
from ..bloques import BlockLinear, BlockFunction, BlockNested, BlockSpace, BlockNestedSecond, BlockSpaceBool, BlockFrameEdit
from ...ide.commons.backgrounds import BackgroundPallete
from ...ide.methods.decorators import Decorator
from ...frames.blocks_widget import Ui_widgetBlock
from ...frames.grafical_widget import Ui_Form_graphical
from ...ide.methods.dialogs import Dialogs

from ..py_bloques.user_blocks import UserBlocks



########################################################################
class GraphicalIDE(object):

    #----------------------------------------------------------------------
    def __init__(self, ide):

        self.main = ide.main
        self.ide = ide

        self.highlightedRect = QtCore.QRect()
        self.inPlace = 0
        self.posY = 20
        self.openID = 1

        self.fileName = None

        self.update_all_blocks()

        self.listBlocks = []
        #self.listSpaces = 0

        self.setup_bloques()

        self.main.tabWidget_files.setCurrentIndex(0)
        self.main.tabWidget_files.setMaximumSize(10000, 10e6)

        widget = self.get_widget("Pinguino")
        self.main.stackedWidget.setCurrentWidget(widget)

        self.constant_update()
        self.constant_auto_raise()


    # #----------------------------------------------------------------------
    # def set_tab(self, tab_name):
        # for index in range(self.main.tabWidget_blocks.count()):
            # if self.main.tabWidget_blocks.tabText(index) == tab_name:
                # self.main.tabWidget_blocks.setCurrentIndex(index)
                # break


    #----------------------------------------------------------------------
    def ide_new_file(self, *args, **kwargs):
        path = kwargs.get("filename", self.ide.get_untitled_name(ext=".gpde"))
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
                        #widget=widget,
                        frame=self,
                        ide=self.ide)

        editor.graphical_area = area

        self.main.tabWidget_files.addTab(editor, filename)

        editor.keyPressEvent = self.__keyPressEvent__

        self.main.tabWidget_files.setCurrentWidget(editor)
        index = self.main.tabWidget_files.currentIndex()
        self.main.tabWidget_files.setTabText(index, filename[:-1])
        self.ide.update_actions()


    #----------------------------------------------------------------------
    def update_all_blocks(self):
        ub = UserBlocks()
        blocks = ub.getBlocks()
        all_sets.update(blocks)


    #----------------------------------------------------------------------
    def __keyPressEvent__(self, event):
        editor = self.main.tabWidget_files.currentWidget()
        #editor.graphical_area

        if not editor.graphical_area.SelectArea.isVisible(): return

        if event.key() == QtCore.Qt.Key_Delete:
            editor.graphical_area.dele_blocks()


    # #----------------------------------------------------------------------
    # def ide_tab_changed(self, *args, **kwargs):
        # self.main.tabWidget_files.setVisible(self.main.tabWidget_files.count() > 0)
        # self.main.frame_logo.setVisible(not self.main.tabWidget_files.count() > 0)
        # self.main.actionClose_file.setEnabled(self.main.tabWidget_files.count() > 0)

        # editor = self.main.tabWidget_files.currentWidget()
        # if getattr(editor, "path", None): self.ide.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+editor.path)
        # else: self.ide.setWindowTitle(os.getenv("PINGUINO_NAME"))

        # index = self.main.tabWidget_files.currentIndex()
        # filename = self.main.tabWidget_files.tabText(index)
        # if filename.endswith("*"): self.main.actionSave_file.setEnabled(True)
        # else: self.main.actionSave_file.setDisabled(True)

        # self.ide.__update_current_dir_on_files__()


    #----------------------------------------------------------------------
    def ide_save_file(self, *args, **kwargs):

        editor = kwargs.get("editor", None)
        if not editor: editor = self.ide.get_tab().currentWidget()
        index = self.ide.get_tab().indexOf(editor)
        #editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        filename = self.main.tabWidget_files.tabText(index)
        save_path = getattr(editor, "path", None)

        if not save_path:
            save_path, filename = Dialogs.set_save_file(self.ide, filename)
            if not save_path: return False
            setattr(editor, "path", save_path)
            self.main.tabWidget_files.setTabText(index, filename)
            self.main.tabWidget_files.setTabToolTip(index, save_path)
            self.ide.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+save_path)



        self.__save_file__(editor=editor)
        return True


    #----------------------------------------------------------------------
    def save_as(self, *args, **kwargs):

        editor = kwargs.get("editor", None)
        if not editor: editor = self.ide.get_tab().currentWidget()
        index = self.ide.get_tab().indexOf(editor)
        #editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        filename = self.main.tabWidget_files.tabText(index)
        save_path = getattr(editor, "path", None)

        save_path, filename = Dialogs.set_save_file(self.ide, filename)
        if not save_path: return False
        setattr(editor, "path", save_path)
        self.main.tabWidget_files.setTabText(index, filename)
        self.main.tabWidget_files.setTabToolTip(index, save_path)
        self.ide.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+save_path)

        self.__save_file__(editor=editor)
        return True



    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def get_pinguino_source_code(self):

        editor = self.main.tabWidget_files.currentWidget()
        #editor.graphical_area.update_code()
        return editor.graphical_area.update_code()


    #----------------------------------------------------------------------
    def __save_file__(self, *args, **kwargs):

        editor = kwargs.get("editor", self.ide.get_tab())
        content = self.get_gpde()
        self.save_raw_parser(content, editor.path)

        # setattr(editor, "last_saved", content)
        # self.ide.ide_remove_backup_file(editor=editor)

        self.ide.editor_saved()



    #----------------------------------------------------------------------
    def save_raw_parser(self, content, filename):

        file_parser = RawConfigParser()
        count = 0
        for block in content:
            count += 1
            name_section = "Block-%d"%count
            file_parser.add_section(name_section)
            for key in block.keys():
                file_parser.set(name_section, key, block[key])
        file_parser.write(codecs.open(filename, "w", "utf-8"))


    #----------------------------------------------------------------------
    def read_raw_parser(self, filename):

        blocks_set = []

        file_parser = RawConfigParser()

        if type(filename) in [str, unicode]:
            file_parser.readfp(codecs.open(filename, "r", "utf-8"))
        else:
            file_parser = filename

        sections = file_parser.sections()
        for section in sections:
            options = file_parser.options(section)
            block = {}
            for option in options:
                file_parser.get(section, option)
                value = file_parser.get(section, option)
                if (type(value) in [str, unicode]) and (value[0] in ["[", "("]):
                    block[option] = eval(file_parser.get(section, option))
                else:
                    block[option] = file_parser.get(section, option)


            blocks_set.append(block)
        return blocks_set




    #----------------------------------------------------------------------
    def get_gpde(self):

        editor = self.main.tabWidget_files.currentWidget()
        blocks = editor.graphical_area.get_project_blocks()

        to_save = []
        for block in blocks:
            to_save.append({
                "basename": block.metadata.basename,
                "position": (block.metadata.pos_.x(), block.metadata.pos_.y()),
                #"position": (block.pos().x(), block.pos().y()),
                "constructor": block.metadata.get_contructor(),
                "name": block.metadata.name,
                "self_id": self.serialize_widgets([block])[0],
                "to": self.serialize_widgets(block.metadata.to),
                "from": self.serialize_widgets(block.metadata.from_),
                #"inside2": self.serialize_widgets(block.metadata.inside),
                "inside": self.serialize_widgets(self.sort_inside(block)),
                "nested": self.serialize_widgets(block.metadata.nested),
                })

        return to_save


    #----------------------------------------------------------------------
    def sort_inside(self, block):

        layout = block.metadata.object_.layout
        if layout is None: return []
        #bloque = block.ARGS[2:]
        #layout_pos = []
        #empty = block.metadata.object_.get_layout_pos_save()
        #fix = 0
        #for i in range(len(bloque)):
            #if bloque[i] in [["space"], ["space_bool"]]:
                #if i in empty:
                    #layout_pos.append(None)
                #else:
                    #layout_pos.append(True)
                    #fix += 1
                    #empty = map(lambda x:x-fix, empty)

        layout_pos = block.metadata.object_.get_layout_pos_save()

        widgets = []
        for i in range(layout.count()):
            widgets.append(layout.itemAt(i).widget())

        widgets = filter(lambda wdg:getattr(wdg, "metadata", False), widgets)

        inside = layout_pos[:]

        if inside.count(True) == len(widgets):
            for i in range(len(inside)):
                if inside[i]: inside[i] = widgets.pop(0)
        else:
            raise Exception("¬¬")

        return inside




    ##----------------------------------------------------------------------
    #def force_inside(self, block, inside):
        #



    #----------------------------------------------------------------------
    def serialize_widgets(self, widgets):

        ser = []
        for widget in widgets:
            #name = widget.__str__()
            if not widget:
                id_ = None
            else:
                id_ = re.findall("[.]*0x[\dabcdef]*[.]*", widget.__str__())[0]
            ser.append(id_)
        return ser



    #----------------------------------------------------------------------
    def ide_open_files(self, *args, **kwargs):

        #filenames = Dialogs.set_open_file(self.ide, ".gpde")
        #for filename in filenames:

        filename = kwargs["filename"]

        if self.ide.ide_check_duplicate_file(filename): return

        self.ide_new_file(filename)
        editor = self.main.tabWidget_files.currentWidget()
        set_blocks = self.read_raw_parser(filename)
        self.load_blocks(set_blocks)
        self.main.tabWidget_files.setTabToolTip(self.main.tabWidget_files.currentIndex(), filename)
        self.main.tabWidget_files.setTabText(self.main.tabWidget_files.currentIndex(), os.path.split(filename)[1])
        self.ide.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+filename)
        setattr(editor, "path", filename)

        self.ide.ide_tab_changed()

    #----------------------------------------------------------------------
    def load_blocks(self, set_blocks):

        editor = self.main.tabWidget_files.currentWidget()
        editor.graphical_area.isOpening = True

        toFitInside = []
        #restaurPos = []
        #all_icons = []

        #dummy, pos = editor.graphical_area.new_bloq("output", all_sets["convert_"], QtCore.QPoint(), "dummy_block", "dummy_block")
        #dummy.metadata.self_id = self.serialize_widgets([dummy])[0]

        for block in set_blocks:
            name = block["name"]
            args = ["", ""] + block["constructor"]
            pos = QtCore.QPoint(*block["position"])
            basename = block["basename"]
            ID = block["self_id"]

            newIcon = editor.graphical_area.new_bloq(name, args, pos, basename)[0]
            newIcon.metadata.self_id = ID

            newIcon.move(pos)
            newIcon.setCursor(QtGui.QCursor(QtCore.Qt.OpenHandCursor))

            newIcon.metadata.to = block["to"]
            newIcon.metadata.from_ = block["from"]
            newIcon.metadata.inside = block["inside"]
            #for i in range(newIcon.metadata.inside.count(None)):
                #newIcon.metadata.inside.remove(None)
            newIcon.metadata.nested = block["nested"]

        self.replace_id_by_widgets()

        for block in editor.graphical_area.get_project_blocks():
            if block.metadata.inside:
                toFitInside.append([block, block.metadata.inside[:]])
                #self.force_inside(block, block.metadata.inside)

        all_dummies = []
        for parent, toFit in toFitInside:
            for ins in toFit:
                wdg = self.get_widget_from_id(ins)
                if wdg:
                    wdg.metadata.add_parent([parent, wdg], force=True)
                else:
                    dummy, pos = editor.graphical_area.new_bloq("output", all_sets["convert_"], QtCore.QPoint(), "dummy_block", "dummy_block")
                    dummy.metadata.self_id = self.serialize_widgets([dummy])[0]
                    dummy.metadata.add_parent([parent, dummy], force=True)
                    all_dummies.append(dummy)
                    #dummy.

        for dummy in all_dummies:
            dummy.metadata.remove_parent()
            dummy.metadata.destroy_this()


        #for block in editor.graphical_area.get_project_blocks():
            #if block.metadata.type_ in "tipo9 tipo7".split(): block.lower()

        #for block in editor.graphical_area.get_project_blocks():
            #if block.metadata.type_ in "tipo4".split(): block.lower()





    #----------------------------------------------------------------------
    def replace_id_by_widgets(self):

        editor = self.main.tabWidget_files.currentWidget()
        for block in editor.graphical_area.get_project_blocks():
            block.metadata.to = map(self.get_widget_from_id, block.metadata.to)
            block.metadata.from_ = map(self.get_widget_from_id, block.metadata.from_)
            block.metadata.inside = map(self.get_widget_from_id, block.metadata.inside)
            block.metadata.nested = map(self.get_widget_from_id, block.metadata.nested)


    #----------------------------------------------------------------------
    def get_widget_from_id(self, id_):

        if type(id_) != type(""): return id_
        if id_ is None: return id_

        editor = self.main.tabWidget_files.currentWidget()
        blocks = editor.graphical_area.get_project_blocks()

        for block in blocks:
            if block.metadata.self_id == id_:
                return block


    #----------------------------------------------------------------------
    def ide_open_file_from_path(self, *args, **kwargs):

        filename = kwargs["filename"]

        if self.ide.ide_check_duplicate_file(filename): return
        self.ide_new_file(filename)
        editor = self.main.tabWidget_files.currentWidget()
        set_blocks = self.read_raw_parser(filename)
        self.load_blocks(set_blocks)

        editor = self.main.tabWidget_files.currentWidget()
        #index = self.main.tabWidget_files.currentIndex()
        #self.main.tabWidget_files.setTabText(index, os.path.split(filename)[1])
        #self.main.tabWidget_files.setTabToolTip(index, filename)
        editor.path = filename
        setattr(editor, "path", filename)
        self.main.tabWidget_files.setTabToolTip(self.main.tabWidget_files.currentIndex(), filename)
        self.main.tabWidget_files.setTabText(self.main.tabWidget_files.currentIndex(), os.path.split(filename)[1])
        self.ide.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+filename)

        self.ide.ide_tab_changed()


    #----------------------------------------------------------------------
    def open_from_source(self, source):
        """"""
        filename = self.ide.get_untitled_name(ext=".gpde")

        self.ide_new_file(filename)
        editor = self.main.tabWidget_files.currentWidget()
        set_blocks = self.read_raw_parser(source)
        self.load_blocks(set_blocks)

        editor = self.main.tabWidget_files.currentWidget()
        editor.path = filename
        setattr(editor, "path", filename)
        self.main.tabWidget_files.setTabToolTip(self.main.tabWidget_files.currentIndex(), filename)
        self.main.tabWidget_files.setTabText(self.main.tabWidget_files.currentIndex(), os.path.split(filename)[1])
        self.ide.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+filename)

        self.ide.ide_tab_changed()

        self.init_positions()


    #----------------------------------------------------------------------
    def init_positions(self):
        """"""
        editor = self.main.tabWidget_files.currentWidget()
        for block in editor.graphical_area.get_project_blocks():
            if block.metadata.from_:

                if block.metadata.name in ["output"]: continue

                if editor.graphical_area.get_type_magnetic(block, block.metadata.from_[0]):
                    pos = editor.graphical_area.get_type_magnetic(block, block.metadata.from_[0])[0][0]
                    block.move(pos)




    ##----------------------------------------------------------------------
    #def save_as_pde(self):

        #editor = self.main.tabWidget_files.currentWidget()
        #filename = editor.path.replace(".gpde", ".pde")
        #file_pde = codecs.open(filename, "w", "utf-8")
        #file_pde.write(self.get_pinguino_source_code())
        #file_pde.close()


    #----------------------------------------------------------------------
    @Decorator.requiere_graphical_mode()
    def get_work_area(self):
        return self.main.tabWidget_files.currentWidget().graphical_area


    #----------------------------------------------------------------------
    def setup_bloques(self):
        self.StartBlocks = all_sets.copy()
        self.allTabs = {}

        self.build_block_tabs()

        self.main.comboBox_blocks.addItems(self.get_block_tabs())

        for tab in self.get_block_tabs():
            tab_set = []

            for key in all_sets.keys():
                if all_sets[key][1] == tab:
                    tab_set.append([key, all_sets[key]])

            self.add_blocks(tab, tab_set)


    #----------------------------------------------------------------------
    def get_tab_set(self, tab_name):
        tab_set = []
        for key in all_sets.keys():
            if all_sets[key][1] == tab_name:
                tab_set.append([key, all_sets[key]])
        return tab_set


    #----------------------------------------------------------------------
    def get_block_tabs(self):
        tabs = ["Search"]
        for key in all_sets.keys(): tabs.append(all_sets[key][1])
        tabs = list(set(tabs))
        tabs.remove("None")
        return tabs


    #----------------------------------------------------------------------
    def build_block_tabs(self, tabs=None):
        if tabs is None: tabs = self.get_block_tabs()
        for tab in tabs:
            widget = QtGui.QWidget()

            ui = Ui_widgetBlock()
            ui.setupUi(widget)

            tool_area = ToolArea(ui.scrollArea, self.get_work_area, self.ide)
            ui.gridLayout_2.addWidget(tool_area, 0, 0, 1, 1)

            # self.main.tabWidget_blocks.addTab(widget, tab)
            widget.setObjectName(tab)
            self.main.stackedWidget.addWidget(widget)

            ui.gridLayout_2.setContentsMargins(9, 9, 9, 9)
            ui.gridLayout_2.setSpacing(10)
            widget.grid_layout = ui.gridLayout_2
            widget.tool_area = tool_area
            tool_area.widget_parent = widget
            tool_area.scroll_area = ui.scrollArea

            #palette = QtGui.QPalette(self.main.centralwidget.parent().palette())
            #widget.setAutoFillBackground(True)
            #palette.setColor(QtGui.QPalette.Window, QtGui.QColor("#ffffff"))
            #widget.setPalette(palette)

            ui.frame.setStyleSheet("""

            QFrame{
                 background-color: #FFF;
             }

            """)

            ui.frame_2.setStyleSheet("""

            QFrame{
                 background-color: #FFF;
             }

            """)

            ui.scrollArea.setFrameShape(QtGui.QFrame.NoFrame)

            widget.mousePressEvent = tool_area.mouse_press_event
            widget.mouseMoveEvent = tool_area.mouse_move_event
            widget.mouseReleaseEvent = tool_area.mouse_release_event
            widget.content_widgets = []

    # #----------------------------------------------------------------------
    # def remove_tab(self, tab_name):
        # for index in range(self.main.tabWidget_blocks.count()):
            # if self.main.tabWidget_blocks.tabText(index) == tab_name:
                # self.main.tabWidget_blocks.removeTab(index)
                # break

    #----------------------------------------------------------------------
    def get_widget(self, tab_name):
        # for index in range(self.main.tabWidget_blocks.count()):
            # if self.main.tabWidget_blocks.tabText(index) == tab_name:
                # widget = self.main.tabWidget_blocks.widget(index)
                # return widget

        for index in range(self.main.stackedWidget.count()):
            # self.main.stackedWidget
            widget = self.main.stackedWidget.widget(index)
            if widget.objectName() == tab_name:
                return widget


    #----------------------------------------------------------------------
    def add_blocks(self, tab, tab_set, count=0, side=0, clear=False):
        widget = self.get_widget(tab)
        grid_layout = widget.grid_layout
        tool_area = widget.tool_area

        if clear: self.clear_area(tab)


        for key, tab in tab_set:

            newIcon = QtGui.QWidget(tool_area)
            eval(Blocks[tab[0]])(newIcon, tab[2:])
            newIcon.setMaximumSize(newIcon.size())

            widget.content_widgets.append(newIcon)

            newIcon.NAME = tab[0]
            newIcon.ARGS = tab
            newIcon.BASENAME = key

            grid_layout.addWidget(newIcon, count, side, 1, 1)
            count += 1

    #----------------------------------------------------------------------
    def add_group_blocks(self, name, new_set):
        tab_set = self.get_tab_set(name)
        new_set = map(lambda x: [x[0], x[2]], new_set)
        self.add_blocks(name, tab_set + new_set)


    #----------------------------------------------------------------------
    def clear_area(self, area):

        # index = self.main.tabWidget_blocks.currentIndex()
        # self.remove_tab(area)
        # self.build_block_tabs([area])
        # self.main.tabWidget_blocks.setCurrentIndex(index)

        widget = self.get_widget(area)
        #grid_layout = widget.grid_layout

        #for index in range(grid_layout.count()):
            #item = grid_layout.itemAt(index)
            #grid_layout.removeItem(item)

        for wid in widget.content_widgets:
            #wid.close()
            #wid.destroy()
            wid.deleteLater()
            widget.content_widgets.remove(wid)



    #----------------------------------------------------------------------
    def get_tab(self):

        return self.ide.get_tab()

    #----------------------------------------------------------------------
    def isActiveWindow(self):

        return self.ide.isActiveWindow()

    #----------------------------------------------------------------------
    def is_graphical(self):

        return self.ide.is_graphical() is True


    #----------------------------------------------------------------------
    @Decorator.timer(50)
    @Decorator.requiere_open_files()
    @Decorator.requiere_graphical_mode()
    @Decorator.requiere_main_focus()
    def constant_update(self):
        if self.ide.is_graphical() is True:
            editor = self.main.tabWidget_files.currentWidget()
            editor.graphical_area.constant_update()


    #----------------------------------------------------------------------
    @Decorator.timer(500)
    @Decorator.requiere_open_files()
    @Decorator.requiere_graphical_mode()
    @Decorator.requiere_main_focus()
    def constant_auto_raise(self):
        editor = self.main.tabWidget_files.currentWidget()
        if not editor.graphical_area.isDragging:
            editor.graphical_area.auto_raise()


    #----------------------------------------------------------------------
    def update_blocks_search_tab(self, text):
        if len(text) < 2: return
        if text == QtGui.QApplication.translate("Frame", "Search Block..."): return
        if not text: return
        bloques = []
        for key in all_sets.keys():
            if all_sets[key][2][0] in ["label", "decorator"]:
                label = all_sets[key][2][1]
                if label.lower().startswith(text.lower()): bloques.append([key, all_sets[key]])

        if not bloques: return

        self.clear_area("Search")
        self.add_blocks("Search", bloques)
        self.ide.set_block_tab("Search")

    #----------------------------------------------------------------------
    def get_all_sets(self):
        return all_sets

