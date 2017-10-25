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