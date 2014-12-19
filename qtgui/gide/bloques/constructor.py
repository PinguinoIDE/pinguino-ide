#!/usr/bin/env python
#-*- coding: utf-8 -*-

#import os
#import sys

from PySide import QtGui, QtCore

#from .widgets import Label
from .widgets.label import Ui_Frame as Label

#from .widgets import ControlSpin
from .widgets.control_spin import Ui_Frame as ControlSpin

#from .widgets import ControlSlider
from .widgets.control_slider import Ui_Frame as ControlSlider

#from .widgets import Name
from .widgets.name import Ui_Frame as Name

#from .inside import Inside, Inside2
from .inside.inside import Ui_Form as Inside
from .inside.inside2 import Ui_Form as Inside2

#from .inside import InsideBool, Inside2Bool
from .inside.inside_bool import Ui_Form as InsideBool
from .inside.inside2_bool import Ui_Form as Inside2Bool


FIX_SYNTAX = ", ; = > < >= <= + -".split()

FONT_SIZE = 15
ADJ = 10  #space at end
ADJ2 = -5  #space on expand

###Windows Xp
##FONT_SIZE = 14
##ADJ = 20
##ADJ2 = -3


#----------------------------------------------------------------------
def changeFontSize(widget):
    #name = widget.__str__()
    #name = name[name.rfind(".")+1:name.find(" ")]

    widget.setStyleSheet("""
    QLineEdit{
        color: #FFFFFF;
        font-family: mono;
        font-weight: normal;
        font-size: 12pt;
    }

    QLabel{
        color: #FFFFFF;
        font-family: mono;
        font-weight: normal;
        font-size: 12pt;
    }

    QPlainTextEdit{
        color: #FFFFFF;
        font-family: mono;
        font-weight: normal;
        font-size: 12pt;
    }

    """)



