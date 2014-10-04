#!/bin/bash

# the current build OS
OS=`./helpers/ostype.sh`

# move into the directory
cd ../ofSketchApp/bin/data/

if [[ $OS == windows ]]; then
    TOOLCHAIN_VERSION=0.1.0
    rm -rf Toolchains
    mkdir Toolchains
    cd Toolchains
    curl https://codeload.github.com/bakercp/ofMinGW/zip/${TOOLCHAIN_VERSION} -o ofMinGW-${TOOLCHAIN_VERSION}.zip
    unzip ofMinGW-${TOOLCHAIN_VERSION}.zip
    mv ofMinGW-${TOOLCHAIN_VERSION} ofMinGW
    rm ofMinGW-${TOOLCHAIN_VERSION}.zip
    cd ..
fi
