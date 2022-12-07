//
// Created by tsfeith on 04/12/22.
// Gaussian Denoiser for Image Processing.
//

#ifndef IMAGEPROCESSING_DENOISER_HPP
#define IMAGEPROCESSING_DENOISER_HPP

#include <Eigen/Eigen>
#include "Image.hpp"
#include <string>

using namespace std;

/**
 * This class implements a Denoiser for Image Processing.
 * It uses a Gaussian Filter (or a mean filter as a special case)
 * to denoise an image.
 * Both the kernel size and the sigma value can be set.
 */

class Denoiser {
private:
    Eigen::ArrayXXd kernel;

public:
    /// Default constructor. Creates a 3x3 Gaussian Filter with sigma = 1.
    Denoiser();
    /// Constructor. Creates a Gaussian Filter with the given kernel size and sigma value.
    Denoiser(int size, double sigma); // NOTE: we could use template here to be more flexible in the sigma typing
    /// Constructor. Uses a custom kernel.
    explicit Denoiser(const Eigen::ArrayXXd& kernel);

    /// Denoises the image without saving it.
    Image denoise(const Image& image, bool show=false);

    /// Simple getter for the kernel.
    [[nodiscard]] Eigen::ArrayXXd getKernel() const;
};


#endif //IMAGEPROCESSING_DENOISER_HPP
