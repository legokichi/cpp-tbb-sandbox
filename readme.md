saninini  ninininiinninini

```sh
sudo docker build \
  --build-arg user_id=$UID \
  --build-arg group_id=$GID \
  -t vscode \
  ./
xhost +local:
docker run \
  --rm \
  --env="DISPLAY=$DISPLAY" \
  --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
  --volume="$(pwd):/opt/workdir" \
  --workdir="/opt/workdir" \
  -ti \
  vscode \
    /usr/bin/code --verbose ./
ibus-setup
```



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

## core dump
* http://d.hatena.ne.jp/JiaLu/20120113/1330396897

```sh
ulimit -c unlimited
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
sudo apt install libtbb2 libtbb-dev
#sudo apt install libboost-all-dev
sudo apt install libeigen3-dev libopenblas-dev liblapacke-dev # libatlas-base-dev liblapack-dev
sudo apt install libopencv-dev
```

```sh
cd ./third_party
git submodule update --init --recursive --depth 1
```

### boost beast
* http://www.boost.org/doc/libs/develop/libs/beast/doc/html/beast/quick_start.html

```sh
cd third_party
git clone --recursive --depth 1 https://github.com/boostorg/boost.git
cd boost
./bootstrap.sh
./b2 headers
./b2 install -j4 --without-mpi --without-python --disable-icu --prefix=../../local
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

### ffmpeg

```sh
git clone --recursive --depth 1 https://github.com/FFmpeg/FFmpeg.git
cd FFmpeg
sudo apt-get install -y  autoconf automake build-essential libass-dev libfreetype6-dev \
  libsdl2-dev libtheora-dev libtool libva-dev libvdpau-dev libvorbis-dev libxcb1-dev libxcb-shm0-dev \
  libxcb-xfixes0-dev pkg-config texinfo wget zlib1g-dev
sudo apt-get install -y yasm nasm
sudo apt-get install -y libx264-dev
sudo apt-get install -y libmp3lame-dev
sudo apt-get install -y libfdk-aac-dev
sudo apt-get install -y libvpx-dev
sudo apt-get install -y libopus-dev
PKG_CONFIG_PATH=../../local/lib/pkgconfig ./configure \
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
make -j4
make install
hash -r
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
  -DCMAKE_INSTALL_PREFIX="/home/legokichi/Github/cpp-tbb-sandbox/local/" \
  -DWEBRTC_ROOT_DIR="/home/legokichi/Github/cpp-tbb-sandbox/third_party/webrtc-checkout/src/" \
  -DWEBRTC_INCLUDE_DIR="/home/legokichi/Github/cpp-tbb-sandbox/third_party/webrtc-checkout/src/" \
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
