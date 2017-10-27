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



#RUN apt-get install -y yasm

#WORKDIR /tmp
#RUN wget -O - http://download.videolan.org/pub/x264/snapshots/x264-snapshot-20171025-2245-stable.tar.bz2 | tar -xjf -
#WORKDIR /tmp/x264-snapshot-20171025-2245-stable
#RUN ./configure \
#  --enable-static \
#  --enable-shared \
#  --enable-pic
#RUN make -j
#RUN make install

#RUN apt-get install -y libmp3lame-dev libfdk-aac-dev

#WORKDIR /tmp 
##RUN git clone --depth 1 http://source.ffmpeg.org/git/ffmpeg.git
#RUN wget -O - https://github.com/FFmpeg/FFmpeg/archive/n2.8.12.tar.gz | tar -xzf -
#WORKDIR /tmp/FFmpeg-n2.8.12
#RUN ./configure \
#  --enable-nonfree \
#  --enable-gpl \
#  --enable-version3 \
#  --cpu=native \
#  --enable-libx264 \
#  --enable-libmp3lame \
#  --enable-libfdk-aac \
#  --disable-programs \
#  --disable-ffplay \
#  --disable-ffprobe \
#  --disable-ffserver \
#  --disable-doc
#RUN make -j
#RUN make install
#RUN ldconfig
