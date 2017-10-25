#!/bin/bash
set -eu

export OPENCV_VERSION=2.4.13.2
cd /tmp
rm -f /tmp/${OPENCV_VERSION}.zip
rm -rf /tmp/opencv-${OPENCV_VERSION}
curl -L -O https://github.com/Itseez/opencv/archive/${OPENCV_VERSION}.zip
unzip ${OPENCV_VERSION}.zip -d .
ln -sf /usr/local/nvidia/lib64/libnvcuvid.so.1 /usr/lib/libnvcuvid.so
ln -sf /usr/local/nvidia/lib64/libcuda.so /usr/lib/libcuda.so
mkdir -p /tmp/opencv-${OPENCV_VERSION}/build
cd /tmp/opencv-${OPENCV_VERSION}/build
cmake \
  -DCMAKE_BUILD_TYPE=RELEASE \
  -DBUILD_TESTS=OFF \
  -DWITH_TBB=ON \
  -DWITH_OPENGL=ON \
  -DWITH_CUDA=ON \
  -DWITH_CUBLAS=ON \
  -DWITH_NVCUVID=ON \
  -DENABLE_FAST_MATH=ON \
  -DCUDA_FAST_MATH=ON \
  -DCUDA_CUDA_LIBRARY=/usr/lib/libcuda.so \
  ..
make -j
make install
rm -f /usr/lib/libcuda.so
rm -f /usr/lib/libnvcuvid.so
rm -f /tmp/${OPENCV_VERSION}.zip
rm -rf /tmp/opencv-${OPENCV_VERSION}