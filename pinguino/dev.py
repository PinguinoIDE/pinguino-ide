#!/usr/bin/env python
#-*- coding: utf-8 -*

import os
from setuptools import setup
from . import version

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
else:
    #Python2
    from ConfigParser import RawConfigParser



########################################################################
class PinguinoLib:
    """"""

    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""

        self.PINGUINO = RawConfigParser()
        self.PINGUINO.readfp(open("PINGUINO", "r"))
        self.PINGUINO = dict(self.PINGUINO.items("PINGUINO"))

        self.BASE_DIR = os.path.abspath(os.path.dirname("PINGUINO"))



    #----------------------------------------------------------------------
    def get_data_files(self):

        PINGUINOLIBS_DIR = os.path.join(self.get_libraries_dir(), self.PINGUINO["name"])
        data_files = []
        mainlib = [(PINGUINOLIBS_DIR, "PINGUINO")]
        for dir_ in ["p8", "p32", "pdl", "examples", "blocks"]:
            path = os.path.join(self.BASE_DIR, dir_)
            if os.path.exists(path):
                for file_ in os.listdir(path):
                    # mainlib.append(os.path.join(dir_, file_))
                    data_files.append((os.path.join(PINGUINOLIBS_DIR, dir_), [os.path.join(dir_, file_)]))

        return data_files


    #----------------------------------------------------------------------
    @classmethod
    def get_libraries_dir(cls):
        """"""

        if os.name in ["posix", "os2"]: #GNU/Linux and MacOS
            return "/usr/share/pinguinolibs"

        # elif os.name == "os2":  #Mac OS X
            # os_kwargs = {}

        elif os.name == "nt":  #Windows
            return "C:\pinguino\libs"



    # #----------------------------------------------------------------------
    # def post_install(self, lib_name):
        # """"""
        # lista = []

        # path_examples = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", lib_name, "examples")
        # dest_examples = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "library_examples", lib_name)
        # lista.append([path_examples, dest_examples])

        # path_blocks = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "usr", lib_name, "blocks")
        # dest_blocks = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "blocks", lib_name)
        # lista.append([path_blocks, dest_blocks])

        # for src, dest in lista:
            # if os.path.exists(src):
                # if os.path.exists(dest): shutil.rmtree(dest)
                # shutil.copytree(src, dest)

        # # self.pinguinoAPI.force_reload_libs()
        # # self.update_reserved_words()
        # # delattr(self, "assistant")
        # self.refresh_libraries()



    #----------------------------------------------------------------------
    def pinguino_setup(self, **kwargs):
        """"""
        return setup(install_requires = ["pinguino",
                                         # "pyside",
                                         ],

                    include_package_data = True,
                    data_files = self.get_data_files(),
                    zip_safe = False,


                    **kwargs
                    )


    #----------------------------------------------------------------------
    def get_setup_template(self):
        return """
#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
from pinguino.dev import PinguinoLib

library = PinguinoLib()
PINGUINO = library.PINGUINO

library.pinguino_setup(

    name = "pinguino-{name}".format(**PINGUINO),
    version = PINGUINO["version"],
    description = PINGUINO["description"],
    description_file = "README.rst",

    author = PINGUINO["name"],
    author_email = "",
    maintainer = "",
    maintainer_email = "",

    url = PINGUINO["url"],
    download_url = "",

    license = "",
    keywords = "microchip, electronic, prototyping, pinguino",

    classifiers=[#list of classifiers in https://pypi.python.org/pypi?:action=list_classifiers

                 ],
)"""

    #----------------------------------------------------------------------
    def get_pinguino_template(self, **kwargs):
        return """
[PINGUINO]
name = {name}
author = {author}
arch = {archs}
repository =
description =
url =
version = 0.1""".format(**kwargs)

