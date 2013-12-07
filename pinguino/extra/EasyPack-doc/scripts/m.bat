rem @echo off

rem Specify variables
set REV=857
set VERSION=01
set REL_DATE=2013-05
set PYTHON_DIR=c:\python26
set ZIP7_EXE=d:\7-zip\7z.exe



set path=%path%;%PYTHON_DIR%

set TARGET_DIR=..\x4-easy-rev%REV%

if "%1"=="r" goto archive

rd /q /s %TARGET_DIR%
rd /q /s dist
rd /q /s build 

%PYTHON_DIR%\python.exe setup.py py2exe


set XOPT=/d /y /s /i
xcopy /d /y .\*				%TARGET_DIR%\
del /q %TARGET_DIR%\*.dll
del /q %TARGET_DIR%\*.pyd
del /q %TARGET_DIR%\*.py

xcopy %XOPT% dist\*			%TARGET_DIR%

xcopy %XOPT% examples		%TARGET_DIR%\examples
xcopy %XOPT% extra			%TARGET_DIR%\extra
xcopy %XOPT% locale			%TARGET_DIR%\locale
xcopy %XOPT% p8				%TARGET_DIR%\p8
xcopy %XOPT% p32			%TARGET_DIR%\p32
xcopy %XOPT% python_lib		%TARGET_DIR%\python_lib
xcopy %XOPT% source			%TARGET_DIR%\source
xcopy %XOPT% theme			%TARGET_DIR%\theme
xcopy %XOPT% win32			%TARGET_DIR%\win32

xcopy %XOPT% wxgui			%TARGET_DIR%\wxgui
del /q /s %TARGET_DIR%\wxgui\*.pyc


copy  /y  extra\EasyPack-doc\config_default.txt	%TARGET_DIR%\.config
del /q %TARGET_DIR%\m.bat
del /q %TARGET_DIR%\setup.py
del /q %TARGET_DIR%\msvc*.dll
del /q %TARGET_DIR%\w9xpopen.exe
del /q %TARGET_DIR%\PinguinoOSX.command
del /q %TARGET_DIR%\*.sh
del /q %TARGET_DIR%\*.log

echo %REV% - EasyPack >  %TARGET_DIR%\extra\EasyPack-doc\revision.txt 
copy /y extra\EasyPack-doc\EasyPack_ReleaseNote_en.txt %TARGET_DIR%\

goto end

:archive
set arc_name=..\Pinguino-X4-EasyPack-Win32-Rev%REV%-v%VERSION%-%REL_DATE%.7z 
del /q %arc_name%
%ZIP7_EXE% a %arc_name% %TARGET_DIR%


:end

rem Created 2013/02 - 2013/05 by avrin
