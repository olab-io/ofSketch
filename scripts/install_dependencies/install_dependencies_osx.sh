#!/bin/bash

if [ $EUID != 0 ]; then
	echo "This script must be run with adminstrator privileges."
	echo ""
	echo "usage:"
	echo "sudo ./install_dependencies.sh"
	exit $exit_code
   exit 1
fi

OSX_VERSION=`sw_vers -productVersion`

OSX_VERSION_MAJOR=`echo $OSX_VERSION | cut -d. -f1`
OSX_VERSION_MINOR=`echo $OSX_VERSION | cut -d. -f2`
OSX_VERSION_REVISION=`echo $OSX_VERSION | cut -d. -f3`

echo $OSX_VERSION_MAJOR.$OSX_VERSION_MINOR

ACTIVE_DEVELOPER_DIRECTORY=`xcode-select --print-path`

echo $ACTIVE_DEVELOPER_DIRECTORY

case "$ACTIVE_DEVELOPER_DIRECTORY" in
	*Xcode*)
		echo "You already have XCode."
	;;
	*CommandLineTools*)
		echo "You have some sort of command line tools."
	;;
	*/*)
		echo "You have set the print-path ..."
	;;
	*)
		echo "Could not detect anything."
esac

case "$OSX_VERSION" in
	10.9*)
		echo "You are running 10.9"
	;;
	10.8*)
		echo "You are running 10.8"
	;;
	*)
		echo "ofSketch does not support OSX ${OSX_VERSION_MAJOR}.${OSX_VERSION_MINOR}"
	;;
esac

