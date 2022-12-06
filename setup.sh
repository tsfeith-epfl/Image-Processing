
# go to libraries and download the necessary ones
cd libraries
# download eigen
git clone https://gitlab.com/libeigen/eigen.git
# install jsoncpp
if [[ "$OSTYPE" == "linux-gnu" ]]; then
    sudo apt-get install libjsoncpp-dev
elif [[ "$OSTYPE" == "darwin"* ]]; then
    brew install jsoncpp
fi
# install opencv
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    sudo apt-get install -y libopencv-dev
elif [[ "$OSTYPE" == "darwin"* ]]; then
    brew install opencv
fi

# install and build googletest (for unit tests)
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake ..
make
