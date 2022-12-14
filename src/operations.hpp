//
// Created by André Charneca on 01.12.22.
//

#ifndef IMAGE_PROCESSING_OPERATIONS_HPP
#define IMAGE_PROCESSING_OPERATIONS_HPP


#include <Eigen/Eigen>
#include <vector>
#include "Image.hpp"

// General //
template <typename T>
Eigen::ArrayXXd normalize(const Eigen::ArrayBase<T>& input);


// Convolution //
Eigen::ArrayXXd applyConvolution(Eigen::ArrayXXd input, Eigen::ArrayXXd kernel);
Image applyConvolution(const Image& input, const Eigen::ArrayXXd& kernel);

// Contour Extractor //
Eigen::ArrayXXd computeGradientX(const Image& input);
Eigen::ArrayXXd computeGradientY(const Image& input);
Image computeGradientMagnitude(const Image& input);

[[maybe_unused]] Image computeGradientDirection(const Image& input);
Image applyThreshold(const Image& input, double threshold);

// Fourier Transform //
Eigen::ArrayXcd dft(Eigen::ArrayXcd input, bool inverse = false);
Eigen::ArrayXXcd dft2(Eigen::ArrayXXcd input, bool inverse = false, bool show_progress = false);

#endif //IMAGE_PROCESSING_OPERATIONS_HPP
