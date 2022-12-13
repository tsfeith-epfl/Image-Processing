//
// Created by André Charneca on 01.12.22.
//

#ifndef IMAGE_PROCESSING_OPERATIONS_HPP
#define IMAGE_PROCESSING_OPERATIONS_HPP

#include <Eigen/Eigen>
#include <vector>
#include "Image.hpp"

// General //
/*!
 * Normalizes the input array.
 * @details This function normalizes the input array to the range [0, 1], and converts it to a double array.
 * @param input Input array
 * @return Normalized array
 */
template <typename T>
Eigen::ArrayXXd normalize(const Eigen::ArrayBase<T>& array){
    //Note: we implement this here because otherwise we would have to provide the template argument (aka normalize<double>)
    // every time we call this function.
    Eigen::ArrayXXd array_doubles = array.template cast<double>();
    array_doubles = (array_doubles - array_doubles.minCoeff()) / (array_doubles.maxCoeff() - array_doubles.minCoeff());
    return array_doubles;
}

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



#endif //IMAGE_PROCESSING_OPERATIONS_HPP
