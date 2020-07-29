#unzip ./external.zip
mkdir -p ./build/linux
cd build
cmake ..
make -j4
cd ..
./build/linux/graphvisualizer
