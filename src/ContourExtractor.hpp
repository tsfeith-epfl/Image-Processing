//
// Created by André Charneca on 06.12.22.
// Contour extractor class using the thresholding of the gradient magnitude
//

#ifndef IMAGEPROCESSING_CONTOUREXTRACTOR_HPP
#define IMAGEPROCESSING_CONTOUREXTRACTOR_HPP

#include <Eigen/Eigen>
#include "Image.hpp"
#include <string>
#include "Denoiser.hpp"

using namespace std;

/**
 * @brief The ContourExtractor class
 * @details This class implements a ContourExtractor for Image Processing.
 * It uses the thresholding of the gradient magnitude to extract contours.
 * The kernel size, sigma value and gradient threshold can be set.
 */

class ContourExtractor {
private:
    double threshold;
    Denoiser denoiser;

public:
    ContourExtractor();
    ContourExtractor(double threshold, int kernelSize, double sigma);
    ContourExtractor(double threshold, const Eigen::ArrayXXd& kernel);

    Image extractContours(const Image& image, bool show=false);

    [[nodiscard]] double getThreshold() const;
    [[nodiscard]] Denoiser getDenoiser() const;

    void setThreshold(double threshold);
    void setDenoiser(const Denoiser& denoiser);
    void setDenoiser(int kernel_size, double sigma);
};

#endif //IMAGEPROCESSING_CONTOUREXTRACTOR_HPP
