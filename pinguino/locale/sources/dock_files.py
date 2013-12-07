#!/usr/bin/env python
#-*- coding: utf-8 -*-

"""-------------------------------------------------------------------------
    ShortDescription.

    author:		Yeison Cardona
    contact:		yeison.eng@gmail.com 
    first release:	31/March/2012
    last release:	08/July/2012
    
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
-------------------------------------------------------------------------"""

import wx, re
import sys, os

from wxgui._trad import _

########################################################################
class File:
    ENABLE = False
    #----------------------------------------------------------------------
    def __initDockFile__(self, IDE):
        self.IDE = IDE
        self.lateralFunc = self.IDE.listCtrlFunc
        self.lateralVars = self.IDE.listCtrlVars
        self.lateralDefi = self.IDE.listCtrlDefi
        
        self.IDE.Bind(wx.EVT_LIST_ITEM_ACTIVATED, self.moveToFunc, self.lateralFunc)
        self.IDE.Bind(wx.EVT_LIST_ITEM_ACTIVATED, self.moveToVar, self.lateralVars)
        self.IDE.Bind(wx.EVT_LIST_ITEM_ACTIVATED, self.moveToDefi, self.lateralDefi)

        self.line = 60
        s3 = (self.IDE.lat.GetSizeTuple()[0] - self.line) / 3

        self.lateralVars.InsertColumn(col=0, format=wx.LIST_FORMAT_LEFT, heading=_("Name"), width=s3)
        self.lateralVars.InsertColumn(col=1, format=wx.LIST_FORMAT_LEFT, heading=_("Type"), width=s3)
        self.lateralVars.InsertColumn(col=2, format=wx.LIST_FORMAT_LEFT, heading=_("In function"), width=s3)
        self.lateralVars.InsertColumn(col=3, format=wx.LIST_FORMAT_LEFT, heading=_("Line"), width=self.line)
    
        self.lateralFunc.InsertColumn(col=0, format=wx.LIST_FORMAT_LEFT, heading=_("Name"), width=s3) 
        self.lateralFunc.InsertColumn(col=1, format=wx.LIST_FORMAT_LEFT, heading=_("Return"), width=s3) 
        self.lateralFunc.InsertColumn(col=2, format=wx.LIST_FORMAT_LEFT, heading=_("Parameters"), width=s3)
        self.lateralFunc.InsertColumn(col=3, format=wx.LIST_FORMAT_LEFT, heading=_("Line"), width=self.line)
        
        self.lateralDefi.InsertColumn(col=0, format=wx.LIST_FORMAT_LEFT, heading=_("Directive"), width=s3) 
        self.lateralDefi.InsertColumn(col=1, format=wx.LIST_FORMAT_LEFT, heading=_("Name"), width=s3) 
        self.lateralDefi.InsertColumn(col=2, format=wx.LIST_FORMAT_LEFT, heading=_("Value"), width=s3) 
        self.lateralDefi.InsertColumn(col=3, format=wx.LIST_FORMAT_LEFT, heading=_("Line"), width=self.line)
        

        self.ENABLE = True
        
    #----------------------------------------------------------------------
    def adjustWidth(self):
        s3 = (self.IDE.lat.GetSizeTuple()[0] - self.line) / 3
        for i in range(4):
            if i == 3:
                self.lateralVars.SetColumnWidth(i, self.line)
                self.lateralFunc.SetColumnWidth(i, self.line)
                self.lateralDefi.SetColumnWidth(i, self.line)
            else:
                self.lateralVars.SetColumnWidth(i, s3)
                self.lateralFunc.SetColumnWidth(i, s3)
                self.lateralDefi.SetColumnWidth(i, s3)
        

    #----------------------------------------------------------------------
    def moveToVar(self, event=None):
        textEdit = self.IDE.stcpage[self.IDE.notebookEditor.GetSelection()]
        textEdit.GotoLine(textEdit.LineCount)
        self.allVars.reverse()
        color = self.IDE.getColorConfig("Highligh", "codenavigation", [120, 255, 152])
        self.IDE.highlightline(int(self.allVars[event.GetIndex()][3])-1, color)
        self.allVars.reverse()
        textEdit.SetFocus()
        
    #----------------------------------------------------------------------
    def moveToFunc(self, event=None):
        textEdit = self.IDE.stcpage[self.IDE.notebookEditor.GetSelection()]
        textEdit.GotoLine(textEdit.LineCount)
        self.allFunc.reverse()
        color = self.IDE.getColorConfig("Highligh", "codenavigation", [120, 255, 152])
        self.IDE.highlightline(int(self.allFunc[event.GetIndex()][3])-1, color)
        self.allFunc.reverse()
        textEdit.SetFocus()
        
    #----------------------------------------------------------------------
    def moveToDefi(self, event=None):
        textEdit = self.IDE.stcpage[self.IDE.notebookEditor.GetSelection()]
        textEdit.GotoLine(textEdit.LineCount)
        self.allDefi.reverse()
        color = self.IDE.getColorConfig("Highligh", "codenavigation", [120, 255, 152])
        self.IDE.highlightline(int(self.allDefi[event.GetIndex()][3])-1, color)
        self.allDefi.reverse()
        textEdit.SetFocus()  
        
    #----------------------------------------------------------------------
    def addVarInListCtrl(self, index, var):
        self.lateralVars.InsertStringItem(0, var[0])        
        self.lateralVars.SetStringItem(0, 1, var[1])         
        self.lateralVars.SetStringItem(0, 2, var[2])        
        self.lateralVars.SetStringItem(0, 3, var[3])    
        self.lateralVars.SetItemData(0, 1)
        
    #----------------------------------------------------------------------
    def addFuncInListCtrl(self, index, var):
        self.lateralFunc.InsertStringItem(0, var[0])        
        self.lateralFunc.SetStringItem(0, 1, var[1])         
        self.lateralFunc.SetStringItem(0, 2, var[2])         
        self.lateralFunc.SetStringItem(0, 3, var[3])   
        self.lateralFunc.SetItemData(0, 1)
        
    #----------------------------------------------------------------------
    def addDefiInListCtrl(self, index, var):
        self.lateralDefi.InsertStringItem(0, var[0])            
        self.lateralDefi.SetStringItem(0, 1, var[1])         
        self.lateralDefi.SetStringItem(0, 2, var[2])        
        self.lateralDefi.SetStringItem(0, 3, var[3])         
        self.lateralDefi.SetItemData(0, 1)
        
    #----------------------------------------------------------------------
    def updateWidthColums(self):
        line = 60
        withAll = (self.lateralDefi.GetSizeTuple()[0])
        lct = [self.lateralVars, self.lateralDefi, self.lateralFunc]
        for lc in lct:
            withUse =  lc.GetColumnWidth(0) + lc.GetColumnWidth(1) + lc.GetColumnWidth(2) + lc.GetColumnWidth(3)
            withExtra = withAll - withUse - 5
            if withExtra != withUse:
                lc.SetColumnWidth(0, lc.GetColumnWidth(0) + withExtra)                
            lc.SetColumnWidth(3, line)
        
    #----------------------------------------------------------------------
    def update_dockFiles(self, event=None):
        #if not self.ENABLE: return

        
        self.updateWidthColums()
        
        if self.IDE.notebookEditor.PageCount > 0:
            self.IDE.panelOutput.Show()
            self.IDE.lat.Show()
        else:
            self.IDE.panelOutput.Hide()
            self.IDE.lat.Hide()
        
        if len(self.IDE.stcpage) < 1:
            self.lateralVars.DeleteAllItems()
            self.lateralFunc.DeleteAllItems()
            if event: event.Skip()
            return 
            
        try:
            textEdit = self.IDE.stcpage[self.IDE.notebookEditor.GetSelection()]
            text = textEdit.GetText().split("\n")
            dirname = self.IDE.filename[self.IDE.notebookEditor.GetSelection()]
            self.filesNeighbors = os.listdir(os.path.dirname(dirname))      
        except:
            return
        
        
        
        self.tiposDatos = "int|float|char|double|"\
                          "u8|u16|u32|u64|"\
                          "BOOL|byte|word|void"
        
        def updateRegex():
            
            ReFunction = "[\s]*(unsigned)*[\s]*(" + self.tiposDatos + ")[\s]*([*])*[\s]*([*\w]+)[\s]*\(([\w ,*.\[\]]*)\)[\s]*"
            
            ReVariable = "[\s]*(volatile|register|static|extern)*[\s]*(unsigned|signed)*[\s]*(short|long)*[\s]*(" + self.tiposDatos + ")[\s]*([*])*[\s]*([ \w\[\]=,{}\"'\*]*);"
            ReStructs  = "[\s]*(typedef)*[\s]*(struct|union|enum)[\s]*([*\w]*)[\s]*(.+)"
            
            ReTypeDef  = "[\s]*(typedef)[\s]*(unsigned|signed)*[\s]*(short|long)*[\s]*(" + self.tiposDatos + ")[\s]*([\w\[\]]*)[\s]*"#([\w]*)"
            
            ReTypeStru = "[\s]*}[\s]*([\w]+)[\s]*;"
            
            ReDefines = "[\s]*#(define|ifndef|endif)[\s]+([\S]*)[\s]+([\S]*)"
            ReInclude = "[\s]*#include[ ]+<[\s]*([\S]*)[\s]*>"
        
            return ReFunction, ReVariable, ReStructs, ReTypeDef, ReDefines, ReInclude, ReTypeStru
  

        self.allVars = [] 
        self.allFunc = []
        self.allDefi = []
        self.autoCompleteWords = []
        currentFunction = ""
        
        ReFunction, ReVariable, ReStructs, ReTypeDef, ReDefines, ReInclude, ReTypeStru = updateRegex()
        
        
        def getVar(var, tipo):
            if "=" in var: var = var[:var.find("=")]
            if "[" in var:
                var = var[:var.find("[")]
                if tipo != "char": tipo = "vect"
            return var, tipo
        
        def getParam(param):
            param = param.split(",")
            ret = []
            for par in param:
                reg = re.match(ReVariable, par)
                
        def getOutTo(a, b, cont):
            if a in cont:
                x = cont.find(a)
                cont = cont[:x]
            if b in cont:
                y = cont.find(b)
                cont = cont+ cont[y:]
            return cont
                
        count = 1
        for linea in text:
            if "//" in linea:
                linea = linea[:linea.find("//")]
            
            reg1 = re.match(ReFunction, linea)
            if reg1 != None:
                if not (linea.replace(" ", "").replace("\n", "").replace("\r", "").endswith(";")):
                    
                    if reg1.group(3) == "*":
                        if reg1.group(1) != None: tipo = "pointer to " + " ".join(reg1.group(1)+reg1.group(2))
                        else:tipo = "pointer to " + reg1.group(2)
                    else:
                        if reg1.group(1) != None: tipo = " ".join(reg1.group(1)+reg1.group(2))
                        else: tipo = reg1.group(2)
                    
                    self.allFunc.append([reg1.group(4),
                                         tipo, 
                                        #reg1.group(2),
                                        reg1.group(5),                                        
                                        str(count)])
                    currentFunction = reg1.group(4)
                                    
            reg2a = re.match(ReVariable, linea)
            reg2b = re.match(ReStructs, linea)
            
            if reg2a != None or reg2b != None:                
                if reg2b != None: #Struct
                    reg2 = reg2b
                    tipo = reg2.group(2)
                    cont = reg2.group(3)
                elif reg2a != None: #Variable
                    reg2 = reg2a
                    t = [reg2.group(1), reg2.group(2), reg2.group(3), reg2.group(4)]
                    for i in range(t.count(None)): t.remove(None)
                    if reg2.group(5) == "*": tipo = "pointer to " + " ".join(t)
                    else: tipo = " ".join(t)
                    cont = reg2.group(6)
                    
                cont = getOutTo("{", "}", cont)
                if "=" in cont: cont = getOutTo("(", ")", cont)
                    
                cont = cont.split(",")
                self.allVars.extend([[getVar(var, tipo)[0],
                                    getVar(var, tipo)[1],
                                    
                                    currentFunction,                                    
                                    str(count)] for var in cont])
                
                
            reg3 = re.match(ReDefines, linea)
            if reg3 != None:
                self.allDefi.append([reg3.group(1),
                                     reg3.group(2),
                                     reg3.group(3),
                                     str(count)])
                
            reg4 = re.match(ReInclude, linea)
            if reg4 != None:
                self.allDefi.append(["include",
                                     reg4.group(1),
                                     "",
                                     str(count)])
                
                
            reg5 = re.match(ReTypeDef, linea)
            if reg5 != None:
                if reg5.group(5) != "":
                    self.tiposDatos += "|%s" %reg5.group(5)
                    self.autoCompleteWords.append(reg5.group(5))
                    ReFunction, ReVariable, ReStructs, ReTypeDef, ReDefines, ReInclude, ReTypeStru = updateRegex()
                
            reg6 = re.match(ReTypeStru, linea)
            if reg6 != None:
                self.tiposDatos += "|%s" %reg6.group(1)
                self.autoCompleteWords.append(reg6.group(1))
                ReFunction, ReVariable, ReStructs, ReTypeDef, ReDefines, ReInclude, ReTypeStru = updateRegex()        
                
            count += 1
            
        self.allDefi.sort()
        self.IDE.allDefi_back.sort()
        self.allFunc.sort()
        self.IDE.allFunc_back.sort()
        self.allVars.sort()
        self.IDE.allVars_back.sort()
              
        if self.IDE.allVars_back != self.allVars:        
            count = 0
            #self.allVars.reverse()
            self.lateralVars.DeleteAllItems()
            for var in self.allVars:
                self.addVarInListCtrl(count, var)
                count += 1
            self.IDE.allVars_back = self.allVars[:]
        
        if self.IDE.allFunc_back != self.allFunc:
            count = 0
            #self.allFunc.reverse()
            self.lateralFunc.DeleteAllItems()
            for var in self.allFunc:
                self.addFuncInListCtrl(count, var)
                count += 1
            self.IDE.allFunc_back = self.allFunc[:]
        
        
        if self.IDE.allDefi_back != self.allDefi:
            count = 0
            #self.allDefi.reverse()
            self.lateralDefi.DeleteAllItems()
            for var in self.allDefi:
                self.addDefiInListCtrl(count, var)
                count += 1
            self.IDE.allDefi_back = self.allDefi[:]
            

        if event != None: event.Skip() #Update NoteBook   
  
