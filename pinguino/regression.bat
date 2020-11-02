echo off
rem --------------------------------------------------------------------
rem file:     regression.bat
rem desc.:    Test all pde. files
rem projects: Pinguino 
rem author:   Regis Blanchot
rem usage:    ./regression.sh directory-to-scan
rem --------------------------------------------------------------------
rem CHANGELOG:
rem 09 Nov. 2012 - RB - first release
rem 26 Jan. 2016 - RB - updated to use with Pinguino IDE v12
rem 03 Jan. 2016 - RB - added multi-file and multi-compiler (8-bit only)
rem 12 Sep. 2016 - RB - added 32-bit support
rem 13 Sep. 2016 - RB - added MS-DOS shell support
rem --------------------------------------------------------------------
rem This is free software; you can redistribute it and/or
rem modify it under the terms of the GNU Lesser General Public
rem License as published by the Free Software Foundation; either
rem version 2.1 of the License, or (at your option) any later version.
rem
rem This library is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
rem Lesser General Public License for more details.
rem
rem You should have received a copy of the GNU Lesser General Public
rem License along with this library; if not, write to the Free Software
rem Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
rem --------------------------------------------------------------------

rem --------------------------------------------------------------------
rem constants
rem --------------------------------------------------------------------

setlocal enableDelayedExpansion

set LOGDIR=log
set PYTHON=C:\Python27\python.exe
set PINGUINO= C:\python27\Lib\site-packages\pinguino\pinguino_cmd.py

set CMP8LIST=--xc8 --sdcc
set BRD8LIST=-p1459 -c1708 -p13k50 -p14k50 -p2455 -p4455 -p2550 -p4550 -p25k50 -p45k50 -p26j50 -p46j50 -p27j53 -p47j53
set BRD32LIST=-p220 -p250 -p270 --olimex220 --olimex440 --olimex440OTG --olimex440Micro --olimexT795 --emperor460 --emperor795 --ubw460 --ubw795

cls

rem --------------------------------------------------------------------
rem check argument
rem --------------------------------------------------------------------

if %1.==. (
    echo WARNING : missing argument
    echo Usage : ./regression.sh directory-to-scan
    echo We scan examples directory by default
    set TARGET=examples
) else (
    set TARGET=%1
)

rem --------------------------------------------------------------------
rem create a log dir.
rem --------------------------------------------------------------------

if not exist %LOGDIR% mkdir %LOGDIR%

rem --------------------------------------------------------------------
rem %%f   : .pde file name with path
rem FNAME : .pde file name without path
rem --------------------------------------------------------------------

for /r %TARGET% %%f in (*.pde) do (
    set FNAME=%%~nxf
    echo ---------------------------------------------------------------
    echo Compiling !FNAME! ...
    echo ---------------------------------------------------------------

    rem ----------------------------------------------------------------
    rem 8-bit
    rem  > %LOGDIR%/!FNAME!.log
    rem if not "%%o"=="%%o:OK" (echo ERROR %%f %%b %%c) else (echo .)
    rem ----------------------------------------------------------------

    for %%b in (%BRD8LIST%) do (
        for %%c in (%CMP8LIST%) do (
            for /f %%o in ('%PYTHON% %PINGUINO% %%c %%b -f %%f') do set result=%%o
            if "!result!"=="OK" (
                echo OK with %%b and %%c >> %LOGDIR%/!FNAME!.log
            ) else (
                echo FAILED with %%b and %%c >> %LOGDIR%/!FNAME!.log
            )
        )
    )

    rem ----------------------------------------------------------------
    rem 32-bit
    rem  > %LOGDIR%/!FNAME!.log
    rem if not "%%o"=="%%o:OK" (echo ERROR %%f %%b %%c) else (echo .)
    rem ----------------------------------------------------------------

    for %%b in (%BRD32LIST%) do (
        for /f %%o in ('%PYTHON% %PINGUINO% %%b -f %%f') do set result=%%o
        if "!result!"=="OK" (
            echo OK with %%b and gcc-p32 >> %LOGDIR%/!FNAME!.log
        ) else (
            echo FAILED with %%b and gcc-p32 >> %LOGDIR%/!FNAME!.log
        )
    )
)
