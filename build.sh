#!/bin/bash

while getopts "c,t,h,--clean,--test,--help" opt; do
    case $opt in
        c|--clean)
            CLEAN=true
        ;;
        t|--test)
            TEST=true
        ;;
        h|--help|\?)
            printf "%s\n" "Usage: $0 [OPTIONS]" \
                "    -h, --help - outputs this message" \
                "    -c, --clean - removes previous build files before building" \
                "    -t, --test - runs tests after building if build is successful"
            exit
        ;;
    esac
done

if [[ $CLEAN == true ]]; then
    echo "Cleaning old build environment"
    rm -rf build
    mkdir build
fi

cd build
cmake .. && make

if [[ !$? && $TEST ]] ; then
    make test
fi
