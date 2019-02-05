#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import logging
from datetime import datetime

from PySide2 import QtGui, QtCore, QtWidgets
import requests
import json

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    #from urllib.request import urlopen
    #from urllib.parse import urlencode
    from configparser import RawConfigParser
else:
    #Python2
    #from urllib import urlopen, urlencode
    from ConfigParser import RawConfigParser

from ...frames.submit_bug import Ui_SubmitBug
from ..methods.dialogs import Dialogs

#if os.getenv("PINGUINO_MODE") == "NORMAL":
    #SUBMIT_SERVER = "http://submit.pinguino.xyz/submit/"
#else:
    #SUBMIT_SERVER = "http://localhost:8000/submit/"

########################################################################
class SubmitBug(QtWidgets.QDialog):

    def __init__(self, parent, details=""):
        super(SubmitBug, self).__init__()
        self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint |
                            QtCore.Qt.WindowSystemMenuHint |
                            QtCore.Qt.WindowStaysOnTopHint |
                            QtCore.Qt.Tool)

        self.submit = Ui_SubmitBug()
        self.submit.setupUi(self)
        self.main = parent

        username, password = self.get_auth()
        self.submit.lineEdit_username.setText(username)
        self.submit.lineEdit_password.setText(password)

        self.submit.plainTextEdit_details.insertPlainText(details)

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

        screen = QtWidgets.QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)


    #----------------------------------------------------------------------
    def update_submit_dialog(self):

        self.main.configIDE.set("Features", "submmit_bugs", self.submit.checkBox_show_this_dialog.isChecked())


    #----------------------------------------------------------------------
    def submit_now(self):
        """"""

        summary = self.submit.lineEdit_summary.text().replace("\n", "<br>")
        details = self.submit.plainTextEdit_details.toPlainText()
        username = self.submit.lineEdit_username.text()
        password = self.submit.lineEdit_password.text()
        repo = self.submit.comboBox_repo.currentText()

        if not details:
            Dialogs.error_message(self, "No details!")
            return

        if not username:
            Dialogs.error_message(self, "No userbane!")
            return

        if not password:
            Dialogs.error_message(self, "No password!")
            return

        environ = self.get_systeminfo()
        logging.info("Submitting bug report.")
        maked = self.make_github_issue(summary, details, repo, environ, username, password)
        #response = urlopen(SUBMIT_SERVER, urlencode({"summary": summary, "details": details, "environ": environ,}).encode("utf-8"))
        #share_link = eval(response.read())["share"]
        self.hide()

        if not maked:
            if details and environ:
                self.save_for_later(summary, details, repo, environ, username, password)
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
        try: data["pyusb"] = str(usb.__version__)
        except: pass

        return "\n".join([": ".join(item) for item in data.items()])


    #----------------------------------------------------------------------
    def save_for_later(self, summary, details, repo, environ, username, password):
        """"""
        parser = RawConfigParser()
        parser.add_section("SUBMIT")
        parser.set("SUBMIT", "summary", summary)
        parser.set("SUBMIT", "details", details)
        parser.set("SUBMIT", "repo", repo)
        parser.set("SUBMIT", "environ", environ)
        parser.set("SUBMIT", "username", username)
        parser.set("SUBMIT", "password", password)

        filename = os.path.join(os.getenv("PINGUINO_USER_PATH"), "submit-{}".format(datetime.now()))
        parser.write(open(filename, "w"))


    #----------------------------------------------------------------------
    def save_auth(self, username, password):
        """"""
        parser = RawConfigParser()
        parser.add_section("AUTH")
        parser.set("AUTH", "username", username)
        parser.set("AUTH", "password", password)

        filename = os.path.join(os.getenv("PINGUINO_USER_PATH"), "submit-auth")
        parser.write(open(filename, "w"))


    #----------------------------------------------------------------------
    def get_auth(self):
        """"""
        if not os.path.exists(os.path.join(os.getenv("PINGUINO_USER_PATH"), "submit-auth")):
            return '', ''

        parser = RawConfigParser()
        filename = os.path.join(os.getenv("PINGUINO_USER_PATH"), "submit-auth")
        parser.readfp(open(filename, "r"))

        username = parser.get("AUTH", "username")
        password = parser.get("AUTH", "password")

        return username, password


    #----------------------------------------------------------------------
    def make_github_issue(self, summary, details, repo, environ, username, password):
        '''Create an issue on github.com using the given parameters.'''
        # Our url to create issues via POST
        url = 'https://api.github.com/repos/{}/{}/issues'.format('PinguinoIDE', repo)
        # Create an authenticated session to create the issue
        #session = requests.session(auth=(username, password))
        session = requests.Session()
        session.auth = (username, password)

        # Create our issue
        issue = {'title': summary,
                 'body': "{}\n\n{}".format(details, environ),
                 'labels': ['submitted-from-ide',
                            'bug',
                            'v{}'.format(os.environ["PINGUINO_VERSION"][:2]),
                            ],
                 }

        # Add the issue to our repository
        r = session.post(url, json.dumps(issue))
        if r.status_code == 201:
            logging.info('Successfully created Issue "{}"'.format(summary))
            self.save_auth(username, password)

        return r.status_code == 201


#----------------------------------------------------------------------
def send_old_submits():

    submits = filter(lambda s:s.startswith("submit-"), os.listdir(os.getenv("PINGUINO_USER_PATH")))
    for submit in submits:
        parser = RawConfigParser()
        filename = os.path.join(os.getenv("PINGUINO_USER_PATH"), submit)
        parser.readfp(open(filename, "r"))

        summary = parser.get("SUBMIT", "summary")
        details = parser.get("SUBMIT", "details")
        repo = parser.get("SUBMIT", "repo")
        environ = parser.get("SUBMIT", "environ")
        username = parser.get("SUBMIT", "username")
        password = parser.get("SUBMIT", "password")


        try:
            url = 'https://api.github.com/repos/{}/{}/issues'.form
            session = requests.Session()
            session.auth = (username, password)
            issue = {'title': summary,
                     'body': "{}\n\n{}".format(details, environ),
                     'labels': ['submitted-from-ide',
                                'bug',
                                'v{}'.format(os.environ["PINGUINO_VERSION"][:2]),
                                ],
                     }
            r = session.post(url, json.dumps(issue))
            os.remove(filename)
        except:
            pass


