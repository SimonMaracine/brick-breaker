#! /bin/bash

cd ..
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBB_CHRONO_TIMER=ON
