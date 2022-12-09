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
    Image image("coins.jpg");

    image.show("Original Image");

    //denoise image
    Denoiser denoiser(3, 2.0);
    Image denoisedImage = denoiser.denoise(image);
    denoisedImage.show("Denoised Image");

    // compute gradients and display
    Image gradientMagnitude = computeGradientMagnitude(denoisedImage);
    Image gradientDirection = computeGradientDirection(denoisedImage);
    gradientMagnitude.show("Gradient Magnitude");
    gradientDirection.show("Gradient Direction");

    // apply thresholding
    Image thresholded = applyThreshold(gradientMagnitude, 0.5);
    thresholded.show("Thresholded");


    return 0;
}
