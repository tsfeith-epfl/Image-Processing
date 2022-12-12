
# go to libraries and download the necessary ones
cd libraries || exit
# download eigen
git clone https://gitlab.com/libeigen/eigen.git
cd .. || exit

# install and build googletest (for unit tests)
git clone https://github.com/google/googletest.git
cd googletest || exit
mkdir build
cd build || exit
cmake ..
make

# install all required libraries
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  sudo apt-get install -y libopencv-dev graphviz doxygen cmake gnuplot
elif [[ "$OSTYPE" == "darwin"* ]]; then
  brew install opencv graphviz doxygen cmake gnuplot
elif [[ "$OSTYPE" == "cygwin" ]]; then
  echo "Cygwin is not supported"
elif [[ "$OSTYPE" == "msys" ]]; then
  echo "MSYS is not supported"
elif [[ "$OSTYPE" == "win32" ]]; then
  echo "Windows is not supported"
elif [[ "$OSTYPE" == "freebsd"* ]]; then
  echo "FreeBSD is not supported"
else
  echo "Unknown OS"
fi