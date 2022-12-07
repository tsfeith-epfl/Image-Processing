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
    // read image and convert to grayscale
    Image image("teapot.png");
    // average channels
    vector<Eigen::ArrayXXd> avg_img = image.getData();

    Eigen::ArrayXXd avg = (avg_img[0] + avg_img[1] + avg_img[2]) / 3;

    Image avg_image(1, avg);

    // display 1 channel image
    cv::Mat avg_mat = avg_image.toCvMat();
    // convert to color
    cv::cvtColor(avg_mat, avg_mat, cv::COLOR_GRAY2BGR);
    cv::imshow("Average Image", avg_mat);
    cv::waitKey(0);


    //Q: the image is not displayed correctly because it only has 1 channel. How to fix this?
    //A: use cv::cvtColor to convert the image to a 3-channel image



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
