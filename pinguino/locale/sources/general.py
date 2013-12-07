#!/usr/bin/env python
#-*- coding: utf-8 -*-

"""-------------------------------------------------------------------------
    General methods of Pinguino IDE.

    author:		Yeison Cardona
    contact:		yeison.eng@gmail.com 
    first release:	02/April/2012
    last release:	09/July/2012

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

import re
import os
import wx

from dic import Snippet, Autocompleter
from get_config import ReadConfig
from load_features import LoadFeatures

########################################################################
class General(ReadConfig, LoadFeatures):

    #----------------------------------------------------------------------
    def updateStatusBar(self, event=None):
        #self.findIndex = -1
        if len(self.stcpage) > 0:
            textEdit = self.getCurrentPage()
            fila = str(textEdit.CurrentLine+1).rjust(3, "0")
            columna = str(textEdit.GetColumn(textEdit.CurrentPos)).rjust(3, "0")
            self.statusBarEditor.SetStatusText(number=1, text="Line %s - Col %s" %(fila, columna))
            if event != None:
                event.Skip()

    #----------------------------------------------------------------------
    def wordUnderCursor(self,function=False):
        line,pos=self.getCurrentPage().CurLine
        so=line.split(" ")
        l=0
        for word in so:
            l+=len(word)+1
            if pos<l: break

        pos=len(word)-l+pos
        if function:
            all=re.findall("(\w*\.*\w*)",word)            
            l=0
            for word in all:
                if word=="": word=" "
                l+=len(word)
                if pos<l: return word  
        else:
            if not word.isalnum():
                all=re.findall("(\w*)",word)  
                l=0
                for word in all:
                    if word=="": word=" "
                    l+=len(word)
                    if pos<l: return word     
            else: return word


    #----------------------------------------------------------------------
    def insertSnippet(self, key):
        textEdit = self.getCurrentPage()
        index = self.wordUnderCursor()

        line = textEdit.GetCurLineUTF8()[0]

        s = 0
        for i in line:
            if i.isspace(): s += 1
            else: break
        rep = Snippet[key][1].replace("\n", "\n"+" "*s).replace("\t", self.getIndent())

        textEdit.InsertText(textEdit.CurrentPos, rep)
        for i in range(Snippet[key][0][0]-1):
            textEdit.LineDown()
        if type(Snippet[key][0][1]) == type(1):
            for i in range(Snippet[key][0][1]):
                textEdit.CharRight()
        elif  type(Snippet[key][0][1]) == type([]):
            s = len(self.getIndent() * Snippet[key][0][1][0])
            for i in range(Snippet[key][0][1][1]+s):
                textEdit.CharRight()


    #----------------------------------------------------------------------
    def getCompleters(self):
        icons = {}
        def addInDict(icon, list):
            for item in list: icons[item] = icon

        varbls = []
        allfunctions = []

        if self.getElse("Main", "Tools", "True") and self.getElse("Tools", "files", "True"):

            for i in self.Files.allVars:
                icons[i[0][:].replace("*", "")] = i[1][:].split()[-1]
                varbls.append(i[0][:].replace("*", ""))

            for i in self.Files.allFunc:
                icons[i[0][:].replace("*", "")] = "function"
                allfunctions.append(i[0][:].replace("*", ""))

            for i in self.Files.allDefi:
                icons[i[1][:].replace("*", "")] = "directive"
                varbls.append(i[1][:].replace("*", ""))

        autoComp = []
        for key in Autocompleter.keys(): autoComp.extend(Autocompleter[key][:])

        completer = allfunctions + self.keywordList + self.reservedword + Snippet.keys() + varbls[:] + autoComp + self.autoCompleteWords

        completersFilter = []
        for i in completer:
            if i not in completersFilter: completersFilter.append(i)
        completersFilter.sort()

        for i in Autocompleter["reserved"]:
            if i in self.keywordList: self.keywordList.remove(i)

        addInDict("snippet", Snippet.keys())
        addInDict("function", self.keywordList + allfunctions)
        addInDict("reserved", Autocompleter["reserved"])
        addInDict("directive", Autocompleter["directive"])

        return completersFilter + self.filesNeighbors, icons


    #----------------------------------------------------------------------
    def addArguments(self, funcionName):
        if self.getElse("Main", "tools", "True") and self.getElse("Tools", "files", "True"):
            for func in self.Files.allFunc:
                if func[0] in [funcionName, "*"+funcionName, "* "+funcionName]:
                    arg = func[2]
                    self.HideNextAutoComplete()
                    textEdit = self.getCurrentPage()
                    textEdit.InsertText(textEdit.CurrentPos, "("+arg+")")
                    textEdit.SetSelection(textEdit.CurrentPos+1, textEdit.CurrentPos+len(arg)+1)
                    return True
        return False

    #----------------------------------------------------------------------
    def HideNextAutoComplete(self):
        self.autocompleteHide = True


    #----------------------------------------------------------------------
    def updateSashs(self):
        """"""
        sashposco = self.getElse("IDE", "sashposco", 500)
        self.splitterCO.SetSashPosition(sashposco)

        sashposcl = self.getElse("IDE", "sashposcl", 500)
        self.splitterCL.SetSashPosition(sashposcl)
        
        if self.getElse("Main", "tools", "True") and self.getElse("Tools", "files", "True"):
            self.Files.adjustWidth()

    #----------------------------------------------------------------------
    def openLast(self):
        try:
            for i in range(self.getConfig("Last", "Last_count")):
                file = self.getConfig("Last", "Last_%d"%i)
                if os.path.isfile(file):
                    self.Open(file)
            if i > 0:
                name = self.getConfig("Last", "Last_Focus")
                if os.path.isfile(name):
                    c = 0
                    for i in self.filename:
                        if i == name: break
                        c += 1
                    self.notebookEditor.SetSelection(c)

        except: pass


    #----------------------------------------------------------------------
    def displaymsg(self, message, clearpanel, force_update=True):
        """ display message in the log window """
        try:
            if clearpanel==1: self.logwindow.Clear()
            try: elf.logwindow.WriteText(message.decode("utf-8", "replace")+"\n")
            except: self.logwindow.WriteText(message+"\n")
            if force_update: self.logwindow.Update()
        except: print message



    #----------------------------------------------------------------------
    def applyPreferences(self):
        self.Hide()
        self.toolbar.Destroy()
        self.buildToolbar()
        self.setEditorFont()
        self.OnSaveAll()
        self.OnCloseAll()
        self.openLast()
        self.updatenotebook()
        self.Show()

    #----------------------------------------------------------------------
    def restartApp(self, event=None):
        self.saveConfig()
        wx.GetApp().OnInit()
        self.Destroy()

    #----------------------------------------------------------------------
    def updateIDE(self):
        self.Fit()

#----------------------------------------------------------------------
def debugTime(function):
    import time
    DEBUG_TIME = True
    def process(*args):
        inicio = time.time()
        retorno = function(*args)
        fin = time.time()
        print function.__name__ + "\tTime: %.7fs" %(fin - inicio)
        return retorno
    if DEBUG_TIME : return process
    else: return function