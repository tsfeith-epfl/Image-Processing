//
// Created by André Charneca on 01.12.22.
//

#include <Eigen/Eigen>
#include "operations.hpp"


/*!
 * Normalizes the input array.
 * @details This function normalizes the input array to the range [0, 1], and converts it to a double array.
 * @param input Input array
 * @return Normalized array
 */
template <typename T>
Eigen::ArrayXXd normalize(const Eigen::ArrayBase<T>& input) {
    Eigen::ArrayXXd output = input.template cast<double>();
    output = (output - output.minCoeff()) / (output.maxCoeff() - output.minCoeff());
    return output;
}
// for templates to work between a .hpp and a .cpp file, we must do this
template Eigen::ArrayXXd normalize<Eigen::ArrayXXd>(const Eigen::ArrayBase<Eigen::ArrayXXd>& input);
template Eigen::ArrayXXd normalize<Eigen::ArrayXXi>(const Eigen::ArrayBase<Eigen::ArrayXXi>& input);
template Eigen::ArrayXXd normalize<Eigen::ArrayXXf>(const Eigen::ArrayBase<Eigen::ArrayXXf>& input);
template Eigen::ArrayXXd normalize<Eigen::Array<long double, Eigen::Dynamic, Eigen::Dynamic>>(const Eigen::ArrayBase<Eigen::Array<long double, Eigen::Dynamic, Eigen::Dynamic>>& input);
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

// ------------------------------------ //
// --- Fourier Transform Operations --- //
// ------------------------------------ //

/*!
 * @brief Function to compute the Fourier transform of a 1D signal
 * @details This function computes the Fourier transform of a 1D complex double Eigen Array.
 * The frequency domain has its origin at the center of the array.
 * @param input Input signal
 * @param inverse Boolean to indicate whether to compute the inverse Fourier transform.
 * @return Output signal as an Eigen::ArrayXcd
 */
Eigen::ArrayXcd dft(Eigen::ArrayXcd input, bool inverse){
    int N = input.size();
    Eigen::ArrayXcd output = Eigen::ArrayXcd::Zero(N);

    complex<double> omega;
    // frequency domain origin is at the center of the array
    int k_min = -N/2;
    int k_max = N/2 - 1;
    int n_min = 0;
    int n_max = N - 1;
    if (inverse){
        n_min = -N/2;
        n_max = N/2 - 1;
        k_min = 0;
        k_max = N - 1;
    }

    for (int k = k_min; k <= k_max; k++) {
        for (int n = n_min; n <= n_max; n++) {
            if (inverse) {
                omega = exp(complex<double>(0, 2 * M_PI * (k*n)/ N));
            } else {
                omega = exp(complex<double>(0, -2 * M_PI * (k*n) / N));
            }
            output(k - k_min) += input(n-n_min) * omega;
        }
    }



    if (inverse) {
        output /= N;
    }

    return output;
}

/*!
 * @brief Function to compute the Fourier transform of a 2D signal
 * @details This function computes the Fourier transform of a 2D complex Eigen Array. It casts the output to a complex double.
 * The 2D DFT is computed by first computing the 1D DFT along each row, and then computing the 1D DFT along each column.
 * @param input Input signal
 * @param inverse Boolean to indicate whether to compute the inverse Fourier transform.
 * @param show_progress Boolean to indicate whether to print progress.
 * @return Output signal as an Eigen::ArrayXXcd
 */
Eigen::ArrayXXcd dft2(Eigen::ArrayXXcd input, bool inverse, bool show_progress){
    int N = input.rows();
    int M = input.cols();
    Eigen::ArrayXXcd output = Eigen::ArrayXXcd::Zero(N, M);
    Eigen::ArrayXcd row = Eigen::ArrayXcd::Zero(N);
    Eigen::ArrayXcd col = Eigen::ArrayXcd::Zero(M);

    for (int i = 0; i < N; i++){
        if (show_progress) {
            cout << "\rComputing row " << i << " of " << N << flush;
        }
        row = input.row(i);
        output.row(i) = dft(row, inverse);
    }

    if (show_progress) cout << endl;

    for (int j = 0; j < M; j++){
        if (show_progress) {
            cout << "\rComputing column " << j << " of " << M << flush;
        }
        col = output.col(j);
        output.col(j) = dft(col, inverse);
    }
    if (show_progress){
        cout << endl;
    }

    return output;
}


