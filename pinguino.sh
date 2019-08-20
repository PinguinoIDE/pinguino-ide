#!/bin/sh

# If you get this error message :
# Could not install packages due to an EnvironmentError:
# Please check proxy URL. It is malformed and could be missing the host.
# Try :
#unset {http,https,ftp}_proxy

PYTHON_VERSION=3
PINGUINO_VERSION=13
XC8_VERSION=2.10


PINGUINO_GITHUB=https://github.com/PinguinoIDE
CONFIGFILE=pinguino.linux.conf
PINGUINODIR=$(pwd)
UPGRADE=--upgrade
USERMODE=--user

# Download files
#sudo apt-get install git python3 python3-pip
echo "Update Pinguino's files ..."
git clone --progress --branch v${PINGUINO_VERSION} ${PINGUINO_GITHUB}/pinguino-ide.git 2> /dev/null || git -C pinguino-ide pull
git clone --progress --branch v${PINGUINO_VERSION} ${PINGUINO_GITHUB}/pinguino-libraries.git 2> /dev/null || git -C pinguino-libraries pull
git clone --progress ${PINGUINO_GITHUB}/pinguino-compilers.git 2> /dev/null || git -C pinguino-compilers pull

# Empty and then write the config. file if necessary
if [ ! -f ${PINGUINODIR}/pinguino-user/pinguino.conf ]; then
  echo "Write the config. file ..."
  cd ${PINGUINODIR}/pinguino-ide/pinguino/qtgui/config
  cat /dev/null > ${CONFIGFILE}
  echo [Paths] >> ${CONFIGFILE}
  echo sdcc_bin = ${PINGUINODIR}/pinguino-compilers/linux64/p8/bin >> ${CONFIGFILE}
  echo gcc_bin  = ${PINGUINODIR}/pinguino-compilers/linux64/p32/bin >> ${CONFIGFILE}
  echo xc8_bin  = /opt/microchip/xc8/v${XC8_VERSION}/bin >> ${CONFIGFILE}
  echo pinguino_8_libs = ${PINGUINODIR}/pinguino-libraries/p8 >> ${CONFIGFILE}
  echo pinguino_32_libs = ${PINGUINODIR}/pinguino-libraries/p32 >> ${CONFIGFILE}
  echo install_path = ${PINGUINODIR}/pinguino-ide >> ${CONFIGFILE}
  echo user_path = ${PINGUINODIR}/pinguino-user >> ${CONFIGFILE}
  echo user_libs = ${PINGUINODIR}/pinguino-user/pinguinolibs >> ${CONFIGFILE}
fi

if [ ! -d ${PINGUINODIR}/pinguino-user/examples ]; then
  echo "Copy examples files ..."
  cp -r ${PINGUINODIR}/pinguino-libraries/examples ${PINGUINODIR}/pinguino-user/examples
fi

if [ ! -d ${PINGUINODIR}/pinguino-user/sources ]; then
  echo "Copy examples files ..."
  cp -r ${PINGUINODIR}/pinguino-libraries/sources ${PINGUINODIR}/pinguino-user/sources
fi

# Update Python's modules
echo "Update Python's modules ..."
python${PYTHON_VERSION} -m pip install ${UPGRADE} ${USERMODE} pip >> /dev/null
python${PYTHON_VERSION} -m pip install ${UPGRADE} ${USERMODE} pyside2 >> /dev/null
python${PYTHON_VERSION} -m pip install ${UPGRADE} ${USERMODE} pyusb >> /dev/null
python${PYTHON_VERSION} -m pip install ${UPGRADE} ${USERMODE} wheel >> /dev/null
python${PYTHON_VERSION} -m pip install ${UPGRADE} ${USERMODE} beautifulsoup4 >> /dev/null
python${PYTHON_VERSION} -m pip install ${UPGRADE} ${USERMODE} setuptools >> /dev/null
python${PYTHON_VERSION} -m pip install ${UPGRADE} ${USERMODE} requests >> /dev/null
# pybluez needs Windows Platform SDK
# bluepy is for Linux only and then failed to compile on Windows
# python${PYTHON_VERSION} -m pip install ${UPGRADE} ${USERMODE} pybluez

# Launch Pinguino's IDE
echo "Launch Pinguino's IDE ..."
cd ${PINGUINODIR}/pinguino-ide
python${PYTHON_VERSION} pinguino-ide.py
