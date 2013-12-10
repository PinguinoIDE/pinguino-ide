#!/usr/bin/env python
#-*- coding: utf-8 -*-

import sys

from PySide import QtCore, QtGui

from .linear import Linear
from .nested import Nested
from .nested_in_code import NestedLinear
from .nested_in_code_next import NestedSecond
from .inside import Floatable, FloatableBool
from .frame_edit import FrameEdit
from .constructor import Constructor, FONT_SIZE, changeFontSize

     

########################################################################
class Properties(object):
    """"""

    #----------------------------------------------------------------------
    def build_menu(self, event):
        
        if not self.ContexMenu:
            if self.Tab == "Saved":
                menu = QtGui.QMenu()
                menu.addAction("Remove Block", self.menuRemoveSaved)      
                menu.exec_(event.globalPos())                
            
            else: return
            
        menu = QtGui.QMenu()
        menu.addAction("Remove block", self.destroy_this)
        if self.Type == "tipo8":
            menu.addSeparator()
            menu.addAction("Set small size", self.setSmallSize)
            menu.addAction("Set medium size", self.setMediumSize)
            menu.addAction("Set large size", self.setLargeSize)
            submenu = QtGui.QMenu("Resize")
            submenu.addAction("Aument width", self.setAumentWidth)
            submenu.addAction("Aument height", self.setAumentHigth)
            submenu.addAction("Decrement width", self.setDecrementWidth)
            submenu.addAction("Decrement height", self.setDecrementHigth)
            menu.addMenu(submenu)
            menu.addSeparator()

        if self.Type == "tipo4":
            menu.addSeparator()
            menu.addAction("Remove arg", self.rmArg)
            menu.addAction("Add arg", self.addArg)
            menu.addSeparator()            
            
            
        #menu.addAction("Save Block", self.saveBlock)
        
            
        menu.addAction("Send to back", self.sendBack)    
        menu.addAction("Bring to front", self.sendFront)        
        menu.exec_(event.globalPos())
    
    
    ##----------------------------------------------------------------------
    #def saveBlock(self):
        #data = self.getConstructor()
        #cont = len(os.listdir(os.path.join("user_blocks", "saved")))
        #name = os.path.join("user_blocks", "saved", "saved_%d"%(cont+1))
        
        #data.insert(0, self.name)
        #data.insert(1, "Saved")
                
        #file = open(name, "w")
        #pickle.dump(data, file)
        
    
    
    #----------------------------------------------------------------------
    def destroy_this(self):
        base = self.widget.BASENAME
        #c = self.widget.parent()
        #while c.objectName() != "": c = c.parent()
        #c.addBlock(base, True)
        #self.widget.destroy()
        self.widget.deleteLater()
        #self.widget.close()
        del self
        #if not global_:
            #if self.Type in ["tipo5", "tipo2"]:
                #try: self.removeParent()
                #except AttributeError: pass  #No parent
            #self.DeletLater = True
            
    ##----------------------------------------------------------------------
    #def menuRemoveSaved(self):
        #base = self.widget.BASENAME
        #os.remove(os.path.join("user_blocks", "saved", base))
        #self.menuRemoveBlock()
            
    #----------------------------------------------------------------------
    def sendBack(self): self.widget.lower()
    
    #----------------------------------------------------------------------
    def sendFront(self): self.widget.raise_()
        

#----------------------------------------------------------------------
def setPartOfBlock(blocks):
    """"""
    for block in blocks:
        block.setObjectName("Block")
        block.setCursor(QtCore.Qt.OpenHandCursor) 
    
    
        

########################################################################
class BlockLinear(Constructor, Properties):
    Type = "tipo1"
    expandible = False
    toType = {"tipo1": [QtCore.QPoint(0, 39)],
              "tipo7": [QtCore.QPoint(0, 39)],
              "tipo9": [QtCore.QPoint(0, 39)]}
    size = 39
    toChild = 0
    child = 0
    endLine = ";"
    
    
    def __str__(self):
        """"""
        return "BlockLinear"

    #----------------------------------------------------------------------
    def __init__(self, widget, bloque, IDE=None):
        
        delay = Linear()
        delay.setupUi(widget)
        widget.setObjectName("Bloq!")
        self.f_expand = lambda :None
        self.IDE = IDE
        self.delay = delay
        
        #self.principalColor = 
        
        self.isideDir = "%s/qtgui/gide/bloques/linear/arte/"%sys.path[0]
        #print "%s/qtgui/gide/bloques/linear/arte/"%sys.path[0]
        delay.frame.setStyleSheet("image: url(%s/qtgui/gide/bloques/linear/arte/linear1.svg);"%sys.path[0])
        delay.area.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/linear/arte/linear2.svg);"%sys.path[0])
        delay.frame_3.setStyleSheet("image: url(%s/qtgui/gide/bloques/linear/arte/linear3.svg);"%sys.path[0])

        setPartOfBlock([delay.frame,
                        delay.area,
                        delay.frame_3])
        
        self.layout = delay.horizontalLayout
        self.layout.setContentsMargins(0, 0, 0, 0)

        self.buildBlock(widget, bloque)
        
        widget.adjustSize()
        self.widget = widget
        self.widget.raise_()
        
        #eff = QtGui.QGraphicsDropShadowEffect()
        #self.widget.setGraphicsEffect(eff)
        
        #effect = QtGui.QGraphicsDropShadowEffect(self.widget)
        #effect.setBlurRadius(8)
        #effect.setOffset(5)
        #effect.setBlurRadius(5)
        #self.widget.setGraphicsEffect(effect)        

        
    ##----------------------------------------------------------------------
    #def resizeAll(self, factor):
        #""""""
        #self.delay.widget
        #self.delay.frame_3
        
        
        #self.delay.area
        
        
        
    #----------------------------------------------------------------------
    def removeParent(self):
        """"""

    #----------------------------------------------------------------------
    def addParent(self, widget, force=False):
        """"""


        
    
