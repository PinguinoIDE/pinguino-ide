#! /usr/bin/python
#-*- coding: utf-8 -*-

import pickle
import sys
import re
import os
from ConfigParser import RawConfigParser

from PySide import QtGui, QtCore

from ..helpers.syntax import Autocompleter
from ..helpers import constants as Constants
from ..helpers.config import Config

########################################################################
class Highlighter(QtGui.QSyntaxHighlighter):
    #----------------------------------------------------------------------
    def __init__(self, parent):
        super(Highlighter, self).__init__(parent)
        color=QtGui.QColor
        
        self.highlightingRules=[]
        self.highlightingRulesMatch=[]
        
        
        namespaces = pickle.load(file(os.path.join(os.environ.get("PINGUINO_USER_PATH"), "reserved.pickle"), "r"))  
        reservadas=QtGui.QTextCharFormat()
        reservadas.setForeground(color("#0000ff"))     
        all_reservadas = Autocompleter["reserved"] + Autocompleter["directive"] + namespaces["all"] + namespaces["arch8"] + namespaces["arch32"]
        self.highlightingRules.append(("\\b("+"|".join(all_reservadas)+")\\b", reservadas))

        dotFuntions=QtGui.QTextCharFormat()
        dotFuntions.setForeground(color("#0000ff"))
        self.highlightingRules.append(("\\b[\D][\w]*\.[\D][\w]*",dotFuntions))
                
        decimal=QtGui.QTextCharFormat()
        decimal.setForeground(color("#ff0000"))
        self.highlightingRules.append(("\\b[\d]+\\b",decimal))
             
        float=QtGui.QTextCharFormat()
        float.setForeground(color("#ff0000"))
        self.highlightingRules.append(("\\b[\d]+\.[\d]+\\b",float))
        
        hexa=QtGui.QTextCharFormat()
        hexa.setForeground(color("#ff0000"))
        self.highlightingRules.append(("\\b0[Xx][A-Fa-f\d]+\\b",hexa))
        
        operators=QtGui.QTextCharFormat()
        operators.setFontWeight(QtGui.QFont.Bold)
        self.highlightingRules.append(("[()\[\]{}<>=\-\+\*\\%#!~&^,]",operators))
        
        bin=QtGui.QTextCharFormat()
        bin.setForeground(color("#ff0000"))
        self.highlightingRules.append(("\\b0[Bb][01]+\\b",bin))
        
        directives=QtGui.QTextCharFormat()
        directives.setForeground(color("#d36820"))
        self.highlightingRules.append(("#[ ]*[define|include|ifndef|endif][ ]*.*", directives))

        doubleQuotation=QtGui.QTextCharFormat() 
        doubleQuotation.setForeground(color("#7f0000"))
        self.highlightingRules.append((r'"[^"\\]*(\\.[^"\\]*)*"', doubleQuotation)) 
        
        singleQuotation=QtGui.QTextCharFormat()
        singleQuotation.setForeground(color("#cc0000"))
        self.highlightingRules.append((r"'[^'\\]*(\\.[^'\\]*)*'", singleQuotation))          
        
        singleComment=QtGui.QTextCharFormat()
        singleComment.setForeground(color("#007F00"))
        self.highlightingRules.append((r'//[^\n]*', singleComment)) 

        self.multiComment=QtGui.QTextCharFormat()
        self.multiComment.setForeground(color("#c81818"))
        #self.multiComment.setFontItalic(self.fontsTypes("Multi line comment")[2])      
        
        self.commentStartExpression = QtCore.QRegExp("/\\*")
        self.commentEndExpression = QtCore.QRegExp("\\*/")         
        
        user=QtGui.QTextCharFormat()
        user.setForeground(QtCore.Qt.darkMagenta)    
        #for word in syntax.user:
            #self.highlightingRules.append((QtCore.QRegExp(word),user))
            
    #----------------------------------------------------------------------
    def highlightBlock(self, text): 
        for pattern, format in self.highlightingRules:
            expression = QtCore.QRegExp(pattern)
            index = expression.indexIn(text)
            while index >= 0:
                length = expression.matchedLength()
                self.setFormat(index, length, format)
                index = expression.indexIn(text, index + length)
                            
        #for pattern, format in self.highlightingRulesMatch:
            #expression = QtCore.QRegExp(pattern)
            ##index = expression.indexIn(text)
            #print expression.capturedTexts()

            #while index >= 0:
                #length = expression.matchedLength()
                #self.setFormat(index, length, format)
                #index = expression.indexIn(text, index + length)
  
        self.buildComment(self.commentStartExpression,
                          self.commentEndExpression,
                          self.multiComment,
                          text)
  
    #----------------------------------------------------------------------
    def buildComment(self,start,end,format,text):
        self.setCurrentBlockState(0)
        startIndex = 0
        if self.previousBlockState() != 1:
            startIndex = start.indexIn(text)
        while startIndex >= 0:
            endIndex = end.indexIn(text, startIndex)
            if endIndex == -1:
                self.setCurrentBlockState(1)
                commentLength = len(text) - startIndex
            else:
                commentLength = endIndex - startIndex + end.matchedLength()
            self.setFormat(startIndex, commentLength, format)
            startIndex = start.indexIn(text, startIndex + commentLength);
    
    #----------------------------------------------------------------------
    def addWord(self,word,tipo):
        color=QtGui.QColor
        newWord=QtGui.QTextCharFormat()
        newWord.setForeground(color(*self.fontsTypes(tipo)[0][:3]))
        if self.fontsTypes(tipo)[1]: singleComment.setFontWeight(QtGui.QFont.Bold)
        newWord.setFontItalic(self.fontsTypes(tipo)[2])
        self.highlightingRules.insert(0,(QtCore.QRegExp(word), newWord)) 
        
    #----------------------------------------------------------------------
    def removeWord(self,word):
        self.highlightingRules.append((QtCore.QRegExp(word), QtGui.QTextCharFormat()))
