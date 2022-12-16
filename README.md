# Image-Processing
Repository for the final project of Programming Concepts in Scientific Computing (MATH-458), by Tomás Feith (342553) and André
Charneca (338621), for the MSc in Computational Science and Engineering at EPFL.

## Description
This project aims to implement a set of image processing algorithms, specifically image denoising, contour detection,
generation of its intensity histogram and frequency filtering with Fourier Transforms. The algorithms are implemented in C++, and the 
documentation is generated using Doxygen. This project was tested on Linux and MacOS.
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

## Usage

### Parameters
The parameters for the program are stored in a `.hpp` file called `parameters.hpp`. It allows to easily tweak the 
parameters of the program without having to pass them as arguments. Each mode of operation has different
parameters, and they are described below.
- Denoising
  - sigma: standard deviation of the Gaussian kernel to apply to the image. Set to 0 to use mean filtering.
  - kernel_size: size of the kernel to apply to the image.
- Contour Detection
  - threshold: threshold to use for the contour detection.
  - sigma: standard deviation of the Gaussian kernel to apply to the image. Set to 0 to use mean filtering.
  - kernel_size: size of the kernel to apply to the image.
- Intensity Histogram
  - bins: number of bins to use in the histogram.
  - max range: maximum value to use in the histogram. Must be between in (`min_range`, 1.].
  - min range: minimum value to use in the histogram. Must be between in [0., `max_range`).
  - log: whether to use a logarithmic scale for the histogram.
- Fourier Filtering
  - show_fourier_progress: whether to show progress in Fourier Transform computations.
  - show_fourier_log_magnitude: whether to show log magnitude of Fourier Transforms.
  - low_cutoff: frequency cutoff for low pass filter. Its the radius of the kept frequency circle, relative to the smallest image dimension.
  - high_cutoff: frequency cutoff for high pass filter. Its the radius of the zero frequency circle, relative to the smallest image dimension.
  - filter_type: chooses the type of filter to be applied. If it's "band", it will use low_cutoff and high_cutoff parameters as the arguments.

### Running the program
To run the program, simply run the executable created in the `build` folder. Even though the program reads the parameters
from the parameters.hpp` file, there are still some parameters that must be passed as arguments. These are:
- mode: the mode of operation of the program. REQUIRED. Possible values:
  - denoise: denoises the image
  - contour: detects the contours of the image
  - histogram: computes the intensity histogram of the image
  - fourier: compute the fourier transform of the image, and apply one of the possible operations on the frequency domain
- input: name of the input image (including the extension). REQUIRED
  - It can be given as an absolute path, or as a relative path to the `images` folder. 
- output: name of the output image (including the extension). OPTIONAL (default: same as input image)

So, for example, running the program for an image 'teapot.png' that we want to denoise, would require the following
command

    ./main --input teapot.png --mode denoise

After running the program, the output will be placed in one of two places:
- If the filename was provided as an absolute path, the output will be placed in the same folder as the input image;
- If the filename was provided as the name of the image inside the `images` folder, the output will be placed in the
`output` folder.

## Features
This project contains the following features:
- The Image class
  - Load gray scale or color images from path.
  - Create images from Eigen arrays.
  - Apply convolutions on images.
  - Convert color image to gray scale using [colorimetric conversion](https://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale).
  - Display images on screen.
  - Save image to output file.
- Image denoising using Gaussian filtering
  - Mean filtering as a special case of Gaussian filtering by setting the standard deviation to 0
  - Choose the size of the kernel and the standard deviation of the Gaussian kernel
- Contour detection using thresholded Sobel filtering
  - Choose the threshold to use for the contour detection
  - Choose the size of the kernel and the standard deviation of the Gaussian kernel
- Intensity histogram computation
  - Choose the number of bins to use in the histogram
  - Choose range of values to use in the histogram
  - Choose whether to use a logarithmic scale for the histogram
- Fourier Filtering
  - Computation of Fourier Transform and Inverse Fourier Transform of image. 
  - Computation of Magnitude, Phase, Real and Imaginary parts of the Fourier Transform.
  - Choose filter mode (low, high or band pass) and frequency cutoffs.
- Other general features
  - Allow the user to provide the arguments in any order
  - Allow the user to provide the input image as an absolute path, or as a relative path to the `images` folder
  - Allow the user to provide a name for the output image, or use the same name as the input image

## Tests
This project contains a suite of unit tests, which are run using the Google Test framework. To run the tests, simply 
run 

        ./test_suite

in the `build` folder. This will run all the tests and show the results in the terminal.

### Test Description
There are comprehensive tests for each of the components of this program. An overview of the implemented tests is
provided below:
- Image object
  - Valid construction (from file or from matrix)
  - Throws exception on invalid construction
  - Getters and setters work as expected
  - Overloaded operators work as expected
  - Channel reduction (RGB to grayscale) works as expected
- Convolutional operators
  - Gives output with same size as input
  - Unit kernel gives same output as input
  - Throws expection on invalid parameters
  - Can be applied directly to Image object or to a matrix
  - Works on multichannel images
- Gradient operators
  - Checks for invalid images (smaller than kernel)
  - Gradient returns correct values
  - Gradient magnitude returns correct values
  - Gradient direction only tested for exceptions
- Denoiser object
  - Constructor throws exception on invalid parameters
  - Getters and setters work as expected
- Intensity histogram object
  - Constructor throws exception on invalid parameters
  - Getters and setters work as expected
  - Histogram works for RGB and Grayscale images
  - Histogram is computed correctly
  - Histogram is plotted without exceptions
- Contour extractor object
  - Constructor throws exception on invalid parameters
  - Getters and setters work as expected
  - Turns RGB image to grayscale
  - Returns binary image
- Fourier Image object
  - Correct computation of FT and IFT
  - Getters and setters work as expected
  - Frequency filters work as expected


The tests implemented on release (16/12/2022) get a line coverage of ~90%. While coverage is just a metric, and it 
shouldn't be the only metric used to evaluate the quality of the tests, it is a good indicator.

For a more detailed description of the tests, please refer to the files in the `tests` folder. The test names should
be self-explanatory.

## TO DO
There are a few functionalities that we would have liked to implement but there was not enough time. These are:
- [ ] Allow for the generation of histograms for each channel, not just the intensity
- [ ] Allow batch processing of images, instead of just one at a time
- [ ] Allow more flexibility to the convolutional methods (padding, stride, dilation,...)
- [ ] Fix histograms of PNG images with transparency. Right now, it omits the alpha channel, meaning that the pixels
take the value of the first three channels. The most affected mode by this is the histogram.
- [ ] Switch the DFT algorithm to use FFT (faster)
- [ ] Allow FT computation on each channel, instead of converting to grayscale image.