########################################################################
class BlockFunction(Constructor, Properties):
    TYPE = "tipo4"
    EXPANDIBLE = True
    TO_TYPE = {"tipo1": [QtCore.QPoint(12, 39)], 
               "tipo7": [QtCore.QPoint(12, 39)], 
               "tipo9": [QtCore.QPoint(12, 39)]}
    
    endLine = ""
    #color_ = "function"
    #CHILD = 1
    
    #NEXT_POS = QtCore.QPoint(11, 35)
    #toChild = 0
    
    #----------------------------------------------------------------------
    def __init__(self, widget, bloque, IDE):
        
        #setColor(color, "nested")
        
        
        Nest = Nested()
        Nest.setupUi(widget)
        widget.setObjectName("Bloq!")
        self.IDE = IDE
        
        
        #self.child = self.CHILD
        self.Type = self.TYPE
        self.toType = self.TO_TYPE
        #self.nextPos = self.NEXT_POS
        self.expandible = self.EXPANDIBLE
        self.size = 0
        self.f_expand = self.expand
        
        #self.color = IDE("theme", self.color_, "ffff00")
        
        self.isideDir = "%s/qtgui/gide/bloques/nested/arte/function/"%sys.path[0]
        Nest.frame_2.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/nested/arte/function/linear2.svg);"%sys.path[0])
        Nest.frame_3.setStyleSheet("image: url(%s/qtgui/gide/bloques/nested/arte/function/linear3.svg);"%sys.path[0])
        Nest.frame_4.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/nested/arte/function/nested2.svg);"%sys.path[0])
        Nest.frame_5.setStyleSheet("image: url(%s/qtgui/gide/bloques/nested/arte/function/nested3.svg);"%sys.path[0])
        Nest.frame.setStyleSheet("image: url(%s/qtgui/gide/bloques/nested/arte/function/nested1.svg);"%sys.path[0])
        
        setPartOfBlock([Nest.frame_2,
                        Nest.frame_3,
                        Nest.frame_4,
                        Nest.frame_5,
                        Nest.frame])        
        
        self.layout = Nest.horizontalLayout
        self.layout.setContentsMargins(0, 0, 0, 0)
        
        self.layout_adds = []
        self.layout_pos = []

        self.buildBlock(widget, bloque, (40, 2))        
        
        widget.adjustSize()
        self.widget = widget
        self.widget.lower()
        
        
    ##----------------------------------------------------------------------
    #def getLine(self):
        #return "funcion()"
    
    #----------------------------------------------------------------------
    def expand(self, size):
        #print size
        final = 46+size
        if final < 77: final = 90
        self.widget.setMinimumSize(0, final+1)
        self.widget.setMaximumSize(100000, final+1)
        
    #----------------------------------------------------------------------
    def addParent(self, widget, force=False):
        """"""
        
    #----------------------------------------------------------------------
    def removeParent(self):
        """"""
        
    #----------------------------------------------------------------------
    def updatePoints(self):
        """"""
        poits = map(lambda x:self.widget.mapToGlobal(x.pos()+self.insidePos), self.inLayouts)
        self.toType["tipo5"] = poits
        
        poits = map(lambda x:self.widget.mapToGlobal(x.pos()+self.insidePos), self.inLayouts_b)
        self.toType["tipo2"] = poits
        
        
        self.toType["tipo1"] = [QtCore.QPoint(12, 39)]      
        self.toType["tipo9"] = [QtCore.QPoint(12, 39)]      
                       
        
        
    #----------------------------------------------------------------------
    def addSubs(self, widget):
        """"""
        widgetMove = widget[1]
        self.widgetMove = widgetMove
        WidgetStatic = widget[0]
        
        b = QtCore.QPoint(12, 39)
        a = widgetMove.metadata.pos_ - WidgetStatic.metadata.pos_

        c = b - a
        if abs(c.y()) < 2:
            return True
        return False
    
    #----------------------------------------------------------------------
    def addArg(self):
        self.constructorCode.pop(-1)
        self.LineCode.pop(-1)
        
        if len(self.layout_adds) > 0:
            self.addSyntax(self.widget, ",")
        self.addToInside(self.widget, False)
        self.addSyntax(self.widget, ")")
        
        #self.layout_pos = []
        #bloque = self.getConstructor()
        #for i in range(len(bloque)):
            #if bloque[i] in [["space"], ["space_bool"]]:
                #self.layout_pos.append(i)
                
        bloque = self.getConstructor()
        n = bloque.count(["space"]) + bloque.count(["space_bool"])
        self.layout_pos = map(lambda x:2*x, range(1, n+1))
                                          
                
                
        self.setFlex(self.layout_adds, self.layout_pos, (40, 2), self.layout_adds_b, self.layout_adds_all)
        #self.IDE.updateInsidePos()
        
    #----------------------------------------------------------------------
    def rmArg(self):
        if len(self.layout_adds) == 0: return
        
        if len(self.layout_adds) > 1:
            self.layout.takeAt(len(self.constructorCode)-4)
            self.constructorCode.pop(-1)
    
        self.layout.removeWidget(self.layout_adds[-1])
        self.layout_adds.pop(-1)
        self.layout_adds_all.pop(-1)
        
        self.LineCode.pop(-1)
        self.LineCode.pop(-1)
        self.LineCode.pop(-1)
        
        self.constructorCode.pop(-1)
        self.constructorCode.pop(-1)
        
        #if len(self.layout_adds) == 0: self.addSyntax(self.widget, "(")
            
        self.addSyntax(self.widget, ")")
        
        #self.layout_pos = []
        #bloque = self.getConstructor()
        #for i in range(len(bloque)):
            #if bloque[i] in [["space"], ["space_bool"]]:
                #self.layout_pos.append(i)
                
        bloque = self.getConstructor()
        n = bloque.count(["space"]) + bloque.count(["space_bool"])
        self.layout_pos = map(lambda x:2*x, range(1, n+1))
        
                
        self.setFlex(self.layout_adds, self.layout_pos, (40, 2), self.layout_adds_b, self.layout_adds_all)
                        
        #self.IDE.updateInsidePos()
        
        
    
