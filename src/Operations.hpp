//
// Created by André Charneca on 01.12.22.
//

#ifndef IMAGE_PROCESSING_OPERATIONS_HPP
#define IMAGE_PROCESSING_OPERATIONS_HPP

/* Convolution Functions */

Eigen::ArrayXXd applyConvolution(Eigen::ArrayXXd input, Eigen::ArrayXXd kernel);
//Image applyConvolution(Image input, Eigen::ArrayXXd kernel);

#endif //IMAGE_PROCESSING_OPERATIONS_HPP
