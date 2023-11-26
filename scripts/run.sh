#! /bin/bash

./build.sh brick-breaker

if [ "$?" -ne 0 ]; then
    exit 1
fi

cd ..
build/brick-breaker
