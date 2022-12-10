//
// Created by André Charneca on 06.12.22.
//

#include "ContourExtractor.hpp"
#include "operations.hpp"

/* Constructors */

/*!
 * @brief Default Constructor for ContourExtractor
 * @details Creates a default ContourExtractor object with default parameters.
 * @return
 */
ContourExtractor::ContourExtractor() {
    this->threshold = 0.3;
    this->denoiser = Denoiser();
}

/*!
 * @brief Parametric constructor for ContourExtractor
 * @details Creates the internal Denoiser object with the given kernel size and sigma value, and sets the threshold to
 * the given value.
 * @param threshold The threshold value for the gradient magnitude.
 * @param kernelSize The size of the kernel. Must be odd.
 * @param sigma The sigma value of the Gaussian Filter. Set to 0 to use a mean filter.
 * @return
 */
ContourExtractor::ContourExtractor(double threshold, int kernelSize, double sigma) {
    if (threshold < 0 || threshold > 1) {
        throw std::invalid_argument("Threshold must be between 0 and 1");
    }
    this->threshold = threshold;
    this->denoiser = Denoiser(kernelSize, sigma);
}

/*!
 * @brief Parametric constructor for ContourExtractor
 * @details Creates the internal Denoiser object from the given kernel, and sets the threshold to the given value.
 * @param threshold The threshold value for the gradient magnitude.
 * @param kernel The kernel to be used for the denoising.
 */
ContourExtractor::ContourExtractor(double threshold, const Eigen::ArrayXXd &kernel) {
    if (threshold < 0 || threshold > 1) {
        throw std::invalid_argument("Threshold must be between 0 and 1");
    }
    this->threshold = threshold;
    this->denoiser = Denoiser(kernel);
}


/* Public Methods */

/*!
 * @brief Getter for the threshold value
 * @return The threshold value
 */
double ContourExtractor::getThreshold() const {
    return this->threshold;
}

/*!
 * @brief Getter for the Denoiser object
 * @return The Denoiser object
 */
Denoiser ContourExtractor::getDenoiser() const {
    return this->denoiser;
}


/*!
 * @brief Setter for the threshold value
 * @param threshold The new threshold value
 */
void ContourExtractor::setThreshold(double threshold) {
    if (threshold < 0 || threshold > 1) {
        throw std::invalid_argument("Threshold must be between 0 and 1");
    }
    this->threshold = threshold;
}

/*!
 * @brief Setter for the Denoiser object
 * @param denoiser The new Denoiser object
 */
void ContourExtractor::setDenoiser(const Denoiser &denoiser) {
    this->denoiser = denoiser;
}

void ContourExtractor::setDenoiser(int kernel_size, double sigma) {
    Denoiser new_denoiser = Denoiser(kernel_size, sigma);
    this->denoiser = new_denoiser;
}

/*!
 * @brief Extracts the contours from the given image
 * @details The image is first denoised, then the gradient is calculated, and the gradient magnitude is thresholded.
 * @param image The image to extract the contours from
 * @return The extracted contours (binary image)
 */
Image ContourExtractor::extractContours(const Image &image, bool show) {
    // convert to grayscale
    Image gray = image;
    gray = gray.reduceChannels();

    // denoise image
    Image denoised = this->denoiser.denoise(gray);

    // compute gradient magnitude
    Image gradientMagnitude = computeGradientMagnitude(denoised);

    // apply thresholding
    Image contours = applyThreshold(gradientMagnitude, this->threshold);

    if (show) {
        contours.show("Contours");
    }

    return contours;
}