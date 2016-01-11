#!/usr/bin/env python
#-*- coding: utf-8 -*-

import unittest
import os
import sys

try:
    import version
except:
    from . import version
sys.path.append(os.path.join(os.getenv("PINGUINO_LIB"), "qtgui", "resources"))

try:
    from qtgui.pinguino_core.pinguino import Pinguino, AllBoards
    from qtgui.pinguino_core.pinguino_config import PinguinoConfig
    from qtgui.pinguino_core.config import Config
except:
    from .qtgui.pinguino_core.pinguino import Pinguino, AllBoards
    from .qtgui.pinguino_core.pinguino_config import PinguinoConfig
    from .qtgui.pinguino_core.config import Config

Pinguino = Pinguino()
PinguinoConfig.set_environ_vars()
PinguinoConfig.check_user_files()
config = Config()
PinguinoConfig.update_pinguino_paths(config, Pinguino)
PinguinoConfig.update_pinguino_extra_options(config, Pinguino)
PinguinoConfig.update_user_libs(Pinguino)


########################################################################
class TestPreprocess(unittest.TestCase):

    #----------------------------------------------------------------------
    def test_delete_comments(self):

        cases = (

            ("//Pinguino Rules!", ""),
            ("/*Pinguino Rules!*/", ""),
            ("/*Pinguino //Rules!*/", ""),
            ("///*Pinguino Rules!*/", ""),

            ("\n".join([
            "#define LED1 0\n",
            "//#define LED2 1\n",
            "/*\n",
            "1\n",
            "2\n",
            "3\n",
            "*/\n",
            "#include <math.h>\n",
            ]),
            "\n".join([
            "#define LED1 0\n",
            "\n",
            "\n",
            "\n",
            "\n",
            "\n",
            "\n",
            "#include <math.h>\n",
            ]),
            )

        )

        for case in cases:
            # print("Testing remove comments for case: {}".format(case))
            got = Pinguino.remove_comments(case[0])
            expected = case[1]
            self.assertMultiLineEqual(got, expected,
                                      "Remove comments: Failure\ngot: '{}'\nexpected: '{}'".format(got, expected))


    #----------------------------------------------------------------------
    @classmethod
    def preprocess(cls, lib, libinstructions):

        def inter(self):
            # print("Testing preprocess pinguino -> c | {} -> {}".format(lib["pinguino"], lib["c"]))
            got, d = Pinguino.replace_word(lib["pinguino"], libinstructions)
            self.assertEqual(got, lib["c"],
                             "Preprocess: Failure\ngot: '{}'\nexpected: '{}'".format(got, lib["c"]))
        return inter



########################################################################
class TestBareMinumumCompilation(unittest.TestCase):

    #----------------------------------------------------------------------
    @classmethod
    def compilation(cls, board, icsp=False, compiler="gcc"):

        code = "void setup(){} void loop(){}"

        def inter(self):

            # print("Testing compilation (bare minimum) for board {name} - {bldr} - {arch}bit - {proc} - {compiler}".format(compiler=compiler, **board.__dict__))

            Pinguino.set_board(board)
            if icsp:
                Pinguino.set_icsp()

            if compiler != "gcc":
                Pinguino.set_8bit_compiler(compiler)

            try:
                Pinguino.compile_string(code)
            except BaseException as msg:
                self.fail("Compilation: impossible compile for {}, {}-bit\n{}".format(board.name, board.arch, str(msg)))

            if not Pinguino.compiled():
                msg = Pinguino.get_errors()
                self.fail("Compilation: impossible compile for {}, {}-bit\n{}".format(board.name, board.arch, str(msg)))


        return inter




libs8 = Pinguino.get_regobject_libinstructions(8)
for lib in libs8:
    test_name = "test_preprocess_8_{}".format(lib["pinguino"].replace(".", "_"))
    setattr(TestPreprocess, test_name, TestPreprocess.preprocess(lib, libs8))

libs32 = Pinguino.get_regobject_libinstructions(8)
for lib in libs32:
    test_name = "test_preprocess_32_{}".format(lib["pinguino"].replace(".", "-"))
    setattr(TestPreprocess, test_name, TestPreprocess.preprocess(lib, libs32))


for board in AllBoards:
    if board.arch == 8:
        for compiler in ["sdcc", "xc8"]:
            test_name = "test_compile_icsp_{}_{}".format(compiler, board.name.replace(" ", "_").replace(".", "_"))
            setattr(TestBareMinumumCompilation, test_name, TestBareMinumumCompilation.compilation(board, icsp=True, compiler=compiler))

            test_name = "test_compile_{}_{}_{}".format(compiler, board.bldr, board.name.replace(" ", "_").replace(".", "_"))
            setattr(TestBareMinumumCompilation, test_name, TestBareMinumumCompilation.compilation(board, icsp=False, compiler=compiler))

    else:
        test_name = "test_compile_{}_{}".format(board.bldr, board.name.replace(" ", "_").replace(".", "_"))
        setattr(TestBareMinumumCompilation, test_name, TestBareMinumumCompilation.compilation(board))



# unittest.main()

suite = unittest.TestSuite()
suite.addTests(unittest.defaultTestLoader.loadTestsFromTestCase(TestBareMinumumCompilation))
suite.addTests(unittest.defaultTestLoader.loadTestsFromTestCase(TestPreprocess))
unittest.TextTestRunner().run(suite)