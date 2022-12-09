//
// Created by André Charneca on 01.12.22.
//

#include <Eigen/Eigen>
#include "operations.hpp"


// -------------------------------------- //
// --- Default Convolution Operations --- //
// -------------------------------------- //
/*!
 * @brief Function to compute the convolution of an image with a kernel
 * @details This function computes the convolution of an image with a kernel. The kernel is assumed to be centered, and
 * it must be odd in size and squared. The image is padded with zeros so that the output image has the same size as the
 * input image.
 * @param input Input image in the form of a matrix
 * @param kernel Kernel to be used in the convolution
 * @return Output image in the form of a matrix
 */
Eigen::ArrayXXd applyConvolution(Eigen::ArrayXXd input, Eigen::ArrayXXd kernel) {
    if (kernel.rows() % 2 == 0 || kernel.cols() % 2 == 0) {
        throw std::invalid_argument("Kernel size must be odd");
    }
    if (kernel.rows() > input.rows() || kernel.cols() > input.cols()) {
        throw std::invalid_argument("Kernel size must be smaller than input size");
    }
    if (kernel.rows() != kernel.cols()) {
        throw std::invalid_argument("Kernel must be square");
    }

    int kernel_size = kernel.rows();
    int kernel_radius = (kernel_size - 1) / 2;
    int input_rows = input.rows();
    int input_cols = input.cols();

    Eigen::ArrayXXd output = Eigen::ArrayXXd::Zero(input_rows, input_cols);

    // apply convolution: O(n^2 * k^2)
    for (int i = 0; i < input_rows; i++) {
        for (int j = 0; j < input_cols; j++) {
            for (int k = 0; k < kernel_size; k++) {
                for (int l = 0; l < kernel_size; l++) {
                    int input_i = i + k - kernel_radius;
                    int input_j = j + l - kernel_radius;
                    // take into account zero padding for the edges
                    if (input_i >= 0 && input_i < input_rows && input_j >= 0 && input_j < input_cols) {
                        output(i, j) += input(input_i, input_j) * kernel(k, l);
                    }
                }
            }
        }
    }
    return output;
}

/*!
 * @brief Function to compute the convolution of an image with a kernel
 * @details This function computes the convolution of an image with a kernel. The kernel is assumed to be centered, and
 * it must be odd in size and squared. The image is padded with zeros so that the output image has the same size as the
 * input image.
 * @param input Input image in the form of an Image object (see Image.hpp)
 * @param kernel Kernel to be used in the convolution
 * @return Output image in the form of an Image object
 */
Image applyConvolution(const Image& input, const Eigen::ArrayXXd& kernel) {
    // There is no need to add all the tests again, since the Image is tested on creation
    // and the 'applyConvolution' function is tested above.
    vector<Eigen::ArrayXXd> output = vector<Eigen::ArrayXXd>();
    // apply the convolution independently to each channel
    for (int i = 0; i < input.getChannels(); i++) {
        output.push_back(applyConvolution(input.getData(i), kernel));
    }
    return Image(output);
}

// ------------------------------------ //
// --- Gradient Specific Operations --- //
// ------------------------------------ //

/*!
 * @brief Function to compute the gradient of an image along the x-axis
 * @details This function computes the x-axis gradient of an image. The gradient is computed using the Sobel operator.
 * @param input Input image as an Image object
 * @return Output image as an Image object
 */
Image computeGradientX(const Image& input){
    // There's no need to throw an expection, maybe just convert the image to grayscale?
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

/*!
 * @brief Function to compute the gradient of an image along the y-axis
 * @details This function computes the y-axis gradient of an image. The gradient is computed using the Sobel operator.
 * @param input Input image as an Image object
 * @return Output image as an Image object
 */
Image computeGradientY(const Image& input){
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

/*!
 * @brief Function to compute the gradient magnitude of an image
 * @details This function computes the gradient magnitude of an image. The gradient along each direction is computed
 * using the Sobel operator, and the magnitude is computed as the 2-norm of the gradient vector.
 * @param input Input image as an Image object
 * @return Output image as an Image object
 */
Image computeGradientMagnitude(const Image& input){
    // compute gradient in x and y direction
    Image gradient_x = computeGradientX(input);
    Image gradient_y = computeGradientY(input);

    Eigen::ArrayXXd output = Eigen::ArrayXXd::Zero(input.getHeight(), input.getWidth());
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


