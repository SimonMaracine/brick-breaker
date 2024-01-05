echo off

cd ..
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBB_CHRONO_TIMER=ON -A x64
cd ..\scripts

pause
