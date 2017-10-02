FROM ubuntu:16.04
ENV DEBIAN_FRONTEND "noninteractive"
RUN apt-get update
RUN apt-get -y \
  -o Dpkg::Options::="--force-confdef" \
  -o Dpkg::Options::="--force-confold" dist-upgrade

RUN apt-get install -y --no-install-recommends \
  software-properties-common \
  ppa-purge \
  build-essential \
  zip unzip zlib1g-dev \
  openssl libssl-dev \
  bzip2 libbz2-dev  \
  binutils \
  zsh vim git \
  cmake autoconf automake pkg-config \
  wget curl \
  screen tree htop \
  sudo

RUN add-apt-repository ppa:ubuntu-toolchain-r/test

RUN echo "# http://apt.llvm.org" >> /etc/apt/sources.list
RUN echo "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-5.0 main" >> /etc/apt/sources.list
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -

RUN apt-get update
RUN apt-get install -y --no-install-recommends \
  clang-5.0 lldb-5.0 lld-5.0 \
  gcc-7 g++-7


## install deps

### boost

WORKDIR /opt
RUN git clone --recursive --depth 1 https://github.com/boostorg/boost.git
WORKDIR /opt/boost
RUN ./bootstrap.sh
RUN ./b2 headers
RUN ./b2 install -j4  --without-mpi --without-python --disable-icu


### libwebrtc

WORKDIR /opt
RUN git clone --recursive --depth 1 https://chromium.googlesource.com/chromium/tools/depot_tools.git
ENV PATH=$PATH:/opt/depot_tools

WORKDIR /opt/webrtc-checkout
RUN fetch --nohooks webrtc
WORKDIR /opt/webrtc-checkout/src
RUN git checkout -b libwebrtc59 refs/remotes/branch-heads/59
RUN gclient sync --with_branch_heads
RUN gn gen out/Release --args='rtc_use_h264=true rtc_use_lto=true'
RUN ninja -C out/Release


### libsourcey

WORKDIR /opt
RUN apt-get install -y ffmpeg
WORKDIR /opt
RUN git clone --recursive --depth 1 https://github.com/sourcey/libsourcey.git
WORKDIR /opt/libsourcey
RUN mkdir build
WORKDIR /opt/libsourcey/build
RUN cmake \
  -DWEBRTC_ROOT_DIR=/opt/webrtc-checkout/src/ \
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
  -DWITH_FFMPEG=OFF \
  -DWITH_OPENCV=OFF \
  -DWITH_WEBRTC=ON \
  ..
RUN make -j
RUN make install

