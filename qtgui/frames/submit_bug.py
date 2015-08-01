# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Desarrollo/Pinguino/GitHub/pinguino-ide/qtgui/frames/submit_bug.ui'
#
# Created: Fri Jul 31 17:14:25 2015
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_SubmitBug(object):
    def setupUi(self, SubmitBug):
        SubmitBug.setObjectName("SubmitBug")
        SubmitBug.resize(571, 365)
        self.verticalLayout_3 = QtGui.QVBoxLayout(SubmitBug)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.label = QtGui.QLabel(SubmitBug)
        self.label.setObjectName("label")
        self.verticalLayout_3.addWidget(self.label)
        spacerItem = QtGui.QSpacerItem(20, 20, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Fixed)
        self.verticalLayout_3.addItem(spacerItem)
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        self.label_2 = QtGui.QLabel(SubmitBug)
        self.label_2.setObjectName("label_2")
        self.verticalLayout.addWidget(self.label_2)
        self.lineEdit_summary = QtGui.QLineEdit(SubmitBug)
        self.lineEdit_summary.setObjectName("lineEdit_summary")
        self.verticalLayout.addWidget(self.lineEdit_summary)
        self.verticalLayout_3.addLayout(self.verticalLayout)
        self.verticalLayout_2 = QtGui.QVBoxLayout()
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.label_3 = QtGui.QLabel(SubmitBug)
        self.label_3.setObjectName("label_3")
        self.verticalLayout_2.addWidget(self.label_3)
        self.plainTextEdit_details = QtGui.QPlainTextEdit(SubmitBug)
        self.plainTextEdit_details.setObjectName("plainTextEdit_details")
        self.verticalLayout_2.addWidget(self.plainTextEdit_details)
        self.verticalLayout_3.addLayout(self.verticalLayout_2)
        self.checkBox_show_this_dialog = QtGui.QCheckBox(SubmitBug)
        self.checkBox_show_this_dialog.setChecked(True)
        self.checkBox_show_this_dialog.setObjectName("checkBox_show_this_dialog")
        self.verticalLayout_3.addWidget(self.checkBox_show_this_dialog)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem1)
        self.pushButton_submit = QtGui.QPushButton(SubmitBug)
        self.pushButton_submit.setObjectName("pushButton_submit")
        self.horizontalLayout.addWidget(self.pushButton_submit)
        self.pushButton_cancel = QtGui.QPushButton(SubmitBug)
        self.pushButton_cancel.setObjectName("pushButton_cancel")
        self.horizontalLayout.addWidget(self.pushButton_cancel)
        self.verticalLayout_3.addLayout(self.horizontalLayout)

        self.retranslateUi(SubmitBug)
        QtCore.QMetaObject.connectSlotsByName(SubmitBug)

    def retranslateUi(self, SubmitBug):
        SubmitBug.setWindowTitle(QtGui.QApplication.translate("SubmitBug", "Submit Bug", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("SubmitBug", "<html><head/><body>\n"
"<p>Please provide as much information as possible about wath caused the bug that you are reporting.</p>\n"
"<p>This feature could recollect extra information about system, like OS, architecture, PYTHONPATH,<br>username, etc.</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("SubmitBug", "Summary:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("SubmitBug", "Details:", None, QtGui.QApplication.UnicodeUTF8))
        self.checkBox_show_this_dialog.setText(QtGui.QApplication.translate("SubmitBug", "Show this dialog on unexpected errors.", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_submit.setText(QtGui.QApplication.translate("SubmitBug", "Submit Now", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_cancel.setText(QtGui.QApplication.translate("SubmitBug", "Cancel", None, QtGui.QApplication.UnicodeUTF8))

