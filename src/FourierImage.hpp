//
// Created by André Charneca on 12.12.22.
//
// Child of Image class. Contains methods to perform Fourier Transformations on the image.

#ifndef IMAGEPROCESSING_FOURIERIMAGE_HPP
#define IMAGEPROCESSING_FOURIERIMAGE_HPP

#include "Image.hpp"
#include "operations.hpp"
/*!
 * @brief The FourierImage class
 * @details This class inherits from the Image class and adds methods to perform Fourier Transformations on the image.
 * It also contains methods to perform inverse Fourier Transformations. The Fourier Transformations are performed using
 * DFT (Discrete Fourier Transform) and DFT2 (2D Discrete Fourier Transform).
 * The frequency domain is centered in the middle of the image.
 */
class FourierImage : public Image {
private:
    /*! Contains the Fourier Transform of the data.*/
    Eigen::ArrayXXcd data_transf;

public:
    using Image::Image; // use constructor inheritance
    explicit FourierImage(const Image& image) : Image(image){};
    void applyTransform(bool show_progress = false);
    FourierImage applyInverseTransform(bool show_progress = false);

    // Getters
    [[nodiscard]] Eigen::ArrayXXcd getTransform() const;
    [[nodiscard]] Eigen::ArrayXXd getMagnitude(bool log=false) const;
    [[nodiscard]] Eigen::ArrayXXd getPhase() const;
    [[nodiscard]] Eigen::ArrayXXd getReal() const;
    [[nodiscard]] Eigen::ArrayXXd getImaginary() const;

    // setters
    void setTransform(const Eigen::ArrayXXcd& transform);

    // filter methods
    void applyLowPassFilter(double cutoff);
    void applyHighPassFilter(double cutoff);
    void applyBandPassFilter(double cutoff1, double cutoff2);

    // override show method
    void show(const string& window_name = "window");
    void showImage(const string& window_name = "window");

};


#endif //IMAGEPROCESSING_FOURIERIMAGE_HPP
