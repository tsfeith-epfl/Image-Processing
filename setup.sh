sudo apt-get install build-essential
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev

cd libraries

git clone https://gitlab.com/libeigen/eigen.git

git clone https://github.com/opencv/opencv.git
cd opencv
mkdir build
cd build
cmake ..
WAIT
make -j8
WAIT
cd ..
cd ..
cd ..

git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake ..
WAIT
make
WAIT