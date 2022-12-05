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

class Denoiser {
private:
    Eigen::ArrayXXd kernel;

public:
    Denoiser();
    Denoiser(int size, double sigma);
    explicit Denoiser(const Eigen::ArrayXXd& kernel);

    Image denoise(const Image& image);
    Image denoise(const Image& image, string output, bool show=false);

    [[nodiscard]] Eigen::ArrayXXd getKernel() const;
};


#endif //IMAGEPROCESSING_DENOISER_HPP
