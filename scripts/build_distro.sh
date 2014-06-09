#!/bin/bash

# To use:
#
# ./build_distro.sh SYSTEM VERSION 
#
# ./build_distro.sh osx 0.1.1 

# the current build system
SYSTEM=$1

# the current semantic version number
OF_SKETCH_VERSION=$2

# current of version
OF_VERSION="0.8.1"

# make the release file name.
OF_RELEASE="of_v${OF_VERSION}_${SYSTEM}_release"

echo "Building..."
echo "           SYSTEM: ${SYSTEM}"
echo "OF_SKETCH_VERSION: ${OF_SKETCH_VERSION}"
echo "       OF_VERSION: ${OF_VERSION}"

# change from the scripts to the build directory
cd ../build/

# make the distro directory name
DIST="ofSketch_v${OF_SKETCH_VERSION}_${SYSTEM}_release"

# make the directory and pipe errors to /dev/null if it exists
rm -r $DIST/ &> /dev/null

mkdir -p $DIST/data

cd $DIST/data

echo "Copy app data ..."
cp -r ../../../ofSketchApp/bin/data/DocumentRoot .
cp -r ../../../ofSketchApp/bin/data/media .
cp -r ../../../ofSketchApp/bin/data/Projects .
cp -r ../../../ofSketchApp/bin/data/Resources .
cp -r ../../../ofSketchApp/bin/data/ssl .
cp -r ../../../ofSketchApp/bin/data/uploads .

mkdir openFrameworks && cd openFrameworks

echo "Copy thinned openFrameworks distribution ..."

cp -r ../../../$OF_RELEASE/addons .
cp -r ../../../$OF_RELEASE/export . 
cp -r ../../../$OF_RELEASE/libs . 
cp -r ../../../$OF_RELEASE/LICENSE.md . 

echo "Pre-building openFrameworks libs..."

cd libs/openFrameworksCompiled/project

make -s -j10 Release

# back to $DIST folder
cd ../../../../..

echo "Copy app bundle..."
cp -r ../../ofSketchApp/bin/ofSketchAppDebug.app ofSketch.app

echo "Copy text docs..."
cp ../../CONTRIBUTING.md .
cp ../../LICENSE.md .
cp ../../README.md .
cp ../../CHANGELOG.md .

echo "Cleaning distro..."

cd data/Projects

rm -rf $(find . -name *.app)
rm -rf $(find . -name obj)

cd ../..

rm -rf $(find . -name .git*)
rm -rf $(find . -name .DS_Store)

echo "Making zip..."

cd ..

zip -r $DIST.zip $DIST 
