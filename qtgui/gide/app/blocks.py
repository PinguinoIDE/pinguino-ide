#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide import QtCore

Blocks = {"linear": "BlockLinear",
          "function": "BlockFunction",
          "nested": "BlockNested",
          "output": "BlockSpace",
          "output-bool": "BlockSpaceBool",
          "nested-second": "BlockNestedSecond",
          "frame-edit": "BlockFrameEdit",
          #"frame-plot": "BlockFramePlot",
          #"frame-debug": "BlockFrameDebug",
          }

CLOSEHANDCURSOR = QtCore.Qt.ClosedHandCursor
OPENHANDCURSOR = QtCore.Qt.OpenHandCursor
# CLOSEHANDCURSOR = QtCore.Qt.SizeAllCursor
# OPENHANDCURSOR = QtCore.Qt.SizeAllCursor