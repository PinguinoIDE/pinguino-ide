#!/usr/bin/env python
#-*- coding: utf-8 -*-

#----------------------------------------------------------------------
def setTab(name):
    global TAB
    TAB = name
    
#----------------------------------------------------------------------
def setDic(dic):
    global set_
    set_ = dic
    
#----------------------------------------------------------------------
def addOper2(name):
    set_[name] = ["output", TAB]
    set_[name].append(["syntax", "("])
    set_[name].append(["space"])
    set_[name].append(["label", name])
    set_[name].append(["space"])
    set_[name].append(["syntax", ")"])

#----------------------------------------------------------------------
def addOper1(name, space="space"):
    set_[name] = ["output", TAB]
    set_[name].append(["label", name])
    set_[name].append(["syntax", "("])
    set_[name].append([space])
    set_[name].append(["syntax", ")"])    

#----------------------------------------------------------------------
def addOper2_bool(name, space="space"):
    set_["".join(name)] = ["output-bool", TAB]
    set_["".join(name)].append(["syntax", "("])
    set_["".join(name)].append([space])
    if type(name) == type([]):
        set_["".join(name)].append(["syntax", name[0]])
        set_["".join(name)].append(["label", name[1]])
        set_["".join(name)].append(["syntax", name[2]])
    else: set_["".join(name)].append(["label", name])
    
    set_["".join(name)].append([space])
    set_["".join(name)].append(["syntax", ")"])

#----------------------------------------------------------------------
def addOper1_bool(name, space="space"):
    set_["".join(name)] = ["output-bool", TAB]
    if type(name) == type([]):
        set_["".join(name)].append(["syntax", name[0]])
        set_["".join(name)].append(["label", name[1]])
        set_["".join(name)].append(["syntax", name[2]])
    else: set_["".join(name)].append(["label", name])
    set_["".join(name)].append(["syntax", "("])
    set_["".join(name)].append([space])
    set_["".join(name)].append(["syntax", ")"])
    
#----------------------------------------------------------------------
def addNewFunct(name, nvars, tipo="linear", bools=[], full=[]):
    set_[name] = [tipo, TAB]
    set_[name].append(["label", name])
    set_[name].append(["syntax", "("])
    
    for i in range(nvars-1):
        if i in bools: set_[name].append(["space-bool"])
        else: set_[name].append(["space"])
        set_[name].append(["syntax", ","])
    if nvars > 0: set_[name].append(["space"])    
    set_[name].append(["syntax", ")"])
    set_[name].append(["full", full])
    
    
    
    
#----------------------------------------------------------------------
def addNewSentence(name, space=False, tipo="linear", extra=""):
    set_[name+extra] = [tipo, TAB]
    set_[name+extra].append(["label", name])
    set_[name+extra].append(["syntax", " "])
    if space: set_[name+extra].append(["space"])
    
    
    
########################################################################
   
ID = 0
#----------------------------------------------------------------------
def newId():
    global ID
    ID += 1
    return "kit" + str(ID)
    


        
            
#########################################################################
#class Block:
    #""""""
    ##----------------------------------------------------------------------
    #def __init__(self):
        #"""Constructor"""
        #self.blocks = []
    
      
    ##----------------------------------------------------------------------
    #def new_block(self, func):
        #""""""
        ##func
        #blocks.append(func.__name__)
        #return func
                  
                  



