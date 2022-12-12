//
// Created by André Charneca on 07.12.22.
//
// File to play around with some code. Remove this file before submission.

#include <iostream>
#include <Eigen/Eigen>
#include "Image.hpp"
#include "operations.hpp"
#include "Denoiser.hpp"
#include "ContourExtractor.hpp"
#include "Histogram.hpp"
#include "parameters.hpp"

using namespace std;

int main(){
    // read image
    Image image("pinwheel.png");

    // create contour extractor
    ContourExtractor contourExtractor(0.3, 5, 2);

    // extract contours
    //Image contours = contourExtractor.extractContours(image, true);

    // histogram
    Histogram histogram(HISTOGRAM_BINS, HISTOGRAM_MIN, HISTOGRAM_MAX, LOG_SCALE);
    histogram.getHistogram(image);
    return 0;
}
//Q: How to use default arguments of functions in c++?
//A:
