#! /usr/bin/python
#-*- coding: utf-8 -*-

from PySide import QtGui, QtCore

########################################################################
class Highlighter(QtGui.QSyntaxHighlighter):

    #----------------------------------------------------------------------
    def __init__(self, parent, START):
        super(Highlighter, self).__init__(parent)
        color = QtGui.QColor

        self.highlightingRules = []

        operators = QtGui.QTextCharFormat()
        operators.setFontWeight(QtGui.QFont.Bold)
        self.highlightingRules.append(("[()\[\]{}<>=\-\+\*\\%#!~&^,]", operators))

        start_command = QtGui.QTextCharFormat()
        start_command.setForeground(color("#729fcf"))
        self.highlightingRules.append((START, start_command))

        sdcc_error_01 = QtGui.QTextCharFormat()
        sdcc_error_01.setForeground(color("#ef292a"))
        self.highlightingRules.append(("ERROR: .*", sdcc_error_01))

        sdcc_error_02 = QtGui.QTextCharFormat()
        sdcc_error_02.setForeground(color("#ef292a"))
        self.highlightingRules.append(("\\b[\d]+: .*", sdcc_error_02))

        reserved = QtGui.QTextCharFormat()
        reserved.setForeground(color("#8ae234"))
        self.highlightingRules.append(("\\b(None|False|True|def|class)\\b", reserved))


    #----------------------------------------------------------------------
    def highlightBlock(self, text):
        for pattern, format_ in self.highlightingRules:
            expression = QtCore.QRegExp(pattern)
            index = expression.indexIn(text)
            while index >= 0:
                length = expression.matchedLength()
                self.setFormat(index, length, format_)
                index = expression.indexIn(text, index + length)

        #self.buildComment(self.commentStartExpression,
                          #self.commentEndExpression,
                          #self.multiComment,
                          #text)

    ##----------------------------------------------------------------------
    #def buildComment(self, start, end, format_, text):
        #self.setCurrentBlockState(0)
        #startIndex = 0
        #if self.previousBlockState() != 1:
            #startIndex = start.indexIn(text)
        #while startIndex >= 0:
            #endIndex = end.indexIn(text, startIndex)
            #if endIndex == -1:
                #self.setCurrentBlockState(1)
                #commentLength = len(text) - startIndex
            #else:
                #commentLength = endIndex - startIndex + end.matchedLength()
            #self.setFormat(startIndex, commentLength, format_)
            #startIndex = start.indexIn(text, startIndex + commentLength)

    ##----------------------------------------------------------------------
    #def addWord(self, word, tipo):
        #color = QtGui.QColor
        #newWord = QtGui.QTextCharFormat()
        #newWord.setForeground(color(*self.fontsTypes(tipo)[0][:3]))
        #if self.fontsTypes(tipo)[1]: newWord.setFontWeight(QtGui.QFont.Bold)
        #newWord.setFontItalic(self.fontsTypes(tipo)[2])
        #self.highlightingRules.insert(0, (QtCore.QRegExp(word), newWord))

    ##----------------------------------------------------------------------
    #def removeWord(self, word):
        #self.highlightingRules.append((QtCore.QRegExp(word), QtGui.QTextCharFormat()))
