//
// Created by tsfeith on 07/12/22.
//

#ifndef IMAGEPROCESSING_HISTOGRAM_HPP
#define IMAGEPROCESSING_HISTOGRAM_HPP

#include "Image.hpp"
#include <fstream>
#include <Eigen/Eigen>


/**
 * @brief The Histogram class
 * @details This class allows to extract an intensity histogram from an image.
 * If the image is RGB, it first converts it to grayscale. The histogram is
 * built with a given number of bins and a given range. The range is given
 * by the minimum and maximum values of the histogram. It is also possible to
 * choose for the y-scale to be logarithmic.
 */
class Histogram {
private:
    /**
     * @var bins
     * Number of bins in the histogram.
     */
    int bins;
    /**
     * @var min_range
     * Minimum value of the histogram range.
     */
    double min_range;
    /**
     * @var max_range
     * Maximum value of the histogram range.
     */
    double max_range;
    /**
     * @var log_scale
     * Boolean to choose if the y-scale is logarithmic or not.
     */
    bool log;

public:
    Histogram();
    explicit Histogram(int bins);
    Histogram(int bins, double min_range, double max_range);
    Histogram(int bins, double min_range, double max_range, bool log_scale);

    [[nodiscard]] int getBins() const;
    [[nodiscard]] double getMinRange() const;
    [[nodiscard]] double getMaxRange() const;
    [[nodiscard]] bool getLog() const;

    void setBins(int bins);
    void setMinRange(double min_range);
    void setMaxRange(double max_range);
    void setLogScale(bool log_scale);

    [[nodiscard]] vector<vector<double>> computeHistogram(Image image) const;

    void getHistogram(const Image &image, bool show = false, const string &output = "") const;

};


#endif //IMAGEPROCESSING_HISTOGRAM_HPP
