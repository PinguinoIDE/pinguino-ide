#! /usr/bin/python2
#-*- coding: utf-8 -*-

from PySide import QtGui

from ..methods.decorators import Decorator


########################################################################
class SearchReplace(object):

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_line_edit_content("main.lineEdit_replace")
    @Decorator.requiere_text_mode()
    def replace_instantaneous(self, text_to_replace):
        empty = not text_to_replace == QtGui.QApplication.translate("Frame", "Replace...")
        self.main.pushButton_replace.setEnabled(empty)
        self.main.pushButton_replace_all.setEnabled(empty)
        if empty: return


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_line_edit_content("main.lineEdit_search")
    @Decorator.requiere_text_mode()
    def search_instantaneous(self, text_to_search):
        empty = not text_to_search == QtGui.QApplication.translate("Frame", "Search...")
        self.main.pushButton_search_previous.setEnabled(empty)
        self.main.pushButton_search_next.setEnabled(empty)

        editor = self.main.tabWidget_files.currentWidget()
        editor.text_edit.setExtraSelections([])
        if not empty: return

        if self.main.checkBox_case_sensitive.isChecked():
            content = editor.text_edit.toPlainText()
            count = content.count(text_to_search)
        else:
            content = editor.text_edit.toPlainText().lower()
            count = content.count(text_to_search.lower())

        if count == 0: message = QtGui.QApplication.translate("Frame", "No words were found.")
        elif  count == 1: message = QtGui.QApplication.translate("Frame", "One word were found.")
        else: message = QtGui.QApplication.translate("Frame", "%d words were found."%count)
        self.main.label_replace_info.setText(message)

        #for i in range(count): self.search_next()

        text_cur = editor.text_edit.textCursor()
        editor.text_edit.moveCursor(text_cur.Start)
        self.__search__(text_to_search)



    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_line_edit_content("main.lineEdit_search")
    def __search__(self, text_to_search):
        if text_to_search == QtGui.QApplication.translate("Frame", "Search..."): return


        self.find_match(word=text_to_search,
                        back=False,
                        sensitive=self.main.checkBox_case_sensitive.isChecked(),
                        whole=False)

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_line_edit_content("main.lineEdit_search")
    @Decorator.requiere_text_mode()
    def search_previous(self):
        self.find_match(word=self.main.lineEdit_search.text(),
                        back=True,
                        sensitive=self.main.checkBox_case_sensitive.isChecked(),
                        whole=False)

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_line_edit_content("main.lineEdit_search")
    @Decorator.requiere_text_mode()
    def search_next(self):
        self.find_match(word=self.main.lineEdit_search.text(),
                        back=False,
                        sensitive=self.main.checkBox_case_sensitive.isChecked(),
                        whole=False)

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_line_edit_content("main.lineEdit_search")
    @Decorator.requiere_line_edit_content("main.lineEdit_replace")
    @Decorator.requiere_text_mode()
    def replace(self):
        editor = self.main.tabWidget_files.currentWidget()
        text_cur = editor.text_edit.textCursor()
        if text_cur.selectedText() == self.main.lineEdit_search.text():
            text_cur.removeSelectedText()
            text_cur.insertText(self.main.lineEdit_replace.text())
            text_cur.clearSelection()
            self.highligh_line(line=None, color="#ffff7f")

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_line_edit_content("main.lineEdit_search")
    @Decorator.requiere_line_edit_content("main.lineEdit_replace")
    @Decorator.requiere_text_mode()
    def replaceall(self):
        self.replace_all_match(wordOld=self.main.lineEdit_search.text(),
                               wordNew=self.main.lineEdit_replace.text(),
                               sensitive=self.main.checkBox_case_sensitive.isChecked(),
                               whole=False)

    #----------------------------------------------------------------------
    def replace_all_match(self, wordOld, wordNew, sensitive=False, whole=False):
        editor = self.main.tabWidget_files.currentWidget()
        text_doc = editor.text_edit.document()
        text_cur = editor.text_edit.textCursor()
        s = text_doc.FindCaseSensitively if sensitive else 0
        w = text_doc.FindWholeWords if whole else 0
        editor.text_edit.moveCursor(text_cur.NoMove, text_cur.KeepAnchor)
        text_cur.beginEditBlock()
        editor.text_edit.moveCursor(text_cur.Start)
        count = 0
        while True:
            result = False
            if sensitive or whole: result = editor.text_edit.find(wordOld, s | w)
            else: result = editor.text_edit.find(wordOld)
            if result:
                tc = editor.text_edit.textCursor()
                if tc.hasSelection(): tc.insertText(wordNew)
                count += 1
            else: break
            #replace = False
        text_cur.endEditBlock()
        self.main.label_replace_info.setText("%d words were replaced."%count)

    #----------------------------------------------------------------------
    def find_match(self, word, back, sensitive, whole):
        editor = self.main.tabWidget_files.currentWidget()
        text_doc = editor.text_edit.document()
        text_cur = editor.text_edit.textCursor()
        b = text_doc.FindBackward if back else 0
        s = text_doc.FindCaseSensitively if sensitive else 0
        w = text_doc.FindWholeWords if whole else 0
        editor.text_edit.moveCursor(text_cur.NoMove, text_cur.KeepAnchor)
        if back or sensitive or whole: editor.text_edit.find(word, b | s | w)
        else:  editor.text_edit.find(word)
        self.highligh_line(line=None, color="#ffff7f", text_cursor=editor.text_edit.textCursor())


