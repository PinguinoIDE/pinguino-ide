@echo off

if exist pinguino.exe goto start_exe
    python.exe pinguino.py
goto end

:start_exe
   pinguino.exe

:end