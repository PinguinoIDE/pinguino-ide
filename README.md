Pinguino IDE
===========
Open source integrated development enviroment (IDE).


  * [Requirements] (#Requirements)
  * [IDE](#ide)
  * [Install (on custom directory)] (#install-on-custom-directory)
  * [Command line](#command-line)
  * [Python API](#python-api)

## Requirements
  * [pyside] (https://pypi.python.org/pypi/PySide)
  * [pyusb==0.4.3] (http://sourceforge.net/projects/pyusb/files/PyUSB%200.x/0.4.3)
  
#### optionals
Used for Pinguino's library manager
  * [gitpython] (https://pypi.python.org/pypi/GitPython)
  * [hgapi] (https://pypi.python.org/pypi/hgapi)
  * [pysvn] (http://pysvn.tigris.org/project_downloads.html)
Used for Pinguino's wiki documentation
  * [beautifulsoup4] (https://pypi.python.org/pypi/beautifulsoup4)


## IDE
Graphical user interface (GUI) in [Python](http://python.org/) and [PySide](http://www.pyside.org/).
```shell
$ python pinguino.py
```
![Pinguino IDE 11.0 - Text mode](https://lh4.googleusercontent.com/-XiJW0FCqZG4/UvryfpUEn1I/AAAAAAAAGUQ/8PWFdAopCEA/w700-h362-no/text_mode.png "Pinguino IDE 11.0 - Text mode")
![Pinguino IDE 11.0 - Graphical mode](https://lh3.googleusercontent.com/-KmOJjxGpdJE/UvryfjZ4BLI/AAAAAAAAGUU/wQt5UPeP614/w700-h365-no/graphical_mode.png "Pinguino IDE 11.0 - Graphical mode")

## Install (on custom directory)
  * Download [Compilers](https://github.com/PinguinoIDE/pinguino-compilers/releases/latest), [Libraries](https://github.com/PinguinoIDE/pinguino-libraries/releases/latest) and [IDE](https://github.com/PinguinoIDE/pinguino-ide/releases/latest) sources.
  * Create a own directory, we use ```PINGUINO-DIR``` with absolute path ```/PATH/TO/PINGUINO-DIR```
  * Uncompress ```pinguino-ide-x.x.x.zip``` and extract the content of ```pinguino-ide-x.x.x``` to ```PINGUINO-DIR```.
  * Uncompress ```pinguino-libraries-x.x.zip``` and extract the content of ```pinguino-libraries-x.X``` to ```PINGUINO-DIR```.
  * Uncompress ```linux.zip``` (win32.zip or macosx.zip) and move the ```linux``` (win32 or macosx) directory to ```PINGUINO-DIR```.
  * Edit [paths.cfg](https://github.com/PinguinoIDE/pinguino-ide/blob/master/paths.cfg) with ```install_path = /PATH/TO/PINGUINO-DIR```.
  * Install [dependences](#requirements).
  * Copy "source" directory (from ```PINGUINO-DIR```) to ```user_path```, see [paths.cfg](https://github.com/PinguinoIDE/pinguino-ide/blob/master/paths.cfg)
  * Run ```python pinguino.py```.
  * On menu ```pinguino >> Set paths```.
 
![Pinguino IDE 11.0 - Set paths dialog](https://lh3.googleusercontent.com/--H1cV_DYjWw/UzYyAUL6-yI/AAAAAAAAGsQ/nr3Tkc4CxFs/w550-h237-no/paths.png)
  * Coding, compile and enjoy.

## Command line
Command line tools for compile and upload *.pde files to Pinguino boards.

**Compiling ~/Desktop/test.pde for Generic 2550 and bootloader version 2**
```shell
$ python pinguino.py -f ~/Desktop/test.pde -l v2 -p2550
using Pinguino 2550 board
using boot2 bootloader
compilation time: 1.383
compiled to: ~/Desktop/test.hex 
```

<br>
**Compiling ~/Desktop/test.pde for Generic 2550 and bootloader version 4**
```shell
$ python pinguino.py -f ~/Desktop/test.pde -l v4 -p2550
using Pinguino 2550 board
using boot2 bootloader
compilation time: 1.383
compiled to: ~/Desktop/test.hex 
```

<br>
**Compiling ~/Desktop/test.pde for PIC32 Pinguino OTG**
```shell
$ python pinguino.py -f ~/Desktop/test.pde -O
using PIC32 Pinguino OTG board
using microchip bootloader
compilation time: 4.019
compiled to: ~/Desktop/test.pde

```

<br>
**Compiling ~/Desktop/test.pde for PIC32 Pinguino OTG and upload code**
```shell
$ python pinguino.py -f ~/Desktop/test.pde -O -x
using PIC32 Pinguino OTG board
using microchip bootloader
compilation time: 1.558
compiled to: ~/Desktop/test.hex
UBW32 found: 241664 bytes free

Erasing...

Writing hex file '/home/yeison/Escritorio/test.hex':............................................................

Resetting device...
```

<br>
**Compiling ~/Desktop/test.pde for Generic 2550, bootloader version 4 and print hex file**
```shell
$ python pinguino.py -f ~/Desktop/test.pde -l v4 -p2550 --hex
using Pinguino 2550 board
using boot4 bootloader
compilation time: 0.449
compiled to: ~/Desktop/test.hex

**********************************************************************
~/Desktop/test.hex
**********************************************************************
:020000040000FA
:100C000002EF06F014EEFFF024EEFFF0F86AA68E75
:100C1000A69CBF0EE96E0E0EEA6E0068ED6A0050EB
:100C2000FDE1F80EF66E0C0EF76E000EF86E090080
:100C3000F5CF05F00900F5CF06F034D00900F5CF67
:100C400000F00900F5CF01F00900F5CF02F009002E
:100C50000900F5CFE9FF0900F5CFEAFF0900090017
:100C60000900F5CF03F00900F5CF04F009000900F1
:100C7000F6CF07F0F7CF08F0F8CF09F000C0F6FF85
:100C800001C0F7FF02C0F8FF03D00900F5CFEEFF67
:100C90000306FBE20406F9E207C0F6FF08C0F7FF0F
:100CA00009C0F8FF0506CAE20606C8E259EC06F0DC
:100CB000FFD7D0B202D0930ED012D350030B800FC7
:100CC0007D0F02E3D3A4FED7F29EF29C70EC06F0F7
:100CD00083EC06F08AEC06F089EC06F0FDD71200F2
:100CE000896A8A6A400E8B6E926A936A9490949293
:100CF0009494949C949E12000100100D0000000535
:100D00000000010000000F0EC16E070EB46E12004D
:060D1000000012001200B9
:00000001FF
**********************************************************************
```

<br>
**Board args**
```
      Board name     Short arg     Long arg

Pinguino 2455         -p2455    --pinguino2455
Pinguino 2550         -p2550    --pinguino2550
Pinguino 25k50        -p25k50   --pinguino25k50
Pinguino 26J50        -p26j50   --pinguino26j50
Pinguino 27J53        -p27j53   --pinguino27j53

Pinguino 4455         -p4455    --pinguino4455
Pinguino 4550         -p4550    --pinguino4550
Pinguino 45k50        -p45k50   --pinguino45k50
Pinguino 46J50        -p46j50   --pinguino46j50
Pinguino 47J53-A      -p47j53A  --pinguino47j53a

Pinguino 32MX250      -p32mx250 --pinguino32mx250
Pinguino 32MX220      -p32mx220 --pinguino32mx220

      Other boards supported
     
FreeJALduino          -J        --freejalduino
PICuno Equo           -eq       --picunoequo
PIC32 Pinguino        -o        --olimex440
PIC32 Pinguino OTG    -O        --olimex440OTG
PIC32 Pinguino Micro  -M        --olimex440Micro
PIC32 Pinguino T795   -T        --olimexT795
PIC32 Pinguino 220    -m        --olimex220
Emperor 460           -e        --emperor460
Emperor 795           -E        --emperor795
UBW32 460             -u        --ubw460
UBW32 795             -U        --ubw795

```

## Python API (temporarily deprecated)
Python module for compile and upload *.pde files to Pinguino boards.
```python
>>> import sys
>>> sys.path.append("[PATH_TO_PINGUINOIDE]/qtgui/pinguino_api")
>>> from pinguino import Pinguino, Board
>>> project = Pinguino()
>>> project.set_board(Board.PIC32_Pinguino_OTG)
>>> project.compile_file("Desktop/test.pde")
>>> project.compiled()
True
>>> result = project.get_result()
>>> for key in result:
    print(key+": "+result[key])https://github.com/PinguinoIDE/pinguino-ide/edit/master/README.md#
...     
hex_file: Desktop/test.hex
code_size: code size: 0 / 262144 bytes (0% used)
time: 1.189
>>> project.upload()
(True, "UBW32 found: 241664 bytes free\n\nErasing...\n\nWriting hex file 'Escritorio/test.hex':............................................................\n\nResetting device...\n")
>>> 
```


```python
>>> import sys
>>> sys.path.append("[PATH_TO_PINGUINOIDE]/qtgui/pinguino_api")
>>> from pinguino import Pinguino, Board
>>> project = Pinguino()
>>> project.set_board(Board.PIC32_Pinguino_OTG)
>>> project.compile_file("Desktop/test.pde")
>>> project.compiled()
False
>>> project.get_errors()
{'preprocess': [], 'compiling': {'c': [], 'asm': []}, 'linking': ["[PATH_TO_PINGUINOIDE]/pinguinoide/pinguino/source/user.c:3:1: error: 'hi' undeclared (first use in this function)\n"]}
>>> 
```
