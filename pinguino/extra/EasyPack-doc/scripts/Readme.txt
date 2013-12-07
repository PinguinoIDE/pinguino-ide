This Packaging script intends to use Pingunio X.4 about rev.800 or later at this moment.

Prerequisite:
  Windows7(32bit):
      This script seems to work only on 32bit Windows so far. 
  Subversion client:
    TortoiseSVN:
       [ http://tortoisesvn.net/downloads.html ]
       In the installation procces, add the selection of 'command line client tools'. 
  7-Zip:
      [ http://www.7-zip.org/ ]
  Pinguino: http://pinguino.cc 
      [ svn checkout http://pinguino32.googlecode.com/svn/ide/ pinguino32 ]
      It would be better to use clean copy from the above repository otherwise some your local changes
      will be included to the package unintentionally.
  Python: v2.6.6    
      [ http://www.python.org/ftp/python/2.6.6/python-2.6.6.msi ]
  Py2exe: v0.6.9
      [ http://sourceforge.net/projects/py2exe/files/py2exe/0.6.9/py2exe-0.6.9.win32-py2.6.exe/download ]
  PySerial: v2.5
      [ http://sourceforge.net/projects/pyserial/files/pyserial/2.5/pyserial-2.5.win32.exe/download ]
  pyusb-1.0.0a2-py2.6:
      [ http://sourceforge.net/projects/pyusb/files/PyUSB%201.0/1.0.0-alpha-2/pyusb-1.0.0a2.zip/download ]
  wxPython: 2.8.12.1(unicode) for Python 2.6
      [ http://downloads.sourceforge.net/wxpython/wxPython2.8-win32-unicode-2.8.12.1-py26.exe ]
   msvcp90.dll: (Using Ver.9.00.30729.1)
      This file will be found in c:\Windows\winsxs or so on.

Packaging:
1. Copy setup.py, m.bat, msvcp90.dll to original Pinguino root folder (existing pinguino_start.bat).
2. Edit m.bat copied.
   Specify REV,VERSION,REL_DATE,PYTHON_DIR,ZIP7_EXE variables.
3. Open MS-DOS command line window, change your folder to original Pinguino root folder and execute 'm.bat'.
   The target Pinguino folder is ..\x4-easy-revNNN. (NNN is revision number)
4. If you'd like to make 7z archive package, execute 'm.bat r' in command line window. 
   The archive file will be generated in the parent folder.


Making update pack:
 If you make a update package with new differential files to EasyPack released,  
 rewrite the content of extra\EasyPack-doc\revision.txt to
 i.e. 
 '872 - Updated EasyPack' so on.





by avrin 
2013/05

