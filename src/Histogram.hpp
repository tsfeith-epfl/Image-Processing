//
// Created by tsfeith on 07/12/22.
//

#ifndef IMAGEPROCESSING_HISTOGRAM_HPP
#define IMAGEPROCESSING_HISTOGRAM_HPP

#include "Image.hpp"
#include <fstream>
#include <Eigen/Eigen>

class Histogram {
private:
    int bins;
    double min_range;
    double max_range;
    bool log;

public:
    Histogram();
    Histogram(int bins);
    Histogram(int bins, double min_range, double max_range);
    Histogram(int bins, double min_range, double max_range, bool log);

    [[nodiscard]] int getBins() const;
    [[nodiscard]] double getMinRange() const;
    [[nodiscard]] double getMaxRange() const;
    [[nodiscard]] bool getLog() const;

    void setBins(int bins);
    void setMinRange(double min_range);
    void setMaxRange(double max_range);
    void setLog(bool log);

    [[nodiscard]] vector<vector<double>> computeHistogram(Image image) const;

    void getHistogram(const Image& image, const string& output) const;

};


#endif //IMAGEPROCESSING_HISTOGRAM_HPP
