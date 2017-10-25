#!/bin/bash
set -eu


cd /tmp
git clone --recursive --depth 1 https://github.com/FFmpeg/FFmpeg.git
cd /tmp/FFmpeg
apt-get install -y --no-install-recommends \
  autoconf automake build-essential libass-dev libfreetype6-dev \
  libsdl2-dev libtheora-dev libtool libva-dev libvdpau-dev libvorbis-dev libxcb1-dev libxcb-shm0-dev \
  libxcb-xfixes0-dev pkg-config texinfo wget zlib1g-dev \
  yasm nasm \
  libx264-dev libmp3lame-dev libfdk-aac-dev \
  libvpx-dev libopus-dev

./configure \
  --pkg-config-flags="--static" \
  \
  --enable-gpl \
  --enable-version3 \
  --enable-nonfree \
  \
  --enable-static \
  --disable-shared \
  --disable-doc \
  \
  --disable-ffplay \
  --disable-ffprobe \
  --disable-ffserver \
  \
  --enable-libass \
  --enable-libfdk-aac \
  --enable-libfreetype \
  --enable-libmp3lame \
  --enable-libopus \
  --enable-libvpx \
  --enable-libx264
make -j4
make install
hash -r