########################################################################
class BlockNested(Constructor, Properties):
    TYPE = "tipo9"
    EXPANDIBLE = True
    TO_TYPE = {"tipo1": [QtCore.QPoint(12, 39)],
               "tipo7": [QtCore.QPoint(12, 39)],
               "tipo9": [QtCore.QPoint(12, 39)]}
    
    endLine = ""
    #color_ = "nested"
    
    #----------------------------------------------------------------------
    def __init__(self, widget, bloque, IDE=None):
        NestLin = NestedLinear()
        NestLin.setupUi(widget)
        widget.setObjectName("Bloq!")
        
        #self.child = self.CHILD
        self.Type = self.TYPE
        self.toType = self.TO_TYPE
        #self.nextPos = self.NEXT_POS
        self.expandible = self.EXPANDIBLE
        self.size = 0
        self.f_expand = self.expand
        
        #self.color = IDE("theme", self.color_, "ffff00")
        
        self.isideDir = "%s/qtgui/gide/bloques/nested/arte/nested/"%sys.path[0]
        
        NestLin.frame_2.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/nested/arte/nested/linear2.svg);"%sys.path[0])
        NestLin.frame_3.setStyleSheet("image: url(%s/qtgui/gide/bloques/nested/arte/nested/linear3.svg);"%sys.path[0])
        NestLin.frame_4.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/nested/arte/nested/nested2.svg);"%sys.path[0])
        NestLin.frame_5.setStyleSheet("image: url(%s/qtgui/gide/bloques/nested/arte/nested/nested32.svg);"%sys.path[0])
        NestLin.frame.setStyleSheet("image: url(%s/qtgui/gide/bloques/nested/arte/nested/nested1sec.svg);"%sys.path[0])
        
        setPartOfBlock([
                        NestLin.frame_2,
                        NestLin.frame_3,
                        NestLin.frame_4,
                        NestLin.frame_5, 
                        NestLin.frame, 
                        ])
        
        self.layout = NestLin.horizontalLayout
        self.layout.setContentsMargins(0, 0, 0, 0)
        
        
        self.layout_adds = []
        self.layout_pos = []

        self.buildBlock(widget, bloque, (40, 2))          
        
        widget.adjustSize()
        self.widget = widget
        self.widget.lower()      
        
    ##----------------------------------------------------------------------
    #def getLine(self):
        #""""""

    #----------------------------------------------------------------------
    def addSubs(self, widget):
        """"""
        widgetMove = widget[1]
        self.widgetMove = widgetMove
        WidgetStatic = widget[0]
        
    
        b = QtCore.QPoint(12, 39)
        #a = widgetMove.DATA["pos"] - WidgetStatic.DATA["pos"]
        a = widgetMove.pos() - WidgetStatic.pos()

        c = b - a
        #print "XXXXXXXXXX", c, a
        #print widgetMove.DATA["pos"], WidgetStatic.DATA["pos"]
        if abs(c.y()) < 2:
            #print "Funciona"
            return True
        return False
   
    
    
    #----------------------------------------------------------------------
    def expand(self, size):
        final = 55+size
        if final < 77: final = 94
        #self.widget.resize(QtCore.QSize(self.widget.size().width(), final+1))
        self.widget.setMinimumSize(0, final + 30)
        self.widget.setMaximumSize(100000, final + 30)
        self.size = self.widget.size().height()
        self.updatePoints()
        
        
    #----------------------------------------------------------------------
    def addParent(self, widget, force=False):
        """"""
        
    #----------------------------------------------------------------------
    def removeParent(self):
        """"""
        
    #----------------------------------------------------------------------
    def updatePoints(self):
        """"""
        poits = map(lambda x:self.widget.mapToGlobal(x.pos()+self.insidePos), self.inLayouts)
        self.toType["tipo5"] = poits
        
        poits = map(lambda x:self.widget.mapToGlobal(x.pos()+self.insidePos), self.inLayouts_b)
        self.toType["tipo2"] = poits
        
        
        
        

        c = QtCore.QPoint(0, self.widget.size().height()-7)
        d = QtCore.QPoint(0, self.widget.size().height()-16)

        self.toType["tipo1"] = [QtCore.QPoint(12, 39), c]
        self.toType["tipo7"] = [QtCore.QPoint(12, 39), d]
        self.toType["tipo9"] = [QtCore.QPoint(12, 39), c]
                       
        

        
        
        
