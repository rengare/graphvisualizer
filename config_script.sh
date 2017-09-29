sudo apt-get install cmake
sudo apt-get install libx11-dev && install libsdl2-dev && libglu1-mesa-dev
sudo apt-get install libsdl2-dev

unzip external.zip

mkdir build && cd build
cmake ..
make
cd ..
./build/linux/graphvisualizer