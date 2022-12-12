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
#include <fstream>

using namespace std;

int main(){
    // read image
    Image image("tiger.png");
    Image gray = image.reduceChannels();

    // count percentage of black pixels
    int black_pixels = 0;
    for (int i = 0; i < gray.getWidth(); i++){
        for (int j = 0; j < gray.getHeight(); j++){
            if (gray(i, j, 0) == 0){
                black_pixels++;
            }
        }
    }
    double percentage = (double) black_pixels / (gray.getHeight() * gray.getWidth());
    cout << "Percentage of black pixels: " << percentage << endl;

    // histogram
    Histogram histogram(HISTOGRAM_BINS, HISTOGRAM_MIN, HISTOGRAM_MAX, LOG_SCALE);
    histogram.getHistogram(image, true);
    return 0;
}

