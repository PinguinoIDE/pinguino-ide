#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide import QtGui

########################################################################
class CompleteIcons(object):
    
    
    #----------------------------------------------------------------------
    def __init__(self):
                
        self.iconFunction = QtGui.QIcon()
        self.iconFunction.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/function.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
                
        self.iconLibrary = QtGui.QIcon()
        self.iconLibrary.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/library.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
                
        self.iconBratches = QtGui.QIcon()
        self.iconBratches.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/vectors.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
                
        self.iconCuotation = QtGui.QIcon()
        self.iconCuotation.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/cuotation.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
                
        self.iconFloat = QtGui.QIcon()
        self.iconFloat.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/float.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
                
        self.iconInt = QtGui.QIcon()
        self.iconInt.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/int.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
                
        self.iconReserved = QtGui.QIcon()
        self.iconReserved.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/reserved.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        
        self.iconBool = QtGui.QIcon()
        self.iconBool.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/bool.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        
        self.iconByte = QtGui.QIcon()
        self.iconByte.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/byte.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        
        self.iconDouble = QtGui.QIcon()
        self.iconDouble.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/double.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        
        self.iconEnum = QtGui.QIcon()
        self.iconEnum.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/enum.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        
        self.iconLong = QtGui.QIcon()
        self.iconLong.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/long.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        
        self.iconShort = QtGui.QIcon()
        self.iconShort.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/short.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        
        self.iconStruct = QtGui.QIcon()
        self.iconStruct.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/struct.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        
        self.iconUnion = QtGui.QIcon()
        self.iconUnion.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/union.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        
        self.iconWord = QtGui.QIcon()
        self.iconWord.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/word.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        
        self.iconSnippet = QtGui.QIcon()
        self.iconSnippet.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/snippet.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        
        self.iconDirectives = QtGui.QIcon()
        self.iconDirectives.addPixmap(QtGui.QPixmap(":/autocompleter/autocompleter/directivas.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        
        