########################################################################
class BlockSpace(Constructor, Properties):
    #NAME = "Block_LineEdit_ToInside"
    TAB = "Outputs"
    TYPE = "tipo5"
    EXPANDIBLE = False
    TO_TYPE = {}
    SIZE = 39
    CHILD = 0
    toChild = 0
    endLine = ""
    #color_ = "inside"

    #----------------------------------------------------------------------
    def __init__(self, widget, bloque, IDE=None):
        self.Inside = Floatable()
        self.Inside.setupUi(widget)
        widget.setObjectName("Bloq!")
        
        self.child = self.CHILD
        self.Type = self.TYPE
        self.toType = self.TO_TYPE
        self.expandible = self.EXPANDIBLE
        self.size = self.SIZE
        self.f_expand = lambda :None
        
        #self.color = IDE("theme", self.color_, "ffff00")
        
        self.isideDir = "%s/qtgui/gide/bloques/inside/arte/in/"%sys.path[0]
        
        self.Inside.frame.setStyleSheet("image: url(%s/qtgui/gide/bloques/inside/arte/in/insert.svg);"%sys.path[0])
        self.Inside.frame_2.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/inside/arte/in/insert3.svg);"%sys.path[0])
        
        setPartOfBlock([self.Inside.frame,
                        self.Inside.frame_2])
        
        self.layout = self.Inside.horizontalLayout
        self.layout.setContentsMargins(0, 0, 0, 0)
        
        
        self.layout_adds = []
        self.layout_pos = []

        self.buildBlock(widget, bloque, (13, 0), inside=True)
        
        
        self.w_parent = None
        self.first = True
        
        
        widget.adjustSize()
        self.widget = widget
        self.widget.raise_()
        
        
    def updateSize(self):
        """"""
        if self.w_parent != None:
            self.w_parent.adjustSize()
        
        
    ##----------------------------------------------------------------------
    #def getLine(self):
        #""""""



########################################################################
class BlockSpaceBool(Constructor, Properties):
    #NAME = "Block_LineEdit_ToInside"
    TAB = "Opers"
    TYPE = "tipo2"
    EXPANDIBLE = False
    TO_TYPE = {}
    SIZE = 39
    CHILD = 0
    toChild = 0
    endLine = ""
    #color_ = "inside"

    #----------------------------------------------------------------------
    def __init__(self, widget, bloque, IDE=None):
        self.Inside = FloatableBool()
        self.Inside.setupUi(widget)
        widget.setObjectName("Bloq!")
        
        self.child = self.CHILD
        self.Type = self.TYPE
        self.toType = self.TO_TYPE
        self.expandible = self.EXPANDIBLE
        self.size = self.SIZE
        self.f_expand = lambda :None
        
        #self.color = IDE("theme", self.color_, "ffff00")
        
        self.isideDir = "%s/qtgui/gide/bloques/inside/arte/bool/"%sys.path[0]
        
        self.Inside.frame.setStyleSheet("image: url(%s/qtgui/gide/bloques/inside/arte/bool/insert.svg);"%sys.path[0])
        self.Inside.frame_2.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/inside/arte/bool/insert3.svg);"%sys.path[0])
        self.Inside.frame_3.setStyleSheet("image: url(%s/qtgui/gide/bloques/inside/arte/bool/insert_.svg);"%sys.path[0])

        setPartOfBlock([self.Inside.frame,
                        self.Inside.frame_2,
                        self.Inside.frame_3])

        self.layout = self.Inside.horizontalLayout
        self.layout.setContentsMargins(0, 0, 0, 0)
        
        
        self.layout_adds = []
        self.layout_pos = []

        self.buildBlock(widget, bloque, (13, 0), inside=True, bool=True)
        
        
        self.w_parent = None
        self.first = True
        
        
        widget.adjustSize()
        self.widget = widget
        self.widget.raise_()
        
        
    def updateSize(self):
        """"""
        if self.w_parent != None:
            self.w_parent.adjustSize()
        
        
    ##----------------------------------------------------------------------
    #def getLine(self):
        #""""""



