#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import pickle
import urllib2

from PySide import QtCore, QtGui
from bs4 import BeautifulSoup

#from ..methods.constants import self.ide_wiki_docs
from ..methods.dialogs import Dialogs
from ...frames.wiki_doc_widget import Ui_WikiDocs

########################################################################
class WikiDock(QtGui.QMainWindow):
    

    #----------------------------------------------------------------------
    def __init__(self):
        super(WikiDock, self).__init__()
        
        self.main_widget = Ui_WikiDocs()
        self.main_widget.setupUi(self)
        
        self.ide_wiki_docs = os.path.join(os.environ.get("PINGUINO_USER_PATH"), "wikidocs.pickle")

        self.to_ignore = ["Examples"]
        
        self.count_lib = 1
        
        self.html_doc = "<html>"
        
        self.build_header()
        self.build_title()
        

        self.set_home()
        
        self.main_widget.textBrowser_doc.setStyleSheet("QTextBrowser {background-color: white;}")
        self.connect(self.main_widget.textBrowser_doc, QtCore.SIGNAL("anchorClicked(QUrl)"), self.open_tab_doc)
        self.connect(self.main_widget.tabWidget, QtCore.SIGNAL("tabCloseRequested(int)"), self.tab_close)        
        
        
        self.centrar()
        
    #----------------------------------------------------------------------
    def tab_close(self, index):
        
        if index == 0: return
        else: self.main_widget.tabWidget.removeTab(index)
        

    #----------------------------------------------------------------------
    def centrar(self):
        
        screen = QtGui.QDesktopWidget().screenGeometry()
        size =  self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)        
        

    #----------------------------------------------------------------------
    def set_home(self, libs=None):
        
        if not libs: libs = self.get_libraries()
        
        libs_html = ""
        self.index_html = ""
        for lib in libs:
            libs_html += self.build_library(**lib)
        
        self.html_doc += self.index_html + "<hr>"
        self.html_doc += libs_html
        
        self.html_doc += "</body></html>"        
    
        self.main_widget.textBrowser_doc.insertHtml(self.html_doc)
        self.main_widget.textBrowser_doc.moveCursor(QtGui.QTextCursor.Start)

        
    #----------------------------------------------------------------------
    def open_tab_doc(self, url):
        
        url = url.toString()
        if url == "__update__":
            reply = Dialogs.confirm_message(self, "This will take a long time.")
            if reply:
                libs = self.update_from_wiki()
                if libs:
                    libs = self.update_from_wiki()
                    pickle.dump(libs, file(self.ide_wiki_docs, "w"))
                else:
                    Dialogs.info_message(self, "Impossible read Wiki page.\n"+"http://wiki.pinguino.cc") 
                self.set_home(libs=libs)
                return
            else:
                self.set_home()
                return
        
        if not "http" in url: return
        
        #self.main.set_url_wiki_docs(url[1:])
        
        #QtGui.q
        
        #self.main_widget.tabWidget.addTab()
        
        self.add_tab()
        self.replace_with_url(url[1:])
        
        
    #----------------------------------------------------------------------
    def add_tab(self):
        
        
        tab = QtGui.QWidget()
        #tab.setObjectName("tab")
        gridLayout = QtGui.QGridLayout(tab)
        gridLayout.setSpacing(0)
        gridLayout.setContentsMargins(0, 0, 0, 0)
        gridLayout.setObjectName("gridLayout")
        textBrowser_doc = QtGui.QTextBrowser(tab)
        textBrowser_doc.setFrameShape(QtGui.QFrame.NoFrame)
        textBrowser_doc.setTextInteractionFlags(QtCore.Qt.LinksAccessibleByKeyboard|QtCore.Qt.LinksAccessibleByMouse|QtCore.Qt.TextBrowserInteraction|QtCore.Qt.TextSelectableByKeyboard|QtCore.Qt.TextSelectableByMouse)
        textBrowser_doc.setOpenExternalLinks(True)
        #textBrowser_doc.setObjectName("textBrowser_doc")
        
        textBrowser_doc.setStyleSheet("QTextBrowser {background-color: white;}")
        
        setattr(tab, "textBrowser", textBrowser_doc)
        
        gridLayout.addWidget(textBrowser_doc, 0, 0, 1, 1)
        self.main_widget.tabWidget.addTab(tab, "")
        
        self.main_widget.tabWidget.setCurrentWidget(tab)
        
        
        
    #----------------------------------------------------------------------
    def replace_with_url(self, url):
        
        index = self.main_widget.tabWidget.currentIndex()
        tab = self.main_widget.tabWidget.currentWidget()
        
        
        html = self.get_html_from_url(url)
        soup = BeautifulSoup(html)
        
        try:
            title = soup.find_all("h1", attrs={"id":"firstHeading"})[0].text
        except:            
            self.main_widget.tabWidget.removeTab(self.main_widget.tabWidget.currentIndex())            
            Dialogs.error_message(self, "Impossible read Wiki docs.\n"+url)
            return 
        
        
        content = soup.find_all("div", attrs={"id": "content"})[0]
        tab.textBrowser.clear()
        
        
        
        delete = []
        delete.append(content.find("h3",attrs={"id":"siteSub"}).extract())
        delete.append(content.find("div",attrs={"id":"jump-to-nav"}).extract())
        delete.append(content.find("div",attrs={"class":"printfooter"}).extract())
        delete.append(content.find("div",attrs={"id":"catlinks"}).extract())
        
        content = str(content.extract())
        
        content = content.replace('<div id="bodyContent">', '<hr><div id="bodyContent">')
        
        for d in delete:
            content = content.replace("", "")
        
        
        
        html = """
            <html>
            <head>
                %s
            </head>
            <body>
                %s
            </body>
        </html>"""% (self.stylesheet(), content)
        
        tab.textBrowser.insertHtml(html)
        tab.textBrowser.moveCursor(QtGui.QTextCursor.Start)
        
        self.main_widget.tabWidget.setTabText(index, title)
        
        
    #----------------------------------------------------------------------
    def get_libraries(self):
        
        if not os.path.isfile(self.ide_wiki_docs):
            libs = self.update_from_wiki()
            pickle.dump(libs, file(self.ide_wiki_docs, "w"))
            return libs
            
        else:
            return pickle.load(file(self.ide_wiki_docs, "r"))
            
        
    #----------------------------------------------------------------------
    def update_from_wiki(self):
        
        url = "http://wiki.pinguino.cc"
        html = self.get_html_from_url(url+"/index.php/Category:Libraries")
        soup = BeautifulSoup(html)
        if not soup.find_all("table"): return []
        
        table = soup.find_all("table")[1]
        
        libs = []
        for lib in table.find_all("a"):
            description, funtions = self.get_functions(url+lib.get("href"))
            libs.append({
                "href": url+lib.get("href"),
                "name": lib.text,
                "functions": funtions,
                "description": description,
            })
            
        return libs
        
    #----------------------------------------------------------------------
    def get_functions(self, url):
        
        html = self.get_html_from_url(url)
        soup = BeautifulSoup(html)
        
        print "\n" + "*" * 70
        print url
        print "*" * 70 + "\n"  
                
        mw_pages = soup.find_all("div", attrs={"id":"mw-pages"})
        
        if not mw_pages: return "", []
        
        mw_pages = mw_pages[0]
        
        try: description = soup.find_all("table")[0].find_all("tr")[1].text
        except: description = ""
        
        funcs = []
        for func in mw_pages.find_all("a"):
            funcs.append({
                "href": "http://wiki.pinguino.cc"+func.get("href"),
                "name": func.text,
            })
            
        return description, funcs
        
    #----------------------------------------------------------------------
    def get_html_from_url(self, url):
        
        page = urllib2.urlopen(url)
        html = page.readlines()
        page.close()
        return "".join(html).replace("&nbsp;", "")
    
    
    
    
        
    
    #----------------------------------------------------------------------
    def build_header(self):
        
        html = """
        <head>
        
        <!--styles-->
        <style>
        
        body{
            margin: 10px;
        }
        
        div#title a:link,
        div#title a:visited,
        div#title a:hover{
            color: #3C3CFF;
	    text-decoration: none;
        }
        
        div#title{
            margin: 10px;
            font-size: 25px;
            font-weight: bold;
        }
        p#update a:link,
        p#update a:visited,
        p#update a:hover{
            color: #3C3CFF;
	    text-decoration: underline;
        }
        
        p#update{
            margin: 0px;
            font-size: 12px;
            font-weight: normal;
        }
	
        div.lib{
            font-size: 16px;
            font-weight: bold;
        }
        
        div.lib a:link,
        div.lib a:visited,
        div.lib a:hover{
            color: #3030C7;
            text-decoration: none;
        }
        
        .list a:link,
        .list a:visited,
        .list a:hover,{
            color: #5555ff;
            text-decoration: underline;
            font-size: 13px;
        }
        
        
        div#des{
            color: #3f3f3f;
            float: left;
        }
        
        
        
        
        
        
        </style>
        
            
        </head>
        """
        self.html_doc += html
        
        
    #----------------------------------------------------------------------
    def stylesheet(self):
        
        
                
        return """
        
        <style type="text/css">
        
        
	body{
            margin: 10px;
	   color: #3f3f3f;
        }
        

        h1#firstHeading{
            color: #3C3CFF;
            margin: 10px;
            font-size: 25px;
            font-weight: bold;
        }
        
        h2{
            
        }        
        */
        
        .source-c {line-height: normal;}
        .source-c li, .source-c pre {
                line-height: normal; border: 0px none white;
        }
        /**
         * GeSHi Dynamically Generated Stylesheet
         * --------------------------------------
         * Dynamically generated stylesheet for c
         * CSS class: source-c, CSS id: 
         * GeSHi (C) 2004 - 2007 Nigel McNie, 2007 - 2008 Benny Baumann
         * (http://qbnz.com/highlighter/ and http://geshi.org/)
         * --------------------------------------
         */
        .c.source-c .de1, .c.source-c .de2 {font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;}
        .c.source-c  {font-family:monospace;}
        .c.source-c .imp {font-weight: bold; color: red;}
        .c.source-c li, .c.source-c .li1 {font-weight: normal; vertical-align:top;}
        .c.source-c .ln {width:1px;text-align:right;margin:0;padding:0 2px;vertical-align:top;}
        .c.source-c .li2 {font-weight: bold; vertical-align:top;}
        .c.source-c .kw1 {color: #b1b100;}
        .c.source-c .kw2 {color: #000000; font-weight: bold;}
        .c.source-c .kw3 {color: #000066;}
        .c.source-c .kw4 {color: #993333;}
        .c.source-c .co1 {color: #666666; font-style: italic;}
        .c.source-c .co2 {color: #339933;}
        .c.source-c .coMULTI {color: #808080; font-style: italic;}
        .c.source-c .es0 {color: #000099; font-weight: bold;}
        .c.source-c .es1 {color: #000099; font-weight: bold;}
        .c.source-c .es2 {color: #660099; font-weight: bold;}
        .c.source-c .es3 {color: #660099; font-weight: bold;}
        .c.source-c .es4 {color: #660099; font-weight: bold;}
        .c.source-c .es5 {color: #006699; font-weight: bold;}
        .c.source-c .br0 {color: #009900;}
        .c.source-c .sy0 {color: #339933;}
        .c.source-c .st0 {color: #ff0000;}
        .c.source-c .nu0 {color: #0000dd;}
        .c.source-c .nu6 {color: #208080;}
        .c.source-c .nu8 {color: #208080;}
        .c.source-c .nu12 {color: #208080;}
        .c.source-c .nu16 {color:#800080;}
        .c.source-c .nu17 {color:#800080;}
        .c.source-c .nu18 {color:#800080;}
        .c.source-c .nu19 {color:#800080;}
        .c.source-c .me1 {color: #202020;}
        .c.source-c .me2 {color: #202020;}
        .c.source-c .ln-xtra, .c.source-c li.ln-xtra, .c.source-c div.ln-xtra {background-color: #ffc;}
        .c.source-c span.xtra { display:block; }
        
        /*]]>*/
        </style>"""    
        
    
    #----------------------------------------------------------------------
    def build_library(self, href, name, description, functions):
        
        if name in self.to_ignore: return ""
        
        name = name.replace("Library", "")
            
    
        html_funcs = self.build_functions(functions, str(self.count_lib))
        

        html = """
        <div class="lib" id="%s">%s. <a href="%s">%s</a></div>
        
        """% (name.replace(" ", "_"), str(self.count_lib), href, name)
        
        self.index_html += """<div class="list">%s. <a href="#%s">%s</div>""" % (str(self.count_lib), name.replace(" ", "_"), name)
        
        if description:
            html += """<div id="des">%s</div>""" % description
           
        
        self.count_lib += 1
        
        return html + html_funcs  # + "<hr>"            
        
    #----------------------------------------------------------------------
    def build_functions(self, functions, number):
        
        
        html = ""
        count = 1
        for func in functions:
            html += """
            <div class="list" id="func">%s.%d <a href="#%s">%s</a></div>
            """% (number, count, func["href"], func["name"])
            count += 1
        
        
        return html + "<br>"
        
    
    #----------------------------------------------------------------------
    def build_title(self):
        
        html = """
        <body>
        <p id="update" align="right"><a href="__update__">Update libraries</a></p>
        <div id="title"><a href="http://wiki.pinguino.cc/">Pinguino Libraries</a></div>
        <hr>
        """
        self.html_doc += html
        
        