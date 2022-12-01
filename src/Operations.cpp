//
// Created by André Charneca on 01.12.22.
//

#include <Eigen/Eigen>
#include "Operations.hpp"

/* Convolution Functions */

Eigen::ArrayXXd applyConvolution(Eigen::ArrayXXd input, Eigen::ArrayXXd kernel) {
    /**
     * Applies a convolution to an Eigen array, with 0 padding such that the output has the same size as the input.
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


    int kernelSize = kernel.rows();
    int kernelCenter = kernelSize / 2;
    int inputSize = input.rows();
    int outputSize = inputSize;
    Eigen::ArrayXXd output = Eigen::ArrayXXd::Zero(outputSize, outputSize);

    for (int i = 0; i < outputSize; i++) {
        for (int j = 0; j < outputSize; j++) {
            for (int k = 0; k < kernelSize; k++) {
                for (int l = 0; l < kernelSize; l++) {
                    int input_i = i + k - kernelCenter;
                    int input_j = j + l - kernelCenter;
                    if (input_i >= 0 && input_i < inputSize && input_j >= 0 && input_j < inputSize) {
                        output(i, j) += input(input_i, input_j) * kernel(k, l);
                    }
                }
            }
        }
    }
    return output;
}


