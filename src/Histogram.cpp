//
// Created by tsfeith on 07/12/22.
//

#include "Histogram.hpp"

Histogram::Histogram() {
    this->bins = 100;
    this->min_range = 0;
    this->max_range = 1;
    this->log = false;
}

Histogram::Histogram(int bins) {
    this->bins = bins;
    this->min_range = 0;
    this->max_range = 1;
    this->log = false;
}

Histogram::Histogram(int bins, double min_range, double max_range) {
    this->bins = bins;
    this->min_range = min_range;
    this->max_range = max_range;
    this->log = false;
}

Histogram::Histogram(int bins, double min_range, double max_range, bool log) {
    this->bins = bins;
    this->min_range = min_range;
    this->max_range = max_range;
    this->log = log;
}

int Histogram::getBins() const {
    return this->bins;
}

double Histogram::getMinRange() const {
    return this->min_range;
}

double Histogram::getMaxRange() const {
    return this->max_range;
}

bool Histogram::getLog() const {
    return this->log;
}

void Histogram::setBins(int bins) {
    this->bins = bins;
}

void Histogram::setMinRange(double min_range) {
    this->min_range = min_range;
}

void Histogram::setMaxRange(double max_range) {
    this->max_range = max_range;
}

void Histogram::setLog(bool log) {
    this->log = log;
}

vector<vector<double>> Histogram::computeHistogram(Image image) const {
    vector<vector<double>> output;
    image = image.reduceChannels();
    // fill in the output with bin values
    for (int i = 0; i < this->bins; i++) {
        // set values with only 3 decimal places
        output.push_back({this->min_range + i * (this->max_range - this->min_range), 0});
    }

    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth(); j++) {
            double value = image(j, i, 0);
            if (value >= this->min_range && value <= this->max_range) {
                int bin = (int) ((value - this->min_range) / (this->max_range - this->min_range) * this->bins);
                if (bin == this->bins) {
                    bin--;
                }
                output[bin][1] += 1;
            }
        }
    }
    return output;
}

void Histogram::getHistogram(const Image &image, const string& output) const {
    vector<vector<double>> hist = this->computeHistogram(image);
    // plot histogram using gnuplot
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set terminal png size 800,600\n");
    fprintf(gnuplotPipe, "set output '%s'\n", output.c_str());
    fprintf(gnuplotPipe, "set style fill solid 1.0 border -1\n");
    fprintf(gnuplotPipe, "set style data histograms\n");
    fprintf(gnuplotPipe, "set xrange [%d:%d]\n", 0, this->bins);
    fprintf(gnuplotPipe, "set xlabel 'Intensity'\n");
    fprintf(gnuplotPipe, "set ylabel 'Frequency'\n");
    fprintf(gnuplotPipe, "set title 'Intensity Histogram'\n");

    if (this->log) {
        fprintf(gnuplotPipe, "set logscale y\n");
    }
    string ticks_positions;
    for (int i = 0; i < 6; i++) {
        ticks_positions += "'" + to_string(i * (this->max_range - this->min_range) / 5 + this->min_range) + "' ";
        ticks_positions += to_string(i * this->bins/5) + ", ";
    }
    fprintf(gnuplotPipe, "set xtics (%s)\n", ticks_positions.c_str());
    fprintf(gnuplotPipe, "plot '-' using 2 notitle lt rgb 'black'\n");
    for (auto & i : hist) {
        fprintf(gnuplotPipe, "%f %f\n", i[0], i[1]);
    }
    fprintf(gnuplotPipe, "e\n");

    // save plot to output file
    fprintf(gnuplotPipe, "set terminal qt size 800,600\n");
    fprintf(gnuplotPipe, "set output\n");
    fprintf(gnuplotPipe, "replot\n");

    fflush(gnuplotPipe);
    pclose(gnuplotPipe);
}