########################################################################
class BlockNestedSecond(Constructor, Properties):
    TYPE = "tipo7"
    EXPANDIBLE = True
    TO_TYPE = {"tipo1": [QtCore.QPoint(12, 39)],
               "tipo7": [QtCore.QPoint(12, 39)],
               "tipo9": [QtCore.QPoint(12, 39)]}
    CHILD = 1
    #NEXT_POS = QtCore.QPoint(11, 35)
    toChild = 0
    
    endLine = ""
    #color_ = "nested"
    
    #----------------------------------------------------------------------
    def __init__(self, widget, bloque, IDE=None):
        NestSec = NestedSecond()
        NestSec.setupUi(widget)
        widget.setObjectName("Bloq!")
        
        self.child = self.CHILD
        self.Type = self.TYPE
        self.toType = self.TO_TYPE
        #self.nextPos = self.NEXT_POS
        self.expandible = self.EXPANDIBLE
        self.size = 0
        self.f_expand = self.expand
        
        #self.color = IDE("theme", self.color_, "ffff00")
        
        self.isideDir = "%s/qtgui/gide/bloques/nested/arte/nested/"%sys.path[0]
        
        NestSec.frame_2.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/nested/arte/nested/linear2.svg);"%sys.path[0])
        NestSec.frame_3.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/nested/arte/nested/linear3.svg);"%sys.path[0])
        NestSec.frame_4.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/nested/arte/nested/nested2.svg);"%sys.path[0])
        NestSec.frame_5.setStyleSheet("image: url(%s/qtgui/gide/bloques/nested/arte/nested/nested32.svg);"%sys.path[0])
        NestSec.frame.setStyleSheet("image: url(%s/qtgui/gide/bloques/nested/arte/nested/nested1.svg);"%sys.path[0])
        
        setPartOfBlock([
                        NestSec.frame_2,
                        NestSec.frame_3,
                        NestSec.frame_4,
                        NestSec.frame_5,
                        NestSec.frame])
        
        
        self.layout = NestSec.horizontalLayout
        self.layout.setContentsMargins(0, 0, 0, 0)
        
        
        self.layout_adds = []
        self.layout_pos = []

        self.buildBlock(widget, bloque, (40, 2))
         
        widget.adjustSize()
        self.widget = widget
        self.widget.lower()
        
        
    ##----------------------------------------------------------------------
    #def getLine(self):
        #""""""

    #----------------------------------------------------------------------
    def addSubs(self, widget):
        """"""
        widgetMove = widget[1]
        self.widgetMove = widgetMove
        WidgetStatic = widget[0]
        
    
        b = QtCore.QPoint(12, 39)
        a = widgetMove.DATA["pos"] - WidgetStatic.DATA["pos"]

        c = b - a
        #print "XXXXXXXXXX", c, a
        #print widgetMove.DATA["pos"], WidgetStatic.DATA["pos"]
        if abs(c.y()) < 2:
            #print "Funciona"
            return True
        return False
       
        
        
    ##----------------------------------------------------------------------
    #def getLine(self):
        #return "funcion()"
    
    
    #----------------------------------------------------------------------
    def expand(self, size):
        final = 55+size
        if final < 77: final = 94
        #self.widget.resize(QtCore.QSize(self.widget.size().width(), final+1))
        self.widget.setMinimumSize(0, final + 30)
        self.widget.setMaximumSize(100000, final + 30)
        self.size = self.widget.size().height()
        
        
        
    #----------------------------------------------------------------------
    def addParent(self, widget, force=False):
        """"""
        
    #----------------------------------------------------------------------
    def removeParent(self):
        """"""
        
    #----------------------------------------------------------------------
    def updatePoints(self):
        """"""
        poits = map(lambda x:self.widget.mapToGlobal(x.pos()+self.insidePos), self.inLayouts)
        self.toType["tipo5"] = poits
        
        poits = map(lambda x:self.widget.mapToGlobal(x.pos()+self.insidePos), self.inLayouts_b)
        self.toType["tipo2"] = poits
        
        

        c = QtCore.QPoint(0, self.widget.size().height()-7)
        d = QtCore.QPoint(0, self.widget.size().height()-16)

        self.toType["tipo1"] = [QtCore.QPoint(12, 39), c]
        self.toType["tipo7"] = [QtCore.QPoint(12, 39), d]
        self.toType["tipo9"] = [QtCore.QPoint(12, 39), c]
                       


