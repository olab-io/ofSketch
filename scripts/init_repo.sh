#!/bin/bash

# To use:
#
# ./init_repo.sh
#
# record the scripts location
SCRIPTS_DIR=`pwd`

# the current build OS
OS=`./helpers/ostype.sh`

# current of version
OF_VERSION="0.8.4"

# make the release file name.
if [[ $OS == windows ]]; then
    OF_RELEASE="of_v${OF_VERSION}_win_cb_release"
else
    OF_RELEASE="of_v${OF_VERSION}_${OS}_release"
fi

echo ""
echo "Initializing ofSketch"
echo ""
echo "--------------------------------------------------"
echo "        OS: ${OS}"
echo "OF_VERSION: ${OF_VERSION}"
echo "OF_RELEASE: ${OF_RELEASE}"
echo "--------------------------------------------------"
echo ""
echo "Initializing ofSketch repo ..."

if [[ $OS == linux* ]]; then
    SUFFIX="tar.gz"
    NUM_CORES=`grep -c ^processor /proc/cpuinfo`
elif [[ $OS == windows ]]; then
    SUFFIX="zip"
    # Number of cores may be set as an environmental variable
    NUM_CORES=8
else
    SUFFIX="zip"
    NUM_CORES=`sysctl hw.ncpu | awk '{print $2}'`
fi

# attempt to determine the installed version
if [ -e "openFrameworks/CHANGELOG.md" ]; then
    INSTALLED_OF_VERSION=$(head -n 1 openFrameworks/CHANGELOG.md | awk '{print $2}')
else
    INSTALLED_OF_VERSION="NONE"
fi

echo "ofSketch INSTALLED_OF_VERSION: ${INSTALLED_OF_VERSION}"
echo "ofSketch REQUIRED_OF_VERSION: ${OF_VERSION}"

if [[ $OF_VERSION != $INSTALLED_OF_VERSION ]]; then
    echo "Downloading ${OF_RELEASE} ..."

    rm -rf openFrameworks/
    mkdir openFrameworks/
    touch openFrameworks/.gitkeep

    # make a temporary directory for the download
    mkdir tmp/ &> /dev/null
    cd tmp/

    # download the latest release
    curl -O "http://www.openframeworks.cc/versions/v${OF_VERSION}/${OF_RELEASE}.${SUFFIX}"

    if [[ $OS == linux* ]]; then
        tar xvf ${OF_RELEASE}.${SUFFIX}
    else
        unzip ${OF_RELEASE}.${SUFFIX}
    fi

    # move the essential bits
    mv ${OF_RELEASE}/addons ../openFrameworks
    mv ${OF_RELEASE}/export ../openFrameworks
    mv ${OF_RELEASE}/libs ../openFrameworks
    mv ${OF_RELEASE}/LICENSE.md ../openFrameworks
    mv ${OF_RELEASE}/CHANGELOG.md ../openFrameworks

    cd ..
    rm -rf tmp/

    # copy the app only makefile
    mkdir -p openFrameworks/libs/openFrameworksCompiled/project/makefileCommon/
    cp ${SCRIPTS_DIR}/patches/libs/openFrameworksCompiled/project/makefileCommon/* openFrameworks/libs/openFrameworksCompiled/project/makefileCommon/

    # copy the 10.9 / Xcode 6 compatible makefiles
    if [[ $OS == osx ]]; then
        cp ${SCRIPTS_DIR}/patches/libs/openFrameworksCompiled/project/osx/config.osx.default.mk openFrameworks/libs/openFrameworksCompiled/project/osx/
    fi

    # copy the win_cb compatible makefile
    if [[ $OS == windows ]]; then
        mkdir -p openFrameworks/libs/openFrameworksCompiled/project/win_cb/
        cp ${SCRIPTS_DIR}/patches/libs/openFrameworksCompiled/project/win_cb/config.win_cb.default.mk openFrameworks/libs/openFrameworksCompiled/project/win_cb/
    fi

    echo "Building openFrameworks distribution ..."

    cd openFrameworks/libs/openFrameworksCompiled/project 

    # copy the win_cb compatible makefile
    if [[ $OS == windows ]]; then
        make -j${NUM_CORES} -s HOST_OS=MINGW32_NT-6.2 PLATFORM_OS=MINGW32_NT-6.2 PLATFORM_ARCH=i686 Release
    else
        make -j${NUM_CORES} -s Release
    fi

    cd ${SCRIPTS_DIR}/openFrameworks

    rm -rf $(find . -name obj)
    rm -rf $(find . -name .git*)
    rm -rf $(find . -name .DS_Store)

    cd ${SCRIPTS_DIR}
    rm -rf ../ofSketchApp/bin/data/openFrameworks
    cp -r openFrameworks ../ofSketchApp/bin/data
fi

echo ""
echo "ofSketch initialization Complete!"
