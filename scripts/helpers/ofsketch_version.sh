#!/bin/bash

VERSION_MAJOR=`cat ${1} | grep VERSION_MAJOR | awk '{ print $3 }'`
VERSION_MINOR=`cat ${1} | grep VERSION_MINOR | awk '{ print $3 }'`
VERSION_PATCH=`cat ${1} | grep VERSION_PATCH | awk '{ print $3 }'`
VERSION_SPECIAL=`cat ${1} | grep VERSION_SPECIAL | awk '{ print $3 }' | sed 's/\"//g'`

if [ -z "$VERSION_SPECIAL" ]; then
  echo ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
else
  echo ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SPECIAL}
fi