########################################################################
class BlockFrameEdit(Constructor, Properties):
    TYPE = "tipo8"
    EXPANDIBLE = False
    TO_TYPE = {}
    CHILD = 1
    toChild = 0
    
    endLine = ""
    #color_ = "nested"
    
    #----------------------------------------------------------------------
    def __init__(self, widget, bloque, IDE=None):
        NestSec = FrameEdit()
        NestSec.setupUi(widget)
        widget.setObjectName("Bloq!")
        
        self.child = self.CHILD
        self.Type = self.TYPE
        self.toType = self.TO_TYPE
        #self.nextPos = self.NEXT_POS
        self.expandible = self.EXPANDIBLE
        self.size = 0
        self.f_expand = None
        
        
        #self.color = IDE("theme", self.color_, "ffff00")
        
        #self.isideDir = "bloques/nested/arte/nested/"
        
        NestSec.frame_8.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame_edit/arte/fill.svg);"%sys.path[0])
        NestSec.frame_10.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame_edit/arte/fill.svg);"%sys.path[0])
        NestSec.frame_6.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame_edit/arte/fill.svg);"%sys.path[0])
        NestSec.frame_7.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame_edit/arte/fill.svg);"%sys.path[0])
        NestSec.frame_11.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame_edit/arte/fill.svg);"%sys.path[0])
        
        NestSec.frame.setStyleSheet("image: url(%s/qtgui/gide/bloques/frame_edit/arte/esq11.svg);"%sys.path[0])
        NestSec.frame_2.setStyleSheet("image: url(%s/qtgui/gide/bloques/frame_edit/arte/esq12.svg);"%sys.path[0])
        NestSec.frame_4.setStyleSheet("image: url(%s/qtgui/gide/bloques/frame_edit/arte/esq22.svg);"%sys.path[0])
        NestSec.frame_3.setStyleSheet("image: url(%s/qtgui/gide/bloques/frame_edit/arte/esq21.svg);"%sys.path[0])
        
        setPartOfBlock([NestSec.frame_8,
                        NestSec.frame_10, 
                        NestSec.frame_6,
                        NestSec.frame_7,
                        NestSec.frame_11,
                        NestSec.frame, 
                        NestSec.frame_2, 
                        NestSec.frame_4,
                        NestSec.frame_3,
                        ])
        
        
        #NestSec.frame_4.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/nested/arte/nested/nested2.svg);"%sys.path[0])
        #NestSec.frame_5.setStyleSheet("image: url(%s/qtgui/gide/bloques/nested/arte/nested/nested32.svg);"%sys.path[0])
        #NestSec.widget.setStyleSheet("image: url(%s/qtgui/gide/bloques/nested/arte/nested/nested1.svg);"%sys.path[0])
        
        self.layout = None
        #self.layout.setContentsMargins(0, 0, 0, 0)
        
        
        self.layout_adds = []
        self.layout_pos = []
        
        #constr = bloque[0]
        
        NestSec.plainTextEdit.appendPlainText(bloque[0])
        
        changeFontSize(NestSec.plainTextEdit, -3)
        

        self.widget = widget
        self.buildBlock(widget, [bloque], (40, 2))
        
        
        self.constructorCode.append(lambda :str(NestSec.plainTextEdit.toPlainText()))  
        #self.constructorCode.append(widget.width)   
        #self.constructorCode.append(widget.height)       
        self.constructorCode.append(lambda :["resize", widget.width(), widget.height()])
         
        
        self.widget.lower()
        
        #self.widget.setMinimumSize(*bloque[1:])
        #self.widget.setMaximumSize(*bloque[1:])
        
       
        
        self.widget.setMinimumSize(*bloque[1][1:])
        self.widget.setMaximumSize(*bloque[1][1:])           
        
        

    #----------------------------------------------------------------------
    def setSmallSize(self):
        self.widget.setMinimumSize(300, 100)
        self.widget.setMaximumSize(300, 100)
        
    #----------------------------------------------------------------------
    def setMediumSize(self):
        self.widget.setMinimumSize(600, 200)
        self.widget.setMaximumSize(600, 200)
        
    #----------------------------------------------------------------------
    def setLargeSize(self):
        self.widget.setMinimumSize(600, 500)
        self.widget.setMaximumSize(600, 500)
        
        
    #----------------------------------------------------------------------
    def setAumentWidth(self):
        self.widget.setMinimumSize(self.widget.size()+QtCore.QSize(100, 0))
        self.widget.setMaximumSize(self.widget.size()+QtCore.QSize(100, 0))
        
    #----------------------------------------------------------------------
    def setAumentHigth(self):
        self.widget.setMinimumSize(self.widget.size()+QtCore.QSize(0, 100))
        self.widget.setMaximumSize(self.widget.size()+QtCore.QSize(0, 100))
        
    #----------------------------------------------------------------------
    def setDecrementWidth(self):
        self.widget.setMinimumSize(self.widget.size()-QtCore.QSize(100, 0))
        self.widget.setMaximumSize(self.widget.size()-QtCore.QSize(100, 0))
        
    #----------------------------------------------------------------------
    def setDecrementHigth(self):
        self.widget.setMinimumSize(self.widget.size()-QtCore.QSize(0, 100))
        self.widget.setMaximumSize(self.widget.size()-QtCore.QSize(0, 100))



