#!/usr/bin/env python
#-*- coding: utf-8 -*-

from ..bloques import *
from ..py_bloques.get_blocks import all_sets
from PySide import QtGui, QtCore
from math import sqrt
#import threading
import re, os

from ..py_bloques import constructor


#from select_area import Ui_Form as Select
from ...frames.select_area import Ui_Selection

from .blocks import Blocks

#from metodos import APP_NAME, APP_VERSION
import time

from ...ide.helpers.dialogs import Dialogs

from .constant import INTRO_CODE, GLOBAL_CODE

TEMPLATES = {"and": {"python":"and", "pinguino":"&&",},
             "not": {"python":"not", "pinguino":"!",},
             "or": {"python":"or", "pinguino":"||",},
             }

from ...ide.helpers.decorators import Decorator



########################################################################
class Metadata(object):
    """"""


        

########################################################################
class revised_methods(object):
    """"""
        
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
    def build_block(self, name, widget, parent, instructions):
        """"""
        return eval(Blocks[name])(widget, instructions[2:], parent)
        
        
    #----------------------------------------------------------------------
    def get_project_blocks(self):
        """"""
        all_ = self.get_recursive_project_blocks(self.children())
        #print "%$%", len(all_)
        return all_
        
    #----------------------------------------------------------------------
    def get_recursive_project_blocks(self, children):
        """"""
        childs = []
        for child in filter(lambda obj:getattr(obj, "metadata", False), children):
            childs.append(child)
            childs.extend(self.get_recursive_project_blocks(self.get_widgets_from_layout(child.metadata.object_.layout)))
        return childs
    
    #----------------------------------------------------------------------
    def get_widgets_from_layout(self, layout):
        """"""
        if layout is None: return []
        widgets = []
        for index in range(layout.count()):
            widgets.append(layout.itemAt(index).widget())
        return filter(lambda obj:getattr(obj, "metadata", False), widgets)
        

    
