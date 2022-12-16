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
 * It first applies a denoising filter to the image to reduce high frequencies.
 * Then, it computes a normalized gradient magnitude (using the Sobel operator)
 * in the range [0, 1]. Finally, it applies a thresholding to the gradient magnitude
 * to extract the contours.
 * The kernel size and sigma value for the denoising filter can be specified.
 * The threshold value can also be specified.
 */
class ContourExtractor {
private:
    /*!The threshold value for the gradient magnitude.*/
    double threshold;

    /*!The Denoiser object used to reduce high frequencies before computing the gradient.*/
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
