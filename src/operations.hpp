//
// Created by André Charneca on 01.12.22.
//

#ifndef IMAGE_PROCESSING_OPERATIONS_HPP
#define IMAGE_PROCESSING_OPERATIONS_HPP

#include <Eigen/Eigen>
#include <vector>
#include "Image.hpp"

Eigen::ArrayXXd applyConvolution(const Eigen::ArrayXXd input, const Eigen::ArrayXXd kernel);
Image applyConvolution(const Image& input, const Eigen::ArrayXXd& kernel);

Eigen::ArrayXXd computeGradientX(const Image& input);
Eigen::ArrayXXd computeGradientY(const Image& input);
Image computeGradientMagnitude(const Image& input);
Image computeGradientDirection(const Image& input);



#endif //IMAGE_PROCESSING_OPERATIONS_HPP