#########################################################################
#class BlockFramePlot(Constructor, Properties):
    #TYPE = "tipo8"
    #EXPANDIBLE = False
    #TO_TYPE = {}
    #CHILD = 1
    #toChild = 0
    
    #endLine = ""
    ##color_ = "nested"
    
    
    ##----------------------------------------------------------------------
    #def __init__(self, widget, bloque, IDE=None):
        #self.NestSec = FramePlot()
        #self.NestSec.setupUi(widget)
        #widget.setObjectName("Bloq!")
        
        #self.child = self.CHILD
        #self.Type = self.TYPE
        #self.toType = self.TO_TYPE
        ##self.nextPos = self.NEXT_POS
        #self.expandible = self.EXPANDIBLE
        #self.size = 0
        #self.f_expand = None        
        
        #self.MAX_LENGTH = 50


        #self.viewer = Viewer()
        
        
        #self.valores_plot = map(lambda x, y: [x, y], range(1, 11), map(lambda y:math.sin(y)/y, range(1, 11)))        


        #table = DataTable()
        #table.rows = self.valores_plot
    
        #chart = AreaChart(table)
        #chart.setHorizontalAxisColumn(0)
        #chart.haxis_vmin = 1.0
        #chart.haxis_vmax = 10.0
        #chart.haxis_step = 1
        #chart.vaxis_vmin = -1.0
        #chart.vaxis_vmax = 1.0
        #chart.vaxis_step = 1
        
        #self.viewer.setGraph(chart)

        #self.NestSec.horizontalLayout_2.addWidget(self.viewer)
        
        #self.NestSec.frame_10.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame/arte/fill.svg);"%sys.path[0])
        #self.NestSec.frame_6.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame/arte/fill.svg);"%sys.path[0])
        #self.NestSec.frame_7.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame/arte/fill.svg);"%sys.path[0])
        #self.NestSec.frame_5.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame/arte/fill.svg);"%sys.path[0])
        #self.NestSec.frame_15.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/widgets/arte/box.png);"%sys.path[0])
        
        #self.NestSec.frame.setStyleSheet("image: url(%s/qtgui/gide/bloques/frame/arte/esq11.svg);"%sys.path[0])
        #self.NestSec.frame_2.setStyleSheet("image: url(%s/qtgui/gide/bloques/frame/arte/esq12.svg);"%sys.path[0])
        #self.NestSec.frame_4.setStyleSheet("image: url(%s/qtgui/gide/bloques/frame/arte/esq22.svg);"%sys.path[0])
        #self.NestSec.frame_3.setStyleSheet("image: url(%s/qtgui/gide/bloques/frame/arte/esq21.svg);"%sys.path[0])
        
        #setPartOfBlock([
                        #self.NestSec.frame_10,
                        #self.NestSec.frame_6,
                        #self.NestSec.frame_7,
                        #self.NestSec.frame_5,
                        #self.NestSec.frame_15, 
                        #self.NestSec.frame,
                        #self.NestSec.frame_2, 
                        #self.NestSec.frame_4, 
                        #self.NestSec.frame_3])        
        
        #self.layout = self.NestSec.horizontalLayout
        #self.layout.setContentsMargins(0, 0, 0, 0)
        
        #self.layout_adds = []
        #self.layout_pos = []
        
        #self.isideDir = "bloques/frame/arte/"
        
        #self.widget = widget        
        #self.buildBlock(widget, bloque, (13, 12))
           
        #self.widget.lower()
        
        #self.constructorCode.append(lambda :["resize", widget.width(), widget.height()])
    
        
        #self.widget.setMinimumSize(*bloque[1][1:])
        #self.widget.setMaximumSize(*bloque[1][1:])
        ##self.widget.setMinimumSize(QtCore.QSize(200, 70))
        ##self.widget.setMaximumSize(QtCore.QSize(200, 70))        
        
        #self.valores_plot = [0]*(self.MAX_LENGTH + 1)
        
        
    ##----------------------------------------------------------------------
    #def addPlot(self, value):
        #""""""
        #self.valores_plot = self.valores_plot[1:]
        #self.valores_plot.append(value)
        #table = DataTable()
        #table.rows = map(lambda x, y: [x, y], range(self.MAX_LENGTH+1), self.valores_plot)
    
        #chart = AreaChart(table)
        #chart.setHorizontalAxisColumn(0)
        #chart.haxis_vmin = 0.0
        #chart.haxis_vmax = float(self.MAX_LENGTH)
        #if not chart.haxis_vmax: chart.haxis_vmax = 10
        #chart.haxis_step = self.MAX_LENGTH / 10
        #if not chart.haxis_step: chart.haxis_step = 10
        #chart.vaxis_vmin = float(min(self.valores_plot))
        #chart.vaxis_vmax = float(max(self.valores_plot))
        #if not chart.vaxis_vmax: chart.vaxis_vmax = 10
        #chart.vaxis_step = (float(max(self.valores_plot)) - float(min(self.valores_plot))) / 10
        #if not chart.vaxis_step: chart.vaxis_step = 10
        
        
        #self.viewer.setGraph(chart)
        

    ##----------------------------------------------------------------------
    #def setSmallSize(self):
        #self.widget.setMinimumSize(350, 200)
        #self.widget.setMaximumSize(350, 200)
        
    ##----------------------------------------------------------------------
    #def setMediumSize(self):
        #self.widget.setMinimumSize(600, 300)
        #self.widget.setMaximumSize(600, 300)
        
    ##----------------------------------------------------------------------
    #def setLargeSize(self):
        #self.widget.setMinimumSize(800, 500)
        #self.widget.setMaximumSize(800, 500)
        



