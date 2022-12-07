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

    return 0;
}

