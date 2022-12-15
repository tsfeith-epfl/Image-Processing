//
// Created by André Charneca on 12.12.22.
//

#include "FourierImage.hpp"

// Transform methods //

/*!
 * @brief Applies the Fourier Transform to the image.
 * @details Applies the Fourier Transform to the image and stores the result in the transform attribute.
 * @param show_progress Whether to show the progress of the computation.
 * @return
 */
void FourierImage::applyTransform(bool show_progress) {
    Eigen::ArrayXXcd input;
    // convert to complex, and to grayscale if necessary
    if (this->getChannels() > 1) {
        std::cerr << "FourierImage::applyTransform: Converting to grayscale..." << endl;
        input = this->reduceChannels().getData(0).cast<std::complex<double>>();
    } else {
        input = this->getData(0).cast<std::complex<double>>();
    }
    this->data_transf = dft2(input, false, show_progress);
}


/*!
 * @brief Applies the inverse Fourier Transform to the image.
 * @details Applies the inverse Fourier Transform to the image, and stores the real part in the data attribute.
 * @param show_progress Whether to show the progress of the computation.
 * @return A new FourierImage object with the result of the inverse transform.
 */
FourierImage FourierImage::applyInverseTransform(bool show_progress) {
    // check if transform has been applied
    if (this->data_transf.size() == 0) {
        throw std::runtime_error("No transform has been applied to the image.");
    }

    // apply inverse transform
    Eigen::ArrayXXcd output = dft2(this->data_transf, true, show_progress);

    // create new fourier image
    Eigen::ArrayXXd output_real = output.real();
    FourierImage result = FourierImage(normalize(output_real));
    result.setTransform(this->data_transf);
    return result;
}


// Getters //

/*!
 * @brief Returns the transform attribute.
 * @details Returns the transform attribute.
 * @return The transform attribute.
 */
Eigen::ArrayXXcd FourierImage::getTransform() const {
    return this->data_transf;
}


/*!
 * @brief Returns the magnitude of the transform attribute.
 * @details Returns the magnitude of the transform attribute as a new Eigen::ArrayXXd object.
 * @return The magnitude of the transform attribute.
 */
Eigen::ArrayXXd FourierImage::getMagnitude(bool log) const {
    // throw error if transform has not been applied
    if (this->data_transf.size() == 0) {
        throw std::runtime_error("No transform has been applied to the image.");
    }

    Eigen::ArrayXXd magnitude = this->data_transf.abs();
    if (log) {
        magnitude = (magnitude + 1e-8).log();
    }
    return magnitude;
}


/*!
 * @brief Returns the phase of the transform attribute.
 * @details Returns the phase of the transform attribute as a new Eigen::ArrayXXd object.
 * @return The phase of the transform attribute.
 */
Eigen::ArrayXXd FourierImage::getPhase() const {
    // throw error if transform has not been applied
    if (this->data_transf.size() == 0) {
        throw std::runtime_error("No transform has been applied to the image.");
    }
    Eigen::ArrayXXd phase = this->data_transf.arg();
    return phase;
}


/*!
 * @brief Returns the real part of the transform attribute.
 * @details Returns the real part of the transform attribute as a new Eigen::ArrayXXd object.
 * @return The real part of the transform attribute.
 */
Eigen::ArrayXXd FourierImage::getReal() const {
    // throw error if transform has not been applied
    if (this->data_transf.size() == 0) {
        throw std::runtime_error("No transform has been applied to the image.");
    }
    Eigen::ArrayXXd real = this->data_transf.real();
    return real;
}


/*!
 * @brief Returns the imaginary part of the transform attribute.
 * @details Returns the imaginary part of the transform attribute as a new Eigen::ArrayXXd object.
 * @return The imaginary part of the transform attribute.
 */
Eigen::ArrayXXd FourierImage::getImaginary() const {
    // throw error if transform has not been applied
    if (this->data_transf.size() == 0) {
        throw std::runtime_error("No transform has been applied to the image.");
    }
    Eigen::ArrayXXd imaginary = this->data_transf.imag();
    return imaginary;
}


