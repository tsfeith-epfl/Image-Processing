//
// Created by André Charneca on 12.12.22.
//
// Child of Image class. Contains methods to perform Fourier Transformations on the image.

#ifndef IMAGEPROCESSING_FOURIERIMAGE_HPP
#define IMAGEPROCESSING_FOURIERIMAGE_HPP

#include "Image.hpp"

class FourierImage : public Image {
private:
    vector<Eigen::ArrayXXcd> transform_data;

public:

};


#endif //IMAGEPROCESSING_FOURIERIMAGE_HPP
