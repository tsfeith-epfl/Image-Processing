//
// Created by André Charneca on 12.12.22.
//
// Child of Image class. Contains methods to perform Fourier Transformations on the image.

#ifndef IMAGEPROCESSING_FOURIERIMAGE_HPP
#define IMAGEPROCESSING_FOURIERIMAGE_HPP

#include "Image.hpp"

class FourierImage : public Image {
private:
    Eigen::ArrayXXcd transform;

public:

    void applyTransform();
    void applyInverseTransform();

    [[nodiscard]] Eigen::ArrayXXcd getTransform() const;
    [[nodiscard]] Eigen::ArrayXXd getMagnitude() const;
    [[nodiscard]] Eigen::ArrayXXd getPhase() const;
    [[nodiscard]] Eigen::ArrayXXd getReal() const;
    [[nodiscard]] Eigen::ArrayXXd getImaginary() const;

    void setTransform(const Eigen::ArrayXXcd& transform);
};


#endif //IMAGEPROCESSING_FOURIERIMAGE_HPP