// Setters //

/*!
 * @brief Sets the transform attribute.
 * @details Sets the transform attribute.
 * @param transform The new transform attribute.
 * @return
 */
void FourierImage::setTransform(const Eigen::ArrayXXcd &transform) {
    // check if transform is valid
    if (transform.rows() != this->getHeight() || transform.cols() != this->getWidth()) {
        throw std::runtime_error("Invalid transform size.");
    }

    this->data_transf = transform;
}


// Filter methods //

/*!
 * @brief Applies a low pass filter.
 * @details Applies a low pass filter to the transform attribute.
 * @param cutoff The cutoff frequency of the filter. It's the radius of the circle that will be kept,
 * relative to the minimum dimension of the image.
 * @return
 */
void FourierImage::applyLowPassFilter(double cutoff) {
    // check if transform has been applied
    if (this->data_transf.size() == 0) {
        throw std::runtime_error("No transform has been applied to the image.");
    }

    // check if cutoff is valid
    if (cutoff < 0) {
        throw std::invalid_argument("Invalid cutoff value.");
    }

    // sizes
    int rows = (int) this->data_transf.rows();
    int cols = (int) this->data_transf.cols();

    // apply filter, assuming the frequency domain is centered in the image
    Eigen::ArrayXXcd output = this->data_transf;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double distance = sqrt(pow(i - (rows - 1) / 2, 2) + pow(j - (cols - 1) / 2, 2));
            if (distance >= cutoff * min(rows, cols) / 2) {
                output(i, j) = 0;
            }
        }
    }
    this->data_transf = output;
}


/*!
 * @brief Applies a high pass filter.
 * @details Applies a high pass filter to the transform attribute.
 * @param cutoff The cutoff frequency of the filter. It's the radius of the circle that will be removed,
 * relative to the minimum dimension of the image.
 * @return
 */
void FourierImage::applyHighPassFilter(double cutoff) {
    // check if transform has been applied
    if (this->data_transf.size() == 0) {
        throw std::runtime_error("No transform has been applied to the image.");
    }

    // check if cutoff is valid
    if (cutoff < 0) {
        throw std::invalid_argument("Invalid cutoff value.");
    }

    // sizes
    int rows = (int) this->data_transf.rows();
    int cols = (int) this->data_transf.cols();

    // apply filter, assuming the frequency domain is centered in the image
    Eigen::ArrayXXcd output = this->data_transf;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double distance = sqrt(pow(i - (rows - 1) / 2, 2) + pow(j - (cols - 1) / 2, 2));
            if (distance <= cutoff * min(rows, cols) / 2) {
                output(i, j) = 0;
            }
        }
    }
    this->data_transf = output;
}


/*!
 * @brief Applies a band pass filter.
 * @details Applies a band pass filter to the transform attribute.
 * @param cutoff1 The first cutoff frequency of the filter. It's the radius of the inner circle that will be removed,
 * relative to the minimum dimension of the image.
 * @param cutoff2 The second cutoff frequency of the filter. It's the radius of the outer circle that will be removed,
 * relative to the minimum dimension of the image.
 * @return
 */
void FourierImage::applyBandPassFilter(double cutoff1, double cutoff2) {
    // check if transform has been applied
    if (this->data_transf.size() == 0) {
        throw std::runtime_error("No transform has been applied to the image.");
    }
    // check if cutoffs are valid
    if (cutoff1 > cutoff2) {
        throw std::invalid_argument("Lower cutoff must be smaller than upper cutoff.");
    }
    if (cutoff1 < 0 || cutoff2 < 0) {
        throw std::invalid_argument("Cutoffs must be positive.");
    }
    // apply high pass filter
    this->applyHighPassFilter(cutoff1);

    // apply low pass filter
    this->applyLowPassFilter(cutoff2);
}
