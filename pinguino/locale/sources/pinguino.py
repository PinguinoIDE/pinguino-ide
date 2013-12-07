#!/usr/bin/env python
#  -*- coding: UTF-8 -*-

"""-------------------------------------------------------------------------
    pinguino

    (c) 2008-2009-2010-2011 Jean-Pierre MANDON <jp.mandon@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
    -------------------------------------------------------------------------"""

import os
import platform
import sys
import threading
import re
import time
import shutil
from subprocess import Popen, STDOUT
import wx

import argparse
from boards import boardlist
from editor import IDE
from editor.frames import framePinguinoX
from uploader import Uploader

from editor.constants import pinguino_version, HOME_DIR, P8_DIR, P32_DIR, TEMP_DIR, \
     SOURCE_DIR

#set words to translations
from wxgui._trad import _

if sys.platform == 'win32':
    os.environ['PATH'] = os.environ['PATH'] + ';' + os.path.join(HOME_DIR,'win32','p8','bin') 
else:
    #Used for Python and Pic32 debug,
    os.environ["LD_LIBRARY_PATH"]="/usr/lib32:%s/linux/p32/bin:/usr/lib:/usr/lib64" % HOME_DIR

########################################################################
class Pinguino(framePinguinoX, IDE):
    global gui

    #----------------------------------------------------------------------
    def __initPinguino__(self, parent):
    
    # ----------------------------------------------------------------------
    # Initializes vars
    # ----------------------------------------------------------------------
        self.autocompleteHide = False
        self.changingBoard = False
        self.boardlist = boardlist
        self.currentLateralDir = os.path.join(os.getcwd(),"examples")
        self.recentsFiles = []
        
        # Autocompleter
        self.autoCompleteWords = []
        self.filesNeighbors = []
        self.allVars_back = [] 
        self.allFunc_back = []
        self.allDefi_back = []
        
        self.setOSvariables()
        
    # ----------------------------------------------------------------------
    # Load config file
    # ----------------------------------------------------------------------
        self.loadConfigFile()
        self.loadConfigIDE()        
        
        
    # ----------------------------------------------------------------------
    # Initializes environment
    # ----------------------------------------------------------------------
        if os.path.isdir(TEMP_DIR) == False: os.mkdir(TEMP_DIR)
            
    
    # ----------------------------------------------------------------------
    # Initializes IDE objects
    # ----------------------------------------------------------------------
        self.initIDEobjects() 


    # ----------------------------------------------------------------------
    # Initializes config
    # ----------------------------------------------------------------------
        self.setEditorFont()
        self.loadRecents()
        
        
    # ----------------------------------------------------------------------
    # Update distribution
    # ----------------------------------------------------------------------
        #This can't be instant
        wx.FutureCall(1000, self.updateSashs)
        
        #Unnecessary on Windows XP and GNU/Linux, totally destructive on mac OS and Windows 7/8
        #self.updateIDE()


    #----------------------------------------------------------------------
    def getDevices(self, arch):
        """"""
        devs = []
        for board in boardlist:
            if board.bldr == "noboot": pass
            elif board.arch == arch: devs.append(board.name)
        return 2, devs  #Columns, List
    
    #----------------------------------------------------------------------
    def setOSvariables(self):
        """Get OS name and define some OS dependant variable"""
        pa = platform.architecture()
        if pa[0]=='64bit': OSarch="64"
        else: OSarch="32"

        if sys.platform == 'darwin':
            self.osdir = 'macosx'
            self.debug_port = '/dev/tty.usbmodem1912'
            self.c8 = 'sdcc'
            self.u32 = 'mphidflash'
            self.make = 'make'

        elif sys.platform == 'win32':
            self.osdir = 'win32'
            self.debug_port = 15
            self.c8 = 'sdcc.exe'
            self.p8 = 'picpgm.exe'
            self.u32 = 'mphidflash.exe'
            self.make = os.path.join(HOME_DIR, self.osdir, 'p32', 'bin', 'make.exe')

        else:
            self.osdir = 'linux'
            self.debug_port = '/dev/ttyACM0'
            self.c8 = 'sdcc'
            self.p8 = 'picpgm'
            self.u32 = 'ubw32'
            self.make = 'make'

        #self.P8_BIN_DIR = os.path.join(HOME_DIR, self.osdir, 'p8', 'bin' + OSarch)
        #self.P32_BIN_DIR = os.path.join(HOME_DIR, self.osdir, 'p32', 'bin' + OSarch)
        self.P8_BIN_DIR = os.path.join(HOME_DIR, self.osdir, 'p8', 'bin')
        self.P32_BIN_DIR = os.path.join(HOME_DIR, self.osdir, 'p32', 'bin')

    #----------------------------------------------------------------------
    def setBoard(self, arch, mode, name, bootloader):

        self.changingBoard = True
        
        self.rw = []
        self.regobject = []
        self.keywordList = []
        self.reservedword = []
        self.libinstructions = []
        
        self.modeCompile = mode
        warning = ""

        # RB 17/09/2013
        for board in boardlist:
            if name == board.name:
                self.curBoard = board
                self.curBoard.bldr = bootloader[0]
                self.curBoard.memstart = int(bootloader[1])
                if arch == 32 :
                    textStatus=self.curBoard.name
                else :
                    if mode != "BOOT" :
                        self.curBoard.bldr = "noboot"
                        self.curBoard.memstart = 0x0000
                        warning = "\nWARNING!! Bootloader will be erased.\n"
                        textStatus = self.curBoard.name + " [ICSP]"
                    else :
                        textStatus=self.curBoard.name + " [" + bootloader[0] + "]"


        """
        if mode == "BOOT":
            for board in boardlist:
                if name == board.name:
                    self.curBoard = board
                    if bootloader == "noboot":
                        self.curBoard.bldr = "noboot"
                        self.curBoard.memstart = 0x0000
                    else:
                        self.curBoard.bldr = bootloader[0]
                        self.curBoard.memstart = int(bootloader[1])
            
            if arch == 8: textStatus=self.curBoard.name + " [" + bootloader[0] + "]"
            if arch == 32: textStatus=self.curBoard.name
            
        else:
            self.curBoard = boardlist[0]
            self.curBoard.name = name
            for board in boardlist:
                if name == board.name: self.curBoard.proc = board.proc
            self.curBoard.board = "PIC"+name.upper()
            self.curBoard.memstart = 0x0000
            #self.curBoard.memend = devlist[self.curBoard.proc][1]*1024 
            warning = "\nWARNING!! this mode can overwrite the bootloader code.\n"
            textStatus = self.curBoard.name + " [ICSP]"
        """

        self.displaymsg(_("Changing board")+"...", 0)
        self.statusBarEditor.SetStatusText(number=2, text=textStatus)
        if sys.platform=='darwin':
            self.readlib(self.curBoard, textStatus) #So slow
        else:
            self.Thread_curBoard = threading.Thread(target=self.readlib, args=(self.curBoard, textStatus))
            self.Thread_curBoard.start()

        if warning: self.displaymsg(warning, False)

    #----------------------------------------------------------------------
    def OnVerify(self, event=None):
        if self.changingBoard:
            self.displaymsg(_("Please wait a moment.")+"\n", 0)
            return
        
        self.displaymsg("", 1)
        
        #self.in_verify=1
        t0 = time.time()
        if self.GetPath()==-1:
            dlg = wx.MessageDialog(self,
                                   _('Open file first !!'),
                                   _('Warning'),
                                   wx.OK | wx.ICON_WARNING)
            result=dlg.ShowModal()
            dlg.Destroy()
            return False
        
        self.displaymsg(_("Board:\t")+" " + self.curBoard.name, 1)
        self.displaymsg(_("Proc:\t")+" " + self.curBoard.proc, 0)
        self.displaymsg(_("File:\t")+" " + self.GetPath(), 0)
        self.displaymsg(_("compiling")+("..."), 0)
        
        self.OnSave()
        filename=self.GetPath()
        filename,extension=os.path.splitext(filename)
        if os.path.exists(filename+".hex"):
            os.remove(filename+".hex")
        if os.path.exists(os.path.join(SOURCE_DIR, "user.c")):
            os.remove(os.path.join(SOURCE_DIR, "user.c"))
        retour=self.preprocess(filename, self.curBoard)
        if retour=="error":
            return False
        # compilation
        if self.curBoard.arch == 8:
            MAIN_FILE="main.hex"
        else:
            MAIN_FILE="main32.hex"
        retour=self.compile(filename, self.curBoard)
        if retour!=0:
            self.displaymsg(_("error while compiling"),0)
            self.displaymsg(_("check highlighted lines in your code"),0)
            self.displaymsg(_("You can review the file stdout (F8) for more information."),0)
        else:
            retour=self.link(filename, self.curBoard)
            if os.path.exists(os.path.join(SOURCE_DIR, MAIN_FILE))!=True:
                self.displaymsg(_("error while linking")+" "+filename+".o",0)
                self.displaymsg(_("You can review the file stdout (F8) for more information."),0)
                return False
            else:
                shutil.copy(os.path.join(SOURCE_DIR, MAIN_FILE), filename+".hex")
                self.displaymsg(_("compilation done"),0)
                self.displaymsg(self.getCodeSize(filename, self.curBoard),0)
                t = "%.1f" % ( time.time() - t0 )
                self.displaymsg( t + " "+_("seconds process time"),0)
                os.remove(os.path.join(SOURCE_DIR, MAIN_FILE))
                #os.remove(filename+".c")
                return True
        #self.in_verify=0
        self.focus()

    #----------------------------------------------------------------------
    def OnUpload(self, event=None, path=None):
        
        if path == None: path = self.GetPath()
        #else: path = filename
        
        if  path != -1:
            filename = path
            filename, extension = os.path.splitext(filename)
            if os.path.exists(filename + '.hex'):
                if self.curBoard.arch == 8:
                    #try:
                    u = Uploader(self.displaymsg, filename, self.curBoard)
                    #except usb.USBError:  #No device
                    #self.displaymsg("No device",0)
                    #return
                    
                else:
                    fichier = open(os.path.join(SOURCE_DIR, 'stdout'), 'w+')
                    sortie=Popen([os.path.join(HOME_DIR, self.osdir, 'p32', 'bin', self.u32),
                                  "-w",
                                  filename+".hex",
                                  "-r",
                                  "-n"],
                                 stdout=fichier, stderr=STDOUT)
                    sortie.communicate()
                    fichier.seek(0)
                    self.displaymsg(fichier.read(),0)
                    fichier.close()
            else:# no file
                dlg = wx.MessageDialog(self,
                                       _('File must be verified/compiled before upload'),
                                       _('Warning!'),
                                       wx.OK | wx.ICON_WARNING)
                result=dlg.ShowModal()
                dlg.Destroy()
        else:# not saved
            dlg = wx.MessageDialog(self,
                                   _('File must be saved before upload'),
                                   _('Warning!'),
                                   wx.OK | wx.ICON_WARNING)
            result=dlg.ShowModal()
            dlg.Destroy()
        if event != None: event.Skip()


    #----------------------------------------------------------------------
    def readlib(self, board, textStatus=""):
        """Load .pdl or .pdl32 files (keywords and libraries)
         trying to find PDL files to store reserved words."""
        
        self.keywordList = []
        if board.arch == 8:
            libext='.pdl'
            libdir=P8_DIR
        else:
            libext='.pdl32'
            libdir=P32_DIR
        #for fichier in os.listdir(os.path.join(libdir, 'lib')):
        for fichier in os.listdir(os.path.join(libdir, 'pdl')):
            filename,extension=os.path.splitext(fichier)
            if extension==libext:
                # check content of the PDL file
                #libfile=open(os.path.join(libdir, 'lib', fichier),'r')
                libfile=open(os.path.join(libdir, 'pdl', fichier),'r')
                for line in libfile:
                    if line!="\n":
                        # arduino's instruction
                        instruction=line[0:line.find(" ")]
                        self.keywordList.append(instruction)
                        # library's instruction
                        cnvinstruction=line[line.find(" ")+1:line.find("#")]
                        # find #include & #define
                        #include=line[line.find("#")+1:len(line)]
                        include=""
                        define=""
                        explode=line.split("#")
                        if len(explode)>=2:
                            include=explode[1]
                        if len(explode)==3:
                            define=explode[2]
                        # append to the list
                        self.libinstructions.append([instruction,cnvinstruction,include,define])
                        #regex = re.compile(r"(^|[' ']|['=']|['{']|[',']|[\t]|['(']|['!'])"+str(instruction))+"[ ]*\(")
                        #regex = re.compile(r"(^|[' ']|['=']|['{']|[',']|[\t]|['(']|['!'])"+str(instruction)+r"([' ']|['=']|['}']|[',']|[';']|[\t]|[')'].*)")
                        #regex = re.compile(r"(^|[' ']|['=']|['{']|[',']|[\t]|['(']|['!'])"+str(instruction)+".*")
                        #regex = re.compile(r'\W%s\W' % re.escape(str(instruction)))
                        regex = re.compile(r"(^|[' ']|['=']|['{']|[',']|[\t]|['(']|['!'])%s\W" % re.escape(str(instruction)))
                        self.regobject.append(regex)
                libfile.close()
        # clean up the keyword list
        self.keywordList.sort()
        self.keywordList = self.ClearRedundancy(self.keywordList)
        self.keywordNum = len(self.keywordList)
        # make reserved words list
        if len(self.libinstructions)!=0:
            for i in range(len(self.libinstructions)):
                chaine=self.libinstructions[i][0]
                self.rw.append(chaine)
                pos = chaine.find(".")
                if pos != -1:
                    self.reservedword.append(chaine[0:pos])
                    self.reservedword.append(chaine[pos+1:len(chaine)])
                else:
                    self.reservedword.append(chaine)
            # sort keywords for short key help
            self.rw.sort(key=lambda x: x.lower())
        # adding fixed reserved word
        fixed_rw=("setup","loop","HIGH","LOW","INPUT","OUTPUT","void","FOSC","MIPS","ON","OFF","TRUE","FALSE")
        for i in range(len(fixed_rw)):
            self.reservedword.append(fixed_rw[i])

        #if gui==True: # or AttributeError: 'Pinguino' object has no attribute 'extraName'
            #self.displaymsg(_("Board config")+":\t"+textStatus, 0, force_update=False)
        #else:
        self.displaymsg(_("Board config")+":\t"+textStatus, 0, force_update=False)	    

        self.changingBoard = False	

    
    #----------------------------------------------------------------------
    def ClearRedundancy(self, myList):
        seen = set()
        out = []
        for item in myList:
            if item not in seen:
                seen.add(item)
                out.append(item)
        return out


    #----------------------------------------------------------------------
    def preprocess(self, filename, board):
        """Read Pinguino File (.pde) and translate it into C language"""
        
        defineword={}
        index=0
        fileline={}

        # delete old define.h and create a new one
        if os.path.exists(os.path.join(SOURCE_DIR, 'define.h')):
            os.remove(os.path.join(SOURCE_DIR, 'define.h'))
        fichier=open(os.path.join(SOURCE_DIR, 'define.h'), 'a')
        fichier.close()

        # rename .pde in user.c
        path,name = os.path.split(filename)
        shutil.copy(filename + '.pde', os.path.join(SOURCE_DIR, 'user.c'))
        fichier = open(os.path.join(SOURCE_DIR, 'user.c'), 'a')
        #fichier.writelines("\n")
        fichier.close()

        ### debug mode
        ##DEBUG_STR = "NODEBUG"
        ##if self.debug == self.ID_USBCDC:
            ##DEBUG_STR = "USBCDCDEBUG"
        ##if self.debug == self.ID_USBBULK:
            ##DEBUG_STR = "USBBULKDEBUG"
        ##if self.debug == self.ID_USBHID:
            ##DEBUG_STR = "USBHIDDEBUG"
        ##if self.debug == self.ID_USBOTG:
            ##DEBUG_STR = "USBOTGDEBUG"
        ##if self.debug == self.ID_UART1:
            ##DEBUG_STR = "UART1DEBUG"
        ##if self.debug == self.ID_UART2:
            ##DEBUG_STR = "UART2DEBUG"
        ##self.adddefine("#define DEBUG_MODE " + DEBUG_STR)

        # board
        #if board.arch == 8:
        #    self.adddefine("#define " + board.board)

        # add #include and #define from user.c to define.h
        fichier = open(os.path.join(SOURCE_DIR, 'user.c'), 'r')
        i=0
        for line in fichier:
            if line.find("#include")!=-1 or line.find("#define")!=-1:
                line = line[:line.find('//')]   # Ignores C++ comments, fixing Issue 11
                self.adddefine(line)    # add to define.h
                fileline[i]="\r\n";    # delete from user.c
                i=i+1
            else:
                fileline[i]=line
                i=i+1
        fichier.close()

        # rewrite file user.c without #include and #define
        fichier = open(os.path.join(SOURCE_DIR, 'user.c'), 'w')
        for cpt in range(i):
            fichier.write(fileline[cpt])
        fichier.close()

        # search and replace arduino keywords in file
        fichier = open(os.path.join(SOURCE_DIR, 'user.c'), 'r')
        content = fichier.read()
        content = self.removecomments(content)
        content = content.split('\n')
        nblines = 0
        for line in content:
            resultline = self.replaceword(line)
            if resultline.find("error")==1:
                line = resultline
                print "error " + resultline
                self.displaymsg("error "+resultline,1)
                return "error"
            fileline[nblines] = resultline
            nblines += 1
        fichier.close()

        # save new tmp file
        fichier = open(os.path.join(SOURCE_DIR, 'user.c'), 'w')
        for i in range(0,nblines):
            fichier.writelines(fileline[i])
        fichier.writelines("\r\n")
        fichier.close()

        # sort define.h
        fichier = open(os.path.join(SOURCE_DIR, 'define.h'), "r")
        lignes = fichier.readlines()
        lignes.sort()
        fichier.close()

        # save sorted lines
        fichier = open(os.path.join(SOURCE_DIR, 'define.h'), "w")
        fichier.writelines(lignes)
        fichier.close()
        return


    #----------------------------------------------------------------------
    def adddefine(self,chaine):
        """ add #define in define.h file """
        fichier = open(os.path.join(SOURCE_DIR, 'define.h'), 'a')
        fichier.writelines(chaine+"\n")
        fichier.close()

    #----------------------------------------------------------------------
    def notindefine(self,chaine):
        """ verify if #define exists in define.h file """
        fichier = open(os.path.join(SOURCE_DIR, 'define.h'), 'r')
        for line in fichier:
            # chaine has been found ?
            if line.find(chaine)!=-1:
                fichier.close()
                return(0)
        fichier.close()
        return(1)

    #----------------------------------------------------------------------
    def replaceword(self,line):
        """ convert pinguino language in C language """
        # replace arduino/pinguino language and add #define or #include to define.h
        for i in range(len(self.libinstructions)):
            if re.search(self.regobject[i], line):
                line = line.replace(str(self.libinstructions[i][0]), str(self.libinstructions[i][1]))
                if self.notindefine("#"+str(self.libinstructions[i][2])) == 1:
                    self.adddefine("#"+str(self.libinstructions[i][2]))
                if self.notindefine("#"+str(self.libinstructions[i][3])) == 1:
                    self.adddefine("#"+str(self.libinstructions[i][3]))
        return line+"\n"


    #----------------------------------------------------------------------
    def removecomments(self, text):
        def replacer(match):
            s = match.group(0)

            if s.startswith('/'):
                #return "" #bug in line number in error info, multiline comments
                return "" + "\n" * (s.count("\n"))
            
            else:
                return s
            
        pattern = re.compile(
            r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
            re.DOTALL | re.MULTILINE
        )
        return re.sub(pattern, replacer, text)


    #----------------------------------------------------------------------
    def compile(self, filename, board):
        """ Compile.
        
        NB :  "--opt-code-size"   deprecated
              "--use-non-free"    implicit -I and -L options for non-free headers and libs
        """

        if board.arch == 8:
            
            fichier = open(os.path.join(SOURCE_DIR, 'stdout'), 'w+')
            if board.bldr == 'boot2':
                sortie = Popen([os.path.join(self.P8_BIN_DIR, self.c8),\
                    "--verbose",\
                    "-mpic16",\
                    "--denable-peeps",\
                    "--obanksel=9",\
                    "--optimize-cmp",\
                    "--optimize-df",\
                    "-p" + board.proc,\
                    "-D" + board.board,\
                    "-D" + board.bldr,\
                    "-I" + os.path.join(P8_DIR, 'sdcc', 'include', 'pic16'),\
                    "-I" + os.path.join(P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                    "-I" + os.path.join(P8_DIR, 'pinguino', 'core'),\
                    "-I" + os.path.join(P8_DIR, 'pinguino', 'libraries'),\
                    "-I" + os.path.dirname(filename),\
                    "--compile-only",\
                    "-o" + os.path.join(SOURCE_DIR, 'main.o'),\
                    os.path.join(SOURCE_DIR, 'main.c')],\
                    stdout=fichier, stderr=STDOUT)
                
                               
            elif board.bldr == 'boot4':
                #"--nostdinc",\
                #"--nostdlib",\
                sortie = Popen([os.path.join(self.P8_BIN_DIR, self.c8),\
                    "--verbose",\
                    "-mpic16",\
                    "--denable-peeps",\
                    "--obanksel=9",\
                    "--optimize-cmp",\
                    "--optimize-df",\
                    # Do not remove --ivt-loc option
                    "--ivt-loc=" + str(board.memstart),\
                    "-p" + board.proc,\
                    "-D" + board.board,\
                    "-D" + board.bldr,\
                    "-I" + os.path.join(P8_DIR, 'sdcc', 'include', 'pic16'),\
                    "-I" + os.path.join(P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                    "-I" + os.path.join(P8_DIR, 'pinguino', 'core'),\
                    "-I" + os.path.join(P8_DIR, 'pinguino', 'libraries'),\
                    "-I" + os.path.dirname(filename),\
                    "--compile-only",\
                    os.path.join(SOURCE_DIR, 'main.c'),\
                    "-o" + os.path.join(SOURCE_DIR, 'main.o')],\
                    stdout=fichier, stderr=STDOUT)
                               
            elif board.bldr == 'noboot':
                sortie = Popen([os.path.join(self.P8_BIN_DIR, self.c8),\
                    "--verbose",\
                    "-mpic16",\
                    "--denable-peeps",\
                    "--obanksel=9",\
                    "--optimize-cmp",\
                    "--optimize-df",\
                    "-p" + board.proc,\
                    "-D" + board.board,\
                    "-D" + board.bldr,\
                    "-I" + os.path.join(P8_DIR, 'sdcc', 'include', 'pic16'),\
                    "-I" + os.path.join(P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                    "-I" + os.path.join(P8_DIR, 'pinguino', 'core'),\
                    "-I" + os.path.join(P8_DIR, 'pinguino', 'libraries'),\
                    "-I" + os.path.dirname(filename),\
                    "--compile-only",\
                    os.path.join(SOURCE_DIR, 'main.c'),\
                    "-o" + os.path.join(SOURCE_DIR, 'main.o')],\
                    stdout=fichier, stderr=STDOUT)
                               
            sortie.communicate()
            if sortie.poll()!=0:
                #
                # Error treatment (RB: fixed 2012-11-15)
                #
                
                # set the file pointer to the beginning of stdout
                fichier.seek(0)

                # read lines until 'error' or 'Error' is found
                for ligne in fichier:
                    # ???
                    #if not "stcpage" in globals():
                    #    break
                    #if (gui == 0):
                        #break

                    # C errors
                    error_pos = ligne.find('error')
                    if (error_pos != -1):
                        #error_line_number = ligne[ ligne.find(":") + 1 : error_pos - 2]
                        if os.name == "nt": err = 2
                        else: err = 1
                        error_line_number = ligne.split(":")[err]
                        #error_message = ligne[ error_pos + 9 : len(ligne)]
                        error_message = ligne.split(":")[-1]
                        error_color = self.getColorConfig("Highligh", "codeerror", [120, 255, 152])
                        # TODO : highlight more than one line / highlight must remain until line is changed
                        if gui: self.highlightline(int(error_line_number)-1, error_color)
                        self.displaymsg("error line " + error_line_number + " " + error_message, 0)
                        
                    # ASM errors
                    error_pos = ligne.find('Error')
                    if (error_pos != -1):
                        # do not display error line number since they are from the ASM file
                        # display error symbol instead
                        error_symbol = ligne[ligne.find("(_") + 2 : ligne.find(").")]
                        textEdit  = getCurrentPage()
                        textPlain = str(textEdit.GetTextUTF8())
                        error_index = textPlain.find(error_symbol)
                        textEdit.SetSelection(error_index, error_index + len(error_symbol))
                        error_line_number = textEdit.LineFromPosition(error_index) + 1
                        error_message = ligne[ error_pos + 13 : ligne.find("(_") - 1]
                        error_color = self.getColorConfig("Highligh", "codeerror", [120, 255, 152])
                        self.highlightline(int(error_line_number)-1, error_color)
                        self.displaymsg("error line " + str(error_line_number) + ": " + error_message.lower() + ": " + error_symbol, 0)

            fichier.close()
            return sortie.poll()

        else:# if board.arch == 32:
            return 0

    #----------------------------------------------------------------------
    def link(self, filename, board):
        """Link.
        
        NB :  "--opt-code-size"   deprecated
              "--use-non-free"    implicit -I and -L options for non-free headers and libs"""
        
        fichier = open(os.path.join(SOURCE_DIR, 'stdout'), 'w+')
        
        if board.arch == 8:
            
            if board.bldr == 'boot2':
                sortie = Popen([os.path.join(self.P8_BIN_DIR, self.c8),\
                    "--verbose",\
                    "-mpic16",\
                    "--denable-peeps",\
                    "--obanksel=9",\
                    "--optimize-cmp",\
                    "--optimize-df",\
                    "--no-crt",\
                    "-Wl-s" + os.path.join(P8_DIR, 'lkr', board.bldr + '.' + board.proc + '.lkr') + ",-m",\
                    "-p" + board.proc,\
                    "-D" + board.bldr,\
                    "-D" + board.board,\
                    "-I" + os.path.join(P8_DIR, 'sdcc', 'include', 'pic16'),\
                    "-I" + os.path.join(P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                    "-I" + os.path.join(P8_DIR, 'pinguino', 'core'),\
                    "-I" + os.path.join(P8_DIR, 'pinguino', 'libraries'),\
                    "-L" + os.path.join(P8_DIR, 'sdcc', 'lib', 'pic16'),\
                    "-L" + os.path.join(P8_DIR, 'sdcc', 'non-free', 'lib', 'pic16'),\
                    'libio' + board.proc + '.lib',\
                    'libdev' + board.proc + '.lib',\
                    'libc18f.lib',\
                    'libm18f.lib',\
                    'libsdcc.lib',\
                    #'-llibio' + board.proc + '.lib',\
                    #'-llibc18f.lib',\
                    #'-llibm18f.lib',\
                    #'-llibsdcc.lib',\
                    "-o" + os.path.join(SOURCE_DIR, 'main.hex'),\
                    os.path.join(P8_DIR, 'obj', 'application_iface.o'),\
                    os.path.join(P8_DIR, 'obj', 'boot_iface.o'),\
                    os.path.join(P8_DIR, 'obj', 'usb_descriptors.o'),\
                    os.path.join(P8_DIR, 'obj', 'crt0ipinguino.o'),\
                    os.path.join(SOURCE_DIR, 'main.o')],\
                    stdout=fichier, stderr=STDOUT)
                    
            elif board.bldr == 'boot4':
                sortie = Popen([os.path.join(self.P8_BIN_DIR, self.c8),\
                    "--verbose", "-V",\
                    "-mpic16",\
                    # optimization
                    "--denable-peeps",\
                    "--obanksel=9",\
                    "--optimize-cmp",\
                    "--optimize-df",\
                    # don't want to link default crt0i.o but crt0i.c
                    "--no-crt",\
                    # move all int. vectors after bootloader code
                    "--ivt-loc=" + str(board.memstart),\
                    # link memory map
                    "-Wl-s" + os.path.join(P8_DIR, 'lkr', board.bldr + '.' + board.proc + '.lkr') + ",-m",\
                    "-p" + board.proc,\
                    "-D" + board.bldr,\
                    "-D" + board.board,\
                    "-I" + os.path.join(P8_DIR, 'sdcc', 'include', 'pic16'),\
                    "-I" + os.path.join(P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                    "-I" + os.path.join(P8_DIR, 'pinguino', 'core'),\
                    "-I" + os.path.join(P8_DIR, 'pinguino', 'libraries'),\
                    "-L" + os.path.join(P8_DIR, 'sdcc', 'lib', 'pic16'),\
                    "-L" + os.path.join(P8_DIR, 'sdcc', 'non-free', 'lib', 'pic16'),\
                    os.path.join(SOURCE_DIR, 'main.o'),\
                    #'libio' + board.proc + '.lib',\
                    'libdev' + board.proc + '.lib',\
                    'libc18f.lib',\
                    'libm18f.lib',\
                    # link the default run-time module (crt0i.o)
                    # except when "-no-crt" option is used
                    'libsdcc.lib',\
                    #'-llibio' + board.proc + '.lib',\
                    #'-llibc18f.lib',\
                    #'-llibm18f.lib',\
                    #'-llibsdcc.lib',\
                    "-o" + os.path.join(SOURCE_DIR, 'main.hex'),\
                    ],\
                    stdout=fichier, stderr=STDOUT)
                    
            elif board.bldr == 'noboot':
                sortie = Popen([os.path.join(self.P8_BIN_DIR, self.c8),\
                    "--verbose",\
                    "-mpic16",\
                    "--denable-peeps",\
                    "--obanksel=9",\
                    "--optimize-cmp",\
                    "--optimize-df",\
                    #"--no-crt",\ we use default run-time module inside libsdcc.lib
                    "-Wl-s" + os.path.join(P8_DIR, 'lkr', board.proc + '_g.lkr') + ",-m",\
                    "-p" + board.proc,\
                    "-D" + board.bldr,\
                    "-D" + board.board,\
                    "-I" + os.path.join(P8_DIR, 'sdcc', 'include', 'pic16'),\
                    "-I" + os.path.join(P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                    "-I" + os.path.join(P8_DIR, 'pinguino', 'core'),\
                    "-I" + os.path.join(P8_DIR, 'pinguino', 'libraries'),\
                    "-L" + os.path.join(P8_DIR, 'sdcc', 'lib', 'pic16'),\
                    "-L" + os.path.join(P8_DIR, 'sdcc', 'non-free', 'lib', 'pic16'),\
                    'libio' + board.proc + '.lib',\
                    'libdev' + board.proc + '.lib',\
                    'libc18f.lib',\
                    'libm18f.lib',\
                    # link the default run-time module
                    'libsdcc.lib',\
                    #'-llibio' + board.proc + '.lib',\
                    #'-llibc18f.lib',\
                    #'-llibm18f.lib',\
                    #'-llibsdcc.lib',\
                    "-o" + os.path.join(SOURCE_DIR, 'main.hex'),\
                    os.path.join(SOURCE_DIR, 'main.o')],\
                    stdout=fichier, stderr=STDOUT)
                    
        else:#if board.arch == 32:
            # "PDEDIR=" + os.path.dirname(self.GetPath()),\
            # can't be used with Command Line version since editor isn't used
            sortie=Popen([self.make,\
                          "--makefile=" + os.path.join(SOURCE_DIR, 'Makefile32.'+self.osdir),\
                          "HOME=" + HOME_DIR,\
                          "PDEDIR=" + os.path.dirname(filename),\
                          "PROC=" + board.proc,\
                          "BOARD=" + board.board],\
                         stdout=fichier, stderr=STDOUT)
                         
        sortie.communicate()
        fichier.seek(0)
        # Check if child process has terminated
        if sortie.poll()!=0:
            for ligne in fichier:
                if ligne.find('error')!=-1:
                    self.displaymsg(ligne, 0)
        fichier.close()
        if sys.platform=='win32':
            if board.board in ['PIC32_PINGUINO_220', 'GENERIC32MX250F128', 'GENERIC32MX220F032']:
                badrecord=":040000059D0040001A\n"
            else:
                badrecord=":040000059D006000FA\n"                
            if os.path.exists(os.path.join(SOURCE_DIR,"main32tmp.hex")):
                fichiersource=open(os.path.join(SOURCE_DIR,"main32tmp.hex"),'r')
                fichierdest=open(os.path.join(SOURCE_DIR,"main32.hex"),'w+')
                for line in fichiersource:
                    if line!=badrecord:
                        fichierdest.writelines(line)
                fichiersource.close()
                fichierdest.close()
                os.remove(os.path.join(SOURCE_DIR,"main32tmp.hex"))
        return sortie.poll()


    #----------------------------------------------------------------------
    def getCodeSize(self, filename, board):
        codesize = 0
        address_Hi = 0
        memfree = board.memend - board.memstart
        fichier = open(filename + ".hex", 'r')
        lines = fichier.readlines()
        for line in lines:
            byte_count = int(line[1:3], 16)
            address_Lo = int(line[3:7], 16)
            record_type= int(line[7:9], 16)
            # extended linear address record
            if record_type == 4:
                address_Hi = int(line[9:13], 16) << 16
                        # address calculation
            address = (address_Hi << 16) + address_Lo
            # code size
            if record_type == 0:
                if address >= board.memstart:
                    codesize = codesize + byte_count
        fichier.close()
        return _("code size: ") + str(codesize) + " / " + str(memfree) + " " + _("bytes") + " (" + str(100*codesize/memfree) + "% " +_("used")+ ")" 



# ------------------------------------------------------------------------------
# getOptions
# ------------------------------------------------------------------------------
def getOptions():
    parser = argparse.ArgumentParser(description='*** Pinguino IDE ***')
    parser.add_argument('-v', '--version', dest='version', action='store_true', default=False, help='show Pinguino IDE version and exit')
    parser.add_argument('-a', '--author', dest='author', action='store_true', default=False, help='show authors of this Pinguino IDE version and exit')
    parser.add_argument('-f', '--filename', dest='filename', nargs=1, default=False, help='filename to process')
    parser.add_argument('-dv', '--dev', dest='dev', nargs=1, default=False, help='set developer mode')
    parser.add_argument('-ul', '--upload', dest='upload', const=True, action='store_const', default=False, help='upload code')
    parser.add_argument('-bt', '--boot', dest='bootloader', nargs=1, default=False, help='set bootloader option')
    
    for b in range(len(boardlist)):
        parser.add_argument(    boardlist[b].shortarg,
                                boardlist[b].longarg,
                                dest='board',
                                const=b,
                                action='store_const',
                                default=False,
                                help='compile code for ' + boardlist[b].board + ' board')
    return parser.parse_args()

# ------------------------------------------------------------------------------
# getVersion
# ------------------------------------------------------------------------------
def getVersion():
    return pinguino_version

#----------------------------------------------------------------------
def setGui(bool):
    global gui
    gui=bool
