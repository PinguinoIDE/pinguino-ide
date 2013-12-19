#! /usr/bin/python
#-*- coding: utf-8 -*-

from PySide import QtGui, QtCore

from .blocks import Blocks
from .tool_area import ToolArea
from ..py_bloques.get_blocks import all_sets
from ..bloques import BlockLinear, BlockFunction, BlockNested, \
     BlockSpace, BlockNestedSecond, BlockSpaceBool, BlockFrameEdit
from ...ide.helpers.backgrounds import BackgroundPallete
from ...frames.blocks_widget import Ui_widgetBlock
from ...frames.grafical_widget import Ui_Form_graphical

########################################################################
class Metodos(object):
    
    #----------------------------------------------------------------------
    #@debug
    def getWorkArea(self):
        return self.main.tabWidget_graphical.currentWidget().graphical_area
        #except: pass  #Clear space
    
    #----------------------------------------------------------------------
    def getWorkAreaPath(self):
        return self.main.tabWidget_graphical.currentWidget().path
        #except: pass  #Clear space
    
    #----------------------------------------------------------------------
    def deletBlocks__(self):
        self.getWorkArea().deletBlocks()
    
    #----------------------------------------------------------------------
    def screenShotArea__(self):
        self.getWorkArea().screenShotArea()
    
    #----------------------------------------------------------------------
    def removeTab(self, tab):
        for i in range(self.ventana.tabWidget.count()):
            if self.ventana.tabWidget.tabText(i) == tab:
                self.ventana.tabWidget.removeTab(i)
                self.allTabs.pop(tab)

 
    #----------------------------------------------------------------------
    def saveSelectionInFile(self, event=None):
        
        fileName = self.setSaveFileName()[0]
        if not fileName: return False        
        
        MetaData = self.getWorkArea().MetaData
        toSave = []
        for key in self.getWorkArea().getUnderSelection():
            #print MetaData[key]["getConstructor"]()
            toSave.append([MetaData[key]["baseName"],
                           #(MetaData[key]["pos"]),
                           [MetaData[key]["pos"].x(), MetaData[key]["pos"].y()],
                           MetaData[key]["getConstructor"](),
                           MetaData[key]["name"],
                           key,
                           MetaData[key]["to"],
                           MetaData[key]["from"],
                           MetaData[key]["inside"],
                           MetaData[key]["nested"],
                           #[MetaData[key]["widget"].x(), MetaData[key]["widget"].y()],
                           ])
            
        #pickle.dump(toSave, open(self.fileName, "w"))
        #if fileName:
        file = open(fileName, "w")
        file.write(FORMAT())
        for line in toSave: file.write(str(line)+"\n")
        file.close()


    #----------------------------------------------------------------------
    def copyblocks(self, event=None, nested_=False):
        MetaData = self.getWorkArea().MetaData
        toSave = []
        pos2 = self.getWorkArea().mapFromGlobal(QtGui.QCursor.pos())
        self.cursorPosOnCopy = pos2
        self.areaPosOnCopy = self.getWorkArea().SelectArea.pos()
            
        if self.getWorkArea().SelectArea.isVisible(): Keys = self.getWorkArea().getUnderSelection()
        else:
            self.getWorkArea().Searched = []
            Keys = self.getWorkArea().getAllRelatedFromID(self.getWorkArea().lastBlockShadown) 
        
        for key in Keys:
            #print MetaData[key]["getConstructor"]()
            toSave.append([MetaData[key]["baseName"],
                           #(MetaData[key]["pos"]),
                           [MetaData[key]["pos"].x()-pos2.x(), MetaData[key]["pos"].y()-pos2.y()],
                           MetaData[key]["getConstructor"](),
                           MetaData[key]["name"],
                           key,
                           MetaData[key]["to"],
                           MetaData[key]["from"],
                           MetaData[key]["inside"],
                           MetaData[key]["nested"],
                           #[MetaData[key]["widget"].x(), MetaData[key]["widget"].y()],
                           ])
            

        self.CopyBlocksRAM = toSave[:]
        
        
        
        
    #----------------------------------------------------------------------
    def pasteblocks(self, event=None):
        if not self.CopyBlocksRAM: return
            
        space = self.CopyBlocksRAM
        
        self.newIDs = {}
        for block in space: self.newIDs[str(block[4])] = self.getWorkArea().getNewID()
        
        toFitInside = []
        all_icons = []
        for block in space:
            name = block[3]
            args = ["", ""] + block[2]
            
            pos = self.getWorkArea().mapFromGlobal(QtGui.QCursor.pos()) + QtCore.QPoint(*block[1])
            
            
            baseName = block[0]
            ID = self.newIDs[str(block[4])]
            
            newIcon, pos2, ID2, MetaData_C = self.getWorkArea().new_ploq(name, args, pos, baseName, ID)
            
            
            fix = lambda lista:map(lambda x:self.newIDs[x], lista)
            
            MetaData_C[ID]["to"] = fix(block[5])
            MetaData_C[ID]["from"] = fix(block[6])
            MetaData_C[ID]["inside"] = fix(block[7])
            MetaData_C[ID]["nested"] = fix(block[8])
            
            newIcon.move(pos)
            if len(fix(block[7])) > 0: toFitInside.append([ID, [fix(block[7])]])
            

            
        for toFit in toFitInside:
            ID = toFit[0]
            Inside = toFit[1][0]
            for ins in Inside:             
                self.getWorkArea().MetaData[ins]["addParent"]([self.getWorkArea().getWidget(ID),self.getWorkArea().getWidget(ins)],
                                                              force = True)



        pos = self.getWorkArea().mapFromGlobal(QtGui.QCursor.pos()) - self.cursorPosOnCopy + self.areaPosOnCopy
            
        self.getWorkArea().SelectArea.move(pos)          
        self.getWorkArea().SelectArea.lower()            
        self.getWorkArea().SelectArea.show()
        self.getWorkArea().hasSelection = True
        
        self.getWorkArea().SelectionAbs = self.getWorkArea().getUnderSelection()
                

        
        
    #----------------------------------------------------------------------
    def toggleDebugFrame(self, event=False):
        if self.ventana.dockWidget_debug.isVisible(): self.ventana.dockWidget_debug.hide()
        else: self.ventana.dockWidget_debug.show()
              


    #----------------------------------------------------------------------
    def setupBloques(self):
        
        self.StartBlocks = all_sets.copy()
        self.allTabs = {}
        
        self.build_block_tabs()
        
        for tab in self.get_block_tabs():
            tab_set = []
            
            for key in all_sets.keys():
                if all_sets[key][1] == tab:
                    tab_set.append([key, all_sets[key]])
                    
            self.add_blocks(tab, tab_set)
                
    #----------------------------------------------------------------------
    def get_tab_set(self, tab_name):
        """"""
        tab_set = []
        for key in all_sets.keys():
            if all_sets[key][1] == tab_name:
                tab_set.append([key, all_sets[key]])        
        return tab_set
                
            
    #----------------------------------------------------------------------
    def get_block_tabs(self):
        """"""
        tabs = []
        for key in all_sets.keys():
            tabs.append(all_sets[key][1])
        tabs = list(set(tabs))
        tabs.remove("None")
        return tabs
            
            
    #----------------------------------------------------------------------
    def build_block_tabs(self):
        """"""
        tabs = self.get_block_tabs()
        for tab in tabs:
            widget = QtGui.QWidget()
            ui = Ui_widgetBlock()
            ui.setupUi(widget)
            tool_area = ToolArea(ui.scrollArea, self.getWorkArea, self.ide)
            ui.gridLayout_2.addWidget(tool_area, 0, 0, 1, 1)
            self.main.tabWidget_blocks.addTab(widget, tab)
            ui.gridLayout_2.setContentsMargins(9, 9, 9, 9)
            ui.gridLayout_2.setSpacing(10)
            widget.grid_layout = ui.gridLayout_2
            widget.tool_area = tool_area
            tool_area.widget_parent = widget
            tool_area.scroll_area = ui.scrollArea
            
            palette = QtGui.QPalette(self.main.centralwidget.parent().palette())
            widget.setAutoFillBackground(True)
            palette.setColor(QtGui.QPalette.Window, QtGui.QColor("#ffffff"))
            widget.setPalette(palette)
            
            ui.scrollArea.setFrameShape(QtGui.QFrame.NoFrame)
            
            widget.mousePressEvent = tool_area.mouse_press_event
            widget.mouseMoveEvent = tool_area.mouse_move_event
            widget.mouseReleaseEvent = tool_area.mouse_release_event
            #widget.mouseGrabber = tool_area.mouse_grabber
            
            widget.content_widgets = []
            
        
            
    ##----------------------------------------------------------------------
    #def get_layout(self, tab_name):
        #""""""
        #for index in range(self.main.tabWidget_blocks.count()):
            #if self.main.tabWidget_blocks.tabText(index) == tab_name:
                #widget = self.main.tabWidget_blocks.widget(index)
                #return widget.grid_layout, widget.tool_area
        
            
    #----------------------------------------------------------------------
    def get_widget(self, tab_name):
        """"""
        for index in range(self.main.tabWidget_blocks.count()):
            if self.main.tabWidget_blocks.tabText(index) == tab_name:
                widget = self.main.tabWidget_blocks.widget(index)
                return widget
        
        
    #----------------------------------------------------------------------
    def add_blocks(self, tab, tab_set, count=0, side=0, ignore_jump=False):
        """"""
        widget = self.get_widget(tab)
        grid_layout = widget.grid_layout
        tool_area = widget.tool_area
        
        #base = tab
        #count = 0
        #side = 0  #left
        #print tab_set
            
        for key, tab in tab_set:
            #if ignore_jump:
                #""""""
                ##if side == 0: side = 1
                ##else: side = 0
                    
            #else:
                #if tab_set.index([key, tab]) == (len(tab_set) / 2) + 1:
                    #count = 0
                    #side = 1  #rigth
            
                    
            #print count
            
            newIcon = QtGui.QWidget(tool_area)
            #newIcon.setMaximumWidth(200)
            nuevo = eval(Blocks[tab[0]])(newIcon, tab[2:], None)
            newIcon.setMaximumSize(newIcon.size())
            

            widget.content_widgets.append(newIcon)
            
            newIcon.NAME = tab[0]
            newIcon.ARGS = tab     
            newIcon.BASENAME = key   #name = all_sets[block][:]         
            
            
            grid_layout.addWidget(newIcon, count, side, 1, 1)
            count += 1

        
        
            
            
    #----------------------------------------------------------------------
    def add_group_blocks(self, name, new_set):
        tab_set = self.get_tab_set(name)
        new_set = map(lambda x: [x[0], x[2]], new_set)
        self.add_blocks(name, tab_set + new_set)
                

    #----------------------------------------------------------------------
    def esTipoBloque(self, evento):
        mine = evento.mimeData()
        for i in list(mine.formats()):
            if str(i).startswith("bloque/name:"): return True


    #----------------------------------------------------------------------
    def centrar(self):
        screen = QtGui.QDesktopWidget().screenGeometry()
        size =  self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)



    #----------------------------------------------------------------------
    def clear_area(self, area):
        """"""
        widget = self.get_widget(area)
        
        for wid in widget.content_widgets:
            wid.close()
            wid.destroy()
            widget.content_widgets.remove(wid)
        
        
            

        
        