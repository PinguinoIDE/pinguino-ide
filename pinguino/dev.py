#!/usr/bin/env python
#-*- coding: utf-8 -*


# #----------------------------------------------------------------------
# def get_libraries_dir():
    # """"""


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


