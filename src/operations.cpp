//
// Created by André Charneca on 01.12.22.
//

#include <Eigen/Eigen>
#include "operations.hpp"

/* Convolution Functions */

Eigen::ArrayXXd applyConvolution(Eigen::ArrayXXd input, Eigen::ArrayXXd kernel) {
    /**
     * Applies a convolution to an Eigen array, with 0-padding. The output has the same size as the input.
     */

    // error: kernel size must be odd
    if (kernel.rows() % 2 == 0 || kernel.cols() % 2 == 0) {
        throw std::invalid_argument("Kernel size must be odd");
    }

    // error: kernel size must be smaller than input size
    if (kernel.rows() > input.rows() || kernel.cols() > input.cols()) {
        throw std::invalid_argument("Kernel size must be smaller than input size");
    }

    // error: kernel must be square
    if (kernel.rows() != kernel.cols()) {
        throw std::invalid_argument("Kernel must be square");
    }


    // this has to work for rectangular inputs
    int kernel_size = kernel.rows();
    int kernel_radius = (kernel_size - 1) / 2;
    int input_rows = input.rows();
    int input_cols = input.cols();

    // initialize output
    Eigen::ArrayXXd output = Eigen::ArrayXXd::Zero(input_rows, input_cols);

    // apply convolution
    for (int i = 0; i < input_rows; i++) {
        for (int j = 0; j < input_cols; j++) {
            for (int k = 0; k < kernel_size; k++) {
                for (int l = 0; l < kernel_size; l++) {
                    int input_i = i + k - kernel_radius;
                    int input_j = j + l - kernel_radius;
                    if (input_i >= 0 && input_i < input_rows && input_j >= 0 && input_j < input_cols) {
                        output(i, j) += input(input_i, input_j) * kernel(k, l);
                    }
                }
            }
        }
    }

    return output;
}

Image applyConvolution(const Image& input, const Eigen::ArrayXXd& kernel) {
    /**
     * Applies a convolution to an Image, with 0-padding. The output has the same size as the input.
     */
     // There is no need to add all the tests again, since the Image is tested on creation
     // and the 'applyConvolution' function is tested above.
     vector<Eigen::ArrayXXd> output = vector<Eigen::ArrayXXd>();
    for (int i = 0; i < input.getChannels(); i++) {
        output.push_back(applyConvolution(input.getData(i), kernel));
    }
    return Image(output);
}

/* Gradient Operations */

Image computeGradientX(const Image& input){
    /**
     * Computes the gradient in the x-direction of an Image.
     */
    // error: input must be grayscale
    if (input.getChannels() != 1) {
        throw std::invalid_argument("Input must be grayscale");
    }

    // initialize Sobel kernel
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(3, 3);
    kernel << -1, 0, 1,
              -2, 0, 2,
              -1, 0, 1;

    // apply convolution
    return applyConvolution(input, kernel);
}

Image computeGradientY(const Image& input){
    /**
     * Computes the gradient in the y-direction of an Image.
     */
    // error: input must be grayscale
    if (input.getChannels() != 1) {
        throw std::invalid_argument("Input must be grayscale");
    }

    // initialize Sobel kernel
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(3, 3);
    kernel << -1, -2, -1,
               0,  0,  0,
               1,  2,  1;

    // apply convolution
    return applyConvolution(input, kernel);
}

Image computeGradientMagnitude(const Image& input){
    /**
     * Computes the gradient magnitude of a grayscale Image.
     */
    // compute gradient in x and y direction
    Image gradient_x = computeGradientX(input);
    Image gradient_y = computeGradientY(input);

    // initialize output
    Eigen::ArrayXXd output = Eigen::ArrayXXd::Zero(input.getHeight(), input.getWidth());

    // compute gradient magnitude
    for (int i = 0; i < input.getHeight(); i++) {
        for (int j = 0; j < input.getWidth(); j++) {
            output(i, j) = sqrt(pow(gradient_x.getData(0)(i, j), 2) + pow(gradient_y.getData(0)(i, j), 2));
        }
    }

    return Image(output);
}

Image computeGradientDirection(const Image& input){
    /**
     * Computes the gradient direction of a grayscale Image, in degrees.
     */

    // compute gradient in x and y direction
    Image gradient_x = computeGradientX(input);
    Image gradient_y = computeGradientY(input);

    // initialize output
    Eigen::ArrayXXd output = Eigen::ArrayXXd::Zero(input.getHeight(), input.getWidth());

    // compute gradient direction
    for (int i = 0; i < input.getHeight(); i++) {
        for (int j = 0; j < input.getWidth(); j++) {
            output(i, j) = atan2(gradient_y.getData(0)(i, j), gradient_x.getData(0)(i, j)) * 180 / M_PI;
        }
    }

    return Image(output);
}


