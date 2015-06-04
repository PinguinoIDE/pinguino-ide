#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os
import shutil
import requests
import urlparse
import hashlib

from PySide import QtGui, QtCore

from ...frames.patches import Ui_Patches
from ..methods.dialogs import Dialogs

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
    from io import StringIO
else:
    #Python2
    from ConfigParser import RawConfigParser
    from cStringIO import StringIO


# BRANCH = "master"
BRANCH = "11.1"
PATCHES = "https://raw.githubusercontent.com/PinguinoIDE/pinguino-ide/{}/patches".format(BRANCH)
RAWFILE = "https://raw.githubusercontent.com/PinguinoIDE/pinguino-ide/{}/"


########################################################################
class Patches(QtGui.QDialog):

    def __init__(self, parent):
        super(Patches, self).__init__()
        self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint |
                            QtCore.Qt.WindowSystemMenuHint |
                            QtCore.Qt.WindowStaysOnTopHint |
                            QtCore.Qt.Tool)

        self.patches = Ui_Patches()
        self.patches.setupUi(self)

        self.main = parent

        self.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+self.windowTitle())

        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/logo/art/windowIcon.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.setWindowIcon(icon)

        self.connect(self.patches.pushButton_install, QtCore.SIGNAL("clicked()"), self.install_patches)
        self.connect(self.patches.pushButton_cancel, QtCore.SIGNAL("clicked()"), self.close)

        self.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)

        self.center_on_screen()


    #----------------------------------------------------------------------
    def center_on_screen(self):

        screen = QtGui.QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)


    #----------------------------------------------------------------------
    def get_patches(self):

        r = requests.get(PATCHES)
        patches = r.text.decode(r.encoding)
        r.close()
        buf = StringIO(patches)
        self.parser = RawConfigParser()
        self.parser.readfp(buf)
        buf.close()

        if not self.main.configIDE.has_section("Patches"):
            self.main.configIDE.add_section("Patches")

        index = 0
        for patch in self.parser.sections():

            if patch in self.main.configIDE.options("Patches"): continue

            self.patches.tableWidget_patches.setRowCount(index+1)

            item = QtGui.QTableWidgetItem(self.parser.get(patch, "patch"))
            item.setCheckState(QtCore.Qt.Checked)
            self.patches.tableWidget_patches.setItem(index, 0, item)
            item = QtGui.QTableWidgetItem(self.parser.get(patch, "summary"))
            self.patches.tableWidget_patches.setItem(index, 1, item)

            index += 1

        return index


    #----------------------------------------------------------------------
    def install_patches(self):

        for patch in self.parser.sections():
            if patch in self.main.configIDE.sections(): continue

            hash_ = self.parser.get(patch, "hash")
            name = self.parser.get(patch, "summary")
            name = (patch + "-" + name).replace(" ", "-").lower()

            path = self.parser.get(patch, "patch")

            dirname = os.path.dirname(path)
            filename = os.path.basename(path)

            dir_ = os.path.join(os.getenv("PINGUINO_USER_PATH"), "patches", os.getenv("PINGUINO_VERSION"), name, dirname)
            os.makedirs(dir_)

            host_file = RAWFILE.format(hash_) + path
            r = requests.get(host_file)
            content = r.text.decode(r.encoding)
            r.close()

            file_ = open(os.path.join(dir_, filename), mode="w")

            file_.write(content)
            file_.close()

            src = os.path.join(dir_, filename)
            dst = os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), path)

            src_hash = hashlib.md5(open(src, 'rb').read()).hexdigest()
            dst_hash = hashlib.md5(open(dst, 'rb').read()).hexdigest()

            if src_hash != dst_hash:
                shutil.copyfile(src, dst)

            self.main.configIDE.config("Patches", patch, self.parser.get(patch, "summary"))
            self.main.configIDE.save_config()

        self.hide()
        reply = Dialogs.confirm_message(self.main, "You need restart for complete the update\nDo you want to restart Pinguino IDE now?")
        if reply:
            self.close()
            self.main.restart_now()

        else:
            self.close()