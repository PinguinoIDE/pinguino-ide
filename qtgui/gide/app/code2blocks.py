#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

from PySide import QtCore, QtGui
import logging

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
    from io import StringIO
else:
    #Python2
    from ConfigParser import RawConfigParser
    from cStringIO import StringIO


from ..py_bloques import constructor as Blocks


########################################################################
class Code2Blocks(object):
    """"""


    #----------------------------------------------------------------------
    def code_to_blocks(self, event=None):
        """"""

        code = self.parse_code()

        self.ide_new_file()
        start_pos = QtCore.QPoint(50, 150)

        work_area = self.ide.get_current_editor().graphical_area

        if code[1] != "{":
            last_block, pos = work_area.new_bloq("function", ['function', 'Pinguino', ['label', 'global'], ['syntax', ' variables']], QtCore.QPoint(50, 150), "global")
            last_block.metadata.pos_ = pos
            start_pos += QtCore.QPoint(500, 0)

        for line in range(len(code)):
            block_type = self.get_block_type(code, line)

            if code[line] in ["{", "}"]:
                continue

            constructor = self.get_contructor(block_type, code[line])
            block, pos = work_area.new_bloq(constructor[0], constructor, start_pos, "block_{}_{}".format(self.ide.get_current_filename(), line))
            block.metadata.pos_ = pos

            if block.metadata.type_ != "tipo4":
                new_pos = last_block.pos() + last_block.metadata.to_type[block.metadata.type_][0]

                if code[line-1] == "{":
                    nested = True
                else:
                    nested = None
                work_area.accept_move(True, block, last_block, nested)
                block.move(new_pos)

            else:
                start_pos += QtCore.QPoint(500, 0)

            work_area.expand_all()

            last_block = block


    #----------------------------------------------------------------------
    def parse_code(self):
        """"""
        code = self.ide.get_current_code()
        code = self.ide.remove_comments(code)

        self.index_count = 0

        l = range(1, 10)
        l.reverse()
        code = code.replace("{", "\n{\n")
        code = code.replace("}", "\n}\n")
        code = code.replace(";", "\n")
        for i in l: code = code.replace(" "*i, " ")
        code = code.replace(", ", ",")

        code = code.split("\n")
        code = filter(lambda x:not x.isspace(), code)
        code = filter(None, code)

        for index in range(len(code)):
            if code[index].endswith(";"): code[index] = code[index][:-1]
            if code[index].startswith(" "): code[index] = code[index][1:]


        #fix for
        finish_for = False
        while not finish_for:
            for line in range(len(code)):
                try:
                    if code[line].startswith("for"):
                        code[line] = ";".join(code[line:line+3])
                        code.pop(line+1)
                        code.pop(line+1)

                    else:
                        finish_for = True

                except IndexError:
                    break




        return code


    #----------------------------------------------------------------------
    def get_block_type(self, code, line):
        """"""
        try:
            # Function
            if code[line+1] == "{" and code[line].split()[0] in ["char*", "char", "int", "float", "u8", "u16", "u32", "void"]:
                return Blocks.Function

            # Nested
            elif code[line+1] == "{" :
                return Blocks.Nested

            # Linear
            else:
                return Blocks.Linear

        except IndexError:
            return Blocks.Linear




    #----------------------------------------------------------------------
    def get_contructor(self, block, linecode):
        """"""
        blk = block("None")

        fill_args = []
        if linecode.count("("):
            name = linecode[:linecode.find("(")]
            args = linecode[linecode.find("(")+1:linecode.find(")")].split(",")


            syntax = ","

            if len(args) == 1:
                args = args[0].split(";")
                syntax = ";"

            # logging.debug(args)

            blk.addLabel(name)
            blk.addSyntax("(")

            for index in range(len(args)):
                if args[index]:

                    if len(args)>index>0:
                        blk.addSyntax(syntax)

                    if args[index] in ["False", "True"]:
                        blk.addSpaceBool()
                    else:
                        blk.addSpace()
                    fill_args.append(self.get_fancy_widget(args[index]))


            blk.addSyntax(")")
        else:
            blk.addLabel(linecode)

        if fill_args:
            blk.fillWith(fill_args)

        return blk.getBlock()


    #----------------------------------------------------------------------
    def get_fancy_widget(self, arg):
        """"""
        if arg in ["True", "False"]:
            return ("choice-B", arg)

        elif arg in ["INPUT", "OUTPUT"]:
            return ("choice-io", arg)

        elif arg in ["HIGH", "LOW"]:
            return ("choice-hl", arg)

        elif arg in ["GREENLED", "YELLOWLED", "USERBUTTON", "RUNLED", "USERLED"]:
            return ("choice-user", arg)

        elif arg in map(lambda x:"D{}".format(x), range(14)):
            return ("choice-D", arg)

        elif arg in map(lambda x:"A{}".format(x), range(6)):
            return ("choice-A", arg)

        elif arg.isdigit():
            return ("spin-int", arg)

        elif arg.count(".") == 1:
            try:
                type(eval("45.5")) == float
                return ("spin-float", arg)
            except:
                return ("edit-value", arg)

        else:
            return ("edit-value", arg)


