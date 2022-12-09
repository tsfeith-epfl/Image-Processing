//
// Created by André Charneca on 07.12.22.
//
// File to play around with some of the code. Remove this file before submission.

#include <iostream>
#include <Eigen/Eigen>
#include "Image.hpp"
#include "operations.hpp"
#include "Denoiser.hpp"
#include "ContourExtractor.hpp"

using namespace std;

int main(){
    // read image
    Image image("pinwheel.png");

    // create contour extractor
    ContourExtractor contourExtractor(0.3, 5, 2);

    // extract contours
    Image contours = contourExtractor.extractContours(image, true);

    return 0;
}