#########################################################################
#class BlockFrameDebug(Constructor, Properties):
    #TYPE = "tipo8"
    #EXPANDIBLE = False
    #TO_TYPE = {}
    #CHILD = 1
    #toChild = 0
    
    #endLine = ""
    ##color_ = "nested"
    
    
    ##----------------------------------------------------------------------
    #def __init__(self, widget, bloque, IDE=None):
        #self.NestSec = FrameDebug()
        #self.NestSec.setupUi(widget)
        #widget.setObjectName("Bloq!")
        
        #self.child = self.CHILD
        #self.Type = self.TYPE
        #self.toType = self.TO_TYPE
        ##self.nextPos = self.NEXT_POS
        #self.expandible = self.EXPANDIBLE
        #self.size = 0
        #self.f_expand = None        
        
        #self.MAX_LENGTH = 50

        
        #self.NestSec.frame_10.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame/arte/fill.svg);"%sys.path[0])
        #self.NestSec.frame_6.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame/arte/fill.svg);"%sys.path[0])
        #self.NestSec.frame_7.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame/arte/fill.svg);"%sys.path[0])
        #self.NestSec.frame_5.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame/arte/fill.svg);"%sys.path[0])
        #self.NestSec.frame_15.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/widgets/arte/box.png);")
        #self.NestSec.plainTextEdit.setStyleSheet("background-image: url(%s/qtgui/gide/bloques/frame/arte/__fill_b.svg); color: rgb(255, 255, 255);"%sys.path[0])
        
        #self.NestSec.frame.setStyleSheet("image: url(%s/qtgui/gide/bloques/frame/arte/esq11.svg);"%sys.path[0])
        #self.NestSec.frame_2.setStyleSheet("image: url(%s/qtgui/gide/bloques/frame/arte/esq12.svg);"%sys.path[0])
        #self.NestSec.frame_4.setStyleSheet("image: url(%s/qtgui/gide/bloques/frame/arte/esq22.svg);"%sys.path[0])
        #self.NestSec.frame_3.setStyleSheet("image: url(%s/qtgui/gide/bloques/frame/arte/esq21.svg);"%sys.path[0])
        
        #setPartOfBlock([
                        #self.NestSec.frame_10,
                        #self.NestSec.frame_6,
                        #self.NestSec.frame_7,
                        #self.NestSec.frame_5,
                        #self.NestSec.frame_15, 
                        #self.NestSec.frame,
                        #self.NestSec.frame_2, 
                        #self.NestSec.frame_4, 
                        #self.NestSec.frame_3])        
        
        #self.layout = self.NestSec.horizontalLayout
        #self.layout.setContentsMargins(0, 0, 0, 0)
        
        #self.layout_adds = []
        #self.layout_pos = []
        
        #self.isideDir = "bloques/frame/arte/"
        
        #self.widget = widget        
        #self.buildBlock(widget, bloque, (13, 12))
           
        #self.widget.lower()
        
        #self.constructorCode.append(lambda :["resize", widget.width(), widget.height()])
    
        
        #self.widget.setMinimumSize(*bloque[1][1:])
        #self.widget.setMaximumSize(*bloque[1][1:])        
        ##self.widget.setMinimumSize(QtCore.QSize(200, 70))
        ##self.widget.setMaximumSize(QtCore.QSize(200, 70))
        
        #self.valores_plot = [0]*(self.MAX_LENGTH + 1)
        
        ##self.NestSec.plainTextEdit.setStyleSheet("background-color: rgb(61, 61, 61);color: rgb(255, 255, 255);")        
        
    ##----------------------------------------------------------------------
    #def addDebug(self, value):
        #""""""
        #self.NestSec.plainTextEdit.insertPlainText(str(value))
        #cursor = self.NestSec.plainTextEdit.textCursor()
        #cursor.movePosition(cursor.End)
        #self.NestSec.plainTextEdit.setTextCursor(cursor)
        

    ##----------------------------------------------------------------------
    #def setSmallSize(self):
        #self.widget.setMinimumSize(350, 200)
        #self.widget.setMaximumSize(350, 200)
        
    ##----------------------------------------------------------------------
    #def setMediumSize(self):
        #self.widget.setMinimumSize(600, 300)
        #self.widget.setMaximumSize(600, 300)
        
    ##----------------------------------------------------------------------
    #def setLargeSize(self):
        #self.widget.setMinimumSize(800, 500)
        #self.widget.setMaximumSize(800, 500)
        





  