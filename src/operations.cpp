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
 * @param input Input image in the form of an array
 * @param kernel Kernel to be used in the convolution
 * @return Output image in the form of an array
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
 * input image. Note: In order to return a valid image, the Image is normalized to [0,1].
 * @param input Input image in the form of an Image object (see Image.hpp)
 * @param kernel Kernel to be used in the convolution
 * @return Output image in the form of an Image object
 */
Image applyConvolution(const Image& input, const Eigen::ArrayXXd& kernel) {

    vector<Eigen::ArrayXXd> output = vector<Eigen::ArrayXXd>();
    // apply the convolution independently to each channel
    for (int i = 0; i < input.getChannels(); i++) {
        output.push_back(applyConvolution(input.getData(i), kernel));
    }

    //normalize each channel to 0,1
    for (int i = 0; i < input.getChannels(); i++) {
        output[i] = (output[i] - output[i].minCoeff()) / (output[i].maxCoeff() - output[i].minCoeff());
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
 * @return Output image as an Eigen::ArrayXXd
 */
Eigen::ArrayXXd computeGradientX(const Image& input){
    Image img_copy = input;
    if (img_copy.getChannels() != 1) {
        img_copy = img_copy.reduceChannels();
    }

    // initialize Sobel kernel, preserving pixel range
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(3, 3);
    kernel << -1, 0, 1,
              -2, 0, 2,
              -1, 0, 1;

    // apply convolution
    Eigen::ArrayXXd output = applyConvolution(img_copy.getData(0), kernel);
    return output;
}

/*!
 * @brief Function to compute the gradient of an image along the y-axis
 * @details This function computes the y-axis gradient of an image. The gradient is computed using the Sobel operator.
 * @param input Input image as an Image object
 * @return Output image as an Eigen::ArrayXXd
 */
Eigen::ArrayXXd computeGradientY(const Image& input){
    Image img_copy = input;

    // convert to grayscale
    if (img_copy.getChannels() != 1) {
        img_copy = img_copy.reduceChannels();
    }

    // initialize Sobel kernel
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(3, 3);
    kernel << -1, -2, -1,
               0,  0,  0,
               1,  2,  1;

    // apply convolution
    Eigen::ArrayXXd output = applyConvolution(img_copy.getData(0), kernel);
    return output;
}

/*!
 * @brief Function to compute the gradient magnitude of an image
 * @details This function computes the gradient magnitude of an image. The gradient along each direction is computed
 * using the Sobel operator, and the magnitude is computed as the 2-norm of the gradient vector.
 * @param input Input image as an Image object
 * @return Output image as an Eigen::ArrayXXd
 */
Image computeGradientMagnitude(const Image& input){
    // compute gradient in x and y direction
    Eigen::ArrayXXd gradient_x = computeGradientX(input);
    Eigen::ArrayXXd gradient_y = computeGradientY(input);

    Eigen::ArrayXXd output = Eigen::ArrayXXd::Zero(input.getHeight(), input.getWidth());
    for (int i = 0; i < input.getHeight(); i++) {
        for (int j = 0; j < input.getWidth(); j++) {
            output(i, j) = sqrt(pow(gradient_x(i, j), 2) + pow(gradient_y(i, j), 2));
        }
    }

    // normalize output to [0,1]
    output = (output - output.minCoeff()) / (output.maxCoeff() - output.minCoeff());

    return Image(output);
}

/*!
 * @brief Function to compute the gradient direction of an image
 * @details This function computes the gradient magnitude of an image. The gradient along each direction is computed
 * using the Sobel operator, and the direction is computed as the arctangent of the y gradient divided by the x gradient.
 * @param input Input image as an Image object
 * @return Output image as an Eigen::ArrayXXd with values
 */
[[maybe_unused]] Image computeGradientDirection(const Image& input){
    // compute gradient in x and y direction
    Eigen::ArrayXXd gradient_x = computeGradientX(input);
    Eigen::ArrayXXd gradient_y = computeGradientY(input);

    // initialize output
    Eigen::ArrayXXd output = Eigen::ArrayXXd::Zero(input.getHeight(), input.getWidth());

    // compute gradient direction
    for (int i = 0; i < input.getHeight(); i++) {
        for (int j = 0; j < input.getWidth(); j++) {
            output(i, j) = atan2(gradient_y(i, j), gradient_x(i, j)) / (2 * M_PI) + 0.5;
        }
    }

    return Image(output);
}

/*!
 * @brief Function to threshold an image
 * @details This function thresholds an image, setting all values above the threshold to 1 and all values below to 0.
 * @param input Input image as an Image object
 * @param threshold double threshold value
 * @return Output image as an Image object
 */
Image applyThreshold(const Image& input, double threshold) {
    Image img_copy = input;
    if (img_copy.getChannels() != 1) {
        img_copy = img_copy.reduceChannels();
    }
    Eigen::ArrayXXd img_data = img_copy.getData(0);
    Eigen::ArrayXXd output = Eigen::ArrayXXd::Zero(input.getHeight(), input.getWidth());

    for (int i = 0; i < input.getHeight(); i++) {
        for (int j = 0; j < input.getWidth(); j++) {
            if (img_data(i, j) > threshold) {
                output(i, j) = 1;
            }
        }
    }
    return Image(output);
}


