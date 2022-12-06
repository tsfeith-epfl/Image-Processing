//
// Created by André Charneca on 06.12.22.
//

#ifndef IMAGEPROCESSING_PARAMETERS_HPP
#define IMAGEPROCESSING_PARAMETERS_HPP

/* Denoiser Parameters */
double DENOISER_SIGMA = 1;
int DENOISER_KERNEL_SIZE = 3;

/* Contour Extractor Parameters */
double CONTOUR_EXTRACTOR_THRESHOLD = 0.5;
double CONTOUR_EXTRACTOR_SIGMA = 1;
int CONTOUR_EXTRACTOR_KERNEL_SIZE = 3;


/* Histogram Parameters */
int HISTOGRAM_BINS = 100;
double HISTOGRAM_MIN = 0;
double HISTOGRAM_MAX = 1;
bool HISTOGRAM_NORMALIZE = true;
bool HISTOGRAM_LOG_Y = false;

#endif //IMAGEPROCESSING_PARAMETERS_HPP
