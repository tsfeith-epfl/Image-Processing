//
// Created by tsfeith on 04/12/22.
//

#include "Denoiser.hpp"

#include <utility>
#include "operations.hpp"


Denoiser::Denoiser() {
    this->kernel = Eigen::ArrayXXd::Zero(3, 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->kernel(i, j) = 1.0 / 9.0;
        }
    }
}

/*!
 * Constructor. Creates a Gaussian Filter with the given kernel size and sigma value.
 * @param size The size of the kernel. Must be odd.
 * @param sigma The sigma value of the Gaussian Filter. Set to 0 to use a mean filter.
 */
Denoiser::Denoiser(int size, double sigma) {
    if (size % 2 == 0) {
        throw invalid_argument("Kernel size must be odd");
    }
    if (size <= 0) {
        throw invalid_argument("Kernel size must be positive");
    }
    if (sigma < 0) {
        throw invalid_argument("Sigma must be non-negative");
    }
    if (sigma == 0) {
        this->kernel = Eigen::ArrayXXd::Zero(size, size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                this->kernel(i, j) = 1.0 / (size * size);
            }
        }
    } else {
        auto size_d = (double) size - 1;
        this->kernel = Eigen::ArrayXXd::Zero(size, size);
        double sum = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                this->kernel(i, j) = exp(-((i - size_d / 2) * (i - size_d / 2) + (j - size_d / 2) * (j - size_d / 2)) / (2 * sigma * sigma));
                sum += this->kernel(i, j);
            }
        }
        this->kernel /= sum;
    }
}

/*!
 * Constructor. Uses a custom kernel.
 * @param kernel The kernel to use. Must be odd. Must be square. Must be normalized.
 */
Denoiser::Denoiser(const Eigen::ArrayXXd& kernel) {
    if (kernel.rows() == 0 || kernel.cols() == 0) {
        throw invalid_argument("Kernel cannot be empty");
    }
    if (kernel.rows() % 2 == 0 || kernel.cols() % 2 == 0) {
        throw invalid_argument("Kernel size must be odd");
    }
    if (kernel.rows() != kernel.cols()) {
        throw invalid_argument("Kernel must be square");
    }
    double sum = 0;
    for (int i = 0; i < kernel.rows(); i++) {
        for (int j = 0; j < kernel.cols(); j++) {
            sum += kernel(i, j);
        }
    }
    if (fabs(sum - 1) > 1e-6) {
        throw invalid_argument("Kernel must be normalized");
    }
    this->kernel = kernel;
}

/*!
 * Denoises the image without saving it.
 * @param image The image to denoise.
 * @param show Whether to show the denoised image.
 * @return The denoised image.
 */
Image Denoiser::denoise(const Image& image, bool show) {
    Image denoised_image = applyConvolution(image, this->kernel);
    if (show) {
        denoised_image.show("Denoised Image");
    }
    return denoised_image;
}

Eigen::ArrayXXd Denoiser::getKernel() const {
    return this->kernel;
}
