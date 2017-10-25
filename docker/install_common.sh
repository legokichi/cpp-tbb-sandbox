#!/bin/bash
set -eu

apt-get install -y libtbb2 libtbb-dev
apt-get install -y libopenblas-dev liblapacke-dev # libatlas-base-dev liblapack-dev
apt-get install -y libeigen3-dev
apt-get install -y libopencv-dev