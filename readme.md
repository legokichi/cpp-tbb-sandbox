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
cd thrid_party
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
cd thrid_party
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

### boost

```sh
cd thrid_party
git clone --recursive --depth 1 https://github.com/boostorg/boost.git
cd boost
./bootstrap.sh
./b2 install -j4 --prefix=../../local
```

### janus

```sh
cd thrid_party
git clone https://github.com/meetecho/janus-gateway.git
cd janus-gateway
sudo apt install libmicrohttpd-dev libjansson-dev libnice-dev \
	libssl-dev libsrtp-dev libsofia-sip-ua-dev libglib2.0-dev \
	libopus-dev libogg-dev libcurl4-openssl-dev pkg-config gengetopt \
	libtool automake
apt install libwebsockets
(
  wget https://github.com/cisco/libsrtp/archive/v1.5.4.tar.gz;
  tar xfv v1.5.4.tar.gz;
  cd libsrtp-1.5.4;
  ./configure --prefix=/usr --enable-openssl;
  make -j4 shared_library;
  sudo make install;
)
sh autogen.sh
./configure \
  --prefix=/opt/janus \
  --disable-websockets \
  --disable-data-channels \
  --disable-rabbitmq \
  --disable-mqtt
make -j4
sudo make install
sudo make configs
```

### libwebrtc
* https://qiita.com/alivelime/items/f1447570eb96eb101cf4

```sh
cd thrid_party
mkdir webrtc-checkout
cd webrtc-checkout
fetch --nohooks webrtc
gclient sync
gn gen out/Default --args='rtc_use_h264=true use_openh264=true'
ninja -C out/Default
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
