//
// Created by André Charneca on 06.12.22.
// Contour extractor class using the Canny algorithm
//

#ifndef IMAGEPROCESSING_CONTOUREXTRACTOR_HPP
#define IMAGEPROCESSING_CONTOUREXTRACTOR_HPP

#include <Eigen/Eigen>
#include "Image.hpp"
#include <string>
#include "Denoiser.hpp"

using namespace std;

class ContourExtractor {
private:
    double threshold;
    Denoiser denoiser;

public:
    ContourExtractor();
    ContourExtractor(double threshold, int kernelSize, double sigma);
    ContourExtractor(double threshold, const Eigen::ArrayXXd& kernel);

    Image extractContours(const Image& image, bool show=false);
    Image extractContours(const Image& image, string output, bool show=false);

    [[nodiscard]] double getThreshold() const;
    [[nodiscard]] Denoiser getDenoiser() const;
};

#endif //IMAGEPROCESSING_CONTOUREXTRACTOR_HPP
