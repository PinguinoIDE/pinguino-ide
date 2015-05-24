
#To hard!!
#This feature will be available in Pinguino 1X (X>2)


# #! /usr/bin/python2
# #-*- coding: utf-8 -*-

# import os

# # Python3 compatibility
# if os.getenv("PINGUINO_PYTHON") is "3":
    # #Python3
    # from configparser import RawConfigParser
    # from io import StringIO
# else:
    # #Python2
    # from ConfigParser import RawConfigParser
    # from cStringIO import StringIO


# ########################################################################
# class Code2Blocks(object):
    # """"""


    # #----------------------------------------------------------------------
    # def code_to_blocks(self, code):
        # """"""
        # self.index_count = 0

        # l = range(1, 10)
        # l.reverse()
        # code = code.replace("{", "\n{\n")
        # code = code.replace("}", "\n}\n")
        # for i in l: code = code.replace(" "*i, " ")
        # code = code.replace(", ", ",")

        # code = code.split("\n")
        # code = filter(lambda x:not x.isspace(), code)
        # code = filter(None, code)


        # for index in range(len(code)):
            # if code[index].endswith(";"): code[index] = code[index][:-1]
            # if code[index].startswith(" "): code[index] = code[index][1:]

        # self.parse_blocks = RawConfigParser()
        # self.process_code(code)

        # return self.parse_blocks


    # #----------------------------------------------------------------------
    # def get_index(self):
        # """"""
        # self.index_count += 1
        # return self.index_count



    # #----------------------------------------------------------------------
    # def add_block(self, name, constructor, basename, position=(0, 0), inside=[], from_=[]):
        # """"""
        # id_ = self.get_index()

        # section_name = "Block-{0}".format(id_)
        # self.parse_blocks.add_section(section_name)

        # self.parse_blocks.set(section_name, "self_id", str(id_))
        # self.parse_blocks.set(section_name, "from", from_)
        # self.parse_blocks.set(section_name, "name", name)
        # self.parse_blocks.set(section_name, "constructor", constructor)
        # self.parse_blocks.set(section_name, "position", position)
        # self.parse_blocks.set(section_name, "basename", basename)
        # self.parse_blocks.set(section_name, "inside", inside)
        # self.parse_blocks.set(section_name, "to", [])
        # self.parse_blocks.set(section_name, "nested", [])

        # return str(id_)


    # #----------------------------------------------------------------------
    # def update_parser(self, id_, option, value):
        # """"""
        # section_name = "Block-{0}".format(id_)
        # self.parse_blocks.set(section_name, option, value)


    # #----------------------------------------------------------------------
    # def from_section(self, id_, option):
        # """"""
        # section_name = "Block-{0}".format(id_)
        # return self.parse_blocks.get(section_name, option)




    # #----------------------------------------------------------------------
    # def process_code(self, code):
        # """"""
        # global_ = self.add_block(name="function", constructor=[['label', 'global'], ['syntax', ' variables']], basename="global_vars", position=(10, 10))

        # last = global_
        # pos_func = 300
        # pos_func_i = 300

        # linear_ = []

        # for line in code:

            # last_iter = last

            # if line.startswith("#define"):
                # d, value1, value2 = line.split()
                # last = self.add_define(last, value1, value2)

            # elif line.startswith("void"):
                # name = line.split()[1].split("(")[0]
                # linear_.append(name)
                # last = self.add_funtion(name, pos=pos_func)
                # pos_func += pos_func_i

            # if line.split("(")[0] in ["pinMode", "digitalWrite", "delay"] + linear_:
                # name = line.split("(")[0]
                # values = line.split("(")[1][:-1].split(",")
                # last = self.add_linear(last, name, values)
                # self.update_parser(last_iter, "to", [last])



    # #----------------------------------------------------------------------
    # def add_define(self, parent, value1, value2):
        # """"""
        # in1 = self.add_block(name="output", constructor=[['edit', value1, 'white', (90, 90, 90)]], basename="edit-value")
        # in2 = self.add_block(name="output", constructor=[['edit', value2, 'white', (90, 90, 90)]], basename="edit-value")
        # define = self.add_block(name="linear", constructor=[['label', '#define'], ['syntax', ' '], ['space'], ['syntax', ' '], ['space'], ['syntax', ' //']], basename= "define", inside=[in1, in2])

        # self.update_parser(in1, "from", [define])
        # self.update_parser(in2, "from", [define])
        # self.update_parser(define, "from", [parent])

        # if self.from_section(parent, "name") in ["function"]:
            # self.update_parser(parent, "nested", [define])
        # else:
            # self.update_parser(parent, "to", [define])

        # return define


    # #----------------------------------------------------------------------
    # def add_funtion(self, name, pos):
        # """"""
        # return self.add_block(name="function", constructor=[['label', name], ['syntax', '()']], basename=name, position=(pos, 10))


    # #----------------------------------------------------------------------
    # def add_linear(self, parent, name, values=["", ""]):
        # """"""
        # sp = []
        # for i in range(1, len(values)+1):
            # sp.append(['space'])
            # if i != len(values): sp.append(['syntax', ', '])


        # cns = [['label', name], ['syntax', '(']] + sp + ['syntax', ')']


        # linear = self.add_block(name="linear", constructor=cns, basename=name, from_=[parent])


        # insides = []
        # for value in values:
            # insides.append(self.add_block(name="output", constructor=[['edit', value, 'white', (90, 90, 90)]], basename="edit-value", from_=[linear]))




        # self.update_parser(linear, "inside", insides)

        # # self.update_parser(linear, "to", [insides[0]])


        # return linear



        # # [Block-10]
        # # self_id = 0x7f21e03f0bd8
        # # from = ['0x7f21e03d9050']
        # # name = linear
        # # constructor = [['label', 'pinMode'], ['syntax', '('], ['space'], ['syntax', ', '], ['space'], ['syntax', ')']]
        # # position = (21, 51)
        # # basename = pinMode
        # # inside = ['0x7f21e03f45f0', '0x7f21e03f4f38']
        # # to = ['0x7f21e03f75f0']
        # # nested = []
