#!/usr/bin/env python
#-*- coding: utf-8 -*-

"""-------------------------------------------------------------------------
    USB debugger output.

    author:		Yeison Cardona
    contact:		yeison.eng@gmail.com 
    first release:	03/March/2012
    last release:	03/July/2012

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
-------------------------------------------------------------------------"""

import wx, serial, threading, sys, time
from wxgui._trad import _

########################################################################
class Debugger:
    #----------------------------------------------------------------------
    def __initDebugger__(self):

        self.history = []
        self.historyIndex = 0

        if sys.platform == 'darwin':
            """"""
            #self.CDC_NAME_PORT = "???" 

        elif sys.platform == 'win32':
            self.CDC_NAME_PORT = "COM%d"
        else:
            self.CDC_NAME_PORT = "/dev/ttyACM%d"
            

    #----------------------------------------------------------------------
    def updateCDCPorts(self, max_ports=20):
        list_ports = []
        for i in range(max_ports):
            try:
                port = serial.Serial(self.CDC_NAME_PORT %i)
                port.close()
                list_ports.append(self.CDC_NAME_PORT %i)
            except: pass
            
        self.choicePort.Enable()
        self.choicePort.Clear()
        self.choicePort.AppendItems(list_ports)
        self.choicePort.SetSelection(len(list_ports))
        
        if len(list_ports) == 0: self.choicePort.AppendItems([_("No ports")])

        if len(list_ports) <= 1:
            self.choicePort.SetSelection(0)
            self.choicePort.Disable()
            
        return list_ports


    #----------------------------------------------------------------------
    def setCDCPort(self,name):
        def connect(event=None):
            self.pinguinoCDC = serial.Serial(name, timeout=1)
            self.logwindow.WriteText(_("Connected")+": "+name+"\n")
        return connect    

    #----------------------------------------------------------------------
    def setDebugger(self, event=None, mode=None):
        if mode == None:
            self.debuggingLine.Hide()
            self.buttonSendDebug.Hide()
            self.choicePort.Hide()
            self.updateIDE()
            return 

        self.logwindow.Clear()
        self.choicePort.Show()
        self.debuggingLine.Show()
        self.buttonSendDebug.Show()
        self.debuggingLine.SetFocus()
        self.debuggingLine.SetSelection(3, 3)
        self.updateIDE()        

        try: self.pinguinoCDC.close()
        except: pass

        self.debuggingTimer(mode)


    #----------------------------------------------------------------------
    def debuggingTimer(self, mode):
        self.updateDebggingLog(_("Debugging!")+"\n")
        if mode == "CDC": self.debugCDC()

    #----------------------------------------------------------------------
    def updateDebggingLog(self, message):
        if message == None:
            self.setDebugger(mode=None)
            return 
        if len(message) > 0:
            self.logwindow.SetInsertionPoint(len(self.logwindow.Value))
            if self.logwindow.GetLineText(self.logwindow.NumberOfLines) != "": self.logwindow.WriteText("\n")
            self.logwindow.WriteText(message)


    #----------------------------------------------------------------------
    def sendLine(self, event=None):
        line = str(self.debuggingLine.Value)
        #if line.startswith(">>>"): line = line.replace(">>>", "")
        self.debugOutMessage = line
        self.logwindow.SetInsertionPoint(len(self.logwindow.Value))
        if self.logwindow.GetLineText(self.logwindow.NumberOfLines) != "": self.logwindow.WriteText("\n")
        self.logwindow.WriteText(line)
        self.debuggingLine.Clear()
        #self.debuggingLine.SetValue(">>>")
        self.debuggingLine.SetInsertionPoint(3)
        self.history.append(line)
        self.historyIndex = 0

    #----------------------------------------------------------------------
    def sendDebugging(self, event):
        if event.GetKeyCode() in [wx.WXK_NUMPAD_ENTER,
                                  wx.WXK_RETURN]:
            self.sendLine()

        elif event.GetKeyCode() == wx.WXK_UP and len(self.history) > 0:
            self.historyIndex += 1
            if self.historyIndex > len(self.history): self.historyIndex = 0
            self.debuggingLine.Clear()
            self.debuggingLine.SetValue(self.history[-self.historyIndex])
            self.debuggingLine.SetInsertionPoint(len(self.debuggingLine.Value))            

        elif event.GetKeyCode() == wx.WXK_DOWN and len(self.history) > 0:
            self.historyIndex -= 1
            if self.historyIndex < 0: self.historyIndex = len(self.history)
            self.debuggingLine.Clear()
            self.debuggingLine.SetValue(self.history[-self.historyIndex])
            self.debuggingLine.SetInsertionPoint(len(self.debuggingLine.Value))

                

    #----------------------------------------------------------------------
    def changeCDCPort(self, event):
        port = event.String
        try: self.pinguinoCDC.close()
        except: pass
        try:
            self.pinguinoCDC = serial.Serial(port, timeout=1)
            self.logwindow.WriteText(_("Connected")+": "+port+"\n")
        except:
            self.logwindow.WriteText(_("No device connected")+"!\n")
            

    #----------------------------------------------------------------------
    def debugCDC(self):
        ports = self.updateCDCPorts()
        
        if len(ports) > 0:
            self.pinguinoCDC = serial.Serial(ports[0], timeout=0.01)
            self.updateDebggingLog(_("Connected")+": "+ports[0]+"\n")

        if len(ports) > 0:
            self.timer_CDC = wx.CallLater(10, self.CDCTimer)  
            
        elif not self.menu.menuItemDebugNone.IsChecked():
            wx.CallLater(250, self.debugCDC)
                           
    #----------------------------------------------------------------------
    def CDCTimer(self, event=None):
        try:
            if self.debugOutMessage != None:
                self.pinguinoCDC.write(self.debugOutMessage)
                self.debugOutMessage = None
            else:
                line = self.pinguinoCDC.readall()
                self.updateDebggingLog(line)

        except UnboundLocalError:
            self.logwindow.WriteText(_("No device connected")+"!")
            self.menu.menuItemDebugNone.Check()
            self.updateDebggingLog(None)      

        except serial.serialutil.SerialException:
            self.logwindow.WriteText("\n"+_("device disconnected")+"!\n")
            self.debugCDC()          
            return
        except:
            pass

        if self.closing or self.menu.menuItemDebugNone.IsChecked(): self.pinguinoCDC.close()
        else:  self.timer_CDC.Restart(10)
