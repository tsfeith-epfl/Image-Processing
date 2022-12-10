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
 * @brief The Denoiser class
 * @details This class implements a Denoiser for Image Processing.
 * It uses a Gaussian Filter (or a mean filter as a special case)
 * to denoise an image.
 * Both the kernel size and the sigma value can be set.
 */

class Denoiser {
private:
    Eigen::ArrayXXd kernel;

public:
    Denoiser();
    Denoiser(int size, double sigma); // NOTE: we could use template here to be more flexible in the sigma typing
    explicit Denoiser(const Eigen::ArrayXXd& kernel);

    Image denoise(const Image& image, bool show=false);

    void setKernel(const Eigen::ArrayXXd& kernel);

    [[nodiscard]] Eigen::ArrayXXd getKernel() const;
};


#endif //IMAGEPROCESSING_DENOISER_HPP
