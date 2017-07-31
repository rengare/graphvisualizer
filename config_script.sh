# system requirements
sudo apt-get install cmake
sudo apt-get install libx11-dev && install libsdl2-dev && libglu1-mesa-dev
sudo apt-get install libsdl2-dev
sudo apt-get install opencl-headers
# arrayfire deps
sudo apt-get install ocl-icd-libopencl1
sudo apt-get install opencl-headers
sudo apt-get install libboost-all-dev
sudo apt-get install libglfw3

# opencl sdk for specific platform need to be installed
# opencl driver for OS need to be installed

# sdl2 build
cd external

rm -rf sdl2-linux && rm -rf sdl2-windows

unzip sdl2.zip -d sdl2-linux

cd sdl2-linux && mv sdl2/* . && rm -rf sdl2
mkdir include/SDL2
mv include/* include/SDL2
mkdir build && cd build

# cmake ..
# make
# sudo make install

# cd ../../

# cd rxcpp

cd ../../../

# project build
mkdir build && cd build
cmake ..
cd ..
./build/linux/graphvisualizer