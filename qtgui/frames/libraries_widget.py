# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Desarrollo/Pinguino/GitHub/pinguino-ide/qtgui/frames/libraries_widget.ui'
#
# Created: Sat Jul 11 16:36:52 2015
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_LibraryManager(object):
    def setupUi(self, LibraryManager):
        LibraryManager.setObjectName("LibraryManager")
        LibraryManager.resize(734, 376)
        self.centralwidget = QtGui.QWidget(LibraryManager)
        self.centralwidget.setObjectName("centralwidget")
        self.gridLayout = QtGui.QGridLayout(self.centralwidget)
        self.gridLayout.setObjectName("gridLayout")
        self.pushButton_close = QtGui.QPushButton(self.centralwidget)
        self.pushButton_close.setMinimumSize(QtCore.QSize(165, 0))
        self.pushButton_close.setMaximumSize(QtCore.QSize(165, 16777215))
        self.pushButton_close.setObjectName("pushButton_close")
        self.gridLayout.addWidget(self.pushButton_close, 1, 1, 1, 1)
        self.tabWidget = QtGui.QTabWidget(self.centralwidget)
        self.tabWidget.setObjectName("tabWidget")
        self.tab_2 = QtGui.QWidget()
        self.tab_2.setObjectName("tab_2")
        self.gridLayout_3 = QtGui.QGridLayout(self.tab_2)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.lineEdit_source = QtGui.QLineEdit(self.tab_2)
        self.lineEdit_source.setObjectName("lineEdit_source")
        self.gridLayout_3.addWidget(self.lineEdit_source, 0, 0, 1, 1)
        self.gridLayout_4 = QtGui.QGridLayout()
        self.gridLayout_4.setObjectName("gridLayout_4")
        self.pushButton_update = QtGui.QPushButton(self.tab_2)
        self.pushButton_update.setEnabled(False)
        self.pushButton_update.setObjectName("pushButton_update")
        self.gridLayout_4.addWidget(self.pushButton_update, 0, 0, 1, 1)
        self.pushButton_remove = QtGui.QPushButton(self.tab_2)
        self.pushButton_remove.setEnabled(False)
        self.pushButton_remove.setObjectName("pushButton_remove")
        self.gridLayout_4.addWidget(self.pushButton_remove, 0, 1, 1, 1)
        self.pushButton_reload_s = QtGui.QPushButton(self.tab_2)
        self.pushButton_reload_s.setObjectName("pushButton_reload_s")
        self.gridLayout_4.addWidget(self.pushButton_reload_s, 0, 2, 1, 1)
        self.gridLayout_3.addLayout(self.gridLayout_4, 4, 0, 1, 3)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.checkBox_sources = QtGui.QCheckBox(self.tab_2)
        self.checkBox_sources.setObjectName("checkBox_sources")
        self.horizontalLayout_3.addWidget(self.checkBox_sources)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem)
        self.gridLayout_3.addLayout(self.horizontalLayout_3, 3, 0, 1, 3)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.radioButton_repo_git = QtGui.QRadioButton(self.tab_2)
        self.radioButton_repo_git.setChecked(True)
        self.radioButton_repo_git.setObjectName("radioButton_repo_git")
        self.horizontalLayout_2.addWidget(self.radioButton_repo_git)
        self.radioButton_repo_hg = QtGui.QRadioButton(self.tab_2)
        self.radioButton_repo_hg.setObjectName("radioButton_repo_hg")
        self.horizontalLayout_2.addWidget(self.radioButton_repo_hg)
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem1)
        self.gridLayout_3.addLayout(self.horizontalLayout_2, 1, 0, 1, 3)
        self.pushButton_from_zip = QtGui.QPushButton(self.tab_2)
        self.pushButton_from_zip.setObjectName("pushButton_from_zip")
        self.gridLayout_3.addWidget(self.pushButton_from_zip, 0, 2, 1, 1)
        self.pushButton_add = QtGui.QPushButton(self.tab_2)
        self.pushButton_add.setObjectName("pushButton_add")
        self.gridLayout_3.addWidget(self.pushButton_add, 0, 1, 1, 1)
        self.tableWidget_sources = QtGui.QTableWidget(self.tab_2)
        self.tableWidget_sources.setAutoFillBackground(True)
        self.tableWidget_sources.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        self.tableWidget_sources.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        self.tableWidget_sources.setEditTriggers(QtGui.QAbstractItemView.NoEditTriggers)
        self.tableWidget_sources.setAlternatingRowColors(True)
        self.tableWidget_sources.setVerticalScrollMode(QtGui.QAbstractItemView.ScrollPerPixel)
        self.tableWidget_sources.setHorizontalScrollMode(QtGui.QAbstractItemView.ScrollPerPixel)
        self.tableWidget_sources.setGridStyle(QtCore.Qt.NoPen)
        self.tableWidget_sources.setObjectName("tableWidget_sources")
        self.tableWidget_sources.setColumnCount(3)
        self.tableWidget_sources.setRowCount(0)
        item = QtGui.QTableWidgetItem()
        self.tableWidget_sources.setHorizontalHeaderItem(0, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget_sources.setHorizontalHeaderItem(1, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget_sources.setHorizontalHeaderItem(2, item)
        self.tableWidget_sources.horizontalHeader().setHighlightSections(False)
        self.tableWidget_sources.horizontalHeader().setStretchLastSection(True)
        self.tableWidget_sources.verticalHeader().setVisible(False)
        self.tableWidget_sources.verticalHeader().setHighlightSections(True)
        self.tableWidget_sources.verticalHeader().setStretchLastSection(False)
        self.gridLayout_3.addWidget(self.tableWidget_sources, 2, 0, 1, 3)
        self.tabWidget.addTab(self.tab_2, "")
        self.tab = QtGui.QWidget()
        self.tab.setObjectName("tab")
        self.gridLayout_2 = QtGui.QGridLayout(self.tab)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.pushButton_reload_l = QtGui.QPushButton(self.tab)
        self.pushButton_reload_l.setObjectName("pushButton_reload_l")
        self.gridLayout_2.addWidget(self.pushButton_reload_l, 3, 1, 1, 1)
        self.pushButton_apply = QtGui.QPushButton(self.tab)
        self.pushButton_apply.setEnabled(False)
        self.pushButton_apply.setObjectName("pushButton_apply")
        self.gridLayout_2.addWidget(self.pushButton_apply, 3, 0, 1, 1)
        self.tableWidget_libs = QtGui.QTableWidget(self.tab)
        self.tableWidget_libs.setAlternatingRowColors(True)
        self.tableWidget_libs.setVerticalScrollMode(QtGui.QAbstractItemView.ScrollPerPixel)
        self.tableWidget_libs.setHorizontalScrollMode(QtGui.QAbstractItemView.ScrollPerPixel)
        self.tableWidget_libs.setGridStyle(QtCore.Qt.NoPen)
        self.tableWidget_libs.setWordWrap(False)
        self.tableWidget_libs.setCornerButtonEnabled(False)
        self.tableWidget_libs.setObjectName("tableWidget_libs")
        self.tableWidget_libs.setColumnCount(4)
        self.tableWidget_libs.setRowCount(0)
        item = QtGui.QTableWidgetItem()
        self.tableWidget_libs.setHorizontalHeaderItem(0, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget_libs.setHorizontalHeaderItem(1, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget_libs.setHorizontalHeaderItem(2, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget_libs.setHorizontalHeaderItem(3, item)
        self.tableWidget_libs.horizontalHeader().setHighlightSections(False)
        self.tableWidget_libs.horizontalHeader().setStretchLastSection(True)
        self.tableWidget_libs.verticalHeader().setVisible(False)
        self.gridLayout_2.addWidget(self.tableWidget_libs, 1, 0, 1, 2)
        self.label_help = QtGui.QLabel(self.tab)
        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.label_help.setFont(font)
        self.label_help.setObjectName("label_help")
        self.gridLayout_2.addWidget(self.label_help, 0, 0, 1, 2)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.checkBox_libs = QtGui.QCheckBox(self.tab)
        self.checkBox_libs.setObjectName("checkBox_libs")
        self.horizontalLayout_4.addWidget(self.checkBox_libs)
        spacerItem2 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_4.addItem(spacerItem2)
        self.gridLayout_2.addLayout(self.horizontalLayout_4, 2, 0, 1, 2)
        self.tabWidget.addTab(self.tab, "")
        self.gridLayout.addWidget(self.tabWidget, 0, 0, 1, 2)
        spacerItem3 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem3, 1, 0, 1, 1)
        LibraryManager.setCentralWidget(self.centralwidget)

        self.retranslateUi(LibraryManager)
        self.tabWidget.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(LibraryManager)
        LibraryManager.setTabOrder(self.lineEdit_source, self.pushButton_add)
        LibraryManager.setTabOrder(self.pushButton_add, self.pushButton_from_zip)
        LibraryManager.setTabOrder(self.pushButton_from_zip, self.tableWidget_sources)
        LibraryManager.setTabOrder(self.tableWidget_sources, self.pushButton_update)
        LibraryManager.setTabOrder(self.pushButton_update, self.pushButton_remove)
        LibraryManager.setTabOrder(self.pushButton_remove, self.tabWidget)
        LibraryManager.setTabOrder(self.tabWidget, self.tableWidget_libs)

    def retranslateUi(self, LibraryManager):
        LibraryManager.setWindowTitle(QtGui.QApplication.translate("LibraryManager", "Library Manager", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_close.setText(QtGui.QApplication.translate("LibraryManager", "Close", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_update.setToolTip(QtGui.QApplication.translate("LibraryManager", "Update library from their repository.", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_update.setText(QtGui.QApplication.translate("LibraryManager", "Update selected", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_remove.setToolTip(QtGui.QApplication.translate("LibraryManager", "Delete directory from disk.", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_remove.setText(QtGui.QApplication.translate("LibraryManager", "Remove selected", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_reload_s.setText(QtGui.QApplication.translate("LibraryManager", "Reload", None, QtGui.QApplication.UnicodeUTF8))
        self.checkBox_sources.setText(QtGui.QApplication.translate("LibraryManager", "Check/Uncheck all", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_repo_git.setText(QtGui.QApplication.translate("LibraryManager", "Git", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_repo_hg.setText(QtGui.QApplication.translate("LibraryManager", "Mercurial", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_from_zip.setText(QtGui.QApplication.translate("LibraryManager", "Install from .zip", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_add.setText(QtGui.QApplication.translate("LibraryManager", "Add", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget_sources.horizontalHeaderItem(0).setText(QtGui.QApplication.translate("LibraryManager", "Name", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget_sources.horizontalHeaderItem(1).setText(QtGui.QApplication.translate("LibraryManager", "Repository", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget_sources.horizontalHeaderItem(2).setText(QtGui.QApplication.translate("LibraryManager", "Web page", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2), QtGui.QApplication.translate("LibraryManager", "Sources", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_reload_l.setText(QtGui.QApplication.translate("LibraryManager", "Reload", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_apply.setText(QtGui.QApplication.translate("LibraryManager", "Apply", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget_libs.horizontalHeaderItem(0).setText(QtGui.QApplication.translate("LibraryManager", "Library", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget_libs.horizontalHeaderItem(1).setText(QtGui.QApplication.translate("LibraryManager", "Architecture", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget_libs.horizontalHeaderItem(2).setText(QtGui.QApplication.translate("LibraryManager", "Author", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget_libs.horizontalHeaderItem(3).setText(QtGui.QApplication.translate("LibraryManager", "Description", None, QtGui.QApplication.UnicodeUTF8))
        self.label_help.setText(QtGui.QApplication.translate("LibraryManager", "Select libraries for enable or disable it, only libraries enabled can be used in the editor.", None, QtGui.QApplication.UnicodeUTF8))
        self.checkBox_libs.setText(QtGui.QApplication.translate("LibraryManager", "Check/Uncheck all", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QtGui.QApplication.translate("LibraryManager", "Libraries", None, QtGui.QApplication.UnicodeUTF8))

