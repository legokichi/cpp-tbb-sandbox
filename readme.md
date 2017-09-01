
```sh
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-7 g++-7
sudo apt install cmake build-essential
sudo apt install libtbb-dev
sudo apt install libboost-all-dev
sudo apt install libatlas-base-dev liblapack-dev 
sudo apt install libopencv-dev
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
