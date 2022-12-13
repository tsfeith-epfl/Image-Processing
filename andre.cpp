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
#include <type_traits>
#include <complex>

using namespace std;

// function to compute DFT of a 1D array
template<typename T>
Eigen::Array<std::complex<T>, Eigen::Dynamic, 1> dft(Eigen::Array<std::complex<T>, Eigen::Dynamic, 1> input, bool inverse = false) {
    int N = input.size();
    Eigen::Array<std::complex<T>, Eigen::Dynamic, 1> output = Eigen::Array<std::complex<T>, Eigen::Dynamic, 1>::Zero(N);

    std::complex<T> omega;
    for (int k = -N / 2; k < N / 2; k++) {
        for (int n = -N / 2; n < N / 2; n++) {
            if (inverse) {
                omega = std::exp(std::complex<T>(0, 2 * M_PI * k * n / N));
            } else {
                omega = std::exp(std::complex<T>(0, -2 * M_PI * k * n / N));
            }
            output(k + N / 2) += input(n + N / 2) * omega;
        }
    }
    if (inverse) {
        output /= N;
    }

    return output;
}

// function to compute FFT of a 2D array, by computing 1D FFTs
// for inverse, we assume the DFT is already centered
template <typename T>
Eigen::Array<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> dft2(Eigen::Array<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> input, bool inverse = false, bool show_progress = false){
    int N = input.rows();
    int M = input.cols();
    Eigen::Array<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> output = Eigen::Array<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic>::Zero(N, M);

    for (int i = 0; i < N; i++){
        if (show_progress) {
            cout << "\rComputing row " << i << " of " << N << flush;
        }

        Eigen::Array<std::complex<T>, Eigen::Dynamic, 1> row = input.row(i);
        Eigen::Array<std::complex<T>, Eigen::Dynamic, 1> row_dft = dft(row, inverse);
        output.row(i) = row_dft;
    }

    if (show_progress) cout << endl;

    for (int j = 0; j < M; j++){
        if (show_progress) {
            cout << "\rComputing column " << j << " of " << M << flush;
        }

        Eigen::Array<std::complex<T>, Eigen::Dynamic, 1> col = output.col(j);
        Eigen::Array<std::complex<T>, Eigen::Dynamic, 1> col_dft = dft(col, inverse);
        output.col(j) = col_dft;
    }
    if (show_progress){
        cout << endl;
    }

    return output;
}

int main(){
    // test fft2 with image
    Image img = Image("ft_man.png");
    cout << "Image size: " << img.getWidth() << " x " << img.getHeight() << endl;
    img = img.reduceChannels();
    // print min and max values
    cout << "Min value: " << img(0).minCoeff() << endl;
    cout << "Max value: " << img(0).maxCoeff() << endl;

    // get input array, after casting to complex
    Eigen::Array<complex<double>, Eigen::Dynamic, Eigen::Dynamic> input = img(0).cast<complex<double>>();

    // compute dft
    auto img_dft = dft2(input, false, true);
    auto img_idft = dft2(img_dft, true, true);

    // print min and max values
    cout << "Recons Min value: " << img_idft.real().minCoeff() << endl;
    cout << "Recons Max value: " << img_idft.real().maxCoeff() << endl;

    //show
    img.show("Original");
    Image img_dft_mag_img = Image(normalize((img_dft.abs()+1e-5).log()));
    cout << img_dft.abs().log() << endl;
    img_dft_mag_img.show("DFT magnitude (log)");

    Image img_dft_phase_img = Image(normalize(img_dft.arg()));
    img_dft_phase_img.show("DFT phase");

    Image img_idft_img = Image(normalize(img_idft.real()));
    img_idft_img.show("IDFT");
    return 0;

}

