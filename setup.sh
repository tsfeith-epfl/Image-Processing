cd libraries

# download eigen
git clone https://gitlab.com/libeigen/eigen.git

# install opencv
sudo apt-get install -y libopencv-dev

# install and build googletest (for unit tests)
cd ..
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake ..
make
