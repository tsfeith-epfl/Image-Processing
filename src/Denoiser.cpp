//
// Created by tsfeith on 04/12/22.
//

#include "Denoiser.hpp"

#include <utility>
#include "Operations.hpp"

Denoiser::Denoiser() {
    this->kernel = Eigen::ArrayXXd::Zero(3, 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->kernel(i, j) = 1.0 / 9.0;
        }
    }
}

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

Image Denoiser::denoise(const Image& image) {
    Image denoised_image = applyConvolution(image, this->kernel);
    return denoised_image;
}

Image Denoiser::denoise(const Image& image, string output, bool show) {
    Image denoised_image = applyConvolution(image, this->kernel);
    denoised_image.save(std::move(output));
    if (show) {
        denoised_image.show("Denoised image");
    }
    return denoised_image;
}

Eigen::ArrayXXd Denoiser::getKernel() const {
    return this->kernel;
}
