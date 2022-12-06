//
// Created by André Charneca on 06.12.22.
//

#include "ContourExtractor.hpp"
#include "operations.hpp"

/* Constructors */

ContourExtractor::ContourExtractor() {
    threshold = 0.5;
    denoiser = Denoiser();
}

ContourExtractor::ContourExtractor(double threshold, int kernelSize, double sigma) {
    this->threshold = threshold;
    denoiser = Denoiser(kernelSize, sigma);
}

/* Public Methods */
double ContourExtractor::getThreshold() const {
    return threshold;
}

Image ContourExtractor::extractContours(const Image& image, bool show) {
    /*
     * * Extracts the contours of an image using the Canny algorithm.
     */

    // First, convert image to grayscale
    Image grayImage = image;
    grayImage.reduceChannels();

    // Apply gaussian smoothing to denoise
    Image denoisedImage = this->denoiser.denoise(image);

    // Compute the gradient of the image
    // Get sobel kernels
    Eigen::ArrayXXd sobelX = Eigen::ArrayXXd::Zero(3, 3);
    Eigen::ArrayXXd sobelY = Eigen::ArrayXXd::Zero(3, 3);
    sobelX << -1, 0, 1,
              -2, 0, 2,
              -1, 0, 1;

    sobelY << -1, -2, -1,
               0,  0,  0,
               1,  2,  1;

    // Apply sobel kernels to image and compute gradient magnitude and direction
    Eigen::ArrayXXd gradientX = applyConvolution(denoisedImage.getData(0), sobelX);
    Eigen::ArrayXXd gradientY = applyConvolution(denoisedImage.getData(0), sobelY);
    Eigen::ArrayXXd gradientMagnitude = (gradientX.pow(2) + gradientY.pow(2)).sqrt();
    Eigen::ArrayXXd gradientDirection = gradientY.atan2(gradientX);

    // implement non-maximum suppression
    Eigen::ArrayXXd suppressedImage;


    return grayImage;//change this
}