########################################################################
class Constructor(object):

    DeletLater = False

    def buildBlock(self, widget, bloque, insidePos=(27, 2), inside=False):
        self.layout_adds = []
        self.layout_adds_b = []
        self.layout_adds_all = []
        layout_pos = []
        self.constructorCode = []
        self.LineCode = []
        self.fullWidgetWith = []
        self.codeStart = {}

        changeFontSize(widget)

        for add in bloque:
            if add[0] == "label": self.addLabel(widget, add[1])
            if add[0] == "space": self.addToInside(widget, inside)
            if add[0] == "space_bool": self.addToInsideBool(widget, inside)
            if add[0] == "edit": self.addLineEdit(widget, add[:])
            if add[0] == "spin_int": self.addSpinInt(widget, add[:])
            if add[0] == "spin_choice": self.addChoice(widget, add[:])
            if add[0] == "spin_float": self.addSpinFloat(widget, add[:])
            if add[0] == "syntax": self.addSyntax(widget, add[1])
            if add[0] == "slider": self.addSlider(widget, add[:])
            #if add[0] == "button": self.addButton(widget, add[:])
            if add[0] == "decorator": self.addDecorator(widget, add[1])
            if add[0] == "help": widget.setToolTip(add[1])
            if add[0] == "full": self.fullWidgetWith = add[1]
            if add[0] == "code": self.codeStart = add[1]


        self.setFlex(self.layout_adds, insidePos, self.layout_adds_b, self.layout_adds_all)
        widget.contextMenuEvent = self.build_menu


    #----------------------------------------------------------------------
    def get_widgets_from_layout(self):
        if self.layout == None: return[]
        widgets = []
        for index in range(self.layout.count()):
            widgets.append((index, self.layout.itemAt(index).widget()))

        return filter(lambda wid:wid[1].objectName().startswith("BLOCK"), widgets)


    ##----------------------------------------------------------------------
    #def get_widgets_from_layout_save(self):
        #if self.layout == None: return[]
        #widgets = []
        #for index in range(self.layout.count()):
            #widgets.append((index, self.layout.itemAt(index).widget()))

        #return filter(lambda wid:wid[1].objectName().startswith("BLOCK"), widgets)




    #----------------------------------------------------------------------
    def buildLabel(self, widget, text="Label", rgb=None):
        widgetLabel = QtGui.QWidget(widget)
        label = Label()
        label.setupUi(widgetLabel)
        label.label.setText(text)

        #changeFontSize(label.label)

        if rgb != None:
            #label.label.setStyleSheet("background-color: rgb(%d, %d, %d);\ncolor: rgb(255, 255, 255);" %rgb)
            label.label.setStyleSheet(label.label.styleSheet()+"""
            QLabel{
                background-color: rgb(%d, %d, %d);
                color: rgb(255, 255, 255);
            }

            """%rgb)

        if len(text) > 3:
            widgetLabel.setMinimumSize(len(text)*(FONT_SIZE+ADJ2)+ADJ, widgetLabel.size().height())
        else:
            widgetLabel.setMinimumSize(len(text)*(FONT_SIZE+ADJ2), widgetLabel.size().height())

        self.LineCode.append(lambda :text)
        return widgetLabel, label.label

    #----------------------------------------------------------------------
    def buildDecorator(self, widget, text="Label", rgb=None):
        widgetLabel = QtGui.QWidget(widget)
        label = Label()
        label.setupUi(widgetLabel)
        label.label.setText(text)
        #changeFontSize(label.label)
        if rgb != None:
            #label.label.setStyleSheet("background-color: rgb(%d, %d, %d);\ncolor: rgb(255, 255, 255);" %rgb)            label.label.setStyleSheet(label.label.styleSheet()+"""
            label.label.setStyleSheet(label.label.styleSheet()+"""
            QLabel{
                background-color: rgb(%d, %d, %d);
                color: rgb(255, 255, 255);
            }

            """%rgb)

        if len(text) > 3:
            widgetLabel.setMinimumSize(len(text)*(FONT_SIZE+ADJ2)+ADJ, widgetLabel.size().height())
        else:
            widgetLabel.setMinimumSize(len(text)*(FONT_SIZE+ADJ2), widgetLabel.size().height())

        return widgetLabel, label.label


    #----------------------------------------------------------------------
    def buildSpinInt(self, widget, start, b_min=-100, b_max=100):
        widgetSpin = QtGui.QWidget(widget)
        spin = ControlSpin()
        spin.setupUi(widgetSpin)

        #icon = QtGui.QIcon()
        #icon.addPixmap(QtGui.QPixmap("%sup.png"%self.isideDir), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        #spin.toolButton_3.setIcon(icon)

        #icon1 = QtGui.QIcon()
        #icon1.addPixmap(QtGui.QPixmap("%sdown.png"%self.isideDir), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        #spin.toolButton_4.setIcon(icon1)


        spin.frame.setStyleSheet("image: url(%sup.png);"%self.isideDir)
        spin.frame_2.setStyleSheet("image: url(%sdown.png);"%self.isideDir)

        edit = spin.lineEdit
        #edit.setStyleSheet("background-image: url(:/bg/bg/box.png);\ncolor: rgb(90, 90, 90);")

        b_up = spin.frame
        b_down = spin.frame_2

        #changeFontSize(edit)

        edit.setStyleSheet(edit.styleSheet()+"""
        QLineEdit{
            background-image: url(:/bg/bg/box.png);
            color: rgb(90, 90, 90);
        }

        """)


        edit.setText(str(start))

        def up(*args):
            try: current = int(edit.text())
            except ValueError: current = start
            if current == b_max: b_next = str(b_min)
            else: b_next = str(current+1)
            edit.setText(b_next)

        def down(*args):
            try: current = int(edit.text())
            except ValueError: current = start
            if current == b_min: b_next = str(b_max)
            else: b_next = str(current-1)
            edit.setText(b_next)

        b_up.setCursor(QtCore.Qt.PointingHandCursor)
        b_down.setCursor(QtCore.Qt.PointingHandCursor)
        b_up.mousePressEvent = up
        b_down.mousePressEvent = down

        edit.setMinimumSize(50, 25)
        edit.setMaximumSize(50, 25)

        self.LineCode.append(edit.text)
        return widgetSpin, edit


    #----------------------------------------------------------------------
    def buildSpinFloat(self, widget, start, step=0.1, b_min=-100.0, b_max=100.0):
        widgetSpin = QtGui.QWidget(widget)
        spin = ControlSpin()
        spin.setupUi(widgetSpin)

        #icon = QtGui.QIcon()
        #icon.addPixmap(QtGui.QPixmap("%sup.png"%self.isideDir), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        #spin.toolButton_3.setIcon(icon)

        #icon1 = QtGui.QIcon()
        #icon1.addPixmap(QtGui.QPixmap("%sdown.png"%self.isideDir), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        #spin.toolButton_4.setIcon(icon1)


        spin.frame.setStyleSheet("image: url(%sup.png);"%self.isideDir)
        spin.frame_2.setStyleSheet("image: url(%sdown.png);"%self.isideDir)


        edit = spin.lineEdit
        b_up = spin.frame
        b_down = spin.frame_2

        edit.setText(str(start))
        #changeFontSize(edit)

        edit.setStyleSheet(edit.styleSheet()+"""
        QLineEdit{
            background-image: url(:/bg/bg/box.png);
            color: rgb(90, 90, 90);
        }

        """)


        def up(*args):
            try: current = float(edit.text())
            except ValueError: current = start
            if current == b_max: b_next = str(b_min)
            else: b_next = str(current+step)
            edit.setText(b_next)

        def down(*args):
            try: current = float(edit.text())
            except ValueError: current = start
            if current == b_min: b_next = str(b_max)
            else: b_next = str(current-step)
            edit.setText(b_next)


        b_up.setCursor(QtCore.Qt.PointingHandCursor)
        b_down.setCursor(QtCore.Qt.PointingHandCursor)
        b_up.mousePressEvent = up
        b_down.mousePressEvent = down


        edit.setMinimumSize(0, 25)
        edit.setMaximumSize(0, 25)

        self.LineCode.append(edit.text)
        return widgetSpin, edit


    #----------------------------------------------------------------------
    def buildName(self, widget, text, background=None, color=(255, 255, 255)):
        widgetName = QtGui.QWidget(widget)
        name = Name()
        name.setupUi(widgetName)
        name.lineEdit.setText(str(text))
        #changeFontSize(name.lineEdit)

        if background == "white" and color != None:
            #name.lineEdit.setStyleSheet("background-image: url(:/bg/bg/box.png);\ncolor: rgb(%d, %d, %d);" %color)
            name.lineEdit.h = name.lineEdit.size().height() - 5

            name.lineEdit.setStyleSheet("""
            QLineEdit{
                background-image: url(:/bg/bg/box.png);
                color: rgb(%d, %d, %d);
                font-family: mono;
            }
            """%color)




        elif background == "white" and color == None:
            #name.lineEdit.setStyleSheet("background-image: url(:/bg/bg/box.png);")
            name.lineEdit.h = name.lineEdit.size().height() - 5
            name.lineEdit.setStyleSheet("""
            QLineEdit{
                background-image: url(:/bg/bg/box.png);
                font-family: mono;
            }
            """)



        elif background == None and color != None:
            #name.lineEdit.setStyleSheet("color: rgb(%d, %d, %d);" %color)
            name.lineEdit.h = name.lineEdit.size().height() + 4
            name.lineEdit.setStyleSheet("""
            QLineEdit{
                color: rgb(%d, %d, %d);
                font-family: mono;
            }
            """%color)


        self.LineCode.append(name.lineEdit.text)
        return widgetName, name.lineEdit


    #----------------------------------------------------------------------
    def buildChoice(self, widget, curr, opc):

        opc = list(opc)
        widgetChoice = QtGui.QWidget(widget)
        choice = ControlSpin()
        choice.setupUi(widgetChoice)
        choice.lineEdit.setText(curr)
        choice.lineEdit.setReadOnly(True)
        #choice.lineEdit.sele(True)

        choice.lineEdit.setStyleSheet("background-image: url(:/bg/bg/box.png); color: rgb(90,90,90);")

        #icon = QtGui.QIcon()
        #icon.addPixmap(QtGui.QPixmap("%sup.png"%self.isideDir), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        #choice.toolButton_3.setIcon(icon)

        choice.frame.setStyleSheet("image: url(%sup.png);"%self.isideDir)
        choice.frame_2.setStyleSheet("image: url(%sdown.png);"%self.isideDir)


        edit = choice.lineEdit
        b_up = choice.frame
        b_down = choice.frame_2
        b_min = 0
        b_max = len(opc) - 1

        #changeFontSize(edit)


        def up(*args):
            try: current = opc.index(edit.text())
            except ValueError: current = 0
            if current == b_max: b_next = b_min
            else: b_next = current+1
            edit.setText(opc[b_next])

        def down(*args):
            try: current = opc.index(edit.text())
            except ValueError: current = 0
            if current == b_min: b_next = b_max
            else: b_next = current-1
            edit.setText(opc[b_next])

        b_up.setCursor(QtCore.Qt.PointingHandCursor)
        b_down.setCursor(QtCore.Qt.PointingHandCursor)
        b_up.mousePressEvent = up
        b_down.mousePressEvent = down

        #widgetLabel.setMinimumSize(len(text)*FONT_SIZE+ADJ, widgetLabel.size().height())

        l = 0
        for ch in opc:
            if len(ch) > l: l = len(ch)


        edit.setMinimumSize(l*(FONT_SIZE+ADJ2)+20, 25)
        edit.setMaximumSize(l*(FONT_SIZE+ADJ2)+20, 25)

        self.LineCode.append(edit.text)
        return widgetChoice, edit

    #----------------------------------------------------------------------
    def buildSliderInt(self, widget, start, b_min=0, b_max=1023):
        widgetSlider = QtGui.QWidget(widget)
        slider = ControlSlider()
        slider.setupUi(widgetSlider)

        edit = slider.lineEdit_2
        #changeFontSize(edit)
        slid = slider.horizontalSlider
        #edit.setStyleSheet("background-image: url(:/general/arte/box.png);\ncolor: rgb(0, 0, 0);")
        #edit.setStyleSheet("color: rgb(255, 255, 255);")
        edit.setStyleSheet(edit.styleSheet()+"""
        QLineEdit {
        color: #FFFFFF;
        }

        """)


        def updateEdit(value): edit.setText(str(value).rjust(4, "0"))
        QtCore.QObject.connect(slid, QtCore.SIGNAL("sliderMoved(int)"), updateEdit)
        edit.setText(str(start).rjust(4, "0"))
        slid.setValue(int(start))

        widgetSlider.setMinimumSize(180, widgetSlider.size().height())
        widgetSlider.setMaximumSize(180, widgetSlider.size().height())

        self.LineCode.append(edit.text)
        return widgetSlider, edit



    ##----------------------------------------------------------------------
    #def buildButton(self, widget, text, checkeable=False, start=False, loop=False):
        #
        #widgetButton = QtGui.QWidget(widget)
        #button = ControlButton()
        #button.setupUi(widgetButton)


        #button.pushButton.setStyleSheet("background-image: url(%s/bloques/widgets/arte/box.png);"%sys.path[0])

        #def down():
            #if button.pushButton.isChecked():
                #button.pushButton.setText("OFF")
                #button.pushButton.setStyleSheet("background-image: url(%s/bloques/widgets/arte/box2.png);"%sys.path[0])
            #else:
                #button.pushButton.setText("ON")
                #button.pushButton.setStyleSheet("background-image: url(%s/bloques/widgets/arte/box.png);"%sys.path[0])

        #def activate():
            #
            #if self.IDE: self.IDE.frame.startPython(self.getLine(), "pynguino_code")



        #if checkeable:
            #button.pushButton.setCheckable(True)
            #button.pushButton.setChecked(start)
            #QtCore.QObject.connect(button.pushButton, QtCore.SIGNAL("clicked()"), down)
            #down()
        #else:
            #QtCore.QObject.connect(button.pushButton, QtCore.SIGNAL("clicked()"), activate)



        #button.pushButton.setText(text)
        #return widgetButton, button.pushButton



    #----------------------------------------------------------------------
    def buidToInside(self, widget, insideType):
        widgetInside = QtGui.QWidget(widget)
        widgetInside.getLine = lambda :""
        if insideType:
            inside = Inside2()
            inside.setupUi(widgetInside)
            widgetInside.setMinimumHeight(34)
            widgetInside.setMaximumHeight(34)
            inside.frame.setStyleSheet("image: url(%s/insert21b.png);"%self.isideDir)
            inside.frame_2.setStyleSheet("background-image: url(%s/insert23b.png);"%self.isideDir)
            #inside.frame_3.setStyleSheet("image: url(bloques/inside/arte/insert22b.png);")


        else:
            inside = Inside()
            inside.setupUi(widgetInside)

            inside.frame.setStyleSheet("image: url(%s/insert21.png);"%self.isideDir)
            inside.frame_2.setStyleSheet("background-image: url(%s/insert23.png);"%self.isideDir)



        widgetInside.setMinimumSize(30, widgetInside.size().height())
        widgetInside.setMaximumSize(30, widgetInside.size().height())

        self.LineCode.append(lambda :"%s")
        return widgetInside



    #----------------------------------------------------------------------
    def buidToInsideBool(self, widget, insideType):
        widgetInside = QtGui.QWidget(widget)
        widgetInside.getLine = lambda :""
        if insideType:
            inside = Inside2Bool()
            inside.setupUi(widgetInside)
            widgetInside.setMinimumHeight(34)
            widgetInside.setMaximumHeight(34)

            inside.frame.setStyleSheet("image: url(%s/insert21b.png);"%self.isideDir)
            inside.frame_2.setStyleSheet("background-image: url(%s/insert23b.png);"%self.isideDir)
            inside.frame_3.setStyleSheet("image: url(%s/insert21b_.png);"%self.isideDir)

        else:
            inside = InsideBool()
            inside.setupUi(widgetInside)
            inside.frame.setStyleSheet("image: url(%s/insert21.png);"%self.isideDir)
            inside.frame_2.setStyleSheet("background-image: url(%s/insert23.png);"%self.isideDir)
            #inside.frame_3.setStyleSheet("image: url(%sinsert22.png);"%self.isideDir)
            inside.frame_3.setStyleSheet("image: url(%s/insert21_.png);"%self.isideDir)


        widgetInside.setMinimumSize(30, widgetInside.size().height())
        widgetInside.setMaximumSize(30, widgetInside.size().height())

        self.LineCode.append(lambda :"%s")
        return widgetInside


    #----------------------------------------------------------------------
    def addParent(self, widget, force=False):
        static, move = widget
        static_layout = static.metadata.object_.layout

        inside_pos, layout_pos = static.metadata.object_.getInsidePos()

        if not force:
            for ins in inside_pos:
                c = ins - move.parent().mapToGlobal(move.pos())
                if abs(c.x()) < 4 and  abs(c.y()) < 4:
                    index = inside_pos.index(ins)
        else:
            index = 0

        static_layout.itemAt(layout_pos[index]).widget().hide()
        static_layout.itemAt(layout_pos[index]).widget().setEnabled(False)
        static_layout.insertWidget(layout_pos[index], move)

        #update parents
        if static.metadata.parent == "None":
            move.metadata.parent = static
        else: move.metadata.parent = static.metadata.parent

        #updtate iside
        static.metadata.inside.append(move)

        #update linecode
        move.getLine = move.metadata.line_code

        #fit heigth
        move.setMinimumHeight(move.height())
        move.setMaximumHeight(move.height())

        self.remove = (
                       #static_layout.itemAt(layout_pos[index]).widget(),
                       static_layout.itemAt(layout_pos[index]+1).widget(),
                       static,
                       move,
                       )


        ##pasado = WidgetStatic.metadata.object_.getWidgtes()[r]
        #pasado = WidgetStatic.metadata.object_.getWidgtes()[r]

        #pasado.hide()
        #self.pasado = pasado
        ##self.layoutPos = layoutPos[r]
        ##WidgetStatic.metadata.inside[r] = widgetMove.ID
        #self.w_parent = WidgetStatic
        #self.w_move = widgetMove
        #self.R = r
        #self.updateSize()
        ##self.w_parent.adjustSize()

        ##self.toAddPoint = WidgetStatic.metadata.object_.inLayouts_all.pop(r)
        ##self.toAddPos = WidgetStatic.metadata.object_.layoutsPos.pop(r)
        #self.toAddPoint = WidgetStatic.metadata.object_.inLayouts_all.pop(r)
        #self.toAddPos = WidgetStatic.metadata.object_.layoutsPos.pop(r)






        ##self.primo = widget
   #-        widgetMove = widget[1]
   #-        self.widgetMove = widgetMove
   #-        WidgetStatic = widget[0]
   #-
   #-        r = 0
   #-        #insidePos, layoutPos = WidgetStatic.metadata.object_.getInsidePos()
   #-        insidePos, layoutPos = WidgetStatic.metadata.object_.getInsidePos()
   #-        #print layoutPos
   #-        for po in insidePos:
   #-
   #-            if not force:
   #-                a = widgetMove.pos()
   #-                #b = po + WidgetStatic.DATA["pos"] + WidgetStatic.metadata.object_.getInsidePoint()
   #-                b = po + WidgetStatic.metadata.pos_ + WidgetStatic.metadata.object_.getInsidePoint()
   #-                poGlobal = po
   #-                aGlobal = widgetMove.parent().mapToGlobal(a)
   #-                c = poGlobal - aGlobal
   #-            else:
   #-                r = 0
   #-                c = QtCore.QPoint()
   #-
   #-            #print c.x()
   #-            if -4 <= c.x() <= 4:
   #-
   #-
   #-                if WidgetStatic.metadata.parent == "None":
   #-                    widgetMove.metadata.parent = WidgetStatic
   #-                else: widgetMove.metadata.parent = WidgetStatic.metadata.parent
   #-
   #-                if layoutPos[r] == layoutPos[-1]:
   #-                    WidgetStatic.metadata.object_.layout.insertWidget(layoutPos[r]+1, widgetMove)
   #-
   #-                else:
   #-                    WidgetStatic.metadata.object_.layout.insertWidget(layoutPos[r], widgetMove)
   #-
   #-                WidgetStatic.metadata.inside.append(widgetMove)
   #-
   #-
   #-
   #-                #widgetMove.getLine = widgetMove.DATA["lineCode"]
   #-                widgetMove.getLine = widgetMove.metadata.line_code
   #-
   #-
   #-                widgetMove.setMinimumHeight(widgetMove.height())
   #-                widgetMove.setMaximumHeight(widgetMove.height())
   #-
   #-                #pasado = WidgetStatic.metadata.object_.getWidgtes()[r]
   #-                pasado = WidgetStatic.metadata.object_.getWidgtes()[r]
   #-
   #-                pasado.hide()
   #-                self.pasado = pasado
   #-                #self.layoutPos = layoutPos[r]
   #-                #WidgetStatic.metadata.inside[r] = widgetMove.ID
   #-                self.w_parent = WidgetStatic
   #-                self.w_move = widgetMove
   #-                self.R = r
   #-                self.updateSize()
   #-                #self.w_parent.adjustSize()
   #-
   #-                #self.toAddPoint = WidgetStatic.metadata.object_.inLayouts_all.pop(r)
   #-                #self.toAddPos = WidgetStatic.metadata.object_.layoutsPos.pop(r)
   #-                self.toAddPoint = WidgetStatic.metadata.object_.inLayouts_all.pop(r)
   #-                self.toAddPos = WidgetStatic.metadata.object_.layoutsPos.pop(r)
   #-
   #-                return
   #-
   #-            r += 1




    #----------------------------------------------------------------------
    def removeParent(self):
        hidden, static, move = self.remove

        static.metadata.parent = "None"
        move.metadata.parent = "None"


        #static.metadata.object_.inLayouts_all.insert(self.R, self.toAddPoint)
        #static.metadata.object_.layoutsPos.insert(self.R, self.toAddPos)
        hidden.show()
        hidden.setEnabled(True)
        static.metadata.object_.layout.removeWidget(static)




    #----------------------------------------------------------------------
    def updatePoints(self):
        layouts = filter(lambda l:l.isEnabled(), self.inLayouts)
        points = map(lambda x:self.widget.mapToGlobal(x.pos()+self.insidePos), layouts)
        self.toType["tipo5"] = points

        layouts_b = filter(lambda l:l.isEnabled(), self.inLayouts_b)
        points = map(lambda x:self.widget.mapToGlobal(x.pos()+self.insidePos), layouts_b)
        self.toType["tipo2"] = points


    #----------------------------------------------------------------------
    def getInsidePos(self):
        #intros = [map(lambda x:self.widget.mapToGlobal(x.pos()+self.insidePos), self.inLayouts_all),
                  #self.get_layout_pos()]
        intros = [map(lambda x:self.widget.mapToGlobal(x.pos()+self.insidePos), self.get_layout_widgets()),
                  self.get_layout_pos()]
        return intros

    ##----------------------------------------------------------------------
    #def getWidgtes(self):
        #return self.inLayouts_all

    #----------------------------------------------------------------------
    def get_layout_pos(self):
        widgets = self.get_widgets_from_layout()
        layoutpos = []
        for index, wid in widgets:
            if wid.objectName() == "BLOCK_SPACE" and wid.isEnabled():
                layoutpos.append(index)
        return layoutpos

    #----------------------------------------------------------------------
    def get_layout_pos_save(self):
        widgets = self.get_widgets_from_layout()
        layoutpos = []
        for index, wid in widgets:
            if wid.objectName() == "BLOCK_SPACE":
                layoutpos.append(wid.isHidden())
            #elif wid.objectName().startswith("BLOCK"):
                #layoutpos.append
        return layoutpos

    #----------------------------------------------------------------------
    def get_layout_widgets(self):
        widgets = self.get_widgets_from_layout()
        layoutpos = []
        for index, wid in widgets:
            if wid.objectName() == "BLOCK_SPACE" and wid.isEnabled():
                layoutpos.append(wid)
        return layoutpos

    #----------------------------------------------------------------------
    def setFlex(self, inLayouts, insidePos, inLayouts_b, inlayouts_all):
        #self.layoutsPos = layoutsPos
        self.inLayouts = inLayouts
        self.insidePos = QtCore.QPoint(*insidePos)

        self.inLayouts_b = inLayouts_b
        self.inLayouts_all = inlayouts_all


    #----------------------------------------------------------------------
    def getInsidePoint(self):
        try: return self.insidePos
        except: return QtCore.QPoint(27, 2)

    def addChoice(self, widget, add):
        widget_choide, lineEdit = self.buildChoice(*[widget] + add[1:])
        widget_choide.setObjectName("BLOCK_CHOICE")
        self.layout.addWidget(widget_choide)
        lineEdit.contextMenuEvent = self.build_menu
        self.set_as_draggable(lineEdit, widget)
        self.constructorCode.append(lambda :["spin_choice"]+[str(lineEdit.text())]+add[2:])

    def addSpinInt(self, widget, add):
        widgetSpin, lineEdit = self.buildSpinInt(*[widget] + add[1:])
        widgetSpin.setObjectName("BLOCK_SPININT")
        self.layout.addWidget(widgetSpin)
        lineEdit.h = lineEdit.size().height()
        QtCore.QObject.connect(lineEdit, QtCore.SIGNAL("textChanged(QString)"), lambda x:self.updateLine(lineEdit))
        self.updateLine(lineEdit)
        lineEdit.contextMenuEvent = self.build_menu
        self.set_as_draggable(lineEdit, widget)
        self.constructorCode.append(lambda :["spin_int"]+[str(lineEdit.text())]+add[2:])

    def addSpinFloat(self, widget, add):
        widgetSpin, lineEdit = self.buildSpinFloat(*[widget] + add[1:])
        widgetSpin.setObjectName("BLOCK_SPINFLOAT")
        self.layout.addWidget(widgetSpin)
        lineEdit.h = lineEdit.size().height()
        QtCore.QObject.connect(lineEdit, QtCore.SIGNAL("textChanged(QString)"), lambda x:self.updateLine(lineEdit))
        self.updateLine(lineEdit)
        lineEdit.contextMenuEvent = self.build_menu
        self.set_as_draggable(lineEdit, widget)
        self.constructorCode.append(lambda :["spin_float", str(lineEdit.text())]+add[2:])

    def addLineEdit(self, widget, add):
        widgetEdit, lineEdit = self.buildName(*[widget] + add[1:])
        widgetEdit.setObjectName("BLOCK_LINEDIT")
        self.layout.addWidget(widgetEdit)
        QtCore.QObject.connect(lineEdit, QtCore.SIGNAL("textChanged(QString)"), lambda x:self.updateLine(lineEdit))
        self.updateLine(lineEdit)
        self.set_as_draggable(lineEdit, widget)
        self.constructorCode.append(lambda :["edit", str(lineEdit.text())] + add[2:])

    def addSlider(self, widget, add):
        widgetSlider, lineEdit = self.buildSliderInt(*[widget] + add[1:])
        widgetSlider.setObjectName("BLOCK_SLIDER")
        self.layout.addWidget(widgetSlider)
        lineEdit.contextMenuEvent = self.build_menu
        self.set_as_draggable(lineEdit, widget)
        self.constructorCode.append(lambda :["slider", str(lineEdit.text())])

    #def addButton(self, widget, add):
        #widgetButton, button = self.buildButton(*[widget] + add[1:])
        #widgetButton.setObjectName("BLOCK_BUTOON")
        #self.layout.addWidget(widgetButton)
        #self.constructorCode.append(lambda :["button"]+add[1:])

    def addLabel(self, widget, text):
        widgetLabel, Label = self.buildLabel(widget, str(text))
        widgetLabel.setObjectName("BLOCK_LABEL")
        self.layout.addWidget(widgetLabel)
        self.constructorCode.append(lambda :["label", str(text)])

    def addDecorator(self, widget, text):
        widgetLabel, Label = self.buildDecorator(widget, str(text))
        widgetLabel.setObjectName("BLOCK_DECORATOR")
        self.layout.addWidget(widgetLabel)
        self.constructorCode.append(lambda :["decorator", str(text)])

    def addToInside(self, widget, inside):
        toInside = self.buidToInside(widget, inside)
        setattr(toInside, "in_type", "space")
        toInside.setObjectName("BLOCK_SPACE")
        self.layout.addWidget(toInside)
        self.layout_adds.append(toInside)
        self.layout_adds_all.append(toInside)
        self.constructorCode.append(lambda :["space"])

    def addToInsideBool(self, widget, inside):
        toInsideBool = self.buidToInsideBool(widget, inside)
        setattr(toInsideBool, "in_type", "space")
        toInsideBool.setObjectName("BLOCK_SPACE")
        self.layout.addWidget(toInsideBool)
        self.layout_adds_b.append(toInsideBool)
        self.layout_adds_all.append(toInsideBool)
        self.constructorCode.append(lambda :["space_bool"])

    def addSyntax(self, widget, text):
        for char in FIX_SYNTAX:
            if text == char: text += " "

        widgetLabel, Label = self.buildLabel(widget, "")
        widgetLabel.setObjectName("BLOCK_SYNTAX")
        self.layout.addWidget(widgetLabel)

        self.LineCode.pop(-1)
        self.LineCode.append(lambda :str(text))
        self.constructorCode.append(lambda :["syntax", str(text)])


    def updateLine(self, lineEdit):
        lineEdit.setMinimumSize(len(lineEdit.text())*(FONT_SIZE+ADJ2)+ADJ, lineEdit.h)
        lineEdit.setMaximumSize(len(lineEdit.text())*(FONT_SIZE+ADJ2)+ADJ, lineEdit.h)


    ##----------------------------------------------------------------------
    #def fullWidgetWith(self, content):
        #
        #self.IDE.ge
        #self.addParent([self.getWorkArea().getWidget("0"),self.getWorkArea().getWidget("1")],
                       #force = True, position = 1)




    #----------------------------------------------------------------------
    def getLine(self):

        code = map(lambda x:str(x()), self.LineCode)

        complete = []
        for i in range(self.layout.count()):
            c = self.layout.itemAt(i).widget()
            try:
                if c.getLine() != "": complete.append(c.getLine().replace("\n", ""))
            except: pass

        code = "".join(code)

        if complete:
            code = code % tuple(complete + ["%s"]*(code.count("%s")-len(complete)))

        return code + self.endLine + "\n"



    #----------------------------------------------------------------------
    def getConstructor(self):

        constr = []
        for code in self.constructorCode:
            constr.append(code())
        return constr[:]

    #----------------------------------------------------------------------
    def setAumentWidth(self):
        self.widget.setMinimumSize(self.widget.size()+QtCore.QSize(100, 0))
        self.widget.setMaximumSize(self.widget.size()+QtCore.QSize(100, 0))

    #----------------------------------------------------------------------
    def setAumentHigth(self):
        self.widget.setMinimumSize(self.widget.size()+QtCore.QSize(0, 100))
        self.widget.setMaximumSize(self.widget.size()+QtCore.QSize(0, 100))

    #----------------------------------------------------------------------
    def setDecrementWidth(self):
        self.widget.setMinimumSize(self.widget.size()-QtCore.QSize(100, 0))
        self.widget.setMaximumSize(self.widget.size()-QtCore.QSize(100, 0))

    #----------------------------------------------------------------------
    def setDecrementHigth(self):
        self.widget.setMinimumSize(self.widget.size()-QtCore.QSize(0, 100))
        self.widget.setMaximumSize(self.widget.size()-QtCore.QSize(0, 100))

    #----------------------------------------------------------------------
    def set_as_draggable(self, lineEdit, widget):
        lineEdit.contextMenuEvent = self.build_menu
        lineEdit.mouseMoveEvent = widget.mouseMoveEvent
        lineEdit.mousePressEvent = widget.mousePressEvent
        lineEdit.mouseReleaseEvent = widget.mouseReleaseEvent
        lineEdit.setCursor(QtCore.Qt.OpenHandCursor)
