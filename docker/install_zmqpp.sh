#!/bin/bash
set -eu

export LIBSODIUM_VERSION=1.0.15
cd /tmp
wget https://github.com/jedisct1/libsodium/releases/download/${LIBSODIUM_VERSION}/libsodium-${LIBSODIUM_VERSION}.tar.gz
tar -xf libsodium-${LIBSODIUM_VERSION}.tar.gz
cd /tmp/libsodium-${LIBSODIUM_VERSION}
./autogen.sh
./configure
make check
make install
ldconfig
rm -rf /tmp/libsodium-${LIBSODIUM_VERSION}.tar.gz
rm -rf /tmp/libsodium-${LIBSODIUM_VERSION}

apt-get install -y libzmq3-dev

cd /tmp
export ZMQPP_VERSION=4.1.2
wget https://github.com/zeromq/zmqpp/archive/${ZMQPP_VERSION}.zip
unzip -o ${ZMQPP_VERSION}.zip
cd /tmp/zmqpp-${ZMQPP_VERSION}
make
make check
make install
make installcheck
ldconfig
rm -rf /tmp/zmqpp-${ZMQPP_VERSION}
rm -rf /tmp/${ZMQPP_VERSION}.zip