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
OF_VERSION="0.8.2-RC1"

# make the release file name.
OF_RELEASE="of_v${OF_VERSION}_${OS}_release"

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

if [[ $OS == linux* ]]
then
    SUFFIX="tar.gz"
    NUM_CORES=`grep -c ^processor /proc/cpuinfo`
else
    SUFFIX="zip"
    NUM_CORES=`sysctl hw.ncpu | awk '{print $2}'`
fi

# move into the directory
cd ../ofSketchApp/bin/data/

# attempt to determin the installed version

if [ -e "openFrameworks/CHANGELOG.md" ]
then
    INSTALLED_OF_VERSION=$(head -n 1 openFrameworks/CHANGELOG.md | awk '{print $2}')
else
    INSTALLED_OF_VERSION="NONE"
fi

echo ""
echo "ofSketch INSTALLED_OF_VERSION: ${INSTALLED_OF_VERSION}"
echo "ofSketch REQUIRED_OF_VERSION: ${OF_VERSION}"
echo ""


if [[ $OF_VERSION != $INSTALLED_OF_VERSION ]]
then
    echo "Downloading ${OF_RELEASE} ..."

    rm -rf openFrameworks/
    mkdir openFrameworks/
    touch openFrameworks/.gitkeep

    # make a temporary directory for the download
    mkdir tmp/ &> /dev/null
    cd tmp/

    # download the latest release
    curl -O "http://www.openframeworks.cc/versions/v${OF_VERSION}/${OF_RELEASE}.${SUFFIX}"

    if [[ $OS == linux* ]]
    then
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
fi

echo "Building openFrameworks distribution ..."

cd openFrameworks/libs/openFrameworksCompiled/project 

make -j${NUM_CORES} -s Release

echo ""
echo "ofSketch initialization Complete!"
