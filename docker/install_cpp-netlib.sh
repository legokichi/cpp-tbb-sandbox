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