########################################################################
class WorkArea(QtGui.QWidget, revised_methods):

    def __init__(self, parent, scroll, widget, frame, ide):
        super(WorkArea, self).__init__(parent)
        
        self.main = ide.main
        self.ide = ide
        self.frame = frame
        self.clearArea = frame.clear_area
        self.scroll = scroll
        #self.add_block = frame.append_block
        self.add_group_blocks = frame.add_group_blocks        

        
        #self.ID = 0
        self.IDused = []
        #self.Historial = []
        #self.HistorialIndex = 0
        #self.MaxHistorialLen = 50
        
        self.SelectionAbs = []

        self.piecePixmaps = []
        self.pieceRects = []
        self.pieceLocations = []
        #self.highlightedRect = QtCore.QRect()
        #self.inPlace = 0
        self.gridLayout = QtGui.QGridLayout(parent)
        self.gridLayout.addWidget(self, 0, 0, 1, 1)
        self.gridLayout.setContentsMargins(1, 0, 0, 0)
        self.gridLayout.setSpacing(0)         
        
        self.isDragging = False
        self.isSelecting = False
        self.isOpening = False
        self.hasSelection = False
        self.isConnecting = False
        #self.ConnectPointStart = QtCore.QPoint()
        self.CHILD = None
        self.POS = QtCore.QPoint(0, 0)
        #self.allConnections = []
        
        
        #self.allFunc = []   
        #self.allFuncBlocks = {}      
        
        self.prepareAccept = None
        
        #self.plainTextEdit = plainTextEdits[0]
        #self.plainTextEdit_2 = plainTextEdits[1]
        
        #self.clearArea = clearArea
        #self.Scroll = Scroll
        
        self.selFin = QtCore.QPoint(0, 0)
        
        self.setAcceptDrops(True)
        
        #self.ctimer = QtCore.QTimer()
        #QtCore.QObject.connect(self.ctimer, QtCore.SIGNAL("timeout()"), self.constantUpdate)
        #self.ctimer.start(10)
        
        self.constantUpdate()
        
        
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
        if self.isSelecting:
            menu.addAction("Delet selected blocks", self.deletBlocks)
            menu.addAction("Take Screenshot from selected area", self.screen_shot_area)
        menu.exec_(event.globalPos())
        
        
        
    #----------------------------------------------------------------------
    def get_type_magnetic(self, child):
        """"""
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
    def get_better_pos(self, posList, pos):
        distancia = lambda pa, pb:sqrt((pa.x()-pb.x())**2+(pa.y()-pb.y())**2)
        
        newPosList = []
        for point in posList:
            
            s = point[0]
            
            h = distancia(s, pos)
            if h < 20 and h != 0.0: newPosList.append((s, h, point[1]))
        
        if len(newPosList) == 0: return pos, False, "None"
            
        h2 = 100
        for p in newPosList:
            if p[1] < h2:
                h2 = p[1]
                poin = p[0]
                ID = p[2]
            
        return poin, True, p[2]
        
    

        
    #----------------------------------------------------------------------
    def new_ploq(self, name, args, pos, baseName, IDuser=None, full=None):
        #global ID
        #self.Frame.main.actionSave_file.setEnabled(True)
        newIcon = QtGui.QWidget(self)
        #if IDuser != None:
            #self.IDused.append(IDuser)
            #newIcon.ID = IDuser
        #else: newIcon.ID = self.getNewID()
        newIcon.NAME = name
        newIcon.ARGS = args
        newIcon.BASENAME = baseName
        #newIcon.COLOR = color
        
        #nuevo = eval(Blocks[name])(newIcon, args[2:], self)
        nuevo = self.build_block(name, newIcon, self, args)
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
        newIcon.metadata.size = nuevo.size
        
        newIcon.metadata.line_code = nuevo.getLine
        newIcon.metadata.get_contructor = nuevo.getConstructor
        
        newIcon.metadata.code_start = nuevo.codeStart
        
    
        #data = {"name": name,
                #"object": nuevo,
                #"baseName": baseName,
                #"widget": newIcon,
                ##"ID": newIcon.ID,
                ##"to": [],
                ##"nested": [],
                ##"from": [],
                ##"inside": [],
                #"type": nuevo.Type,
                #"getConstructor": nuevo.getConstructor,
                #"lineCode": nuevo.getLine,
                #"size": nuevo.size,
                #"addParent": nuevo.addParent,
                #"parent": "None",
                #"removeParent": nuevo.removeParent,
                #"layout": nuevo.layout,
                #"expand": nuevo.f_expand,
                #"pos": pos,
                #"toType": nuevo.toType,
                #"expandible": nuevo.expandible,
                #"codeStart": nuevo.codeStart,
                #}
        


        pos2 = self.mapFromGlobal(pos)
        
        newIcon.move(pos2)
        newIcon.show()
        
        #################################################################################
        #################################################################################
        #################################################################################
        if full:
            if full == [False]: pass
            else: self.fullWidgetWith(newIcon.ID, full)
        else: self.fullWidgetWith(newIcon, nuevo.fullWidgetWith)
        #################################################################################
        #################################################################################
        #################################################################################
            
        
        return newIcon, pos2
       
        
    #----------------------------------------------------------------------
    def fullWidgetWith(self, parent, content):
        for block in content:
            name = all_sets[block.replace("-", "_")][:]
            NAME = name[0]
            ARGS = name     
            BASENAME = block 
            child, pos2 = self.new_ploq(NAME, ARGS, QtCore.QPoint(), BASENAME)
            child.metadata.add_parent([parent.metadata.widget, child.metadata.widget], force=True)
            child.metadata.from_.append(parent)

        
        
    #----------------------------------------------------------------------
    def mouseMoveEvent(self, event=None, eventPos=None, child=None, pos = QtCore.QPoint(0, 0)):
        if event != None:
            eventPos = event.pos()
            self.selFin = eventPos
            self.setSelection()
            
        self.repaint()
        
        if child == None: child, pos = self.getChildDrag()
        

        if child != None:
            
            while child.objectName() != "Bloq!": child = child.parent()            
            
            if child.parent() != self:
            
                child.setParent(self)
                
                child.move(eventPos+QtCore.QPoint(-5, -20))
                ##self.MetaData[child.ID]["pos"] = eventPos #+ QtCore.QPoint(0, 0)
                ##self.MetaData[child.ID]["object"].removeParent()
                child.metadata.pos_ = eventPos #+ QtCore.QPoint(0, 0)
                child.metadata.object_.removeParent()
                child.show()
                self.mousePressEvent(event)
                return
                
            else:
                point = eventPos - pos
                

            listPos = self.get_type_magnetic(child)
            
            point, accept, parent = self.get_better_pos(listPos, point)
            
            self.prepareAccept = [accept, child, parent, point]
            self.accep_move(False, child, parent, point)
            
            
            relative = point - child.pos()
            child.move(point)
            
            self.move_group(child, point - child.metadata.pos_)
            
            ######self.moveGroupInside(child.ID, point - self.MetaData[child.ID]["pos"])  
            #self.moveGroupInside(child, point - child.metadata.pos_)            
        
            
            self.SelectArea.show()
            
            if self.SelectArea.isVisible():
                under = self.SelectionAbs[:]
                #self.Frame.debugPrint(" ".join(under))
                #print underself.SelectionAbs
                    
                self.SelectArea.hide()
                
                if len(under) > 1:
                    self.SelectArea.show()
                    selected = under
                    if child in selected: selected.remove(child)
                    for Id in selected:
                        if Id.metadata.from_ == []:
                            #print Id
                 
                            npos = relative + Id.metadata.widget.pos()
                            Id.metadata.widget.move(npos)
                            Id.metadata.pos_ = npos
                            self.move_group(Id, relative)
                            #self.moveGroupInside(Id, relative)
                    self.SelectArea.move(self.SelectArea.pos()+relative)                
            
            
            ######self.MetaData[child.ID]["pos"] = point
            child.metadata.pos_ = point
            #print ""
            #self.expandAll()

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

    def setSelection(self):
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

            
            
    def accep_move(self, accept, child, _ID, point):
        
        if accept:
            #print "Accept"
            child.metadata.from_.append(_ID)
            child.metadata.add_parent([_ID.metadata.widget, child.metadata.widget])
            #self.MetaData[child.ID]["from"].append(_ID)
            #self.MetaData[child.ID]["addParent"]([self.getWidget(_ID), self.getWidget(child.ID)])
            
            toPos = False
            #if self.getType(_ID) in ["tipo4", "tipo7", "tipo9"]:
            if _ID.metadata.type_ in ["tipo4", "tipo7", "tipo9"]:
                #toPos = self.MetaData[_ID]["object"].addSubs([self.getWidget(_ID), self.getWidget(child.ID)])
                toPos = _ID.metadata.object_.addSubs([_ID.metadata.widget, child.metadata.widget])                

            
            if toPos:
                #if not child.ID in self.MetaData[_ID]["nested"]:
                    #self.MetaData[_ID]["nested"].append(child.ID)
                    #if child.ID in self.MetaData[_ID]["to"]:
                        #self.MetaData[_ID]["to"].remove(child.ID)
                if not child in _ID.metadata.nested:
                    _ID.metadata.nested.append(child)
                    if child in _ID.metadata.to:
                        _ID.metadata.to.remove(child)
                        
            else:
                #if self.getType(child.ID) in ["tipo5", "tipo2"]:
                    #if not child.ID in self.MetaData[_ID]["inside"]:
                        #self.MetaData[_ID]["inside"].append(child.ID)
                if child.metadata.type_ in ["tipo5", "tipo2"]:
                    if not child in _ID.metadata.inside:
                        _ID.metadata.inside.append(child)
                else:
                    #if not child.ID in self.MetaData[_ID]["to"]:
                        #self.MetaData[_ID]["to"].append(child.ID)
                        #if child.ID in self.MetaData[_ID]["nested"]:
                            #self.MetaData[_ID]["nested"].remove(child.ID)
                    if not child in _ID.metadata.to:
                        _ID.metadata.to.append(child)
                        if child in _ID.metadata.nested:
                            _ID.metadata.nested.remove(child)

            #self.saveHistorial()
            
        else:
    
            child.metadata.from_ = []
            for blk in self.get_project_blocks():
            #for key in self.MetaData.keys():
                ##u = self.MetaData[key]["to"]
                ##o = self.MetaData[child.ID]["toChild"]
                ##p = self.MetaData[key]
                if child in blk.metadata.to:
                    blk.metadata.to.remove(child)
                    ##self.MetaData[child.ID]["removeParent"]() 
                    
                if child in blk.metadata.inside:
                    blk.metadata.inside.remove(child)
                    ##self.MetaData[child.ID]["removeParent"]()
                    
                if child in blk.metadata.nested:
                    blk.metadata.nested.remove(child)
                    ##self.MetaData[child.ID]["removeParent"]()
                              
                    #_ID = key
                    break                
                
                
        
                
    #----------------------------------------------------------------------
    def expandAll(self, release=False):
        #var = 30
        #for _ID in self.MetaData.keys():
            #if self.MetaData[_ID]["expandible"]:
                #size1 = self.getWidget(_ID).size().height()
                #size = self.getHeight(_ID, self.getNested, self.getTo)
                #self.MetaData[_ID]["expand"](size)
                #size2 = self.getWidget(_ID).size().height()
                #s = size2 - size1 
                ##if release:
                #if not self.isOpening:
                    #self.moveGroup(_ID, QtCore.QPoint(0, s), nested=False)
                    
        for block in self.get_project_blocks():
            if block.metadata.expandible:
                size1 = block.metadata.widget.size().height()
                size = self.getHeight(block, self.getNested, self.getTo)
                block.metadata.expand(size)
                size2 = block.metadata.widget.size().height()
                s = size2 - size1 
                #if release:
                if not self.isOpening:
                    """"""
                    ###################################################################
                    self.move_group(block, QtCore.QPoint(0, s), ignore_nested=True)
                    ###################################################################
                
        if self.isOpening: self.isOpening = False
            
                
    #----------------------------------------------------------------------
    def getHeight(self, block, func, func2=None):
        size = 0
        #listIDs = self.getNested(ID)
        listIDs = func(block)
        #if func2 != None: listIDs.extend(func2(ID))
            
        for blk in listIDs:
            if blk.metadata.expandible:
                #size += self.getWidget(ID).height() - 7
                size += blk.metadata.widget.height() - 7
            else:
                size += blk.metadata.size
            size += self.getHeight(blk, self.getTo)
        return size


    #----------------------------------------------------------------------
    def move_group(self, child, point, ignore_nested=False):
        """"""
        if not ignore_nested:
            IDs = self.getTo(child) + self.getNested(child)
        else:
            IDs = self.getTo(child)
            
        
        if not IDs: return
        
        toMove = []
        toMove.extend(IDs)
        while IDs:
            nIDs = []
            for ID in IDs:
                if ID: nIDs.extend(self.getTo(ID) + self.getNested(ID))
            toMove.extend(nIDs)
            IDs = nIDs
        
        for ID in toMove:
            #ID.metadata.widget.move(ID.metadata.widget.pos() + point)
            ID.move(ID.pos() + point)
            #print self.getWidget(ID).pos() + point
            ID.metadata.pos_ = ID.pos()  # + point
            

    ##----------------------------------------------------------------------
    #def moveGroupInside(self, ID, point):
        ##print point
        #orig = ID
        #IDs = self.getInside(ID)         
        #if IDs == ["None"] * len(IDs): return
        
        #toMove = []
        #toAdd = []
        #toMove.extend(IDs)
        #while IDs != ["None"] * len(IDs):
            #nIDs = []
            #for ID in IDs:
                #if ID != "None": nIDs.extend(self.getInside(ID))
            #toMove.extend(nIDs)
            #IDs = nIDs
        #for i in range(toMove.count("None")): toMove.remove("None")
        
        
        #for ID in toMove:
            #self.MetaData[ID]["pos"] = self.getWidget(orig).pos() + point
            ##print "pos:", self.getWidget(orig).pos()
            ##print point

    ##----------------------------------------------------------------------
    #def getWidget(self, ID):
        #return self.MetaData[ID]["widget"]
    
    #----------------------------------------------------------------------
    def getTo(self, ID):
        return ID.metadata.to
    
    #----------------------------------------------------------------------
    def getNested(self, ID):
        return ID.metadata.nested
    
    ##----------------------------------------------------------------------
    #def getInside(self, ID):
        #return self.MetaData[ID]["inside"]
    
    ##----------------------------------------------------------------------
    #def getBasename(self, ID):
        #return self.MetaData[ID]["baseName"]
    
    ##----------------------------------------------------------------------
    #def getFrom(self, ID):
        #return self.MetaData[ID]["from"][-1]
    
    ##----------------------------------------------------------------------
    #def getType(self, ID):
        #return self.MetaData[ID]["type"]    

    ##----------------------------------------------------------------------
    #def getExpandible(self, ID):
        #return self.MetaData[ID]["expandible"]
    
    #----------------------------------------------------------------------
    def getSome(self, ID, some):
        return self.MetaData[ID][some]    


    ##----------------------------------------------------------------------
    #def setPosID(self, ID, point):
        #self.MetaData[str(ID)]["pos"] = point
        
        
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
            
            #self.shadownBlock([child])
            
                
            
            #QtCore.QTimer.singleShot(0, lambda: self.debugBlock(child.ID))
            
            pos = child.pos()
            #print pos, child.size()
            
            if event.modifiers() == QtCore.Qt.ControlModifier:
                #Copy a complete block
                self.frame.copyblocks(nested_=True)
                self.frame.pasteblocks()
                #if self.MetaData[child.ID]["parent"] != "None":
                    #pos += self.MetaData[self.MetaData[child.ID]["parent"]]["widget"].pos()
                #fullw = map(self.getBasename, self.MetaData[child.ID]["inside"])
                #if not fullw: fullw = [False]
                #child, pos2, ID, none = self.NewBloq(child.NAME, child.ARGS, self.mapToGlobal(pos), child.BASENAME, full=fullw)
                
    
            if event.modifiers() == QtCore.Qt.ShiftModifier:
                #Copy a basic block
                if self.MetaData[child.ID]["parent"] != "None":
                    pos += self.MetaData[self.MetaData[child.ID]["parent"]]["widget"].pos()
                child, pos2, ID, none = self.new_ploq(child.NAME, child.ARGS, self.mapToGlobal(pos), child.BASENAME, full=[False])
    

            if child.childAt(child.mapFromGlobal(event.globalPos())):
                #child.setCursor(QtGui.QCursor(QtCore.Qt.ClosedHandCursor))
                #self.isDragging = True
                child.childAt(child.mapFromGlobal(event.globalPos())).setCursor(QtCore.Qt.ClosedHandCursor)
            
            self.setChildDrag(child, event.pos()-pos)
            
            
            #self.saveHistorial()            

        else:
            #self.shadownBlock()
            self.selInicio = event.pos()
            self.isSelecting = True
            
        
        
    ##----------------------------------------------------------------------
    #def shadownBlock(self, IDs=[]):
        #try:
            #for id in self.lastBlockShadown:
                #self.MetaData[id]["widget"].setGraphicsEffect(None)
        #except:  pass
        
        #IDs.extend(self.getUnderSelection(inside=False))
        
        ##self.Frame.debugPrint("Under: "+"-".join(IDs))
        
        #self.lastBlockShadown = []
        #for ID in IDs:
            #wget = ID.metadata.widget
            #effect = QtGui.QGraphicsDropShadowEffect(wget)
            #effect.setOffset(0)
            #effect.setBlurRadius(7)
            ##if self.isSelecting: effect.setColor(QtCore.Qt.darkYellow)
            
            ##effect.setColor(QtCore.Qt.darkYellow)
            #effect.setColor(QtGui.QColor("#3771c8"))
            
                
            ##effect.setColor(QtGui.QColor("#"+self.Frame.colorEspecific[self.MetaData[ID]["name"]]))
            #wget.setGraphicsEffect(effect)
            #self.lastBlockShadown.append(ID)
  
            
    #----------------------------------------------------------------------
    def mouseReleaseEvent(self, event):
        
            
        if self.CHILD != None:
            #self.CHILD.setCursor(QtGui.QCursor(QtCore.Qt.OpenHandCursor))
            
            if self.CHILD.childAt(self.CHILD.mapFromGlobal(event.globalPos())):
                self.CHILD.childAt(self.CHILD.mapFromGlobal(event.globalPos())).setCursor(QtCore.Qt.OpenHandCursor)            
                
                
            self.frame.main.actionSave_file.setEnabled(True)
            #self.shadownBlock()            
           
            
        if self.getUnderSelection() == []:
            self.SelectArea.hide()
            self.SelectionAbs = []
        else: self.SelectionAbs = self.getUnderSelection()
        #print self.getUnderSelection()
    
        
        if self.prepareAccept != None:
            self.accep_move(*self.prepareAccept)
        #self.MetaData[self.CHILD.ID]["widget"].adjustSize()
        
        
        self.CHILD = None
        self.prepareAccept = None
        self.expandAll(release=True)
        self.updateUser() 
        #self.saveHistorial()
        
        #self.isDragging = False
        self.paintEvent()
        
            
    #----------------------------------------------------------------------
    def setChildDrag(self, child, pos):
        self.CHILD = child
        self.POS = pos
        
    #----------------------------------------------------------------------
    def getChildDrag(self):
        #temp = self.CHILD
        #self.CHILD = None
        return self.CHILD, self.POS

    #----------------------------------------------------------------------
    @Decorator.timer(10)
    def constantUpdate(self):
        #if not self.MetaData: return
            
        for block in self.get_project_blocks():
            block.metadata.widget.adjustSize()
            
        #for key in self.MetaData.keys():
            #if self.MetaData[key]["object"].DeletLater:
                #self.removeBlock(key)
            #else:
                #self.MetaData[key]["widget"].adjustSize()
            
        
        self.expandAll()
        
        #self.update()  #Con esta línea %CPU>88% O_o
        
            
    

    def paintEvent(self, event=None):
        
        painter = QtGui.QPainter()
        painter.begin(self)
        painter.fillRect(self.rect(), QtCore.Qt.white)
        
        self.drawGrid(QtGui.QColor("#e6eef4"), painter, 15)
        
        
        #if self.isDragging: self.drawTrash(painter, False)


        painter.end()
        self.painter = painter
        
    #----------------------------------------------------------------------
    def drawGrid(self, color, painter, space):
        painter.setPen(QtGui.QPen(QtGui.QBrush(QtGui.QColor(color)), 1, QtCore.Qt.DashLine))        
        
        j = 0
        for i in range(0, self.size().height(), space):
            painter.drawLine(QtCore.QPoint(0, i), QtCore.QPoint(self.size().height(), i))
            j += 1
        
        j = 0
        for i in range(0, self.size().width(), space):
            painter.drawLine(QtCore.QPoint(i, 0), QtCore.QPoint(i, self.size().width()))
            j += 1            

                
    #----------------------------------------------------------------------
    def getCodeStart(self):
        codepi = ""
        codepy = ""
        for block in self.get_project_blocks():
        #for key in self.MetaData.keys():
            if block.metadata.code_start:
                
                codepi += block.metadata.code_start.get("pinguino", "")[0]
                codepy += block.metadata.code_start.get("python", "")
                
                func, type_ = block.metadata.code_start.get("pinguino", ["", ["", ""]])[1]
                if func: self.dicTypes[func] = type_
                
                #if "pinguino" in self.MetaData[key]["codeStart"].keys():
                    #codepi += self.MetaData[key]["codeStart"]["pinguino"]
                #if "python" in self.MetaData[key]["codeStart"].keys():
                    #codepy += self.MetaData[key]["codeStart"]["python"]
                
        return codepi, codepy
                
    #----------------------------------------------------------------------
    def updateCode(self):
        self.allVars = []
        self.PinguinoCode = INTRO_CODE() + GLOBAL_CODE()
        self.PyCode = ""
               
        pi, py = self.getCodeStart()
        self.PinguinoCode += pi
        self.PyCode += py
        
        #for key in self.MetaData.keys():
        for block in self.get_project_blocks():
            if block.metadata.basename in ["global-vars", "global-Vars", "global_Vars", "global_vars"]:
                self.PinguinoCode += "//"                
                self.PinguinoCode += self.getCodeFrom(key).replace("{\n", "").replace("}\n", "")
                self.PyCode += "#"                                
                self.PyCode += self.getCodeFrom(key).replace("\n{\n", ":\n").replace("}\n", "").replace("    ", "")
        self.PinguinoCode += "\n\n"
        self.PyCode += "\n\n"
        
        gloval_vars = ""
        if len(self.allVars) > 0: gloval_vars = "global " + ", ".join(self.allVars)
        
        funcs = []
        for section in ["user_function", "interfaz_function", "setup", "loop", "main_py"]:
            
            #for key in self.MetaData.keys():
            for key in self.get_project_blocks():
                if key.metadata.basename == section :
                    
                    funcs.append(section)
                    
                    self.listTypeVars = {}                    
                    code = self.getCodeFrom(key)
                    
                    if code == section + "()\n": code = section + "(){}\n"
                    
                    if self.functionHasReturn(key):
                        ret = self.functionHasReturn(key, True)
                        
                        typeVar = self.listTypeVars.get(ret, "")
                        
                        if not typeVar:
                            ret = ret.replace("\"", "").replace("'", "").replace(" ", "")
                            #print ret, ret.isalpha() and len(ret) == 1
                            if ret.isdigit(): typeVar = "int"
                            elif ret.count(".") == 1 and ret.replace(".", "").isdigit(): typeVar = "float"
                            elif ret.isalpha() and len(ret) == 1: typeVar = "char"
                            elif ret.isalpha(): typeVar = "char*"
                            
                        self.PinguinoCode += typeVar + " "
                        self.dicTypes[code[:code.find("(")]] = typeVar
                        
                    self.PinguinoCode += code.replace("(%s)", "()")
                        
                    
                    tempCode = self.getCodeFrom(key).replace("}\n", "")
                    for i in range(10):  #10 nested max
                        if i != 0: gloval_vars = ""
                        indent = "".join([" "] * 4 * i)
                        tempCode = tempCode.replace("\n%s{\n"%indent, ":\n    "+gloval_vars+"\n")
                    
                    if tempCode.count("\n") == 1: tempCode = tempCode.replace('\n',':\n    """"""')
                    self.PyCode += "#" + "-" * 70 + "\n"
                    self.PyCode += "def " + tempCode
                    self.PinguinoCode += "\n\n"
                    self.PyCode += "\n\n"
        
        ExtraCodePinguino, ExtraCodePynguino = "", ""
        for f in ["loop", "setup"]:
            if not f in funcs:
                ExtraCodePinguino += "\n\n" + f + "(){}\n"
                ExtraCodePynguino += "\n\ndef " + f + "(): pass\n"
                    
        plots = re.findall("\s(__update_plot__[\w]*)\(", self.PyCode)
        for plot in plots: self.PyCode = self.PyCode.replace(plot, "__update_plot__")
        
        increm = re.findall("(.*[\w]+[+-]{2}.*)", self.PyCode)
        increm_ = re.findall("([\w]+[+-]{2})", self.PyCode)
        increm__ = map( lambda x:(x[:-2], x[-1]), increm_)
        syntax = map(None, increm, increm_, increm__)
        for incremento in syntax:
            linea, incr, part = incremento
            linea_post = linea.replace(incr, part[0]) + "\n" + self.countIndent(linea) + part[0] + part[1] + "=1"
            self.PyCode = self.PyCode.replace(linea, linea_post)
            
        decrem = re.findall("(.*[+-]{2}[\w]+.*)", self.PyCode)
        decrem_ = re.findall("([+-]{2}[\w]+)", self.PyCode)
        decrem__ = map( lambda x:(x[2:], x[0]), decrem_)
        syntax = map(None, decrem, decrem_, decrem__)
        for decremento in syntax:
            linea, incr, part = decremento
            linea_post = self.countIndent(linea) + part[0] + part[1] + "=1\n" + linea.replace(incr, part[0])
            self.PyCode = self.PyCode.replace(linea, linea_post)
        
        
        for key in TEMPLATES.keys():
            self.PinguinoCode = self.PinguinoCode.replace("@"+key+"@", TEMPLATES[key]["pinguino"])
            self.PyCode = self.PyCode.replace("@"+key+"@", TEMPLATES[key]["python"])
        
    
        self.PinguinoCode += ExtraCodePinguino
        #self.PynguinoCode = INTRO_CODE_PY() + GLOBAL_CODE_PYNGUINO() + self.PyCode + ExtraCodePynguino
        #self.PythonCode = INTRO_CODE_PY() + GLOBAL_CODE_PYTHON() + self.PyCode    
        
        


    #----------------------------------------------------------------------
    def countIndent(self, linea):
        indent = 0
        for letra in linea:
            if letra != " ": return indent * " "
            indent += 1
        return indent * " "


    #----------------------------------------------------------------------
    def getCodeFrom(self, ID):
        code = ""
        incrementar = False
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
                    #if typeVar == "char": code += "* "
                    #else:code += " "
                        #code += self.MetaData[ID]["lineCode"]().replace(nameVar, nameVar+"[]")
                        #code += self.MetaData[ID]["lineCode"]()
                    #else:
                    code += ID.metadata.line_code()
                #except:
                    #code += tempCode + "\n"
                   
                try: self.listTypeVars[tempCode[:tempCode.find("=")]] = typeVar
                except: pass
                    
                    
            else:
                code += ID.metadata.line_code()
                    
        elif ID.metadata.basename in ["repeat", "for"]:
            line = ID.metadata.line_code()
            line = line.replace("\n", "")
            line = line[line.find("(")+1:-1].split(";")
            code += line[0] + ";\n"
            code += "while (%s)\n" % line[1]
            incrementar = True
        
        
            	
        else: code += ID.metadata.line_code()
        
        if len(ID.metadata.nested) > 0:
            bloque = ID.metadata.nested[0]
            if not incrementar:
                code += "{\n" + "\n".join(map(lambda x:"    "+x, self.getCodeFrom(bloque).split("\n"))) + "}\n\n"
            else:
                code += "{\n" + "\n".join(map(lambda x:"    "+x, self.getCodeFrom(bloque).split("\n"))) + "%s;}\n\n" % line[2]               
                
            
        if len(ID.metadata.to) > 0:
            bloque = ID.metadata.to[0]
            code += self.getCodeFrom(bloque)
        
        return code
    
    #----------------------------------------------------------------------
    def getNameFunction(self, ID):
        code = ID.metadata.get_contructor()
        return code[0][1]
    
    #----------------------------------------------------------------------
    def getNameVar(self, ID):
        code = ID.metadata.line_code()
        return code[:code.find("=")]
    
    #----------------------------------------------------------------------
    def getCountVarsFunction(self, ID):
        code = ID.metadata.get_contructor()
        return code.count(["space"])
          
    #----------------------------------------------------------------------
    def functionHasReturn(self, ID, ret=False):
        code = self.getCodeFrom(ID).split("\n")
        for line in code:
            if bool(re.match(" *return", line)):
                if ret: return re.match(" *return (.*);", line).group(1)
                else: return True
        return False
                
        
    #----------------------------------------------------------------------
    def addFunction(self, name, countVars, _return, tab="User", prename="", ID__=None):
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
            
        #all_sets["call-function-%s"%name] = [tipo, tab]
        #all_sets["call-function-%s"%name].append(["syntax", prename])        
        #all_sets["call-function-%s"%name].append(["edit", name])
        #all_sets["call-function-%s"%name].append(["syntax", "("])
        
        #if ID__: all_sets["call-function-%s"%name].append(["syntax", ID__+", "])            

        #all_sets["call-function-%s"%name].extend([["space"], ["syntax", ","]]*countVars)
        
        #all_sets["call-function-%s"%name].append(["syntax", ")"])
        #self.add_block("call-function-%s"%name)
    
    ##----------------------------------------------------------------------
    #def addFunctionInterfaz(self, name):
        #tipo = "linear"
        #all_sets["call-function-%s"%name] = [tipo, "Interface"]
        #all_sets["call-function-%s"%name].append(["label", name])
        #all_sets["call-function-%s"%name].append(["syntax", "()"])
        #all_sets["call-function-%s"%name].append(["button", "Activate", False, False])
        
        #self.add_block("call-function-%s"%name)
        
        
    #----------------------------------------------------------------------
    def expandSize(self, child):
        if child.metadata.type_ == "tipo8":
            child.metadata.object_.setSmallSize()
        
        
    #----------------------------------------------------------------------
    def addVar(self, name):
        """"""
        tmp = constructor.Output("User")
        tmp.addEdit(name, None, (255, 255, 255))
        
        return ["User", "call-var-%s"%name, tmp.getBlock()]
    
    #----------------------------------------------------------------------
    def updateUser(self):
        """"""
        self.clearArea("User")
        
        #self.clearArea("Interface")
        #self.addGroupBlocks("Interface")
        
        #self.allVars = []
        
        new_blocks = []
        
        for block in self.get_project_blocks():
        #for key in self.MetaData.keys():
            
            if block.metadata.basename == "user_function":
                name = self.getNameFunction(block)
                new_blocks.append(self.addFunction(name, self.getCountVarsFunction(block), self.functionHasReturn(block))        )
            
            elif block.metadata.basename == "asign":
                name = self.getNameVar(block)
                new_blocks.append(self.addVar(name))
                
            ##elif block.metadata.basename == "interfaz_function":
                ##name = self.getNameFunction(keblocky)
                ##self.addFunctionInterfaz(name)
                
            ##elif block.metadata.basename == "frame_plot":
                ##name = self.getNameFunction(block)
                ###self.addFunction(name, 0, False, tab="Interface", args=[key], prename="__update_plot__", ID__=key)  
                ##self.addFunction(name, 1, False, tab="Interface", prename="__update_plot__", ID__=key)        

        #self.add_block(new_blocks)
        
        self.add_group_blocks("User", new_blocks)        
                
                
    #----------------------------------------------------------------------
    def deletBlocks(self):
        IDs = self.getUnderSelection()
        for Id in IDs:
            self.MetaData[Id]["object"].DeletLater = True
        
        if not self.isSelecting:
            for Id in self.lastBlockShadown:
                self.MetaData[Id]["object"].DeletLater = True        
        self.SelectArea.hide()
        
        
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
            #for key in self.MetaData.keys():
                #pos = self.MetaData[key]["pos"]
                #if self.MetaData[key]["parent"] == "None":
                    #if x1 <= pos.x() <= x2 and y1 <= pos.y() <= y2: IDs.append(key)
                    
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
            

        #self.Scroll
        return IDs
    
    #----------------------------------------------------------------------
    def getAllRelatedFromID(self, IDs):
        all_ = []
        
        def getNear(Id):
            allo = []
            for ID in self.MetaData.keys():
                colection = self.MetaData[ID]["nested"] + self.MetaData[ID]["inside"] + self.MetaData[ID]["to"] + self.MetaData[ID]["from"]
                if Id in colection:
                    if ID in self.Searched:
                        allo.extend([ID]+colection)
                    else:
                        self.Searched.append(ID)
                        allo.extend(getNear(ID)+colection)
                    
            return allo
        
        
        for IDD in IDs:
            all_.extend(getNear(IDD))
                    
                        
        all_ = list(set(all_))
        
        
        
        return all_
        

    def removeBlock(self, key):
        self.MetaData[key]["object"].destroy_this()
        
        try:
            for ins in self.MetaData[key]["inside"]:
                self.MetaData[ins]["object"].DeletLater = True
        except KeyError: 
            pass
        
        for key_ in self.MetaData.keys():
            if key in self.MetaData[key_]["to"]: self.MetaData[key_]["to"].remove(key)
            if key in self.MetaData[key_]["inside"]: self.MetaData[key_]["inside"].remove(key)
            if key == self.MetaData[key_]["from"]: self.MetaData[key_]["from"] = []     
            if key in self.MetaData[key_]["nested"]: self.MetaData[key_]["nested"].remove(key)               
        
        b = self.MetaData.pop(key)
        del b
        #self.saveHistorial()

       
        
    #----------------------------------------------------------------------
    def updateFilesCode(self):
        self.updateCode()
        
        file = open("code/pinguino_code.pde", "w")
        file.write(self.PinguinoCode)
        file.close()
        
        file = open("code/python_code.py", "w")
        file.write(self.PythonCode)
        file.close()
        
        file = open("code/pynguino_code.py", "w")
        file.write(self.PynguinoCode)
        file.close()        
        
       
      


    #----------------------------------------------------------------------
    def reOrderSpace(self, space):
        for block, pos in space:
            #print pos
            block.move(*pos)