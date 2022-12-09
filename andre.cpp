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
    Image image("teapot.png");

    image.show("Original Image");

    // compute gradients and display
    Eigen::ArrayXXd gradientX = computeGradientX(image);
    Eigen::ArrayXXd gradientY = computeGradientY(image);
    Image gradientMagnitude = computeGradientMagnitude(image);
    Image gradientDirection = computeGradientDirection(image);

    // normalize gradients and display
    gradientX = (gradientX - gradientX.minCoeff()) / (gradientX.maxCoeff() - gradientX.minCoeff());
    gradientY = (gradientY - gradientY.minCoeff()) / (gradientY.maxCoeff() - gradientY.minCoeff());
    Image gradientXImage = Image(1, gradientX);
    Image gradientYImage = Image(1, gradientY);
    gradientXImage.show("Gradient X");
    gradientYImage.show("Gradient Y");
    gradientMagnitude.show("Gradient Magnitude");
    gradientDirection.show("Gradient Direction");





    /*
    // apply gradients and show
    Image gradientX = computeGradientX(image);
    gradientX.show("Gradient X");

    Image gradientY = computeGradientY(image);
    gradientY.show("Gradient Y");

    Image gradientMagnitude = computeGradientMagnitude(image);
    gradientMagnitude.show("Gradient Magnitude");

    Image gradientDirection = computeGradientDirection(image);
    gradientDirection.show("Gradient Direction");*/


    return 0;
}
