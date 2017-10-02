```sh
docker build -t cpp_tbb .
docker run --rm --net=host -v `pwd`:/opt/cpp_tbb --workdir=/opt/cpp_tbb -ti cpp_tbb /usr/bin/zsh
```

## vscode shortcut

* `Ctrl + Shift + P` - command
* `Ctrl + Shift + B` - build
* `Ctrl + Shift + I` - code format all (or right click)
* `Ctrl + K Ctrl + F` - selective format (or right click)
* `Ctrl + T` - symbol search
* `Ctrl + Shift + F10` - Peek Definition
* `F12` - Go to Definition
* `F9` - add break point
* `Ctrl + Shift + D` - open debugger
* `F5` - start debugging
* `-exec -enable-pretty-printing` - input debug console

## install debug info
* gdb usage
  * https://doss.eidos.ic.i.u-tokyo.ac.jp/html/gdb_step_into_libraries.html
  * http://d.hatena.ne.jp/embedded/20130120/p1
  * https://www.hiroom2.com/2016/07/14/ubuntu-16-04-apt-sourceでソースコードをダウンロードする/

```sh
sudo apt libc6-dbg
mkdir debug
cd debug
apt source libc6
```

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
git checkout -b libsourcey refs/remotes/branch-heads/59
gclient sync
gn gen out/Release --args='rtc_use_h264=true rtc_use_lto=true'
ninja -C out/Release
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

### libsourcey
first, build libwebrtc on branch-heads/59

```sh
sudo apt-get update
sudo apt-get install -y build-essential pkg-config git cmake openssl libssl-dev
sudo apt-get install --yes libavcodec-ffmpeg-dev libavdevice-ffmpeg-dev libavfilter-ffmpeg-dev libavformat-ffmpeg-dev libswresample-ffmpeg-dev libpostproc-ffmpeg-dev
git clone --recursive --depth 1 https://github.com/sourcey/libsourcey.git
cd libsourcey
mkdir build
cd build
cmake \
  -DCMAKE_INSTALL_PREFIX="/home/legokichi/Github/cpp_tbb/local/" \
  -DWEBRTC_ROOT_DIR="/home/legokichi/Github/cpp_tbb/third_party/webrtc-checkout/src/" \
  -DWEBRTC_INCLUDE_DIR="/home/legokichi/Github/cpp_tbb/third_party/webrtc-checkout/src/" \
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
make -j4
make install
```

### cpp-netlib

```sh
git clone --recursive --depth 1 https://github.com/cpp-netlib/cpp-netlib.git
mkdir -p cpp-netlib/build
cd cpp-netlib/build
cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DBoost_INCLUDE_DIR="/home/legokichi/Github/cpp_tbb/local/include" \
  -DBoost_LIBRARY_DIR="/home/legokichi/Github/cpp_tbb/local/lib" \
  -DCMAKE_INSTALL_PREFIX="/home/legokichi/Github/cpp_tbb/local/" \
  -DBUILD_TESTS=OFF \
  ..
make -j4
make install
mkdir ../deps/uri/build
cd ../deps/uri/build
cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_TESTS=OFF \
  -DCMAKE_INSTALL_PREFIX="/home/legokichi/Github/cpp_tbb/local/" \
  ..
```
### uri

```sh
git clone --depth 1 --recursive https://github.com/cpp-netlib/uri
mkdir -p uri/build
cd uri/build
cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="/home/legokichi/Github/cpp_tbb/local/" \
  ..
make -j4
ctest
make install
```