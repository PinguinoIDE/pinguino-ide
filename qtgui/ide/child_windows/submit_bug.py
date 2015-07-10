#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os
import logging

from PySide import QtGui, QtCore
import requests

from ...frames.submit_bug import Ui_SubmitBug

if os.getenv("PINGUINO_MODE") == "NORMAL":
    SUBMIT_SERVER = "http://submit-pinguino.rhcloud.com/submit/"
else:
    SUBMIT_SERVER = "http://localhost:8000/submit/"

########################################################################
class SubmitBug(QtGui.QDialog):

    def __init__(self, parent, summary=""):
        super(SubmitBug, self).__init__()
        self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint |
                            QtCore.Qt.WindowSystemMenuHint |
                            QtCore.Qt.WindowStaysOnTopHint |
                            QtCore.Qt.Tool)

        self.submit = Ui_SubmitBug()
        self.submit.setupUi(self)
        self.main = parent

        self.submit.plainTextEdit_details.insertPlainText(summary)

        self.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+self.windowTitle())

        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/logo/art/windowIcon.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.setWindowIcon(icon)

        self.connect(self.submit.pushButton_submit, QtCore.SIGNAL("clicked()"), self.submit_now)
        self.connect(self.submit.pushButton_cancel, QtCore.SIGNAL("clicked()"), self.close)
        self.connect(self.submit.checkBox_show_this_dialog, QtCore.SIGNAL("clicked()"), self.update_submit_dialog)

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
    def update_submit_dialog(self):

        self.main.configIDE.set("Features", "submmit_bugs", self.submit.checkBox_show_this_dialog.isChecked())


    #----------------------------------------------------------------------
    def submit_now(self):
        """"""
        try:
            summary = self.submit.lineEdit_summary.text()
            details = self.get_systeminfo() + "\n\nDetails:\n" + self.submit.plainTextEdit_details.toPlainText()
            logging.info("Sending: {}".format(details))
            requests.post(SUBMIT_SERVER, data={"summary": summary, "details": details,})

        except requests.ConnectionError:
            logging.error("ConnectionError")

        self.close()


    #----------------------------------------------------------------------
    def get_systeminfo(self):

        data = {}
        try: data["os.name"] = str(os.name)
        except: pass
        try: data["os.environ"] = str(os.environ)
        except: pass
        try: data["os.uname"] = str(os.uname())
        except: pass
        try: data["sys.argv"] = str(sys.argv)
        except: pass
        try: data["sys.flags"] = str(sys.flags)
        except: pass
        try: data["sys.platform"] = str(sys.platform)
        except: pass
        try: data["sys.version"] = str(sys.version)
        except: pass
        try: data["platform.architecture"] = str(platform.architecture())
        except: pass
        try: data["platform.dist"] = str(platform.dist())
        except: pass
        try: data["platform.linux_distribution"] = str(platform.linux_distribution())
        except: pass
        try: data["platform.mac_ver"] = str(platform.mac_ver())
        except: pass
        try: data["platform.system"] = str(platform.system())
        except: pass
        try: data["platform.win32_ver"] = str(platform.win32_ver())
        except: pass
        try: data["platform.libc_ver"] = str(platform.libc_ver())
        except: pass
        try: data["platform.machine"] = str(platform.machine())
        except: pass
        try: data["platform.platform"] = str(platform.platform())
        except: pass
        try: data["platform.release"] = str(platform.release())
        except: pass

        return "\n" + "#" + "-" * 80 + "\n#" + "-" * 80 + "\n" + "\n".join([": ".join(item) for item in data.items()]) + "\n#" + "-" * 80 + "\n#" + "-" * 80
