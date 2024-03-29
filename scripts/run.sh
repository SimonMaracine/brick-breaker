#! /bin/bash

if [ ! -z "$1" ]; then
    TARGET="$1"
else
    echo "No target specified"
    exit 1
fi

./build.sh $TARGET

if [ "$?" -ne 0 ]; then
    exit 1
fi

cd ..
build/clients/$TARGET/$TARGET
