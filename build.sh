#!/bin/bash
#
# Script for building the WARG computer-vision project
#
# Notes: the -c and -t parameters should not be removed
#           since they are used by the jenkins server when buiding
#
# This file is part of WARG's computer-vision
#
# Copyright (c) 2015-2016, Waterloo Aerial Robotics Group (WARG)
# All rights reserved.
#
# This software is licensed under a modified version of the BSD 3 clause license
# that should have been included with this software in a file called COPYING.txt
# Otherwise it is available at:
# https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
#

while getopts "c,t,h,i,d:" opt; do
    case $opt in
        d)
            WORKSPACE_DIR=$OPTARG
        ;;
        c)
            CLEAN=true
        ;;
        t)
            TEST=true
        ;;
	i)
	    INSTALL=true
	;;
        h|\?)
            printf "%s\n" "Usage: $0 [OPTIONS]" \
                "Script to build the WARG computer-vision project"\
                "   Should either be run in the root of computer-vision or with the --dir option"\
                "   specifying the project root"\
                "    -c [clean]         - removes previous build files before building" \
                "    -d [directory]     - builds with the given dir as the project root" \
                "    -h [help]          - outputs this message" \
                "    -t [test]          - runs tests after building if build is successful"
            exit 1
        ;;
    esac
done

if [ -z $WORKSPACE_DIR ]; then
    WORKSPACE_DIR=`pwd`
fi

if [[ $CLEAN == true ]]; then
    echo "Cleaning old build environment"
    rm -rf $WORKSPACE_DIR/build
    mkdir $WORKSPACE_DIR/build
fi

mkdir -p $WORKSPACE_DIR/build
cd $WORKSPACE_DIR/build
cmake .. && make

RESULT=$?

if [[ !$RESULT && $TEST ]] ; then
    make test
fi

if [[ !$RESULT && $INSTALL ]] ; then
    sudo make install
fi
