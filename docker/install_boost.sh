#!/bin/bash
set -eu

# http://www.boost.org/doc/libs/develop/libs/beast/doc/html/beast/quick_start.html

cd /tmp
git clone --recursive --depth 1 https://github.com/boostorg/boost.git
cd /tmp/boost
./bootstrap.sh
./b2 headers
./b2 install -j4 --without-mpi --without-python --disable-icu

rm -rf /tmp/boost