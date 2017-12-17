#!/usr/bin/env bash

cd ..
mkdir -p build
cd build
cmake ../code
make
./HWC2_TEST
