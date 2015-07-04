#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os

from PySide import QtGui, QtCore


########################################################################
class BoardConfig(object):

    def __init__(self):

        self.build_devices_arch()

        all_groups = [self.main.frame_arch,
                      self.main.frame_prgmode,
                      self.main.frame_bootloader,
                      self.main.frame_devices_8,
                      self.main.frame_devices_32]

        #FIXME: add correct options
        self.HEAPSIZE = {"512 byte": 512,
                         "1024 byte": 1024,}

        self.main.comboBox_heapsize.clear()
        self.main.comboBox_heapsize.addItems(list(self.HEAPSIZE.keys()))

        self.OPTIMIZATION = "-O2 -O3 -Os".split()
        self.main.comboBox_optimization.clear()
        self.main.comboBox_optimization.addItems(self.OPTIMIZATION)
        self.init_groups()


        self.connect(self.main.radioButton_mode_bootloader, QtCore.SIGNAL("clicked()"), self.update_mode)
        self.connect(self.main.radioButton_mode_icsp, QtCore.SIGNAL("clicked()"), self.update_mode)

        self.connect(self.main.radioButton_arch_8, QtCore.SIGNAL("clicked()"), self.update_mode)
        self.connect(self.main.radioButton_arch_32, QtCore.SIGNAL("clicked()"), self.update_mode)

        self.connect(self.main.radioButton_bootloader_v1_v2, QtCore.SIGNAL("clicked()"), self.update_mode)
        self.connect(self.main.radioButton_bootloader_v4, QtCore.SIGNAL("clicked()"), self.update_mode)

        # self.connect(self.main.pushButton_accept, QtCore.SIGNAL("clicked()"), self.accept_config)
        # self.connect(self.main.pushButton_defaul, QtCore.SIGNAL("clicked()"), self.restore_default)


        self.load_config()


    # #----------------------------------------------------------------------
    # def terminate_config(self, event=None):

        # self.configIDE.load_config()
        # self.close_advance()
        # self.close()


    #----------------------------------------------------------------------
    def update_mips16(self):
        self.configIDE.set("Board", "mips16", self.main.checkBox_mips16.isChecked())


    #----------------------------------------------------------------------
    def update_heapsize(self):
        self.configIDE.set("Board", "heapsize", self.main.checkBox_heapsize.isChecked())


    # #----------------------------------------------------------------------
    # def accept_config(self):

        # self.save_config()
        # self.configIDE.save_config()
        # self.main.statusbar_ide(self.main.get_status_board())
        # self.close_advance()
        # self.close()


    #----------------------------------------------------------------------
    def load_config(self):

        self.configIDE.load_config()

        arch = self.configIDE.config("Board", "arch", 8)
        self.main.radioButton_arch_8.setChecked(arch == 8)
        self.main.radioButton_arch_32.setChecked(arch == 32)
        self.main.frame_advance.setVisible(arch == 32)

        mode = self.configIDE.config("Board", "mode", "bootloader")
        self.main.radioButton_mode_bootloader.setChecked(mode == "bootloader")
        self.main.radioButton_mode_icsp.setChecked(mode == "icsp")

        bootloader = self.configIDE.config("Board", "bootloader", "v1_v2")
        self.main.radioButton_bootloader_v1_v2.setChecked(bootloader == "v1_v2")
        self.main.radioButton_bootloader_v4.setChecked(bootloader == "v4")

        mips16 = self.configIDE.config("Board", "mips16", True)
        self.main.checkBox_mips16.setChecked(mips16)

        heapsize = self.configIDE.config("Board", "heapsize", 512)
        heapsize_values = list(self.HEAPSIZE.values())
        index = heapsize_values.index(heapsize)
        self.main.comboBox_heapsize.setCurrentIndex(index)

        optimization = self.configIDE.config("Board", "optimization", "-O3")
        index = self.OPTIMIZATION.index(optimization)
        self.main.comboBox_optimization.setCurrentIndex(index)

        self.update_mode()


    #----------------------------------------------------------------------
    def save_config(self):

        if self.main.radioButton_arch_8.isChecked(): arch = 8
        else: arch = 32
        self.configIDE.set("Board", "arch", arch)

        if self.main.radioButton_mode_bootloader.isChecked(): mode = "bootloader"
        else: mode = "icsp"
        self.configIDE.set("Board", "mode", mode)

        if self.main.radioButton_bootloader_v1_v2.isChecked(): bootloader = "v1_v2"
        else: bootloader = "v4"
        self.configIDE.set("Board", "bootloader", bootloader)

        name = self.configIDE.config("Board", "board_"+str(arch), None)
        self.configIDE.set("Board", "board", name)

        self.configIDE.set("Board", "mips16", self.main.checkBox_mips16.isChecked())
        heapsize = self.HEAPSIZE[self.main.comboBox_heapsize.currentText()]
        self.configIDE.set("Board", "heapsize", heapsize)
        self.configIDE.set("Board", "optimization", self.main.comboBox_optimization.currentText())


        self.configIDE.save_config()


    #----------------------------------------------------------------------
    def update_mode(self):

        mode_boot = self.main.radioButton_mode_bootloader.isChecked()
        arch_8 = self.main.radioButton_arch_8.isChecked()
        self.main.frame_bootloader.setVisible(mode_boot and arch_8)

        self.main.frame_devices_32.setVisible(not arch_8)
        self.main.frame_devices_8.setVisible(arch_8)

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
            self.update_mode()
        return dummy


    #----------------------------------------------------------------------
    def build_devices_arch(self):

        #8bits
        name_checked = self.configIDE.config("Board", "board_8", "Pinguino 2550")
        arch_8 = list(filter(lambda board:board.arch==8, self.pinguinoAPI._boards_))
        # arch_8.sort()

        count = 0
        side = 0  #left
        for board in arch_8:
            if arch_8.index(board) == (len(arch_8) // 2) + 1:
                count = 0
                side = 1  #rigth

            radio = QtGui.QRadioButton(self.main.frame_devices_8)
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

            radio = QtGui.QRadioButton(self.main.frame_devices_32)
            self.main.gridLayout_device_32.addWidget(radio, count, side, 1, 1)
            radio.setText(board.name)
            radio.setToolTip(board.proc)
            if name_checked == board.name: radio.setChecked(True)
            self.connect(radio, QtCore.SIGNAL("clicked()"), self.set_board_name(board.name, "32"))
            count += 1

