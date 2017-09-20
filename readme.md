## install build tools

```sh
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-7 g++-7
sudo apt install cmake build-essential
```

## install libraries

```sh
sudo apt install libtbb-dev
sudo apt install libboost-all-dev
sudo apt install libatlas-base-dev liblapack-dev
sudo apt install libopencv-dev
sudo apt install ffmpeg
```

```sh
cd ./third_party
git submodule update --init --recursive --depth 1
```

### ffmpeg

```sh
cd third_party
git clone --recursive --depth 1 https://github.com/FFmpeg/FFmpeg.git
cd FFmpeg
sudo apt-get -y install autoconf automake build-essential libass-dev libfreetype6-dev \
  libsdl2-dev libtheora-dev libtool libva-dev libvdpau-dev libvorbis-dev libxcb1-dev libxcb-shm0-dev \
  libxcb-xfixes0-dev pkg-config texinfo wget zlib1g-dev
sudo apt-get install yasm nasm
sudo apt-get install libx264-dev
sudo apt-get install libmp3lame-dev
sudo apt-get install libfdk-aac-dev
sudo apt-get install libvpx-dev
sudo apt-get install libopus-dev
PATH=../../local/bin:$PATH \
PKG_CONFIG_PATH=../../local/lib/pkgconfig \
./configure \
  --pkg-config-flags="--static" \
  --extra-cflags="-I../../local/include" \
  --extra-ldflags="-L../../local/lib" \
  --bindir=../../local/bin \
  --prefix=../../local \
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
PATH=../../local/bin:$PATH \
make -j4
make install
hash -r
```


### opencv

```sh
cd third_party
git clone --recursive --depth 1 https://github.com/opencv/opencv.git
cd opencv
sudo apt-get install build-essential
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
mkdir -p build
cd build
cmake \
  -DCMAKE_BUILD_TYPE=RELEASE \
  -DBUILD_DOCS=OFF \
  -DBUILD_EXAMPLES=OFF \
  -DBUILD_TESTS=OFF \
  -BUILD_PERF_TESTS=OFF \
  -DBUILD_WITH_DEBUG_INFO=OFF \
  -DBUILD_SHARED_LIBS=OFF \
  -DWITH_OPENCL=ON \
  -DWITH_CUDA=OFF \
  -DWITH_TBB=ON \
  -DCMAKE_INSTALL_PREFIX=../../../local \
  ../
make -j4
make install
```

### boost beast
* http://www.boost.org/doc/libs/develop/libs/beast/doc/html/beast/quick_start.html

```sh
cd third_party
git clone --recursive --depth 1 https://github.com/boostorg/boost.git
cd boost
./bootstrap.sh
./b2 headers
./b2 install -j4 --prefix=../../local
```

### libwebrtc
* https://qiita.com/alivelime/items/f1447570eb96eb101cf4

```sh
cd third_party
mkdir webrtc-checkout
cd webrtc-checkout
fetch --nohooks webrtc
gclient sync
cd src
gn gen out/Release --args='rtc_use_h264=true rtc_use_lto=true'
ninja -C out/Release
cd ..
mkdir -p ../../local/include/webrtc
cp -rf src/api ../../local/include/webrtc/
cp -rf src/base ../../local/include/webrtc/
cp -rf src/p2p ../../local/include/webrtc/
cp -rf src/pc ../../local/include/webrtc/
cp -rf src/rtc_base ../../local/include/webrtc/
cp -rf src/video ../../local/include/webrtc/
cp -rf src/audio ../../local/include/webrtc/
cp -rf src/media ../../local/include/webrtc/
cp -rf src/ortc ../../local/include/webrtc/
```

### json
* http://pickles-ochazuke.hatenablog.com/entry/2017/08/02/172805

```sh
cd third_party
git clone --depth 1 --recursive https://github.com/nlohmann/json.git 
cd json
mkdir -p ../../local/include/json
cp src/json.hpp ../../local/include/json/
```

### crow

```sh
sudo apt install libboost-all-dev
git clone --depth 1 --recursive https://github.com/ipkn/crow.git
cd crow
mkdir build
cd build
cmake ..
make -j4
cd ..
mkdir -p ../../local/include/crow
cp amalgamate/crow_all.h ../../local/include/crow/crow.h
```


## libfoo.so を作る
gcc -c foo.cpp
gcc -shared -o libfoo.so foo.o 

## libfoo.so を動的リンク
gcc -c main.cpp
gcc -o main main.o /path/to/libfoo.so # パスでリンク
gcc -o main main.o -lfoo # $LIBRARY_PATH でリンク

## libfoo.so を静的リンク
gcc -c main.cpp
gcc -o main main.o /path/to/libfoo.so # パスでリンク
gcc -static -o main main.o -lfoo # -l を -static でリンク
