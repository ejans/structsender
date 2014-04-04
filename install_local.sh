#!/bin/bash

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=${UBX_MODULES}/../.. ..
make
make install
