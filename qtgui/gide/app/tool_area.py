#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide import QtGui, QtCore

from .blocks import Blocks
from ..bloques import BlockLinear, BlockFunction, BlockNested, \
     BlockSpace, BlockNestedSecond, BlockSpaceBool, BlockFrameEdit
from ...ide.helpers.decorators import Decorator


########################################################################
class ToolArea(QtGui.QWidget):
    """"""
    def __init__(self, parent=None, workArea=None, ide=None):
        """"""
        super(ToolArea, self).__init__(parent)

        self.ide = ide
        self.main = ide.main

        self.piecePixmaps = []
        self.pieceRects = []
        self.pieceLocations = []
        self.highlightedRect = QtCore.QRect()
        self.inPlace = 0
        
        self.gridLayout_2 = QtGui.QGridLayout(parent)
        self.gridLayout_2.setContentsMargins(*[0]*4)
        self.gridLayout_2.setSpacing(0)
        self.gridLayout_2.addWidget(self, 0, 0, 1, 1)        
        
        self.isDragging = False
        self.CHILD = None
        self.POS = QtCore.QPoint(0, 0)
        
        self.NUEVO = False
        self.workArea = workArea
        
        self.setAcceptDrops(True)
        self.widget = parent
        
 
    #----------------------------------------------------------------------
    def mouse_move_event(self, event):
        """"""
        
        onBlock, pos = self.get_child_drag()
        
        if not self.NUEVO and onBlock != None:
            
            while onBlock.objectName() != "Bloq!":
                onBlock = onBlock.parent()
                
            
            name = onBlock.NAME
            args = onBlock.ARGS
            baseName = onBlock.BASENAME
            #color = onBlock.COLOR
            
            finalBlock, self.npos = self.workArea().new_bloq(name, args, self.mapToGlobal(self.workArea().pos()), baseName)

            tempBlock = QtGui.QWidget(self.widget_parent)
            nuevo = eval(Blocks[name])(tempBlock, args[2:], None)
   
   
            #print "*" * 89
   
            fix_pos = onBlock.pos()
            fix_pos -= QtCore.QPoint(self.scroll_area.horizontalScrollBar().value(), self.scroll_area.verticalScrollBar().value())
            

            tempBlock.move(fix_pos)
            tempBlock.NAME = name
            tempBlock.show()
            tempBlock.raise_()
            
            #tempBlock.COLOR = color
            
            self.finalBlock = finalBlock
            self.tempBlock = tempBlock
            self.NUEVO = True
        
        if onBlock != None:
            
            point = event.pos() - pos
            self.GlobalPoint = point+self.npos-QtCore.QPoint(10, 10)
            point -= QtCore.QPoint(self.scroll_area.horizontalScrollBar().value(), self.scroll_area.verticalScrollBar().value())
            
            self.tempBlock.move(point)
            
            self.workArea().SelectArea.hide()
            self.workArea().SelectArea.resize(QtCore.QSize(0, 0))
            self.workArea().mouseMoveEvent(None, self.GlobalPoint, self.finalBlock)
            self.workArea().expand_size(self.finalBlock)
            

    #----------------------------------------------------------------------
    def get_tab(self):
        """"""
        return self.ide.get_tab()
            
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def mouse_press_event(self, event):
        """"""
        self.workArea().forExpand = True
        child = self.widget_parent.childAt(event.pos())
        if child:
            while child.objectName() != "Bloq!":
                child = child.parent()
                if not child: return
                    
            if child.childAt(child.mapFromGlobal(event.globalPos())):
                child.childAt(child.mapFromGlobal(event.globalPos())).setCursor(QtCore.Qt.ClosedHandCursor)
                self.block_close_hand = child.childAt(child.mapFromGlobal(event.globalPos()))

            self.set_child_drag(child, event.pos()-child.pos())

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def mouse_release_event(self, event):
        """"""
        self.workArea().forExpand = False
        child, pos = self.get_child_drag()
        
        try:
            self.block_close_hand.setCursor(QtCore.Qt.OpenHandCursor)
            del self.block_close_hand
        except: pass
        
        
        if child != None:
            
            if child.childAt(child.mapFromGlobal(event.globalPos())):
                child.childAt(child.mapFromGlobal(event.globalPos())).setCursor(QtCore.Qt.OpenHandCursor) 
                        
            
            while child.objectName() != "Bloq!":
                child = child.parent()
                
            try: self.tempBlock.deleteLater()
            except: pass
            
            try:
                if 0 < self.GlobalPoint.x() - self.workArea().scroll.horizontalScrollBar().value() < self.workArea().scroll.size().width() and \
                   0 < self.GlobalPoint.y() - self.workArea().scroll.verticalScrollBar().value() < self.workArea().scroll.size().height():
                    pass
                else:
                    self.finalBlock.deleteLater()
            except:
                return

            
            if self.finalBlock.childAt(self.finalBlock.mapFromGlobal(event.globalPos())):
                self.finalBlock.childAt(self.finalBlock.mapFromGlobal(event.globalPos())).setCursor(QtCore.Qt.OpenHandCursor)           
                
            
            self.CHILD = None
            self.NUEVO = False
            
            
        self.main.actionSave_file.setEnabled(True)
        self.workArea().mouseReleaseEvent(event)

            
            
    #----------------------------------------------------------------------
    def set_child_drag(self, child, pos):
        """"""
        self.CHILD = child
        self.POS = pos
        
    #----------------------------------------------------------------------
    def get_child_drag(self):
        """"""
        return self.CHILD, self.POS
