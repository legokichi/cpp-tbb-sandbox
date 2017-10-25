
# http://pickles-ochazuke.hatenablog.com/entry/2017/08/02/172805

cd third_party
git clone --depth 1 --recursive https://github.com/nlohmann/json.git 
cd json
mkdir -p ../../local/include/json
cp src/json.hpp ../../local/include/json/