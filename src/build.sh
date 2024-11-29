#!/bin/bash

export PIN_ROOT=/home/ksgoura/os_aware_memory_controller/pin-3.15-98253-gb56e429b1-gcc-linux
cd ramulator2/
mkdir ./build
cd build
cmake ..
make
cd ../../
cd ./pin/pin_exec
make -j 16
cd ../../
cmake .
make -j 16