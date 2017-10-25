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