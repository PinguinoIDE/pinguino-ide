pinguino-ide
===========
Open hardware electronics prototyping platform.
Open source integrated development enviroment (IDE).

  * [IDE](#ide)
  * [Command line](#command-line)
  * [Python API](#python-api)


## IDE
Graphical user interface (GUI) in [Python](http://python.org/) and [PySide](http://www.pyside.org/).
```shell
$ cd pinguinoide
python pinguino.py
```


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
$ cd pinguinoide/pinguino
$ python pinguino.py -f ~/Desktop/test.pde -O
using PIC32 Pinguino OTG board
using microchip bootloader
compilation time: 4.019
compiled to: ~/Desktop/test.pde

```

<br>
**Compiling ~/Desktop/test.pde for PIC32 Pinguino OTG and upload code**
```shell
$ cd pinguinoide/pinguino
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
$ cd pinguinoide/pinguino
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
Pinguino 4455         -p4455    --pinguino4455
Pinguino 2550         -p2550    --pinguino2550
Pinguino 4550         -p4550    --pinguino4550
FreeJALduino          -J        --freejalduino
PICuno Equo           -eq       --picunoequo
Pinguino 25k50        -p25k50   --pinguino25k50
Pinguino 45k50        -p45k50   --pinguino45k50
Pinguino 26J50        -p26j50   --pinguino26j50
Pinguino 46J50        -p46j50   --pinguino46j50
Pinguino 27J53        -p27j53   --pinguino27j53
Pinguino 47J53-A      -p47j53A  --pinguino47j53a
PIC32 Pinguino        -o        --olimex440
PIC32 Pinguino OTG    -O        --olimex440OTG
PIC32 Pinguino Micro  -M        --olimex440Micro
PIC32 Pinguino T795   -T        --olimexT795
PIC32 Pinguino 220    -m        --olimex220
GENERIC32MX250F128    -N        --generic250
GENERIC32MX220F032    -n        --generic220
Emperor 460           -e        --emperor460
Emperor 795           -E        --emperor795
UBW32 460             -u        --ubw460
UBW32 795             -U        --ubw795

```

## Python API
Python module for compile and upload *.pde files to Pinguino boards.
```python
>>> import sys
>>> sys.path.append("[PATH_TO_PINGUINOIDE]/pinguinoide/pinguino")
>>> from pinguino import Pinguino, Board
>>> project = Pinguino()
>>> project.set_board(Board.PIC32_Pinguino_OTG)
>>> project.compile_file("Desktop/test.pde")
>>> project.compiled()
True
>>> result = project.get_result()
>>> for key in result:
    print(key+": "+result[key])
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
>>> sys.path.append("[PATH_TO_PINGUINOIDE]/pinguinoide/pinguino")
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
