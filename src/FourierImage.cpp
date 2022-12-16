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

/*!
 * *@brief Method to show Fourier Image in a window.
 * @details This overrides Image::show() method. If the Fourier Transform has been computed,
 * it shows the grayscale image, the log magnitude of the FT and the phase of the FT in the same window.
 * Otherwise, it just shows the image.
 * @param window_name Name of the window
 * @return
 */
void FourierImage::show(const string& window_name){
    // check if FT has been computed
    if (this->data_transf.size() == 0) {
        // show image
        this->showImage(window_name);
    } else {
        cv::Mat image, log_mag_img, phase_img;
        Eigen::ArrayXXd log_mag = this->getMagnitude(true);
        Eigen::ArrayXXd phase = this->getPhase();

        image = Image(3,this->getData(0)).toCvMat();
        log_mag_img = Image(3,normalize(log_mag)).toCvMat();
        phase_img = Image(3, normalize(phase)).toCvMat();

        cv::namedWindow(window_name + " (Image)", cv::WINDOW_NORMAL);
        cv::namedWindow(window_name + " (FT Log Magnitude)", cv::WINDOW_NORMAL);
        cv::namedWindow(window_name + " (FT Phase)", cv::WINDOW_NORMAL);

        // resize windows to occupy a third of the screen
        int screen_width = 1980;
        int screen_height = 1080;

        // move and resize windows
        cv::moveWindow(window_name + " (Image)", 0, 0);
        cv::moveWindow(window_name + " (FT Log Magnitude)", screen_width/3, 0);
        cv::moveWindow(window_name + " (FT Phase)", 2*screen_width/3, 0);

        cv::imshow(window_name + " (Image)", image);
        cv::imshow(window_name + " (FT Log Magnitude)", log_mag_img);
        cv::imshow(window_name + " (FT Phase)", phase_img);

        cv::resizeWindow(window_name + " (Image)", screen_width / 3, screen_height / 3);
        cv::resizeWindow(window_name + " (FT Log Magnitude)", screen_width / 3, screen_height / 3);
        cv::resizeWindow(window_name + " (FT Phase)", screen_width / 3, screen_height / 3);

        // wait for key press
        cv::waitKey(0);

    }
}

/*!
 * @brief Method to show only the Image, without the FT.
 * @details Calls Image::show() method to show only the image.
 * @param window_name
 * @return
 */
void FourierImage::showImage(const string& window_name) {
    Image::show(window_name);
}