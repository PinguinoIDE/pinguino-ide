#! /usr/bin/python2
#-*- coding: utf-8 -*-

import re
import os
from math import sqrt
from datetime import datetime

from PySide import QtGui, QtCore

from .blocks import Blocks
#from .constant import INTRO_CODE
from ..bloques import BlockLinear, BlockFunction, BlockNested, \
     BlockSpace, BlockNestedSecond, BlockSpaceBool, BlockFrameEdit
from ..py_bloques.get_blocks import all_sets
from ..py_bloques import constructor
from ...frames.select_area import Ui_Selection
from ...ide.methods.dialogs import Dialogs
#from ...ide.methods.decorators import Decorator

#TEMPLATES = {"and": {"python":"and", "pinguino":"&&",},
             #"not": {"python":"not", "pinguino":"!",},
             #"or": {"python":"or", "pinguino":"||",},
             #}

exclude_auto_open = ["button_pressed", "button_released"]

########################################################################
class Metadata(object):
    pass

########################################################################
class WorkArea(QtGui.QWidget):

    #def __init__(self, parent, scroll, widget, frame, ide):
    def __init__(self, parent, scroll, frame, ide):

        super(WorkArea, self).__init__(parent)

        self.main = ide.main
        self.ide = ide
        self.frame = frame
        self.clearArea = frame.clear_area
        self.scroll = scroll
        self.add_group_blocks = frame.add_group_blocks

        self.SelectionAbs = []

        self.piecePixmaps = []
        self.pieceRects = []
        self.pieceLocations = []

        self.gridLayout = QtGui.QGridLayout(parent)
        self.gridLayout.addWidget(self, 0, 0, 1, 1)
        self.gridLayout.setContentsMargins(1, 0, 0, 0)
        self.gridLayout.setSpacing(0)

        self.isDragging = False
        self.isSelecting = False
        self.isOpening = False
        self.hasSelection = False
        self.isConnecting = False
        self.CHILD = None
        self.POS = QtCore.QPoint(0, 0)

        self.prepareAccept = None

        self.selFin = QtCore.QPoint(0, 0)

        self.setAcceptDrops(True)

        #self.constant_update()


        self.dicTypes = {str(type(0)): "int",
                         str(type(0.1)): "float",
                         str(type("")): "char",
                         }



        self.SelectArea = QtGui.QWidget(self)
        select = Ui_Selection()
        select.setupUi(self.SelectArea)
        self.SelectArea.hide()

        self.contextMenuEvent = self.build_menu


    #----------------------------------------------------------------------
    def build_menu(self, event):
        menu = QtGui.QMenu()

        if self.SelectArea.isVisible():
            #on selection
            menu.addAction(QtGui.QApplication.translate("Graphical", "Delete selected blocks"), self.dele_blocks)
            menu.addAction(QtGui.QApplication.translate("Graphical", "Take Screenshot from selected area"), self.screen_shot_area)

        else:
            #normal menu
            #menu.addAction(QtGui.QApplication.translate("Graphical", "Export code to pinguino editor"), self.export_code_to_pinguino_editor)
            menu.addAction(self.main.actionExport_code_to_editor)
            menu.addAction(self.main.actionInsert_Block)
            menu.addAction(self.main.actionView_Pinguino_code)

        menu.setStyleSheet("""
        QMenu {
            font-family: inherit;
            font-weight: normal;
            }

        """)

        menu.exec_(event.globalPos())


    #----------------------------------------------------------------------
    def export_code_to_pinguino_editor(self):

        code = self.update_code()

        self.ide.switch_ide_mode(False)
        self.ide.new_file()

        editor = self.main.tabWidget_files.currentWidget()
        editor.text_edit.setText(code)


    #----------------------------------------------------------------------
    def get_type_magnetic(self, child):

        pos = []
        localType = child.metadata.type_
        for blk in self.get_project_blocks():
            blk.metadata.object_.updatePoints()


            #print "@" * 50
            if localType in blk.metadata.to_type.keys() and blk != child:
                for i in blk.metadata.to_type[localType]:

                    if child.metadata.type_ in ["tipo5", "tipo2"]:
                        _pos_ = self.mapFromGlobal(i)
                        #print _pos_


                    else: _pos_ = i+blk.pos()

                    pos.append([_pos_, blk])
        #for d in pos:
            #print d

        return pos


    #----------------------------------------------------------------------
    def get_better_pos(self, posList, pos, child):

        distancia = lambda pa, pb:sqrt((pa.x()-pb.x())**2+(pa.y()-pb.y())**2)

        if not posList: return pos, False, "None"

        newPosList = []
        for point in posList:

            s = point[0]
            b = point[1]

            if b.metadata.type_ == "tipo1":
                if b.metadata.to:
                    if not child.metadata.type_ in "tipo2 tipo5".split(): continue

            elif b.metadata.type_ in "tipo4".split():
                if b.metadata.nested:
                    if not child.metadata.type_ in "tipo2 tipo5".split(): continue

            elif b.metadata.type_ in "tipo7 tipo9".split():
                if b.metadata.to and b.metadata.nested:
                    if not child.metadata.type_ in "tipo2 tipo5".split(): continue

            h = distancia(s, pos)
            if h < 20 and h != 0.0: newPosList.append((s, h, point[1]))

        if not newPosList: return pos, False, "None"

        h2 = 100
        for p in newPosList:
            if p[1] < h2:
                h2 = p[1]
                poin = p[0]
                #ID = p[2]

        return poin, True, p[2]


    #----------------------------------------------------------------------
    #def new_bloq(self, name, args, pos, baseName, IDuser=None, full=None):
    def new_bloq(self, name, args, pos, baseName, full=None):

        newIcon = QtGui.QWidget(self)

        newIcon.NAME = name
        newIcon.ARGS = args
        newIcon.BASENAME = baseName

        nuevo = self.build_block(name, newIcon, args)
        nuevo.name = name
        nuevo.ContexMenu = True

        newIcon.metadata = Metadata()

        newIcon.metadata.to = []
        newIcon.metadata.nested = []
        newIcon.metadata.from_ = []
        newIcon.metadata.inside = []
        newIcon.metadata.pos_ = pos
        newIcon.metadata.parent = "None"

        newIcon.metadata.name = name
        newIcon.metadata.basename = baseName
        newIcon.metadata.widget = newIcon

        newIcon.metadata.type_ = nuevo.Type
        newIcon.metadata.object_ = nuevo
        newIcon.metadata.to_type = nuevo.toType

        newIcon.metadata.expandible = nuevo.expandible
        newIcon.metadata.expand = nuevo.f_expand

        newIcon.metadata.add_parent = nuevo.addParent
        newIcon.metadata.remove_parent = nuevo.removeParent

        newIcon.metadata.size = nuevo.size

        newIcon.metadata.line_code = nuevo.getLine
        newIcon.metadata.get_contructor = nuevo.getConstructor

        newIcon.metadata.code_start = nuevo.codeStart

        newIcon.metadata.destroy_this = nuevo.destroy_this

        pos2 = self.mapFromGlobal(pos)

        newIcon.move(pos2)
        newIcon.show()

        if full:
            if full == [False]: pass
            else: self.fill_widget_with(newIcon, full)
        else: self.fill_widget_with(newIcon, nuevo.fullWidgetWith)

        return newIcon, pos2


    #----------------------------------------------------------------------
    def fill_widget_with(self, parent, content):

        for block in content:
            name = all_sets.get(block.replace("-", "_"), None)
            if name is None: return
            else: name = name[:]
            NAME = name[0]
            ARGS = name
            BASENAME = block
            child = self.new_bloq(NAME, ARGS, QtCore.QPoint(), BASENAME)[0]
            child.metadata.add_parent([parent.metadata.widget, child.metadata.widget], force=True)
            child.metadata.from_.append(parent)



    #----------------------------------------------------------------------
    def mouseMoveEvent(self, event=None, eventPos=None, child=None, pos = QtCore.QPoint(0, 0)):

        self.isDragging = True
        if event != None:
            eventPos = event.pos()
            self.selFin = eventPos
            self.set_selection()

        self.repaint()

        if child == None: child, pos = self.get_child_drag()


        if child != None:

            while child.objectName() != "Bloq!": child = child.parent()

            if child.parent() != self:

                child.setParent(self)

                child.move(eventPos+QtCore.QPoint(-5, -20))
                child.metadata.pos_ = eventPos
                child.metadata.object_.removeParent()
                child.show()
                self.mousePressEvent(event)
                return

            else:
                point = eventPos - pos


            listPos = self.get_type_magnetic(child)

            point, accept, parent = self.get_better_pos(listPos, point, child)

            self.prepareAccept = [accept, child, parent]
            self.accept_move(False, child, parent)


            relative = point - child.pos()
            child.move(point)

            self.move_group(child, point - child.metadata.pos_)


            self.SelectArea.show()
            under = self.SelectionAbs[:]

            if self.SelectArea.isVisible() and (child in under):

                #self.SelectArea.hide()

                if len(under) > 1:
                    self.SelectArea.show()
                    selected = under
                    #if child in selected: selected.remove(child)
                    selected.remove(child)
                    for Id in selected:
                        if Id.metadata.from_ == []:
                            #print Id

                            npos = relative + Id.metadata.widget.pos()
                            Id.metadata.widget.move(npos)
                            Id.metadata.pos_ = npos
                            self.move_group(Id, relative)
                            #self.moveGroupInside(Id, relative)
                    self.SelectArea.move(self.SelectArea.pos()+relative)

            else:
                self.SelectArea.hide()


            child.metadata.pos_ = point

            globalMax = self.scroll.mapToGlobal(QtCore.QPoint(self.scroll.size().width(), self.scroll.size().height()-40))
            globalMin = self.scroll.mapToGlobal(self.scroll.pos())
            globalPoint = self.mapToGlobal(point)

            if event != None:

                if globalPoint.y() < globalMin.y():
                    value = self.scroll.verticalScrollBar().value() - 10
                    self.scroll.verticalScrollBar().setValue(value)
                elif globalPoint.y() > globalMax.y():
                    value = self.scroll.verticalScrollBar().value() + 10
                    self.scroll.verticalScrollBar().setValue(value)

                if globalPoint.x() < globalMin.x():
                    value = self.scroll.horizontalScrollBar().value() - 10
                    self.scroll.horizontalScrollBar().setValue(value)
                elif globalPoint.x() > globalMax.x():
                    value = self.scroll.horizontalScrollBar().value() + 10
                    self.scroll.horizontalScrollBar().setValue(value)


    #----------------------------------------------------------------------
    def set_selection(self):

        if self.isSelecting:
            s = self.selFin-self.selInicio
            x = s.x()
            y = s.y()
            if x > 0 and y > 0: self.SelectArea.move(self.selInicio)
            if x < 0 and y > 0: self.SelectArea.move(self.selFin.x(), self.selInicio.y())
            if x < 0 and y < 0: self.SelectArea.move(self.selFin)
            if x > 0 and y < 0: self.SelectArea.move(self.selInicio.x(), self.selFin.y())

            self.SelectArea.resize(QtCore.QSize(abs(x), abs(y)))

            #self.SelectArea.raise_()
            self.SelectArea.lower()
            self.SelectArea.show()
            #self.shadownBlock()
            self.hasSelection = True


    #----------------------------------------------------------------------
    def accept_move(self, accept, child, _ID):

        if accept:
            #print point
            child.metadata.from_.append(_ID)
            child.metadata.add_parent([_ID.metadata.widget, child.metadata.widget])

            toPos = False
            if _ID.metadata.type_ in ["tipo4", "tipo7", "tipo9"]:
                toPos = _ID.metadata.object_.addSubs([_ID.metadata.widget, child.metadata.widget])


            if toPos:
                if not child in _ID.metadata.nested:
                    _ID.metadata.nested.append(child)
                    if child in _ID.metadata.to:
                        _ID.metadata.to.remove(child)

            else:
                if child.metadata.type_ in ["tipo5", "tipo2"]:
                    if not child in _ID.metadata.inside:
                        _ID.metadata.inside.append(child)
                else:
                    if not child in _ID.metadata.to:
                        _ID.metadata.to.append(child)
                        if child in _ID.metadata.nested:
                            _ID.metadata.nested.remove(child)

        else:

            child.metadata.from_ = []
            for blk in self.get_project_blocks():
                if child in blk.metadata.to:
                    blk.metadata.to.remove(child)

                if child in blk.metadata.inside:
                    blk.metadata.inside.remove(child)

                if child in blk.metadata.nested:
                    blk.metadata.nested.remove(child)

                    break




    #----------------------------------------------------------------------
    def expand_all(self):

        for block in self.get_project_blocks():
            if block.metadata.expandible:
                size1 = block.metadata.widget.size().height()
                try: size = self.get_height(block, self.get_metadata_nested)
                except RuntimeError: continue
                block.metadata.expand(size)
                size2 = block.metadata.widget.size().height()
                s = size2 - size1
                #if release:
                if not self.isOpening:
                    self.move_group(block, QtCore.QPoint(0, s), ignore_nested=True)

        if self.isOpening: self.isOpening = False


    #----------------------------------------------------------------------
    def get_height(self, block, func):

        size = 0
        #listIDs = self.getNested(ID)
        listIDs = func(block)
        #if func2 != None: listIDs.extend(func2(ID))

        for blk in listIDs:
            if blk.metadata.type_ == "tipo7":
                size += blk.metadata.widget.height() - 7 - 9

            elif blk.metadata.expandible:
                #size += self.getWidget(ID).height() - 7
                size += blk.metadata.widget.height() - 7

            else:
                size += blk.metadata.size
            size += self.get_height(blk, self.get_metadata_to)
        return size


    #----------------------------------------------------------------------
    def move_group(self, child, point, ignore_nested=False):

        if not ignore_nested:
            IDs = self.get_metadata_to(child) + self.get_metadata_nested(child)
        else:
            IDs = self.get_metadata_to(child)


        if not IDs: return

        toMove = []
        toMove.extend(IDs)
        while IDs:
            nIDs = []
            for ID in IDs:
                if ID: nIDs.extend(self.get_metadata_to(ID) + self.get_metadata_nested(ID))
            toMove.extend(nIDs)
            IDs = nIDs

        for ID in toMove:
            #ID.metadata.widget.move(ID.metadata.widget.pos() + point)
            ID.move(ID.pos() + point)
            #print self.getWidget(ID).pos() + point
            ID.metadata.pos_ = ID.pos()  # + point



    #----------------------------------------------------------------------
    def get_metadata_to(self, ID):
        return ID.metadata.to

    #----------------------------------------------------------------------
    def get_metadata_nested(self, ID):
        return ID.metadata.nested


    #----------------------------------------------------------------------
    def mousePressEvent(self, event):

        if event.button() == QtCore.Qt.RightButton: return
        self.hasSelection = False
        if len(self.getUnderSelection()) == 0:
            self.SelectArea.resize(QtCore.QSize(0, 0))

        self.SelectArea.hide()
        self.forExpand = True
        child = self.childAt(event.pos())

        if child != None:

            editor = self.ide.get_tab()
            if not editor.tabText(editor.currentIndex()).endswith("*"):
                editor.setTabText(editor.currentIndex(), editor.tabText(editor.currentIndex())+"*")
            self.isSelecting = False
            while child.objectName() != "Bloq!":
                child = child.parent()
                if child == None:
                    child.sendBack()
                    return

            if not child.childAt(child.mapFromGlobal(event.globalPos())):
                return


            #if not child.metadata.type_ in "tipo4 tipo9 tipo7".split():
                #child.raise_()


            pos = child.pos()
            self.global_close_hand = [child.childAt(child.mapFromGlobal(event.globalPos()))]

            if event.modifiers() == QtCore.Qt.ControlModifier:
                #Copy full block
                if child.metadata.parent != "None":
                    pos += child.metadata.parent.metadata.widget.pos()
                fullw = map(lambda wdg:wdg.metadata.basename, child.metadata.inside)
                child.childAt(child.mapFromGlobal(event.globalPos())).setCursor(QtCore.Qt.ClosedHandCursor)
                child = self.new_bloq(child.NAME, child.ARGS, self.mapToGlobal(pos), child.BASENAME, full=fullw)[0]


            if event.modifiers() == QtCore.Qt.ShiftModifier:
                #Copy a basic block
                if child.metadata.parent != "None":
                    pos += child.metadata.parent.metadata.widget.pos()
                child.childAt(child.mapFromGlobal(event.globalPos())).setCursor(QtCore.Qt.ClosedHandCursor)
                child = self.new_bloq(child.NAME, child.ARGS, self.mapToGlobal(pos), child.BASENAME, full=[False])[0]


            if child.childAt(child.mapFromGlobal(event.globalPos())):
                child.childAt(child.mapFromGlobal(event.globalPos())).setCursor(QtCore.Qt.ClosedHandCursor)

            self.set_child_drag(child, event.pos()-pos)



        else:
            self.selInicio = event.pos()
            self.isSelecting = True



    #----------------------------------------------------------------------
    def mouseReleaseEvent(self, event):

        self.isDragging = False

        if self.CHILD != None:
            if self.CHILD.childAt(self.CHILD.mapFromGlobal(event.globalPos())):
                self.CHILD.childAt(self.CHILD.mapFromGlobal(event.globalPos())).setCursor(QtCore.Qt.OpenHandCursor)
                for ch in self.global_close_hand:
                    ch.setCursor(QtCore.Qt.OpenHandCursor)

            self.frame.main.actionSave_file.setEnabled(True)

        if self.getUnderSelection() == []:
            self.SelectArea.hide()
            self.SelectionAbs = []
        else: self.SelectionAbs = self.getUnderSelection()


        if self.prepareAccept != None:
            self.accept_move(*self.prepareAccept)


        self.CHILD = None
        self.prepareAccept = None
        self.expand_all()
        self.update_user()

        #self.paintEvent()


    #----------------------------------------------------------------------
    def set_child_drag(self, child, pos):

        self.CHILD = child
        self.POS = pos

    #----------------------------------------------------------------------
    def get_child_drag(self):

        return self.CHILD, self.POS

    #----------------------------------------------------------------------
    #@Decorator.requiere_no_dragging()
    def constant_update(self):

        for block in self.get_project_blocks():
            block.metadata.widget.adjustSize()
        self.expand_all()


    #----------------------------------------------------------------------
    #@Decorator.requiere_no_dragging()
    def auto_raise(self):

        on_sort = []
        for block in self.get_project_blocks():
            on_sort.append((block.metadata.object_.widget.size().height(), block))

        on_sort.sort(key=lambda li: li[0])
        for h, block in on_sort:
            block.lower()

        self.SelectArea.lower()


    #----------------------------------------------------------------------
    def paintEvent(self, event=None):
        painter = QtGui.QPainter()
        painter.begin(self)
        painter.fillRect(self.rect(), QtCore.Qt.white)

        self.draw_grid(QtGui.QColor("#e6eef4"), painter, 15)

        painter.end()

    #----------------------------------------------------------------------
    def draw_grid(self, color, painter, space):

        painter.setPen(QtGui.QPen(QtGui.QBrush(QtGui.QColor(color)), 1, QtCore.Qt.DashLine))

        #j = 0
        for i in range(0, self.size().height(), space):
            painter.drawLine(QtCore.QPoint(0, i), QtCore.QPoint(self.size().height(), i))
            #j += 1

        #j = 0
        for i in range(0, self.size().width(), space):
            painter.drawLine(QtCore.QPoint(i, 0), QtCore.QPoint(i, self.size().width()))
            #j += 1


    #----------------------------------------------------------------------
    def get_code_start(self):

        codepi = ""
        #codepy = ""
        for block in self.get_project_blocks():
            if block.metadata.code_start:

                codepi += block.metadata.code_start.get("pinguino", "")[0]
                #codepy += block.metadata.code_start.get("python", "")

                func, type_ = block.metadata.code_start.get("pinguino", ["", ["", ""]])[1]
                if func: self.dicTypes[func] = type_

        #return codepi, codepy
        return codepi

    #----------------------------------------------------------------------
    def update_code(self):

        self.allVars = []
        pinguino_code = ""

        pi = self.get_code_start()
        pinguino_code += pi

        for block in self.get_project_blocks():
            if block.metadata.basename == "global_vars":
                pinguino_code += "//  "
                pinguino_code += self.get_code_from(block).replace("{\n", "").replace("}\n", "").replace("    ", "")

        #gloval_vars = ""
        #if len(self.allVars) > 0: gloval_vars = "global " + ", ".join(self.allVars)

        funcs = []
        for section in ["user_function", "interfaz_function", "setup", "loop", "main_py"]:

            for key in self.get_project_blocks():
                if key.metadata.basename == section :

                    funcs.append(section)

                    self.listTypeVars = {}
                    code = self.get_code_from(key).replace("\n{\n", "{\n")

                    if code == section + "()\n": code = section + "(){}\n"

                    if self.function_has_return(key):

                        #If function has return, get it, else false
                        ret = self.function_has_return(key, ret=True)

                        typeVar = self.listTypeVars.get(ret, "")

                        if not typeVar:
                            ret = ret.replace("\"", "").replace("'", "").replace(" ", "")
                            if ret.isdigit(): typeVar = "int"
                            elif ret.count(".") == 1 and ret.replace(".", "").isdigit(): typeVar = "float"
                            elif ret.isalpha() and len(ret) == 1: typeVar = "char"
                            elif ret.isalpha(): typeVar = "char*"

                        pinguino_code += typeVar + " "
                        self.dicTypes[code[:code.find("(")]] = typeVar

                    pinguino_code += code.replace("(%s)", "()")

                    #pinguino_code += "\n"

        pinguino_code_extra = ""
        for f in ["loop", "setup"]:
            if not f in funcs:
                pinguino_code_extra += "\n" + f + "(){\n    }\n"

        #for key in TEMPLATES.keys():
            #pinguino_code = pinguino_code.replace("@"+key+"@", TEMPLATES[key]["pinguino"])

        pinguino_code += pinguino_code_extra

        global_ = ""
        if pinguino_code.find("__i__") > 0: global_ += "int __i__ = 0;\n"
        if pinguino_code.find("__j__") > 0: global_ += "int __j__ = 0;\n"

        if global_: global_ = "//  blocks variables, don't edit this\n" + global_ + "\n"

        header_code = """//------------------------------------------------------------------
// Pinguino source code generated automatically.
//
// Created: %s
// by: %s %s
//
//  WARNING! All changes made in this file will be lost!
//------------------------------------------------------------------\n
"""% (datetime.today().strftime("%Y-%m-%d"), os.getenv("PINGUINO_NAME"), os.getenv("PINGUINO_VERSION"))

        pinguino_code = header_code + global_ + pinguino_code

        return pinguino_code



    #----------------------------------------------------------------------
    def count_indent(self, linea):

        indent = 0
        for letra in linea:
            if letra != " ": return indent * " "
            indent += 1
        return indent * " "


    #----------------------------------------------------------------------
    def get_code_from(self, ID, auto_open=True):

        code = ""
        #incrementar = False
        if ID.metadata.basename == "asign":
            tempCode = ID.metadata.line_code().replace("\n", "").replace("%s", "1")
            nameVar = tempCode[:tempCode.find("=")]
            if not nameVar in self.allVars:
                self.allVars.append(nameVar)
                #try:
                if tempCode[tempCode.find("=")+1:-1].replace(" ", "") in ["True", "False"]:
                    code += "char "
                else:
                    try:
                        typeVar = self.dicTypes[tempCode[tempCode.find("=")+1:tempCode.find("(")]]
                        nameVar = tempCode[tempCode.find("=")+1:-1]

                    except KeyError:
                        try:
                            #typeVar = self.dicTypes[str(type(eval(tempCode[tempCode.find("=")+1:-1])))]
                            typeVar = self.dicTypes[str(type(eval(tempCode[tempCode.find("=")+1:tempCode.find("(")])))]
                            if typeVar == "char":
                                vvar = tempCode[tempCode.find("=")+1:tempCode.find("(")].replace("\"", "").replace("'", "")
                                if len(vvar) > 1: typeVar = "char*"
                            nameVar = tempCode[tempCode.find("=")+1:-1]

                        except NameError:
                            typeVar = ""

                    except:
                        typeVar = self.dicTypes["<type '" + tempCode[tempCode.find("=")+1:tempCode.find(" ")] + "'>"]
                        nameVar = tempCode[tempCode.find("=")+1:tempCode.find(" ")]

                    code += typeVar + " "
                    code += ID.metadata.line_code().replace("=", " = ", 1)

                try: self.listTypeVars[tempCode[:tempCode.find("=")]] = typeVar
                except: pass


            else:
                code += ID.metadata.line_code().replace("=", " = ", 1)



        else: code += self.fix_syntax_code(ID.metadata.line_code())

        if len(ID.metadata.nested) > 0:
            bloque = ID.metadata.nested[0]

            #if auto_open: code += "{"
            auto_open = not ID.metadata.basename in exclude_auto_open
            #print bloque.metadata.basename, exclude_auto_open
            if auto_open: code += "{"

            #if not incrementar:
            code += "\n" + "\n".join(map(lambda x:"    "+x, self.get_code_from(bloque, auto_open=auto_open).split("\n"))) + "}\n\n"
            #else:
                #code += "\n" + "\n".join(map(lambda x:"    "+x, self.get_code_from(bloque, auto_open=auto_open).split("\n"))) + "%s;}\n\n" % line[2]

        elif ID.metadata.type_ in ["tipo9"]:
            code = code.replace("\n", "{}\n")


        if len(ID.metadata.to) > 0:
            bloque = ID.metadata.to[0]
            code += self.get_code_from(bloque)

        return code

    #----------------------------------------------------------------------
    def fix_syntax_code(self, code):

        #fix_syntax = "> < >= <= =".split()
        #for char in fix_syntax:
            #code = code.replace(char, " "+char+" ")
        return code


    #----------------------------------------------------------------------
    def get_name_function(self, ID):

        code = ID.metadata.get_contructor()
        return code[0][1]

    #----------------------------------------------------------------------
    def get_name_var(self, ID):

        code = ID.metadata.line_code()
        return code[:code.find("=")]

    #----------------------------------------------------------------------
    def get_count_vars_function(self, ID):

        code = ID.metadata.get_contructor()
        return code.count(["space"])

    #----------------------------------------------------------------------
    def function_has_return(self, ID, ret=False):

        code = self.get_code_from(ID).split("\n")
        for line in code:
            if bool(re.match(" *return", line)):
                if ret: return re.match(" *return (.*);", line).group(1)
                else: return True
        return False


    #----------------------------------------------------------------------
    def add_function(self, name, countVars, _return, prename="", ID__=None):

        if _return: tipo = "output"
        else: tipo = "linear"

        #tmp = constructor.Output("User")
        tmp = getattr(constructor, tipo.capitalize())("User")

        tmp.addSyntax(prename)
        tmp.addLabel(name)
        tmp.addSyntax("(")

        if ID__: tmp.addSyntax(ID__+", ")

        for var in range(countVars):
            tmp.addSpace()
            tmp.addSyntax(", ")

        tmp.addSyntax(")")

        return ["User", "call-function-%s"%name, tmp.getBlock()]


    #----------------------------------------------------------------------
    def expand_size(self, child):

        if child.metadata.type_ == "tipo8":
            child.metadata.object_.setSmallSize()

    #----------------------------------------------------------------------
    def add_variable(self, name):

        tmp = constructor.Output("User")
        tmp.addEdit(name, None, (255, 255, 255))

        return ["User", "call-var-%s"%name, tmp.getBlock()]

    #----------------------------------------------------------------------
    def update_user(self):

        self.clearArea("User")

        new_blocks = []

        for block in self.get_project_blocks():
        #for key in self.MetaData.keys():

            if block.metadata.basename == "user_function":
                name = self.get_name_function(block)
                new_blocks.append(self.add_function(name, self.get_count_vars_function(block), self.function_has_return(block)))

            elif block.metadata.basename == "asign":
                name = self.get_name_var(block)
                new_blocks.append(self.add_variable(name))


        self.add_group_blocks("User", new_blocks)


    #----------------------------------------------------------------------
    def dele_blocks(self):

        IDs = self.getUnderSelection()
        for Id in IDs:
            Id.metadata.destroy_this()

        self.SelectArea.hide()
        self.SelectionAbs = []


    #----------------------------------------------------------------------
    def getUnderSelection(self, inside=True, nested=False):

        IDs = []

        if self.SelectArea.isVisible():
            a = self.SelectArea.pos()
            x1 = a.x()
            y1 = a.y()
            b = self.SelectArea.size()
            x2 = x1 + b.width()
            y2 = y1 + b.height()

            for block in self.get_project_blocks():
            #for key in self.MetaData.keys():
                pos = block.metadata.pos_
                if block.metadata.parent == "None":
                    if x1 <= pos.x() <= x2 and y1 <= pos.y() <= y2: IDs.append(block)


        if inside:
            extra = []
            for Id in IDs: extra.extend(Id.metadata.inside)
            IDs = list(set(IDs + extra))

        if nested:
            extra = []
            for Id in IDs: extra.extend(Id.metadata.nested)
            IDs = list(set(IDs + extra))

        return IDs

    ##----------------------------------------------------------------------
    #def getAllRelatedFromID(self, IDs):
        #

        #all_ = []

        #def getNear(Id):
            #allo = []
            #for ID in self.MetaData.keys():
                #colection = self.MetaData[ID]["nested"] + self.MetaData[ID]["inside"] + self.MetaData[ID]["to"] + self.MetaData[ID]["from"]
                #if Id in colection:
                    #if ID in self.Searched:
                        #allo.extend([ID]+colection)
                    #else:
                        #self.Searched.append(ID)
                        #allo.extend(getNear(ID)+colection)

            #return allo


        #for IDD in IDs:
            #all_.extend(getNear(IDD))

        #all_ = list(set(all_))

        #return all_

    ##----------------------------------------------------------------------
    #def remove_block(self, key):
        #
        #self.MetaData[key]["object"].destroy_this()

        #try:
            #for ins in self.MetaData[key]["inside"]:
                #self.MetaData[ins]["object"].DeletLater = True
        #except KeyError:
            #pass

        #for key_ in self.MetaData.keys():
            #if key in self.MetaData[key_]["to"]: self.MetaData[key_]["to"].remove(key)
            #if key in self.MetaData[key_]["inside"]: self.MetaData[key_]["inside"].remove(key)
            #if key == self.MetaData[key_]["from"]: self.MetaData[key_]["from"] = []
            #if key in self.MetaData[key_]["nested"]: self.MetaData[key_]["nested"].remove(key)

        #b = self.MetaData.pop(key)
        #del b
        ##self.saveHistorial()



    #----------------------------------------------------------------------
    def screen_shot_area(self):
        self.SelectArea.hide()
        image = QtGui.QPixmap.grabWidget(self,
                                       QtCore.QRect(self.SelectArea.pos().x(),
                                                    self.SelectArea.pos().y(),
                                                    self.SelectArea.width(),
                                                    self.SelectArea.height()))

        filename = self.ide.get_tab().tabText(self.ide.get_tab().currentIndex())
        filename = os.path.splitext(filename)[0] + ".png"
        filename = Dialogs.set_save_image(self.ide, filename)
        if filename: image.save(filename, "png")


    #----------------------------------------------------------------------
    def build_block(self, name, widget, instructions):

        return eval(Blocks[name])(widget, instructions[2:])


    #----------------------------------------------------------------------
    def get_project_blocks(self):

        all_ = self.get_recursive_project_blocks(self.children())
        #print "%$%", len(all_)
        return all_

    #----------------------------------------------------------------------
    def get_recursive_project_blocks(self, children):

        childs = []
        for child in filter(lambda obj:getattr(obj, "metadata", False), children):
            childs.append(child)
            childs.extend(self.get_recursive_project_blocks(self.get_widgets_from_layout(child.metadata.object_.layout)))
        return childs

    #----------------------------------------------------------------------
    def get_widgets_from_layout(self, layout):

        if layout is None: return []
        widgets = []
        for index in range(layout.count()):
            widgets.append(layout.itemAt(index).widget())
        return filter(lambda obj:getattr(obj, "metadata", False), widgets)

