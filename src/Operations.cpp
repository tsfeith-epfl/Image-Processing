//
// Created by André Charneca on 01.12.22.
//

#include <Eigen/Eigen>
#include "Operations.hpp"

/* Convolution Functions */

Eigen::ArrayXXd applyConvolution(Eigen::ArrayXXd input, Eigen::ArrayXXd kernel) {
    /**
     * Applies a convolution to an Eigen array, with 0-padding. The output has the same size as the input.
     */

    // error: kernel size must be odd
    if (kernel.rows() % 2 == 0 || kernel.cols() % 2 == 0) {
        throw std::invalid_argument("kernel size must be odd");
    }

    // error: kernel size must be smaller than input size
    if (kernel.rows() > input.rows() || kernel.cols() > input.cols()) {
        throw std::invalid_argument("kernel size must be smaller than input size");
    }

    // error: kernel must be square
    if (kernel.rows() != kernel.cols()) {
        throw std::invalid_argument("kernel must be square");
    }

    // error: empty input
    if (input.rows() == 0 || input.cols() == 0) {
        throw std::invalid_argument("empty input");
    }

    // error: empty kernel
    if (kernel.rows() == 0 || kernel.cols() == 0) {
        throw std::invalid_argument("empty kernel");
    }

    // this has to work for rectangular inputs
    int kernel_size = kernel.rows();
    int kernel_radius = (kernel_size - 1) / 2;
    int input_rows = input.rows();
    int input_cols = input.cols();

    // initialize output
    Eigen::ArrayXXd output = Eigen::ArrayXXd::Zero(input_rows, input_cols);

    // apply convolution
    for (int i = 0; i < input_rows; i++) {
        for (int j = 0; j < input_cols; j++) {
            for (int k = 0; k < kernel_size; k++) {
                for (int l = 0; l < kernel_size; l++) {
                    int input_i = i + k - kernel_radius;
                    int input_j = j + l - kernel_radius;
                    if (input_i >= 0 && input_i < input_rows && input_j >= 0 && input_j < input_cols) {
                        output(i, j) += input(input_i, input_j) * kernel(k, l);
                    }
                }
            }
        }
    }

    return output;
}


