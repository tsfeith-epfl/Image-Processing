//
// Created by André Charneca on 06.12.22.
//

#include "ContourExtractor.hpp"
#include "operations.hpp"

/* Constructors */

/*!
 * @brief Default Constructor for ContourExtractor
 * @details Creates a default Denoiser object, and sets the threshold to 0.3.
 * @return
 */
ContourExtractor::ContourExtractor() {
    threshold = 0.3;
    denoiser = Denoiser();
}

/*!
 * @brief Parametric constructor for ContourExtractor
 * @details Creates a Denoiser object with the given kernel size and sigma value, and sets the threshold to the given value.
 * @param threshold The threshold value for the gradient magnitude.
 * @param kernelSize The size of the kernel. Must be odd.
 * @param sigma The sigma value of the Gaussian Filter. Set to 0 to use a mean filter.
 * @return
 */
ContourExtractor::ContourExtractor(double threshold, int kernelSize, double sigma) {
    this->threshold = threshold;
    denoiser = Denoiser(kernelSize, sigma);
}

/* Public Methods */

/*!
 * @brief Getter for the threshold value
 * @return The threshold value
 */
double ContourExtractor::getThreshold() const {
    return threshold;
}

/*!
 * @brief Getter for the Denoiser object
 * @return The Denoiser object
 */
Denoiser ContourExtractor::getDenoiser() const {
    return denoiser;
}


/*!
 * @brief Setter for the threshold value
 * @param threshold The new threshold value
 */
void ContourExtractor::setThreshold(double threshold) {
    this->threshold = threshold;
}

/*!
 * @brief Setter for the Denoiser object
 * @param denoiser The new Denoiser object
 */
void ContourExtractor::setDenoiser(const Denoiser& denoiser) {
    this->denoiser = denoiser;
}

/*!
 * @brief Extracts the contours from the given image
 * @details The image is first denoised, then the gradient is calculated, and the gradient magnitude is thresholded.
 * @param image The image to extract the contours from
 * @return The extracted contours (binary image)
 */
Image ContourExtractor::extractContours(const Image& image, bool show) {
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

