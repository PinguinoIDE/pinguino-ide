Pinguino IDE 12
===============


### Install from PyPI ###
```shell
pip install pyside
pip install requests
pip install pinguino
```


### Install from source code ###

```shell
git clone https://github.com/PinguinoIDE/pinguino-ide.git Pinguino12-dev
cd Pinguino12-dev
git checkout v12-dev
mv ~/.pinguino ~/.pinguino.copy
python pinguino.py
```


### Run Pinguino with downloaded compilers and libraries ###
For special users (Arch Linux).

Download with git.
```shell
git clone https://github.com/PinguinoIDE/pinguino-ide.git Pinguino12-dev
git clone https://github.com/PinguinoIDE/pinguino-compilers.git
git clone https://github.com/PinguinoIDE/pinguino-libraries.git
```

Move to V12 branch.
```shell
cd Pinguino12-dev
git checkout v12-dev
```

Edit with your paths
```shell
vim pinguino/qtgui/config/pinguino.linux.conf
```

pinguino.linux.conf should seem like:
```
[Paths]

compilers = full_path_to_pinguino-compilers-master
libraries = full_path_to_pinguino-libraries-master
ide = full_path_to_pinguino-ide

sdcc_bin = ${compilers}/linux64/p8/bin
gcc_bin = ${compilers}/linux64/p8/bin
xc8_bin = /opt/microchip/xc8/v1.[0-9][0-9]/bin
pinguino_8_libs = $(libraries)/p8
pinguino_32_libs = $(libraries)/p32
install_path = ${ide}

user_path = ~/Pinguino/v12
user_libs = ${user_path}/pinguinolibs
```

Enjoy.
```shell
python pinguino.py
```

----
[Report Bugs](https://github.com/PinguinoIDE/pinguino-ide/issues)

