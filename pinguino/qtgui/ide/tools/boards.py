#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

from PySide2 import QtGui, QtCore, QtWidgets
from collections import OrderedDict

########################################################################
class Boards(object):

    def __init__(self):

        self.build_devices_arch()

        all_groups = [self.main.frame_arch,
                      self.main.frame_prgmode,
                      # self.main.frame_bootloader,
                      self.main.frame_devices_8,
                      self.main.frame_devices_32]

        #self.HEAPSIZE = self.get_heapsize()

        #self.main.spinBox_heapsize.clear()
        #self.main.comboBox_heapsize.addItems(list(self.HEAPSIZE.keys()))

        self.OPTIMIZATION = "-O2 -O3 -Os".split()
        self.main.comboBox_optimization.clear()
        self.main.comboBox_optimization.addItems(self.OPTIMIZATION)
        self.init_groups()

        self.connect(self.main.radioButton_mode_usb, QtCore.SIGNAL("clicked()"), self.update_config)
        self.connect(self.main.radioButton_mode_icsp, QtCore.SIGNAL("clicked()"), self.update_config)
        self.connect(self.main.radioButton_mode_bt, QtCore.SIGNAL("clicked()"), self.update_config)

        self.connect(self.main.radioButton_compiler_sdcc, QtCore.SIGNAL("clicked()"), self.update_config)
        self.connect(self.main.radioButton_compiler_xc8, QtCore.SIGNAL("clicked()"), self.update_config)

        self.connect(self.main.radioButton_arch_8, QtCore.SIGNAL("clicked()"), self.update_config)
        self.connect(self.main.radioButton_arch_32, QtCore.SIGNAL("clicked()"), self.update_config)

        self.connect(self.main.checkBox_mips16, QtCore.SIGNAL("clicked()"), self.save_config)
        self.connect(self.main.spinBox_heapsize, QtCore.SIGNAL("valueChanged(QString)"), self.save_config)
        self.connect(self.main.comboBox_optimization, QtCore.SIGNAL("currentIndexChanged(QString)"), self.save_config)



        # self.connect(self.main.radioButton_bootloader_v1_v2, QtCore.SIGNAL("clicked()"), self.update_config)
        # self.connect(self.main.radioButton_bootloader_v4, QtCore.SIGNAL("clicked()"), self.update_config)

        self.load_config()

    ##----------------------------------------------------------------------
    #def update_mips16(self):
        #""""""
        #self.configIDE.set("Board", "mips16", self.main.checkBox_mips16.isChecked())
        #self.save_config()


    ##----------------------------------------------------------------------
    #def update_heapsize(self):
        #""""""
        #self.configIDE.set("Board", "heapsize", self.main.comboBox_heapsize.currentText())
        #self.save_config()


    ##----------------------------------------------------------------------
    #def update_optimization(self):
        #""""""
        #self.configIDE.set("Board", "optimization", self.main.comboBox_optimization.currentText())
        #self.save_config()


    ##----------------------------------------------------------------------
    #def get_heapsize(self):
        #""""""
        #heapsize = OrderedDict()
        #for b in range(0, 8000+512, 512):
            #heapsize["{} bytes".format(b)] = b

        #return heapsize

    #----------------------------------------------------------------------
    def load_config(self):

        self.configIDE.load_config()

        arch = self.configIDE.config("Board", "arch", 8)
        self.main.radioButton_arch_8.setChecked(arch == 8)
        self.main.radioButton_arch_32.setChecked(arch == 32)
        self.main.frame_advance.setVisible(arch == 32)

        compiler = self.configIDE.config("Board", "compiler", "SDCC")  #SDCC default compiler
        self.main.radioButton_compiler_sdcc.setChecked(compiler=="SDCC")
        self.main.radioButton_compiler_xc8.setChecked(compiler=="XC8")

        mode = self.configIDE.config("Board", "mode", "usb")
        self.main.radioButton_mode_usb.setChecked(mode == "usb")
        self.main.radioButton_mode_icsp.setChecked(mode == "icsp")
        self.main.radioButton_mode_bt.setChecked(mode == "bt")

        # bootloader = self.configIDE.config("Board", "bootloader", "v1_v2")
        # self.main.radioButton_bootloader_v1_v2.setChecked(bootloader == "v1_v2")
        # self.main.radioButton_bootloader_v4.setChecked(bootloader == "v4")

        mips16 = self.configIDE.config("Board", "mips16", True)
        self.main.checkBox_mips16.setChecked(mips16)

        heapsize = self.configIDE.config("Board", "heapsize", 0)
        #heapsize_values = list(self.HEAPSIZE.values())
        #index = heapsize_values.index(heapsize)
        self.main.spinBox_heapsize.setValue(heapsize)

        optimization = self.configIDE.config("Board", "optimization", "-O3")
        index = self.OPTIMIZATION.index(optimization)
        self.main.comboBox_optimization.setCurrentIndex(index)

        self.update_config()


    #----------------------------------------------------------------------
    def save_config(self, *args, **kwargs):

        if self.main.radioButton_arch_8.isChecked(): arch = 8
        else: arch = 32
        self.configIDE.set("Board", "arch", arch)

        if self.main.radioButton_compiler_sdcc.isChecked(): compiler = "SDCC"
        else: compiler = "XC8"
        self.configIDE.set("Board", "compiler", compiler)

        if self.main.radioButton_mode_usb.isChecked(): mode = "usb"
        if self.main.radioButton_mode_icsp.isChecked(): mode = "icsp"
        if self.main.radioButton_mode_bt.isChecked(): mode = "bt"
        self.configIDE.set("Board", "mode", mode)

        # if self.main.radioButton_bootloader_v1_v2.isChecked(): bootloader = "v1_v2"
        # else: bootloader = "v4"
        # self.configIDE.set("Board", "bootloader", bootloader)

        name = self.configIDE.config("Board", "board_"+str(arch), None)
        self.configIDE.set("Board", "board", name)

        self.configIDE.set("Board", "mips16", self.main.checkBox_mips16.isChecked())
        heapsize = self.main.spinBox_heapsize.value()
        self.configIDE.set("Board", "heapsize", heapsize)
        self.configIDE.set("Board", "optimization", self.main.comboBox_optimization.currentText())

        self.configIDE.save_config()


    #----------------------------------------------------------------------
    def update_config(self):

        mode_prog = self.main.radioButton_mode_usb.isChecked()
        arch_8 = self.main.radioButton_arch_8.isChecked()
        # self.main.frame_bootloader.setVisible(mode_prog and arch_8)

        self.main.frame_devices_32.setVisible(not arch_8)
        self.main.frame_devices_8.setVisible(arch_8)
        self.main.frame_compiler.setVisible(arch_8)

        self.main.frame_advance.setVisible(not arch_8)
        self.save_config()
        self.status_info.setText(self.get_status_board())


    #----------------------------------------------------------------------
    def init_groups(self):

        arch_8 = self.main.radioButton_arch_8.isChecked()
        self.main.frame_devices_32.setVisible(not arch_8)
        self.main.frame_devices_8.setVisible(arch_8)


    #----------------------------------------------------------------------
    def set_board_name(self, name, arch):

        def dummy():
            self.configIDE.set("Board", "board_"+arch, name)
            self.update_config()
        return dummy


    #----------------------------------------------------------------------
    def build_devices_arch(self):

        #8bits
        name_checked = self.configIDE.config("Board", "board_8", "Pinguino 45k50")  #Pinguino 45k50 default board
        arch_8 = list(filter(lambda board:board.arch==8, self.pinguinoAPI._boards_))
        # arch_8.sort()

        count = 0
        side = 0  #left
        for board in arch_8:
            if arch_8.index(board) == (len(arch_8) // 2) + 1:
                count = 0
                side = 1  #rigth

            radio = QtWidgets.QRadioButton(self.main.frame_devices_8)
            self.main.gridLayout_device_8.addWidget(radio, count, side, 1, 1)
            radio.setText(board.name)
            radio.setToolTip(board.proc)
            if name_checked == board.name: radio.setChecked(True)
            self.connect(radio, QtCore.SIGNAL("clicked()"), self.set_board_name(board.name, "8"))
            count += 1


        #32bits
        name_checked = self.configIDE.config("Board", "board_32", "PIC32 Pinguino OTG")
        arch_32 = list(filter(lambda board:board.arch==32, self.pinguinoAPI._boards_))
        # arch_32.sort()

        count = 0
        side = 0  #left
        for board in arch_32:
            if arch_32.index(board) == (len(arch_32) // 2) + 1:
                count = 0
                side = 1  #rigth

            radio = QtWidgets.QRadioButton(self.main.frame_devices_32)
            self.main.gridLayout_device_32.addWidget(radio, count, side, 1, 1)
            radio.setText(board.name)
            radio.setToolTip(board.proc)
            if name_checked == board.name: radio.setChecked(True)
            self.connect(radio, QtCore.SIGNAL("clicked()"), self.set_board_name(board.name, "32"))
            count += 1

