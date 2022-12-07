# Image-Processing
Repository for the final project of Programming Concepts in Scientific Computing (MATH-458), by Tomás Feith and André Charneca.

## Description
#TODO - Add description

## Setup

### Dependencies
This project uses the following libraries:
- [CMake](https://cmake.org/): a cross-platform build system generator.
- [OpenCV](https://opencv.org/): for image reading and writing
- [Eigen](http://eigen.tuxfamily.org): for image storing and manipulation (as matrices)
- [Doxygen](http://www.doxygen.nl/): for documentation generation
- [GraphViz](https://www.graphviz.org/): needed for building documentation with Doxygen

Before building the project, make sure you have the dependencies installed. For that purpose we provide a bash script
that will install all the dependencies in your system (compatible with Linux and Mac). To run it, simply run

    ./setup.sh

in the root directory of the project. This script will install OpenCV, CMake, Doxygen and GraphViz using the package
manager, and will download Eigen and place it in the `libraries` folder.

### Building
We recommend using CLion to build the project. If you are using CLion, simply open the project and build it. If you are
not using CLion, you can build the project using CMake. To do so, run the following commands in the root directory of 
the project:

    mkdir build
    cd build
    cmake ..
    make

This will create an executable called `main` in the `build` folder. To run it, simply run

    ./main

in the `build` folder. This command will show the help screen.

## Repo Structure

After setup, the project will have the following structure:

    .
    ├── build                  # Folder where the executable is created (not necessarily named build)
    │   └── ...
    ├── googletest             # Folder with the Google Test library
    │   └── ...
    ├── images                 # Folder where the images *must* be placed
    │   └── ...
    ├── output                 # Folder where the output images will be placed
    │   └── ...
    ├── libraries
    │   └── eigen
    ├── src
    │   ├── Denoiser.cpp
    │   ├── Denoiser.hpp
    │   ├── Image.cpp
    │   ├── Image.hpp
    │   ├── location.hpp
    │   ├── operations.cpp
    │   └── operations.hpp
    ├── test                   # Folder with all the unit tests
    │   └── ...
    ├── CMakeLists.txt
    ├── main.cpp
    ├── parameters.hpp
    ├── README.md
    └── setup.sh

## Usage

### Parameters
The parameters for the program are stored in a `.hpp` file called `parameters.hpp`. It allows to easily tweak the 
parameters of the program without having to recompile it or pass them as arguments. Each mode of operation has different
parameters, and they are described below.
- Denoising
  - sigma: standard deviation of the Gaussian kernel to apply to the image. Set to 0 to use mean filtering. Default: 1.0
  - kernel_size: size of the kernel to apply to the image. Default: 3
- Contour Detection
  - #TODO - Add parameters
- Intensity Histogram
  - bins: number of bins to use in the histogram. Default: 100
  - range: range of the histogram. Default: [0, 1]
  - normalize: whether to normalize the histogram. Default: true
  - log: whether to use a logarithmic scale for the histogram. Default: false

### Running the program
To run the program, simply run the executable created in the `build` folder. Even though the program reads the parameters
from the parameters.hpp` file, there are still some parameters that must be passed as arguments. These are:
- mode: the mode of operation of the program. OPTIONAL (default: denoising). Possible values:
  - denoise: denoises the image
  - contour: detects the contours of the image
  - intensity_histogram: computes the intensity histogram of the image
- input: name of the input image (including the extension). REQUIRED
- output: name of the output image (including the extension). OPTIONAL (default: same as input image)

After running the program, the output will be placed in one of two places:
- If the filename was provided as an absolute path, the output will be placed in the same folder as the input image;
- If the filename was provided as the name of the image inside the `images` folder, the output will be placed in the
`output` folder.
