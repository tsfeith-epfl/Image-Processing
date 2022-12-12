# Image-Processing
Repository for the final project of Programming Concepts in Scientific Computing (MATH-458), by Tomás Feith and André 
Charneca.

## Description
This project aims to implement a set of image processing algorithms, specifically image denoising, contour detection,
generation of its intensity histogram and computation of its FFT. The algorithms are implemented in C++, and the 
documentation is generated using Doxygen.
This project contains an extensive suite of unit tests, which are run using the Google Test framework.

## Setup

### Dependencies
This project uses the following libraries:
- [CMake](https://cmake.org/): a cross-platform build system generator.
- [OpenCV](https://opencv.org/): for image reading and writing
- [Eigen](http://eigen.tuxfamily.org): for image storing and manipulation (as matrices)
- [gnuplot](http://www.gnuplot.info/): for plotting the intensity histograms
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
    ├── documentation          # Folder where Doxygen documentation is created
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
    │   ├── ContourExtractor.cpp
    │   ├── ContourExtractor.hpp
    │   ├── Denoiser.cpp
    │   ├── Denoiser.hpp
    │   ├── Histogram.cpp
    │   ├── Histogram.hpp
    │   ├── Image.cpp
    │   ├── Image.hpp
    │   ├── location.hpp       # Helper file generated on build to know the absolute location of the project
    │   ├── operations.cpp
    │   └── operations.hpp
    ├── test                   # Folder with all the unit tests
    │   └── ...
    ├── CMakeLists.txt
    ├── Doxyfile.in
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
  - threshold: threshold to use for the contour detection. Default: 0.3
  - sigma: standard deviation of the Gaussian kernel to apply to the image. Set to 0 to use mean filtering. Default: 2.0
  - kernel_size: size of the kernel to apply to the image. Default: 5
- Intensity Histogram
  - bins: number of bins to use in the histogram. Default: 500
  - max range: maximum value to use in the histogram. Must be between in (`min_range`, 1.]. Default: 1.0
  - min range: minimum value to use in the histogram. Must be between in [0., `max_range`). Default: 0.0
  - log: whether to use a logarithmic scale for the histogram. Default: false

### Running the program
To run the program, simply run the executable created in the `build` folder. Even though the program reads the parameters
from the parameters.hpp` file, there are still some parameters that must be passed as arguments. These are:
- mode: the mode of operation of the program. REQUIRED. Possible values:
  - denoise: denoises the image
  - contour: detects the contours of the image
  - histogram: computes the intensity histogram of the image
- input: name of the input image (including the extension). REQUIRED
  - It can be given as an absolute path, or as a relative path to the `images` folder. 
- output: name of the output image (including the extension). OPTIONAL (default: same as input image)

After running the program, the output will be placed in one of two places:
- If the filename was provided as an absolute path, the output will be placed in the same folder as the input image;
- If the filename was provided as the name of the image inside the `images` folder, the output will be placed in the
`output` folder.

## Documentation

There is extensive documentation for the project, which can be generated using Doxygen. To generate the documentation,
run the following commands in the root directory of the project:

    doxygen Doxyfile.in

This will write the documentation to the `documentation` folder. To view it, go into `documentation/html` and open the
`index.html` file in your browser.

## TODO
Some functionality we wanted to implement/fix but didn't have time.
- [ ] Allow for the generation of histograms for each channel, not just the intensity
- [ ] Allow batch processing of images, instead of just one at a time
- [ ] Allow more flexibility to the convolutional methods (padding, stride, dilation,...)
- [ ] Fix histograms of PNG images with transparency. Right now, it still counts the transparent pixels for the intensity histogram.
