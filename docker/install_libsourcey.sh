#!/bin/bash
set -eu

#apt-get install --yes libavcodec-ffmpeg-dev libavdevice-ffmpeg-dev libavfilter-ffmpeg-dev libavformat-ffmpeg-dev libswresample-ffmpeg-dev libpostproc-ffmpeg-dev

cd /tmp
git clone --recursive --depth 1 https://github.com/sourcey/libsourcey.git
mkdir /tmp/libsourcey/build
cd /tmp/libsourcey/build
cmake \
  -DWEBRTC_ROOT_DIR="/opt/webrtc-checkout/src/" \
  -DWEBRTC_INCLUDE_DIR="/opt/webrtc-checkout/src/" \
  -DCMAKE_BUILD_TYPE=RELEASE \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_MODULES=OFF \
  -DBUILD_APPLICATIONS=OFF \
  -DBUILD_SAMPLES=OFF \
  -DBUILD_TESTS=OFF \
  -DBUILD_MODULE_archo=ON \
  -DBUILD_MODULE_base=ON \
  -DBUILD_MODULE_crypto=ON \
  -DBUILD_MODULE_http=ON \
  -DBUILD_MODULE_json=ON \
  -DBUILD_MODULE_av=ON \
  -DBUILD_MODULE_net=ON \
  -DBUILD_MODULE_pacm=ON \
  -DBUILD_MODULE_pluga=ON \
  -DBUILD_MODULE_sked=ON \
  -DBUILD_MODULE_socketio=ON \
  -DBUILD_MODULE_stun=ON \
  -DBUILD_MODULE_symple=ON \
  -DBUILD_MODULE_turn=ON \
  -DBUILD_MODULE_util=ON \
  -DBUILD_MODULE_uv=ON \
  -DBUILD_MODULE_webrtc=ON \
  -DWITH_FFMPEG=ON \
  -DWITH_OPENCV=ON \
  -DWITH_WEBRTC=ON \
  ..
make -j
make install

rm -rf /tmp/libsourcey