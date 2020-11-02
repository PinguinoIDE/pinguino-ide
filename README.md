Pinguino IDE 13
===============

### Install from source code ###
```shell
mkdir -p ~/Pinguino/v13
cd ~/Pinguino/v13
git clone https://github.com/PinguinoIDE/pinguino-ide.git pinguino-ide
git clone https://github.com/PinguinoIDE/pinguino-libraries.git pinguino-libraries 
```

### Launch ###
```shell
python ~/Pinguino/pinguino-ide/pinguino-ide.py
or
./pinguino.bat (Windows)
./pinguino.sh  (Linux)
```

### Update ###
For special users (Arch Linux).
```shell
cd ~/Pinguino/pinguino-ide
git checkout
cd ~/Pinguino/pinguino-libraries
git checkout
```

### Config. file ###
```shell
Edit pinguino/qtgui/config/pinguino.linux.conf or pinguino.windows.conf

[Paths]
sdcc_bin = "full_path_to_pinguino-compilers"/p8/bin
gcc_bin = "full_path_to_pinguino-compilers"/p32/bin
xc8_bin = "full_path_to_xc8"/bin
pinguino_8_libs = "full_path_to_pinguino-ide"/p8
pinguino_32_libs = "full_path_to_pinguino-ide"/p32
install_path = "full_path_to_pinguino-ide"

user_path = ~/Pinguino/v13
user_libs = ${user_path}/pinguinolibs
```

----
[Report Bugs](https://github.com/PinguinoIDE/pinguino-ide/issues)

