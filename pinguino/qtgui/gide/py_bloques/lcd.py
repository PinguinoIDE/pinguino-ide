#!/usr/bin/env python
#-*- coding: utf-8 -*-

from . import constructor as Block
from .constructor import Kit

Kit = Kit()

########################################################################
class LCD(object):

    LCD_noAutoScroll = lambda n:Kit.addFunct("LCD", "lcd.noAutoscroll", 0)
    LCD_autoscroll = lambda n:Kit.addFunct("LCD", "lcd.autoscroll", 0)
    LCD_rightToLeft = lambda n:Kit.addFunct("LCD", "lcd.rightToLeft", 0)
    LCD_leftToRight = lambda n:Kit.addFunct("LCD", "lcd.leftToRight", 0)
    LCD_scrollDisplayRight = lambda n:Kit.addFunct("LCD", "lcd.scrollDisplayRight", 0)
    LCD_scrollDisplayLeft = lambda n:Kit.addFunct("LCD", "lcd.scrollDisplayLeft", 0)

    LCD_blink = lambda n:Kit.addFunct("LCD", "lcd.blink", 0)
    LCD_noBlink = lambda n:Kit.addFunct("LCD", "lcd.noBlink", 0)
    LCD_cursor = lambda n:Kit.addFunct("LCD", "lcd.cursor", 0)
    LCD_noCursor = lambda n:Kit.addFunct("LCD", "lcd.noCursor", 0)
    LCD_display = lambda n:Kit.addFunct("LCD", "lcd.display", 0)
    LCD_noDisplay = lambda n:Kit.addFunct("LCD", "lcd.noDisplay", 0)
    LCD_clear = lambda n:Kit.addFunct("LCD", "lcd.clear", 0)
    LCD_home = lambda n:Kit.addFunct("LCD", "lcd.home", 0)

    LCD_printFloat = lambda n:Kit.addFunct("LCD", "lcd.printFloat", 1, fill=["spin-float"])
    LCD_print = lambda n:Kit.addFunct("LCD", "lcd.print", 1, fill=["edit"])
    LCD_printf = lambda n:Kit.addFunct("LCD", "lcd.printf", 2, fill=["edit", "more-args"])

    LCD_command = lambda n:Kit.addFunct("LCD", "lcd.command", 1)

    LCD_write = lambda n:Kit.addFunct("LCD", "lcd.write", 1, fill=["edit_c"])
    LCD_write = lambda n:Kit.addFunct("LCD", "lcd.write", 1, fill=["edit_c"])


    #----------------------------------------------------------------------
    def LCD_pins_8bit(self):

        pm = Block.Linear("LCD")
        pm.addLabel("lcd.pins")
        pm.addDecorator(" (8bits)")
        pm.addSyntax("(")

        pm.addArg("RS")
        pm.addSyntax(",")
        pm.addDecorator(" ")
        pm.addArg("E")
        pm.addSyntax(",")
        pm.addDecorator(" ")

        pm.addDecorator("D0-D7=")

        for i in range(7):
            pm.addSpace()
            pm.addSyntax(",")
        pm.addSpace()

        pm.fillWith(["spin-int"]*10)

        pm.addSyntax(")")
        return pm.getBlock()


    #----------------------------------------------------------------------
    def LCD_pins_4bit(self):

        pm = Block.Linear("LCD")
        pm.addLabel("lcd.pins")
        pm.addDecorator(" (4bits)")
        pm.addSyntax("(")

        pm.addArg("RS")
        pm.addSyntax(",")
        pm.addDecorator(" ")
        pm.addArg("E")
        pm.addSyntax(",")
        pm.addDecorator(" ")

        pm.addDecorator("D4-D7=")

        for i in range(4):
            pm.addSpace()
            pm.addSyntax(",")

        for i in range(3):
            pm.addSyntax("0, ")
        pm.addSyntax("0")

        pm.fillWith(["spin-int"]*6)

        pm.addSyntax(")")
        return pm.getBlock()


    #----------------------------------------------------------------------
    def LCD_begin(self):

        pm = Block.Linear("LCD")
        pm.addLabel("lcd.begin")
        pm.addSyntax("(")
        pm.addArg("COL")
        pm.addSyntax(",")
        pm.addDecorator(" ")
        pm.addArg("ROW")
        pm.addSyntax(")")
        pm.addContextHelp("set up the LCD's number of columns and rows")
        pm.fillWith(["spin-int", "spin-int"])
        return pm.getBlock()


    #----------------------------------------------------------------------
    def LCD_setCursor(self):

        pm = Block.Linear("LCD")
        pm.addLabel("lcd.setCursor")
        pm.addSyntax("(")
        pm.addArg("COL")
        pm.addSyntax(",")
        pm.addDecorator(" ")
        pm.addArg("ROW")
        pm.addSyntax(")")
        pm.fillWith(["spin-int", "spin-int"])
        return pm.getBlock()


    #----------------------------------------------------------------------
    def LCD_printNumber(self):

        pm = Block.Linear("LCD")
        pm.addLabel("lcd.printNumber")
        pm.addSyntax("(")
        pm.addSpace()
        pm.addSyntax(",")
        pm.addArg(" base")
        pm.addSyntax(")")
        pm.fillWith(["spin-int", "number-base"])
        return pm.getBlock()
