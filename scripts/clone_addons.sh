#!/bin/bash

if [ -z $1 ]; then
    PREFIX="https://github.com/"
else
    PREFIX="git@github.com:"
fi

PWD=`pwd`

ADDONS_MAKE=${PWD}/../ofSketchApp/addons.make

OF_ROOT=../../..

ADDONS_PATH=${OF_ROOT}/addons

cd $ADDONS_PATH

while read ADDON; do

	# clone the repo and consume any errors
	git clone ${PREFIX}bakercp/${ADDON}.git &> /dev/null

	cd ${ADDON}

	# update to the latest

  echo `pwd`

	if [ ${ADDON} == ofxHTTP ]; then
    git checkout develop
  elif [ ${ADDON} == ofxNetworkUtils ]; then
    git checkout develop
  else
    git checkout master
  fi

	git pull


	# back to addons path
	cd -

done < $ADDONS_MAKE
