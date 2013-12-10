#!/usr/bin/env python
#-*- coding: utf-8 -*-

from ..bloques import *
from PySide import QtGui, QtCore

#from blocks import allBlocks

from .blocks import Blocks
#from blocks import * 

from ...ide.helpers.decorators import Decorator


class ToolArea(QtGui.QWidget):


    def __init__(self, parent=None, workArea=None, ide=None):
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
        
        
    ##----------------------------------------------------------------------
    #def esTipoBloque(self, evento):
        #mine = evento.mimeData()
        #for i in list(mine.formats()):
            #if str(i).startswith("bloque/name:"): return True
    
    ##----------------------------------------------------------------------
    #def getNameClass(self, evento):
        #mine = evento.mimeData()
        #for i in list(mine.formats()):
            #if str(i).startswith("bloque/name:"):
                #return str(i)[str(i).find(":")+1:]
        
    ##----------------------------------------------------------------------
    #def dragEnterEvent(self, event):
        #if self.esTipoBloque(event):
            #if event.source() == self:
                #event.setDropAction(QtCore.Qt.MoveAction)
                #event.accept()
            #else:
                #event.acceptProposedAction()
        #else:
            #event.ignore()

    #----------------------------------------------------------------------
    def get_better_pos(self, posList, pos):
        distancia = lambda pa, pb:sqrt((pa.x()-pb.x())**2+(pa.y()-pb.y())**2)
        
        newPosList = []
        for point in posList:
            h = distancia(point, pos)
            if h < 20 and h != 0.0: newPosList.append((point, h))
            #print h, 
            
        #print ""
        
        if len(newPosList) == 0:
            return pos, False

        #print newPosList
            
        h2 = 100
        for p in newPosList:
            if p[1] < h2:
                h2 = p[1]
                poin = p[0]
            
        return poin, True
        
                
    ###----------------------------------------------------------------------
    ##def dropEvent(self, event):
        ##global ID
        ##if self.esTipoBloque(event):
            ##itemData = event.mimeData().data('bloque/name:'+self.getNameClass(event))
            ##dataStream = QtCore.QDataStream(itemData, QtCore.QIODevice.ReadOnly)
            
            ##pixmap = QtGui.QPixmap()
            ##offset = QtCore.QPoint()
            
            ##if self.isDragging:
                ##offset = QtCore.QPoint()
                ###print "YES"
            ##dataStream >> pixmap >> offset
            ##if not self.isDragging:
                ##offset = QtCore.QPoint(pixmap.width()/2, pixmap.height()/2)
                ###print "NO"


            ##newIcon = QtGui.QWidget(self)
            ##newIcon.NAME = self.getNameClass(event)
            ##nuevo = eval(self.getNameClass(event))(newIcon)

            ##p = event.pos() - offset
            
            ###listPos = self.getTypeMagnetic(newIcon.ID)
            ###p, accept = self.getBetterPos(listPos, p)
            
            ###if accept and MetaData[newIcon.ID]["expandible"]:
                ###MetaData[newIcon.ID]["object"].expand()
            
            ###MetaData[newIcon.ID]["pos"] = p
            ##newIcon.move(p)
            
            
            ##newIcon.show()
            
            ##self.isDragging = False
            

            ##if event.source() == self:
                ##event.setDropAction(QtCore.Qt.MoveAction)
                ##event.accept()
            ##else:
                ##event.acceptProposedAction()
        ##else:
            ##event.ignore()        
        
        
        
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
            
            finalBlock, self.npos = self.workArea().new_ploq(name, args, self.mapToGlobal(self.workArea().pos()), baseName)

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
            #self.onBlock = onBlock
            self.NUEVO = True
            #self.prePos = onBlock.pos()
        
        
        if onBlock != None:
            
            point = event.pos() - pos
            self.GlobalPoint = point+self.npos-QtCore.QPoint(10, 10)
            point -= QtCore.QPoint(self.scroll_area.horizontalScrollBar().value(), self.scroll_area.verticalScrollBar().value())
            
            
            self.tempBlock.move(point)
            
            self.workArea().SelectArea.hide()
            self.workArea().SelectArea.resize(QtCore.QSize(0, 0))
            self.workArea().mouseMoveEvent(None, self.GlobalPoint, self.finalBlock)
            #self.workArea().shadownBlock([self.ID])
            self.workArea().expandSize(self.finalBlock)
            
            

    #----------------------------------------------------------------------
    def get_tab(self):
        """"""
        return self.ide.get_tab()
            
    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def mouse_press_event(self, event):
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
        self.workArea().forExpand = False
        child, pos = self.get_child_drag()
        
        try:
            self.block_close_hand.setCursor(QtCore.Qt.OpenHandCursor)
            del self.block_close_hand
        except: pass
        
        
        if child != None:
            
            #child.childAt(child.mapFromGlobal(event.globalPos())).setCursor(QtCore.Qt.OpenHandCursor)
            
            if child.childAt(child.mapFromGlobal(event.globalPos())):
                child.childAt(child.mapFromGlobal(event.globalPos())).setCursor(QtCore.Qt.OpenHandCursor) 
               
            
            #self.workArea.Frame.sizeTab = self.workArea.Frame.ventana.tabWidget.width()
            #self.workArea.Frame.ctimer_de.start(1)            
            
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
            
            #self.workArea.
            
           
            
        self.main.actionSave_file.setEnabled(True)
        self.workArea().mouseReleaseEvent(event)
        #self.workArea().saveHistorial()
        #self.workArea().saveHistorial()
        
        
            
            
    #----------------------------------------------------------------------
    def set_child_drag(self, child, pos):
        self.CHILD = child
        self.POS = pos
        
    #----------------------------------------------------------------------
    def get_child_drag(self):
        return self.CHILD, self.POS
        
    
    ##----------------------------------------------------------------------
    #def paintEvent(self, event):
        #painter = QtGui.QPainter()
        #painter.begin(self)
        #painter.end()
    
    