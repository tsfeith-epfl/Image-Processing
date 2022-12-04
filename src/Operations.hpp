//
// Created by André Charneca on 01.12.22.
//

#ifndef IMAGE_PROCESSING_OPERATIONS_HPP
#define IMAGE_PROCESSING_OPERATIONS_HPP

#include <Eigen/Eigen>
#include <vector>
#include "Image.hpp"

Eigen::ArrayXXd applyConvolution(Eigen::ArrayXXd input, Eigen::ArrayXXd kernel);
Image applyConvolution(const Image& input, const Eigen::ArrayXXd& kernel);

#endif //IMAGE_PROCESSING_OPERATIONS_HPP
