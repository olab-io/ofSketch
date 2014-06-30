#!/bin/bash

# To use:
#
# ./build_release.sh
#

# record the scripts' location
SCRIPTS_DIR=`pwd`

# the current build OS
OS=`./helpers/ostype.sh`

# the current semantic version number, strip the "v"
OF_SKETCH_VERSION=`git describe --abbrev=0 --tags | sed 's/v//g'`

cd ../ofSketchApp/bin/data

if [ -e "openFrameworks/CHANGELOG.md" ]
then
    INSTALLED_OF_VERSION=$(head -n 1 openFrameworks/CHANGELOG.md | awk '{print $2}')
else
    echo "No openFrameworks distribution installed in the ofSketch/bin/data/openFrameworks folder."
    echo "Please run the init_repo.sh script and then try again."
    exit 1
fi

OF_SKETCH_RELEASE="ofSketch_v${OF_SKETCH_VERSION}_${OS}_release"


echo ""
echo "Building ofSketch Release"
echo ""
echo "--------------------------------------------------"
echo "               OS: ${OS}"
echo "       OF_VERSION: ${INSTALLED_OF_VERSION}"
echo "OF_SKETCH_VERSION: ${OF_SKETCH_VERSION}"
echo "OF_SKETCH_RELEASE: ${OF_SKETCH_RELEASE}"
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

cd $SCRIPTS_DIR

mkdir ../releases &> /dev/null
cd ../releases 
rm -rf $OF_SKETCH_RELEASE &> /dev/null

mkdir -p $OF_SKETCH_RELEASE/data

cd $OF_SKETCH_RELEASE/data

echo `pwd`

echo "Copy app data ..."
cp -r ../../../ofSketchApp/bin/data/* .

cd ..

echo `pwd`

echo "Copy text docs..."
cp ../../CONTRIBUTING.md .
cp ../../LICENSE.md .
cp ../../README.md .
cp ../../CHANGELOG.md .

echo `pwd`
echo "Cleaning distro..."

cd data/Projects

echo `pwd`

# remove any built project apps
rm -rf $(find . -name *.app)
rm -rf $(find . -name *App)
rm -rf $(find . -name libs)

cd ../..

echo `pwd`

rm -rf $(find . -name obj)
rm -rf $(find . -name .git*)
rm -rf $(find . -name .DS_Store)

echo `pwd`

echo "Copy app ..."

echo `pwd`

if [[ $OS == linux* ]]
then
    cp ../../ofSketchApp/bin/ofSketchApp ofSketch
else
    cp -r ../../ofSketchApp/bin/ofSketchApp.app ofSketch.app
fi

echo "Compressing ..."

cd ..

if [[ $OS == linux* ]]
then
    tar -cvzf $OF_SKETCH_RELEASE.tar.gz $OF_SKETCH_RELEASE 
else
    zip -r $OF_SKETCH_RELEASE.zip $OF_SKETCH_RELEASE 
fi

echo "${OF_SKETCH_RELEASE} Complete!"