########################################################################
class Kit(object):
    
    #----------------------------------------------------------------------
    
    def __init__(self):
        """constructor"""

    #----------------------------------------------------------------------
    def addLabel(self, str_label):
        self.bloque.append(["label", str_label])
        
    #----------------------------------------------------------------------
    def addSpace(self):
        self.bloque.append(["space"])
    #----------------------------------------------------------------------
    def addSpaceBool(self):
        self.bloque.append(["space_bool"])
        
    #----------------------------------------------------------------------
    def addSyntax(self, str_syntax):
        self.bloque.append(["syntax", str_syntax])
        
    #----------------------------------------------------------------------
    def addButton(self, str_label, checkeable=False, checked=False):
        self.bloque.append(["button", str_label, checkeable, checked])
        
    #----------------------------------------------------------------------
    def addSpinChoice(self, str_default, list_choices):
        self.bloque.append(["spin_choice", str_default, list_choices])
        
    #----------------------------------------------------------------------
    def addEdit(self, str_default, background="white", foreground=(90, 90, 90)):
        self.bloque.append(["edit", str_default, background, foreground])
        
    #----------------------------------------------------------------------
    def addSpinInt(self, start=0, min_=-100, max_=100):
        self.bloque.append(["spin_int", start, min_, max_])
        
    #----------------------------------------------------------------------
    def addSpinFloat(self, start=0.1, step=0.1, min_=-100, max_=100):
        self.bloque.append(["spin_float", start, step, min_, max_])
        
    #----------------------------------------------------------------------
    def addSlider(self, start=0, min_=0, max_=1023):
        self.bloque.append(["slider", start, min_, max_])
        
    #----------------------------------------------------------------------
    def addDecorator(self, str_label):
        self.bloque.append(["decorator", str_label])
        
    #----------------------------------------------------------------------
    def addContextHelp(self, str_help):
        self.bloque.append(["help", str_help])
        
    #----------------------------------------------------------------------
    def addCode(self, code_pinguino, code_python=""):
        self.bloque.append(["code", {"pinguino": code_pinguino, "python": code_python}])
    
    #----------------------------------------------------------------------
    def fillWith(self, list_name_blocks):
        self.bloque.append(["full", list_name_blocks])        
    
    #----------------------------------------------------------------------
    def getBlock(self):
        return self.bloque
    
    #----------------------------------------------------------------------
    def getType(self, t, tab):
        if t == "linear":return(Linear(tab))
        elif t == "output":return(Output(tab))
        elif t == "output-bool":return(OutputBool(tab))
        elif t == "function":return(Function(tab))
    

    #----------------------------------------------------------------------
    def addFunct(self, tab, name, nvars, t="linear", fill=[]):
        
        pm = self.getType(t, tab)
        
        pm.addLabel(name)
        pm.addSyntax("(")
        
        for i in range(nvars):
            pm.addSpace()
            if not i == nvars - 1: pm.addSyntax(",")
        pm.addSyntax(")")
        
        if fill: pm.fillWith(fill)
        
        return pm.getBlock()                      
                      
                 
    #----------------------------------------------------------------------
    def addOper(self, tab, label, form="output", spaces=[]):
        
        pm = self.getType(form, tab)
        
        trad = False
        if label.startswith("@") and label.endswith("@"):
            trad = True
            label = label[1:-1]
        
        if len(spaces) == 1:
            if trad: pm.addSyntax("@")
            pm.addLabel(label)
            if trad: pm.addSyntax("@")
            pm.addSyntax("(")
            if spaces[0] == "space": pm.addSpace()
            elif spaces[0] == "space_bool": pm.addSpaceBool()
            pm.addSyntax(")")
        elif len(spaces) == 2:
            pm.addSyntax("(")
            if spaces[0] == "space": pm.addSpace()
            elif spaces[0] == "space_bool": pm.addSpaceBool()
            if trad: pm.addSyntax("@")
            pm.addLabel(label) 
            if trad: pm.addSyntax("@")
            if spaces[1] == "space": pm.addSpace()
            elif spaces[1] == "space_bool": pm.addSpaceBool()
            pm.addSyntax(")")
            
        return pm.getBlock()
        



    
class Linear(Kit):
    def __init__(self, tab): self.bloque = ["linear", tab]
            
class Output(Kit):
    def __init__(self, tab): self.bloque = ["output", tab]
            
class OutputBool(Kit):
    def __init__(self, tab): self.bloque = ["output-bool", tab]
            
class Function(Kit):
    def __init__(self, tab): self.bloque = ["function", tab]
            
class Nested(Kit):
    def __init__(self, tab): self.bloque = ["nested", tab]
    
class NestedNext(Kit):
    def __init__(self, tab): self.bloque = ["nested-second", tab]
    
class FrameText(Kit):
    def __init__(self, tab): self.bloque = ["frame-edit", tab]

class FramePlot(Kit):
    def __init__(self, tab): self.bloque = ["frame-plot", tab]
    
class FrameDebug(Kit):
    def __init__(self, tab): self.bloque = ["frame-debug", tab]


