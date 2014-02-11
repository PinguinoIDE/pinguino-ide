#! /usr/bin/python
#-*- coding: utf-8 -*-

#import pickle
#import sys
#import re
#import os
#from ConfigParser import RawConfigParser

from PySide import QtGui, QtCore

from ..methods.syntax import Autocompleter

########################################################################
class Highlighter(QtGui.QSyntaxHighlighter):
    
    #----------------------------------------------------------------------
    def __init__(self, parent):
        super(Highlighter, self).__init__(parent)
        color = QtGui.QColor
        
        self.highlightingRules = []
        self.highlightingRulesMatch = []
        
        reservadas = QtGui.QTextCharFormat()
        reservadas.setForeground(color("#0000ff"))     
        all_reservadas = Autocompleter["reserved"] + Autocompleter["directive"]
        #namespaces = pickle.load(open(os.path.join(os.environ.get("PINGUINO_USER_PATH"), "reserved.pickle"), "r"))
        #namespaces = filter(lambda s:not "." in s, namespaces["all"])
        #all_reservadas += namespaces
        self.highlightingRules.append(("\\b("+"|".join(all_reservadas)+")\\b", reservadas))

        dotFuntions = QtGui.QTextCharFormat()
        dotFuntions.setForeground(color("#0000ff"))
        self.highlightingRules.append(("\\b[\D][\w]*\.[\D][\w]*", dotFuntions))
                
        decimal = QtGui.QTextCharFormat()
        decimal.setForeground(color("#ff0000"))
        self.highlightingRules.append(("\\b[\d]+\\b", decimal))
             
        float_ = QtGui.QTextCharFormat()
        float_.setForeground(color("#ff0000"))
        self.highlightingRules.append(("\\b[\d]+\.[\d]+\\b", float_))
        
        hexa = QtGui.QTextCharFormat()
        hexa.setForeground(color("#ff0000"))
        self.highlightingRules.append(("\\b0[Xx][A-Fa-f\d]+\\b", hexa))
        
        operators = QtGui.QTextCharFormat()
        operators.setFontWeight(QtGui.QFont.Bold)
        self.highlightingRules.append(("[()\[\]{}<>=\-\+\*\\%#!~&^,]", operators))
        
        bin_ = QtGui.QTextCharFormat()
        bin_.setForeground(color("#ff0000"))
        self.highlightingRules.append(("\\b0[Bb][01]+\\b", bin_))
        
        directives = QtGui.QTextCharFormat()
        directives.setForeground(color("#d36820"))
        self.highlightingRules.append(("#[ ]*[define|include|ifndef|endif|pragma][ ]*.*", directives))

        doubleQuotation = QtGui.QTextCharFormat() 
        doubleQuotation.setForeground(color("#7f0000"))
        self.highlightingRules.append((r'"[^"\\]*(\\.[^"\\]*)*"', doubleQuotation)) 
        
        singleQuotation = QtGui.QTextCharFormat()
        singleQuotation.setForeground(color("#cc0000"))
        self.highlightingRules.append((r"'[^'\\]*(\\.[^'\\]*)*'", singleQuotation))          
        
        singleComment = QtGui.QTextCharFormat()
        singleComment.setForeground(color("#007F00"))
        self.highlightingRules.append((r'//[^\n]*', singleComment)) 

        self.multiComment = QtGui.QTextCharFormat()
        self.multiComment.setForeground(color("#c81818"))   
        
        self.commentStartExpression = QtCore.QRegExp("/\\*")
        self.commentEndExpression = QtCore.QRegExp("\\*/")         
            
    #----------------------------------------------------------------------
    def highlightBlock(self, text): 
        for pattern, format_ in self.highlightingRules:
            expression = QtCore.QRegExp(pattern)
            index = expression.indexIn(text)
            while index >= 0:
                length = expression.matchedLength()
                self.setFormat(index, length, format_)
                index = expression.indexIn(text, index + length)
                            
        #for pattern, format in self.highlightingRulesMatch:
            #expression = QtCore.QRegExp(pattern)
            ##index  =  expression.indexIn(text)
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
    def buildComment(self, start, end, format_, text):
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
            self.setFormat(startIndex, commentLength, format_)
            startIndex = start.indexIn(text, startIndex + commentLength)
    
    #----------------------------------------------------------------------
    def addWord(self, word, tipo):
        color = QtGui.QColor
        newWord = QtGui.QTextCharFormat()
        newWord.setForeground(color(*self.fontsTypes(tipo)[0][:3]))
        if self.fontsTypes(tipo)[1]: newWord.setFontWeight(QtGui.QFont.Bold)
        newWord.setFontItalic(self.fontsTypes(tipo)[2])
        self.highlightingRules.insert(0, (QtCore.QRegExp(word), newWord)) 
        
    #----------------------------------------------------------------------
    def removeWord(self, word):
        self.highlightingRules.append((QtCore.QRegExp(word), QtGui.QTextCharFormat()))
