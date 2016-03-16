#! /usr/bin/python2
#-*- coding: utf-8 -*-

from PySide import QtGui, QtCore

import os
import sys
import shutil
import logging
from datetime import datetime
from zipfile import ZipFile
import subprocess

from ..methods.decorators import Decorator
from ..methods.dialogs import Dialogs
from ..methods.parser import PinguinoParser

from pinguino.dev import PinguinoLib


# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
    from os import makedirs
else:
    #Python2
    from ConfigParser import RawConfigParser

    #----------------------------------------------------------------------
    def makedirs(name, mode=0o777, exist_ok=False):
        """makedirs(name [, mode=0o777][, exist_ok=False])

        Super-mkdir; create a leaf directory and all intermediate ones.  Works like
        mkdir, except that any intermediate path segment (not just the rightmost)
        will be created if it does not exist. If the target directory already
        exists, raise an OSError if exist_ok is False. Otherwise no exception is
        raised.  This is recursive.

        """
        head, tail = os.path.split(name)
        if not tail:
            head, tail = os.path.split(head)
        if head and tail and not os.path.exists(head):
            try:
                makedirs(head, mode, exist_ok)
            except FileExistsError:
                # Defeats race condition when another thread created the path
                pass
            cdir = os.curdir
            if isinstance(tail, bytes):
                cdir = bytes(os.curdir, 'ASCII')
            if tail == cdir:           # xxx/newdir/. exists if xxx/newdir exists
                return
        try:
            os.mkdir(name, mode)
        except OSError:
            # Cannot rely on checking for EEXIST, since the operating system
            # could give priority to other errors like EACCES or EROFS
            if not exist_ok or not os.path.isdir(name):
                raise



