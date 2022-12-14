//
// Created by André Charneca on 07.12.22.
//
// File to play around with some code. Remove this file before submission.

#include <iostream>
#include <Eigen/Eigen>
#include "Image.hpp"
#include "Denoiser.hpp"
#include "ContourExtractor.hpp"
#include "Histogram.hpp"
#include "parameters.hpp"
#include <fstream>
#include "operations.hpp"
#include "FourierImage.hpp"
#include <complex>

using namespace std;

int main_old(){
    // test fft2 with image
    Image img = Image("tiger.png");
    cout << "Image size: " << img.getWidth() << " x " << img.getHeight() << endl;
    img = img.reduceChannels();
    // print min and max values
    cout << "Min value: " << img(0).minCoeff() << endl;
    cout << "Max value: " << img(0).maxCoeff() << endl;

    // get input array, after casting to complex
    Eigen::ArrayXXcd input = img(0).cast<complex<double>>();

    // compute dft

    Eigen::ArrayXXcd img_dft = dft2(input, false, true);
    Eigen::ArrayXXcd img_idft = dft2(img_dft, true, true);

    // print min and max values
    cout << "Recons Min value: " << img_idft.real().minCoeff() << endl;
    cout << "Recons Max value: " << img_idft.real().maxCoeff() << endl;

    //show
    img.show("Original");
    Eigen::ArrayXXd mag_log = (img_dft.abs()+1e-5).log();
    Image img_dft_mag_img = Image(normalize(mag_log));
    img_dft_mag_img.show("DFT magnitude (log)");

    Eigen::ArrayXXd phase = img_dft.arg();
    Image img_dft_phase_img = Image(normalize(phase));
    img_dft_phase_img.show("DFT phase");

    Eigen::ArrayXXd img_idft_real = img_idft.real();
    Image img_idft_img = Image(normalize(img_idft_real));
    img_idft_img.show("IDFT");
    /*
    */
    return 0;
}

int main(){
    // test dft2
    Eigen::ArrayXXd input = Eigen::ArrayXXd::Random(4,4);
    input << 1, 2, 3, 4,
             5, 6, 7, 8,
             9, 10, 11, 12,
             13, 14, 15, 16;
    input = input/16;
    FourierImage img = FourierImage(1,input);
    cout << "Input: " << endl << input << endl;
    cout << "Fourier Transform" << endl;
    img.applyTransform(true);
    cout << img.getTransform() << endl;

    /*
    // show magnitude
    Image(normalize(img.getMagnitude(true))).show("Magnitude (log)");

    // apply high pass filter
    img.applyBandPassFilter(0.1, 0.5);

    // show magnitude
    Image(normalize(img.getMagnitude(true))).show("Magnitude (log) - band pass");

    // show inverse transform
    FourierImage(img.applyInverseTransform(true)).show("Inverse transform");
     */
    return 0;
}

