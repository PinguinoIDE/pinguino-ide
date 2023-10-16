CLS 
@ECHO OFF

@REM force latest Python 3.X version to be used.
SET BIN=py -3

@ECHO ---------------------------
@ECHO ABOUT UNSIGNED DRIVERS ...
@ECHO ---------------------------
@ECHO Press "Win + X"
@ECHO Navigate to "Shutdown" and then "Shift + Left Click" on the "Restart" option.
@ECHO Advanced Boot menu : "Troubleshooting" > "Advanced Options" > "Start-up Settings"
@ECHO Just click on the "Restart" button to continue.
@ECHO Press F7 to select the 7th option "Disable driver signature enforcement."
@ECHO As soon as you press the key, your system will boot into Windows.
@ECHO Now you can install any unsigned driver you wish.
@ECHO After installing, restart your system, and the Driver Signature Enforcement
@ECHO will be automatically enabled from the next reboot.
@ECHO If you ever want to install another unsigned driver,
@ECHO you have to go through the above process again.
@ECHO ---------------------------
@ECHO CHECKING PYTHON MODULES ...
@ECHO ---------------------------
%BIN% -m pip install --upgrade --user pip
%BIN% -m pip install --upgrade --user pyside2
%BIN% -m pip install --upgrade --user pyusb
%BIN% -m pip install --upgrade --user wheel
%BIN% -m pip install --upgrade --user beautifulsoup4
%BIN% -m pip install --upgrade --user setuptools
%BIN% -m pip install --upgrade --user requests
@REM pybluez needs Windows Platform SDK
@REM bluepy is for Linux only and then failed to compile on Windows
@REM %BIN% -m pip install --upgrade --user pybluez
@ECHO ---------------------------
@ECHO LAUNCHING THE PINGUINO IDE ...
@ECHO ---------------------------
CD %USERPROFILE%\Dvpt\pinguino\pinguino-ide
%BIN% pinguino-ide.py
@ECHO ---------------------------
@ECHO CLOSING THE PINGUINO IDE ...
@ECHO ---------------------------