########################################################################
class Project(object):
    """"""

    #----------------------------------------------------------------------
    def __init__(self):
        """"""
        self.connect(self.main.actionAdd_existing_directory, QtCore.SIGNAL("triggered()"), self.select_existing_directory)
        self.connect(self.main.actionNew_project, QtCore.SIGNAL("triggered()"), self.new_project)
        self.connect(self.main.actionNew_library, QtCore.SIGNAL("triggered()"), self.__show_library_template__)
        # self.connect(self.main.actionImport_library_project, QtCore.SIGNAL("triggered()"), self.import_library)
        self.connect(self.main.pushButton_newproject, QtCore.SIGNAL("clicked()"), self.new_project)
        self.connect(self.main.pushButton_openproject, QtCore.SIGNAL("clicked()"), self.open_project)
        self.connect(self.main.pushButton_newlibrary, QtCore.SIGNAL("clicked()"), self.__show_library_template__)
        self.connect(self.main.actionClose_project, QtCore.SIGNAL("triggered()"), self.close_project)
        self.connect(self.main.actionSave_project, QtCore.SIGNAL("triggered()"), self.save_project)
        self.connect(self.main.actionSave_project_as, QtCore.SIGNAL("triggered()"), self.save_project_as)
        self.connect(self.main.actionAdd_current_file, QtCore.SIGNAL("triggered()"), self.add_current_file)
        self.connect(self.main.actionOpen_project, QtCore.SIGNAL("triggered()"), self.open_project)
        self.connect(self.main.actionAdd_existing_file, QtCore.SIGNAL("triggered()"), self.select_existing_file)
        self.connect(self.main.treeWidget_projects, QtCore.SIGNAL("itemExpanded(QTreeWidgetItem*)"), self.expand_tree)
        self.connect(self.main.treeWidget_projects, QtCore.SIGNAL("itemDoubleClicked(QTreeWidgetItem*,int)"), self.open_from_tree)

        self.connect(self.main.pushButton_compilelibrary, QtCore.SIGNAL("clicked()"), self.compile_library)
        self.connect(self.main.pushButton_packagelibrary, QtCore.SIGNAL("clicked()"), self.package_library)

        self.connect(self.main.treeWidget_projects, QtCore.SIGNAL("itemChanged(QTreeWidgetItem*,int)"), self.update_check_status)
        # self.connect(self.main.treeWidget_projects, QtCore.SIGNAL("itemClicked(QTreeWidgetItem*,int)"), self.update_check_status)


    #----------------------------------------------------------------------
    @Decorator.show_tab("Project")
    @Decorator.alert_tab("Project")
    def open_project(self):

        self.project_saved = True
        file_project = Dialogs.set_open_file(self, exts="*.ppde PINGUINO")
        if file_project is None: return

        if file_project.endswith("PINGUINO"):
            self.import_library(file_project)

        elif file_project.endswith(".ppde"):
            self.open_project_from_path(file_project)
            self.update_project_status()


    #----------------------------------------------------------------------
    @Decorator.show_tab("Project")
    @Decorator.alert_tab("Project")
    def open_project_from_path(self, filename):
        """"""
        try:
            self.project_saved = True
            self.ConfigProject = RawConfigParser()
            self.ConfigProject.readfp(open(filename, "r"))
            self.ConfigProject.filename = filename

            project_name = self.reload_project(open_=True)
            logging.debug("Opening \"{}\" project.".format(project_name))

            self.update_recents_projects(self.ConfigProject.filename)

            self.ide_close_all()
            if self.is_library():
                for path in self.get_libs_file():
                    self.ide_open_file_from_path(filename=path)

        except:
            Dialogs.error_message(self, "File seems to be corrupted!")

        self.save_recents()



    #----------------------------------------------------------------------
    def reload_project(self, open_=False):
        """"""

        if (not self.is_project()) and (not open_): return

        self.main.treeWidget_projects.clear()

        project_name = self.ConfigProject.get("Main", "name")
        self.main.treeWidget_projects.setHeaderLabel(project_name)

        if not self.ConfigProject.has_section("Ignore"): to_ignore = []
        elif not self.ConfigProject.options("Ignore"): to_ignore = []
        else: to_ignore = [self.ConfigProject.get("Ignore", option) for option in self.ConfigProject.options("Ignore")]

        inherits_status = self.get_inherits_from_project()

        for option in self.ConfigProject.options("Dirs"):
            if not self.ConfigProject.get("Dirs", option) in to_ignore:
                self.add_existing_directory(self.ConfigProject.get("Dirs", option), to_ignore, check_duplicated=False, inherits_status=inherits_status, open_=True, library=self.is_library())

        for path, checked in self.get_files_from_project().items():
            if path in to_ignore: continue
            self.add_existing_file(path, check_duplicated=False, check=checked, open_=True)

        self.update_project_status(project_name)
        return project_name


    #----------------------------------------------------------------------
    def select_existing_directory(self):
        """"""
        dir_project = Dialogs.set_open_dir(self)
        if dir_project in self.get_dirs_from_project(): return False

        self.add_existing_directory(dir_project, inherits_status=True)
        return dir_project



    #----------------------------------------------------------------------
    def add_existing_directory(self, dir_project, to_ignore=[], check_duplicated=True, inherits_status=dict(), open_=False, library=False):
        """"""

        self.take_from_ignore(dir_project)
        if check_duplicated:
            if dir_project in self.get_dirs_from_project(): return

        # flags = QtCore.Qt.ItemIsSelectable|QtCore.Qt.ItemIsEditable|QtCore.Qt.ItemIsDragEnabled|QtCore.Qt.ItemIsUserCheckable|QtCore.Qt.ItemIsEnabled
        # flags = QtCore.Qt.ItemIsSelectable|QtCore.Qt.ItemIsDragEnabled|QtCore.Qt.ItemIsUserCheckable|QtCore.Qt.ItemIsEnabled

        if library is True:
            flags = QtCore.Qt.ItemIsSelectable|QtCore.Qt.ItemIsDragEnabled|QtCore.Qt.ItemIsEnabled
        else:
            flags = QtCore.Qt.ItemIsSelectable|QtCore.Qt.ItemIsDragEnabled|QtCore.Qt.ItemIsUserCheckable|QtCore.Qt.ItemIsEnabled


        parent = self.add_new_tree(os.path.basename(dir_project), self.main.treeWidget_projects, dir_project, flags)
        inherits = self.generate_tree(dir_project, parent, levels=50, flags=flags, to_ignore=to_ignore, inherits_status=inherits_status, library=library)
        parent.setExpanded(True)

        if not open_:
            self.add_dir_to_config(dir_project)
            self.add_inherits_to_config(inherits)


    #----------------------------------------------------------------------
    def add_current_file(self):
        """"""
        editor = self.get_tab().currentWidget()
        self.add_existing_file(editor.path)


    #----------------------------------------------------------------------
    def select_existing_file(self):
        """"""
        new_file = Dialogs.set_open_file(self)
        if new_file in self.get_files_from_project(): return
        self.add_existing_file(new_file)


    #----------------------------------------------------------------------
    def add_existing_file(self, path, check_duplicated=True, check=True, open_=False):
        """"""
        self.take_from_ignore(path)
        if check_duplicated:
            if path in self.get_files_from_project(): return

        filename = os.path.split(path)[1]
        parent = self.main.treeWidget_projects
        # flags = QtCore.Qt.ItemIsDragEnabled|QtCore.Qt.ItemIsUserCheckable|QtCore.Qt.ItemIsEnabled
        flags = QtCore.Qt.ItemIsSelectable|QtCore.Qt.ItemIsDragEnabled|QtCore.Qt.ItemIsUserCheckable|QtCore.Qt.ItemIsEnabled

        self.add_new_file_item(filename, parent, path, flags, check=check)

        if not open_:
            self.add_file_to_config(path)



    #----------------------------------------------------------------------
    def set_project_name(self):
        """"""
        project_name = Dialogs.get_text(self, "Project name", default="untitled_project")

        if project_name is False:
            return

        self.main.treeWidget_projects.setHeaderLabel(project_name)

        if not self.ConfigProject.has_section("Main"): self.ConfigProject.add_section("Main")
        options = self.ConfigProject.options("Main")
        self.ConfigProject.set("Main", "name", project_name)
        self.ConfigProject.set("Main", "library", False)

        self.main.widget_library.setVisible(False)

        self.save_project(default=True)

        logging.debug("Created \"{}\" project.".format(project_name))

        return project_name
        # self.main.treeWidget_projects.setHeaderLabel(project_name)


    #----------------------------------------------------------------------
    def set_library_name(self, project_name):
        """"""
        self.main.treeWidget_projects.setHeaderLabel(project_name)

        if not self.ConfigProject.has_section("Main"): self.ConfigProject.add_section("Main")
        options = self.ConfigProject.options("Main")
        self.ConfigProject.set("Main", "name", project_name)
        self.ConfigProject.set("Main", "library", True)

        self.main.widget_library.setVisible(True)

        self.save_project(default=True)

        logging.debug("Created \"{}\" library.".format(project_name))

        return project_name


    #----------------------------------------------------------------------
    def is_project(self):
        """"""
        if "PINGUINO_PROJECT" in os.environ:
            return bool(os.environ["PINGUINO_PROJECT"])
        else:
            return False


    #----------------------------------------------------------------------
    def is_library(self):
        """"""
        if os.environ.get("PINGUINO_PROJECT", False):
            if self.ConfigProject.has_option("Main", "library"):
                r = self.ConfigProject.get("Main", "library")
                if type(r) == str:
                    return r == "True"
                elif type(r) == bool:
                    return r
            else:
                self.ConfigProject.set("Main", "library", False)
                return False
        else:
            return False


    #----------------------------------------------------------------------
    def get_libs_file(self):
        """"""
        libs = []
        if self.ConfigProject.has_option("Main", "lib"):
            libs.append(self.ConfigProject.get("Main", "lib"))
        if self.ConfigProject.has_option("Main", "lib32"):
            libs.append(self.ConfigProject.get("Main", "lib32"))
        return libs


    #----------------------------------------------------------------------
    def get_project_name(self):
        """"""
        return self.ConfigProject.get("Main", "name")


    #----------------------------------------------------------------------
    def get_project_files(self):
        """"""
        #files = [file_ for file_, checked in self.get_files_from_project().iteritems() if checked]
        #files.extend([file_ for file_, checked in self.get_inherits_from_project().iteritems() if checked])
        files = [file_ for file_, checked in self.get_files_from_project().items() if checked]
        files.extend([file_ for file_, checked in self.get_inherits_from_project().items() if checked])
        return files


    # #----------------------------------------------------------------------
    # def get_main_upload(self):
        # """"""
        # header = self.main.treeWidget_projects.header()
        # filename = self.get_tab().currentWidget().path
        # return getattr(header, "main_upload", filename)


    #----------------------------------------------------------------------
    @Decorator.show_tab("Project")
    @Decorator.alert_tab("Project")
    def new_project(self):
        """"""
        self.project_saved = False
        self.ConfigProject = RawConfigParser()
        project_name = self.set_project_name()

        if project_name:
            Dialogs.info_message(self, QtGui.QApplication.translate("Dialogs", "Create or add an existing directory for project."))
            self.select_existing_directory()
            self.update_project_status(project_name)


    #----------------------------------------------------------------------
    def save_project_as(self):
        """"""
        self.project_saved = False
        self.save_project(silent=False)



    #----------------------------------------------------------------------
    def save_project(self, silent=False, default=False):
        """"""

        if default:
            default_path = os.path.join(os.getenv("PINGUINO_USER_PATH"), "projects")

            if not os.path.exists(default_path):
                makedirs(default_path, exist_ok=True)
            self.ConfigProject.filename = os.path.join(default_path, self.get_project_name()) + ".ppde"
            self.ConfigProject.write(open(self.ConfigProject.filename, "w"))
            self.project_saved = True
            return

        if self.project_saved:
            self.ConfigProject.write(open(self.ConfigProject.filename, "w"))

        elif not silent:
            save = Dialogs.set_save_file(self, self.get_project_name()+".ppde")
            if save[0]:
                path, filename = save
                self.ConfigProject.filename = path
                self.ConfigProject.write(open(self.ConfigProject.filename, "w"))
                self.project_saved = True

        self.update_recents_projects(self.ConfigProject.filename)


    #----------------------------------------------------------------------
    def add_dir_to_config(self, dirpath):
        """"""
        if not self.ConfigProject.has_section("Dirs"): self.ConfigProject.add_section("Dirs")
        options = self.ConfigProject.options("Dirs")

        if dirpath in self.get_dirs_from_project(): return

        index = 0
        while "dir_%d"%index in options:
            index += 1

        self.ConfigProject.set("Dirs", "dir_%d"%index, dirpath)
        self.save_project(silent=True)


    #----------------------------------------------------------------------
    def add_inherits_to_config(self, inherits):
        """"""
        if not self.ConfigProject.has_section("Inherits"): self.ConfigProject.add_section("Inherits")
        options = self.ConfigProject.options("Inherits")

        index = 0
        while "inherit__%d"%index in options:
            index += 1

        for path in inherits:
            if path in self.get_inherits_from_project(): continue
            self.ConfigProject.set("Inherits", "inherit_%d"%index, path)
            self.ConfigProject.set("Inherits", "inherit_%d_checked"%index, "1")
            index += 1
        self.save_project(silent=True)


    #----------------------------------------------------------------------
    def add_file_to_config(self, filepath):
        """"""
        if not self.ConfigProject.has_section("Files"): self.ConfigProject.add_section("Files")
        options = self.ConfigProject.options("Files")

        if filepath in self.get_files_from_project(): return

        index = 0
        while "file_%d"%index in options:
            index += 1

        self.ConfigProject.set("Files", "file_%d"%index, filepath)
        self.ConfigProject.set("Files", "file_%d_checked"%index, "1")

        self.save_project(silent=True)


    #----------------------------------------------------------------------
    def hide_element_to_config(self, path):
        """"""
        if not self.ConfigProject.has_section("Ignore"): self.ConfigProject.add_section("Ignore")
        options = self.ConfigProject.options("Ignore")

        index = 0
        while "ignore_%d"%index in options:
            index += 1

        self.ConfigProject.set("Ignore", "ignore_%d"%index, path)


        logging.debug("Removing \"{}\" from project.".format(path))
        for path_inherit, option in self.get_inherits_option_from_project().items():
            if path_inherit.startswith(path):
                self.ConfigProject.remove_option("Inherits", option)
                self.ConfigProject.remove_option("Inherits", option+"_checked")
                logging.debug("Removing \"{}\" from project.".format(path_inherit))


        for path_files, option in self.get_files_option_from_project().items():
            if path_files.startswith(path):
                self.ConfigProject.remove_option("Files", option)
                self.ConfigProject.remove_option("Files", option+"_checked")
                logging.debug("Removing \"{}\" from project.".format(path_files))




        self.save_project(silent=True)


    #----------------------------------------------------------------------
    def update_project_status(self, project="reload"):
        """"""
        if project == "reload":
            pass
        elif project:
            os.environ["PINGUINO_PROJECT"] = project
        else:
            os.environ["PINGUINO_PROJECT"] = ""

        self.main.treeWidget_projects.setVisible(bool(project))
        self.main.widget_noproject.setVisible(not bool(project))
        self.main.widget_library.setVisible(self.is_library())

        self.main.treeWidget_projects.contextMenuEvent = self.project_context_menu
        Decorator.update_toolbar()

        open_project = bool(os.environ["PINGUINO_PROJECT"])
        self.main.actionNew_project.setEnabled(not open_project)
        self.main.actionOpen_project.setEnabled(not open_project)
        self.main.actionSave_project.setEnabled(open_project)
        self.main.actionSave_project_as.setEnabled(open_project)
        self.main.actionClose_project.setEnabled(open_project)
        self.main.actionAdd_existing_directory.setEnabled(open_project)
        self.main.actionAdd_current_file.setEnabled(open_project)
        self.main.actionAdd_existing_file.setEnabled(open_project)
        self.main.actionAdd_new_file.setEnabled(open_project)
        self.main.actionAdd_new_blocks_file.setEnabled(open_project)
        # self.main.actionSet_current_file_as_main_upload.setEnabled(open_project)



    #----------------------------------------------------------------------
    def get_current_item(self):
        """"""
        return self.main.treeWidget_projects.currentItem()




    #----------------------------------------------------------------------
    def project_context_menu(self, event):
        menu = QtGui.QMenu()

        if hasattr(self.get_current_item(), "path"):
            if os.path.isfile(self.get_current_item().path):
                # option for files
                # menu.addAction("Set as main upload file", self.set_as_main_upload)
                menu.addAction("Remove file", self.remove_file)
            else:
                # option for dirs
                menu.addAction("Remove directory", self.remove_directory)

            # option for files and dirs
            menu.addAction("Remove exclude from project", self.remove_from_tree)

        # general options
        menu.addAction("Rename...", self.rename)
        menu.addSeparator()
        menu.addAction("Add existing directory...", self.select_existing_directory)
        menu.addAction("Add existing file...", self.select_existing_file)
        menu.addSeparator()
        if hasattr(self.get_current_item(), "path"):
            menu.addAction("Add new file...", self.add_new_file)
            menu.addAction("Add new blocks file...", self.add_new_blocks_file)
            menu.addAction("Add new directory...", self.add_new_directory)
        menu.addAction("Close project", self.close_project)
        menu.addAction("Open project...", self.open_project)
        menu.addSeparator()
        menu.addAction("Reload project", self.reload_project)

        menu.setStyleSheet("""
        QMenu {
            font-family: inherit;
            font-weight: normal;
            }

        """)

        menu.exec_(event.globalPos())


    # #----------------------------------------------------------------------
    # def set_as_main_upload(self):
        # """"""
        # item = self.get_current_item()
        # item.setForeground(0, QtGui.QColor("#FF0000"))

        # item.setText(0, "{} [main upload file]".format(item.text(0)))

        # if not self.ConfigProject.has_section("Main"): self.ConfigProject.add_section("Main")
        # self.ConfigProject.set("Main", "main_upload", item.path)
        # self.save_project(True)

        # header = self.main.treeWidget_projects.header()
        # header.main_upload = item.path


    #----------------------------------------------------------------------
    def remove_file(self):
        """"""
        item = self.get_current_item()
        os.remove(item.path)
        item.removeChild(item)


    #----------------------------------------------------------------------
    def remove_directory(self):
        """"""
        item = self.get_current_item()
        shutil.rmtree(item.path)
        item.removeChild(item)


    #----------------------------------------------------------------------
    def remove_from_tree(self):
        """"""
        item = self.get_current_item()
        self.hide_element_to_config(item.path)
        item.removeChild(item)


    #----------------------------------------------------------------------
    def take_from_ignore(self, path):
        """"""
        if self.ConfigProject.has_section("Ignore"):
            options = self.ConfigProject.options("Ignore")
            for option in options:
                if self.ConfigProject.get("Ignore", option) == path:
                    self.ConfigProject.remove_option("Ignore", option)


    #----------------------------------------------------------------------
    def get_default_project_dir(self):
        """"""
        dirs = self.get_dirs_from_project()
        if dirs:  return dirs[0]

        files = self.get_files_from_project().keys()
        if files: return files[0]

        # return QtCore.QDir.home().path()
        return os.getenv("PINGUINO_DEFAULT_FILES")


    #----------------------------------------------------------------------
    def get_dirs_from_project(self):
        """"""
        if not self.ConfigProject.has_section("Dirs"): return []
        if not self.ConfigProject.options("Dirs"): return []
        return [self.ConfigProject.get("Dirs", option) for option in self.ConfigProject.options("Dirs")]


    #----------------------------------------------------------------------
    def get_ignore_from_project(self):
        """"""
        if not self.ConfigProject.has_section("Ignore"): return []
        if not self.ConfigProject.options("Ignore"): return []
        return [self.ConfigProject.get("Ignore", option) for option in self.ConfigProject.options("Ignore")]


    #----------------------------------------------------------------------
    def get_files_from_project(self):
        """"""
        if not self.ConfigProject.has_section("Files"): return {}
        if not self.ConfigProject.options("Files"): return {}
        return {self.ConfigProject.get("Files", option):bool(int(self.ConfigProject.get("Files", "{}_checked".format(option))))
                for option in filter(lambda f:not f.endswith("_checked"), self.ConfigProject.options("Files"))}


    #----------------------------------------------------------------------
    def get_inherits_from_project(self):
        """"""
        if not self.ConfigProject.has_section("Inherits"): return {}
        if not self.ConfigProject.options("Inherits"): return {}
        return {self.ConfigProject.get("Inherits", option):bool(int(self.ConfigProject.get("Inherits", "{}_checked".format(option))))
                for option in filter(lambda f:not f.endswith("_checked"), self.ConfigProject.options("Inherits"))}


    #----------------------------------------------------------------------
    def get_files_option_from_project(self):
        """"""
        if not self.ConfigProject.has_section("Files"): return {}
        if not self.ConfigProject.options("Files"): return {}
        return {self.ConfigProject.get("Files", option):option
                for option in filter(lambda f:not f.endswith("_checked"), self.ConfigProject.options("Files"))}


    #----------------------------------------------------------------------
    def get_inherits_option_from_project(self):
        """"""
        if not self.ConfigProject.has_section("Inherits"): return {}
        if not self.ConfigProject.options("Inherits"): return {}
        return {self.ConfigProject.get("Inherits", option):option
                for option in filter(lambda f:not f.endswith("_checked"), self.ConfigProject.options("Inherits"))}



    #----------------------------------------------------------------------
    def update_check_status(self, item, int_):
        """"""
        status = item.checkState(int_)
        if status: status = "1"
        else: status = "0"

        if hasattr(item, "path"):

            path = item.path

            if not os.path.isfile(path):
                return

            if not path.endswith(".pde"):
                return

            inherits = self.get_inherits_option_from_project()
            files = self.get_files_option_from_project()

            if path in inherits:
                self.ConfigProject.set("Inherits", inherits[path]+"_checked", status)

            elif path in files:
                    self.ConfigProject.set("Files", files[path]+"_checked", status)

            else:
                self.update_new_file(path, status)
                # files = self.get_files_option_from_project()
                # self.ConfigProject.set("Files", files[path]+"_checked", status)

            self.save_project(silent=True)


    #----------------------------------------------------------------------
    def update_new_file(self, path, status):
        """
        Add new file and define their status.
        """
        dirs = self.get_dirs_from_project()
        for dir_ in dirs:
            if path.startswith(dir_):
                # add as inherits
                inherits = self.get_inherits_option_from_project()

                x = 0
                while "inherit_{}".format(x) in inherits.values():
                    x += 1

                self.ConfigProject.set("Inherits", "inherit_{}".format(x), path)
                self.ConfigProject.set("Inherits", "inherit_{}_checked".format(x), status)
                return

        files = self.get_files_option_from_project()
        x = 0
        while "files_{}".format(x) in files.values():
            x += 1
            self.ConfigProject.set("Files", "files_{}".format(x), path)
            self.ConfigProject.set("Files", "files_{}_checked".format(x), status)


        # add as file



    #----------------------------------------------------------------------
    def close_project(self):
        """"""
        name = self.get_project_name()
        self.project_saved = False
        self.main.treeWidget_projects.clear()
        self.update_project_status("")
        logging.debug("Closing \"{}\" project.".format(name))


    #----------------------------------------------------------------------
    def add_new_file(self):
        """"""
        path = self.get_current_item().path
        if os.path.isfile(path):
            path = os.path.dirname(self.get_current_item().path)

        filename = Dialogs.get_text(self, "New file", default=self.get_untitled_name(ext=".pde")[:-1])
        new_file = os.path.join(path, filename)
        nf = open(new_file, "w")
        nf.close()

        self.ide_open_file_from_path(filename=new_file)

        if new_file in self.get_files_option_from_project():
            self.add_existing_file(new_file)

        if self.is_project():
            self.reload_project()

    #----------------------------------------------------------------------
    def add_new_blocks_file(self):
        """"""
        path = self.get_current_item().path
        if os.path.isfile(path):
            path = os.path.dirname(self.get_current_item().path)


        filename = Dialogs.get_text(self, "New blocks file", default=self.get_untitled_name(ext=".gpde")[:-1])
        new_file = os.path.join(path, filename)
        nf = open(new_file, "w")
        nf.close()

        if self.get_current_item().path in self.get_files_option_from_project():
            self.add_existing_file(new_file)

        if self.is_project():
            self.reload_project()


    #----------------------------------------------------------------------
    def add_new_directory(self):
        """"""
        path = self.get_current_item().path
        if os.path.isfile(path):
            path = os.path.dirname(self.get_current_item().path)

        dirname = Dialogs.get_text(self, "New directory", default="untitled-dir")
        new_dir = os.path.join(path, dirname)
        makedirs(new_dir, exist_ok=True)

        if self.get_current_item().path in self.get_files_option_from_project():
            self.add_existing_directory(new_dir)

        if self.is_project():
            self.reload_project()

    #----------------------------------------------------------------------
    def rename(self):
        """"""
        # path = self.get_current_item().path
        path = getattr(self.get_current_item(), "path", "")
        name = Dialogs.get_text(self, "New name", default=os.path.basename(path))
        if not name: return

        new_name = os.path.join(os.path.dirname(path), name)
        files = self.get_files_option_from_project()
        os.rename(path, new_name)

        if path in files:
            self.ConfigProject.set("Files", files[path], new_name)

        if self.is_project():
            self.reload_project()


    #----------------------------------------------------------------------
    def update_recents_projects(self, filename):

        if filename in self.recent_projects:
            self.recent_projects.remove(filename)
        self.recent_projects.insert(0, filename)
        self.recent_projects = self.recent_projects[:10]

        self.update_recents_menu_project()


    #----------------------------------------------------------------------
    def update_recents_menu_project(self):

        self.main.menuRecentProjects.clear()
        for file_ in self.recent_projects:
            action = QtGui.QAction(self)
            filename = os.path.split(file_)[1]

            len_ = 40
            if len(file_) > len_:
                file_path_1 = file_[:int(len_/2)]
                file_path_2 = file_[int(-len_/2):]
                file_path = file_path_1 + "..." + file_path_2
            else: file_path = file_

            if os.path.isfile(file_):
                action.setText(filename+" ("+file_path+")")
                self.connect(action, QtCore.SIGNAL("triggered()"), self.ide_menu_recent_event(file_))
                action.ActionEvent = self.ide_menu_recent_event

                self.main.menuRecentProjects.addAction(action)

        self.main.menuRecentProjects.addSeparator()
        self.main.menuRecentProjects.addAction(QtGui.QApplication.translate("Dialogs", "Clear recent files"), self.clear_recents_projects)


    #----------------------------------------------------------------------
    def clear_recents_projects(self):

        self.main.menuRecentProjects.clear()
        self.main.menuRecentProjects.addSeparator()
        self.main.menuRecentProjects.addAction(QtGui.QApplication.translate("Dialogs", "Clear recent files"), self.clear_recents_projects)
        self.recent_projects = []


    #----------------------------------------------------------------------
    @Decorator.show_tab("Project")
    @Decorator.alert_tab("Project")
    def new_library(self, **kwargs):
        """"""
        library_name = kwargs["libname"]
        library_dir = kwargs["libpath"]

        self.project_saved = False
        self.ConfigProject = RawConfigParser()
        self.set_library_name(library_name)

        if library_dir:  #FIXME no necesario
            self.create_library_template(**kwargs)
        else:
            self.close_project()

        self.add_existing_directory(os.path.join(library_dir, library_name), inherits_status=True)
        self.update_project_status(library_name)

        if self.is_project():
            self.reload_project()

    #----------------------------------------------------------------------
    def create_library_template(self, libpath, libname, author, description, _8bit, _32bit):
        """"""
        library_dir = os.path.join(libpath, libname)
        example_dir = os.path.join(library_dir, "examples")
        # tests_dir = os.path.join(library_dir, "tests")
        pinguino_file = os.path.join(library_dir, "PINGUINO")
        # lib = os.path.join(library_dir, "{}.lib".format(libname))
        # self.ConfigProject.set("Main", "lib", lib)

        makedirs(library_dir, exist_ok=True)
        makedirs(example_dir, exist_ok=True)
        # makedirs(tests_dir, exist_ok=True)

        self.add_existing_directory(library_dir, inherits_status=True)

        lib_template = """/*-----------------------------------------------------
Author: {}--<>
Date: {}
Description:

-----------------------------------------------------*/


void my_private_function(){{


}}

PUBLIC u8 my_function(){{
    // This function could be used as {}.my_function

}}
""".format(author, datetime.now().strftime("%Y-%m-%d %H:%M:%S"), libname)

        if _8bit:
            lib = os.path.join(library_dir, "{}.lib".format(libname))
            self.ConfigProject.set("Main", "lib", lib)
            lib_file = open(lib, mode="w")
            lib_file.write(lib_template)
            lib_file.close()

        if _32bit:
            lib32 = os.path.join(library_dir, "{}.lib32".format(libname))
            self.ConfigProject.set("Main", "lib32", lib32)
            lib_file = open(lib32, mode="w")
            lib_file.write(lib_template)
            lib_file.close()

        example_file = open(os.path.join(example_dir, "example.pde"), mode="w")
        example_file.close()

        example_file = open(os.path.join(example_dir, "example.gpde"), mode="w")
        example_file.close()

        # test_file = open(os.path.join(tests_dir, "test.pde"), mode="w")
        # test_file.close()

        archs = []
        if _8bit: archs.append("8bit")
        if _32bit: archs.append("32bit")
        parse_lib = RawConfigParser()
        parse_lib.add_section("PINGUINO")
        parse_lib.set("PINGUINO", "name", libname)
        parse_lib.set("PINGUINO", "author", author)
        parse_lib.set("PINGUINO", "author_email", "")
        parse_lib.set("PINGUINO", "arch", ", ".join(archs))
        parse_lib.set("PINGUINO", "repository", "")
        parse_lib.set("PINGUINO", "description", description)
        parse_lib.set("PINGUINO", "url", "")
        parse_lib.set("PINGUINO", "version", "0.1")
        parse_lib.write(open(pinguino_file, "w"))

        self.ide_open_file_from_path(filename=lib)



    #----------------------------------------------------------------------
    def compile_library(self):
        """"""
        self.ide_save_all()
        self.update_library_project()

        lib_name = self.get_project_name()
        lib = self.ConfigProject.get("Main", "lib")

        archs = self.get_library_archs()
        filenames = []

        for arch in archs:

            lib_dir = os.path.dirname(lib)
            if not os.path.isdir(os.path.join(lib_dir, arch)):
                makedirs(os.path.join(lib_dir, arch), exist_ok=True)
            if not os.path.isdir(os.path.join(lib_dir, "pdl")):
                makedirs(os.path.join(lib_dir, "pdl"), exist_ok=True)

            define = os.path.join(lib_dir, arch, "{}.h".format(lib_name))
            userc = os.path.join(lib_dir, arch, "{}.c".format(lib_name))
            pdl = os.path.join(lib_dir, "pdl", "{}.pdl{}".format(lib_name, "" if arch=="p8" else "32"))

            filenames.append(define)
            filenames.append(userc)
            filenames.append(pdl)

            if arch == "p8":
                arch_num = 8
            elif arch == "p32":
                arch_num = 32

            libinstructions = self.pinguinoAPI.get_regobject_libinstructions(arch_num)
            self.pinguinoAPI.preprocess([lib], define_output=define, userc_output=userc, ignore_spaces=self.is_project(), libinstructions=libinstructions)
            # self.pinguinoAPI.preprocess([lib], libinstructions=libinstructions)

            userc_file = open(userc, mode="r")
            userc_content = userc_file.read()
            userc_file.close()

            include = "#include <{}.h>".format(lib_name)

            parser = PinguinoParser([{"filename":userc, "content":userc_content}])

            # functions = self.get_functions([{"filename":userc, "content":userc_content}])
            functions = parser.get_functions()
            userc_content = userc_content.split("\n")

            pdl_content = []

            for function in functions:
                if function["return"].startswith("PUBLIC"):
                    userc_content[function["line"]-1] = userc_content[function["line"]-1].replace("PUBLIC ", "", 1)
                    userc_content[function["line"]-1] = userc_content[function["line"]-1].replace(function["name"], "{}_{}".format(lib_name, function["name"]), 1)
                    pdl_content.append("{} {}#include <{}.c>\n".format("{}.{}".format(lib_name, function["name"]), "{}_{}".format(lib_name, function["name"]), lib_name))
                # else:
                    # userc_content[function["line"]-1] = userc_content[function["line"]-1].replace(function["name"], "{}_{}".format(lib_name, function["name"]), 1)


            header_content = """//------------------------------------------------------------------
// Pinguino Library source code generated automatically.
//
// {} for {}bit
//
// Created: {}
// by: {}
//
//  WARNING! All changes made in this file will be lost!
//------------------------------------------------------------------\n
""".format(lib_name, arch_num, datetime.now().strftime("%Y-%m-%d %H:%M:%S"), os.getenv("PINGUINO_FULLNAME"))


            userc_file = open(userc, mode="w")
            userc_file.writelines([header_content] + [include] + ["\n".join(userc_content)])
            userc_file.close()

            pdl_file = open(pdl, mode="w")
            pdl_file.writelines([header_content] + pdl_content + ["\n"])
            pdl_file.close()

        if self.is_project():
            self.reload_project()

        self.add_library_to_env(lib_dir)
        Dialogs.info_message(self, "Library \"{}\" compiled.".format(lib_name))

        for filename in filenames:
            logging.debug("Generated: \"{}\"".format(filename))




    #----------------------------------------------------------------------
    def add_library_to_env(self, lib_dir):
        """"""
        os_name = os.getenv("PINGUINO_OS_NAME")
        if os_name == "windows":
            separator = ";"
        elif os_name == "linux":
            separator = ":"
        #FIXME_OSX
        # elif os_name == "macosx":
            # separator = ":"


        if os.path.exists(os.path.join(lib_dir, "p32")):
            os.environ["PINGUINO_P32"] = os.path.join(lib_dir, "p32")

        if os.path.exists(os.path.join(lib_dir, "p8")):
            os.environ["PINGUINO_P8"] = os.path.join(lib_dir, "p8")

        if os.path.exists(os.path.join(lib_dir, "pdl")):
            # os.environ["PINGUINO_PDL"] = os.listdir(os.path.join(lib_dir, "pdl"))
            os.environ["PINGUINO_PDL"] = separator.join([os.path.join(lib_dir, "pdl", filename) for filename in os.listdir(os.path.join(lib_dir, "pdl")) if filename.endswith(".pdl") or filename.endswith(".pdl32")])


    #----------------------------------------------------------------------
    def get_library_archs(self):
        """"""
        archs = []
        if self.ConfigProject.has_option("Main", "lib"):
            archs.append("p8")
        if self.ConfigProject.has_option("Main", "lib32"):
            archs.append("p32")

        return archs


    #----------------------------------------------------------------------
    def get_library_version(self):
        """"""
        parse_lib = RawConfigParser()
        parse_lib.readfp(open(os.path.join(self.get_library_path(), "PINGUINO"), "r"))
        return parse_lib.get("PINGUINO", "version")


    #----------------------------------------------------------------------
    def get_library_path(self):
        """"""
        return os.path.dirname(self.get_library_libs()[0])


    #----------------------------------------------------------------------
    def get_library_libs(self):
        """"""
        libs = []
        if self.ConfigProject.has_option("Main", "lib"):
            libs.append(self.ConfigProject.get("Main", "lib"))
        if self.ConfigProject.has_option("Main", "lib32"):
            libs.append(self.ConfigProject.get("Main", "lib32"))

        return libs


    #----------------------------------------------------------------------
    def set_library_libs(self, lib=None, lib32=None):
        """"""
        if lib:
            self.ConfigProject.set("Main", "lib", lib)

        if lib32:
            self.ConfigProject.set("Main", "lib32", lib32)


    #----------------------------------------------------------------------
    def package_library(self):
        """"""
        cwd = os.getcwd()
        os.chdir(self.get_library_path())


        try:
            manifest = open(os.path.join(self.get_library_path(), "MANIFEST.in"), "w+")
            manifest.write(PinguinoLib.get_manifest_template())
            manifest.close()

            setup = open(os.path.join(self.get_library_path(), "setup.py"), "w+")
            setup.write(PinguinoLib.get_setup_template())
            setup.close()

            dest = "-d{}".format(os.path.join(self.get_library_path(), "packages"))
            p = subprocess.Popen([sys.executable, "setup.py", "sdist", dest, "--formats=zip"],
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE)
            out, err = p.communicate()
            self.write_log(out.decode())
            # self.write_log(err.decode())

            # if err:
                # raise Exception(err.decode())

            os.remove(os.path.join(self.get_library_path(), "MANIFEST.in"))
            os.remove(os.path.join(self.get_library_path(), "setup.py"))

            info = list(filter(lambda f:f.endswith(".egg-info"), os.listdir(self.get_library_path())))
            if info:
                try:
                    os.remove(os.path.join(self.get_library_path(), info[0]))
                except:
                    shutil.rmtree(os.path.join(self.get_library_path(), info[0]))

            if self.is_project():
                self.reload_project()
            Dialogs.info_message(self, "Package created in '{}'".format(os.path.join(self.get_library_path(), "packages")))


        except Exception as err:
            logging.error(err)

        os.chdir(cwd)

    # #----------------------------------------------------------------------
    # def package_library(self):
        # """"""
        # library_dir = self.get_library_path()
        # library_versioned_name = "{}-{}".format(self.get_project_name(), self.get_library_version())
        # package_name = "{}.zip".format(library_versioned_name)
        # package = ZipFile(os.path.join(library_dir, package_name), "w")

        # files = [
            # "pdl",
            # "examples",
            # # "{}.lib".format(self.get_project_name()),
            # # "{}.lib32".format(self.get_project_name()),
            # "p8",
            # "p32",
            # "PINGUINO",
            # "README.md",
            # "README",
        # ]

        # for target in files:
            # if os.path.exists(os.path.join(library_dir, target)):
                # package.write(os.path.join(library_dir, target), arcname=os.path.join(library_versioned_name, target))
                # logging.debug("Adding {} to {}".format(target, package_name))

                # if os.path.isdir(os.path.join(library_dir, target)):

                    # for root, dirs, files in os.walk(os.path.join(library_dir, target)):
                        # for file in files:
                            # package.write(os.path.join(root, file), arcname=os.path.join(library_versioned_name, os.path.split(root)[1], file))
                            # logging.debug("Adding {} to {}".format(os.path.join(root, file), package_name))


        # package.close()


    #----------------------------------------------------------------------
    def import_library(self, filename):
        """"""

        parce_pinguino = RawConfigParser()
        parce_pinguino.readfp(open(filename, "r"))

        library_dir = os.path.dirname(filename)
        library_name = parce_pinguino.get("PINGUINO", "name")

        self.project_saved = False
        self.ConfigProject = RawConfigParser()
        self.set_library_name(library_name)


        if "{}.lib".format(library_name) in os.listdir(library_dir):
            self.ConfigProject.set("Main", "lib", os.path.join(library_dir, "{}.lib".format(library_name)))

        if "{}.lib32".format(library_name) in os.listdir(library_dir):
            self.ConfigProject.set("Main", "lib32", os.path.join(library_dir, "{}.lib32".format(library_name)))

        self.add_existing_directory(library_dir, inherits_status=True, library=True)
        self.update_project_status(library_name)

        # self.reload_project()
        self.ide_open_file_from_path(filename=filename)


    #----------------------------------------------------------------------
    def update_library_project(self):
        """"""
        libs = []
        path = self.get_library_path()
        for file_ in os.listdir(path):
            if file_.endswith(".lib"):
                self.set_library_libs(lib=os.path.join(path, file_))
                libs.append("8bit")
            elif file_.endswith(".lib32"):
                self.set_library_libs(lib32=os.path.join(path, file_))
                libs.append("32bit")

        parse_lib = RawConfigParser()
        parse_lib.readfp(open(os.path.join(self.get_library_path(), "PINGUINO"), "r"))
        parse_lib.set("PINGUINO", "arch", ", ".join(set(libs)))
        self.ConfigProject.set("Main", "name", parse_lib.get("PINGUINO", "name"))
        # self.save_project(silent=False, default=False)
        parse_lib.write(open(os.path.join(self.get_library_path(), "PINGUINO"), "w"))


