sudo apt-get install cmake libx11-dev libsdl2-dev libglu1-mesa-dev -y
sudo apt-get install libsdl2-dev -y

unzip external.zip

mkdir build && cd build
cmake ..
make
cd ..
./build/linux/graphvisualizer
