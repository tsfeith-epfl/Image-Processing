//
// Created by tsfeith on 07/12/22.
//

#include "Histogram.hpp"

/*!
 * @brief Default constructor for the Histogram class
 * @details This constructor sets the default values for the Histogram class.
 * The default values are 500 bins, a range from 0. to 1. and a linear scale.
 */
Histogram::Histogram() {
    this->bins = 500;
    this->min_range = 0;
    this->max_range = 1;
    this->log = false;
}

/*!
 * @brief Constructor for the Histogram class
 * @details This constructor sets the number of bins for the Histogram class.
 * The range is set to [0, 1] and the scale is linear.
 * @param bins Number of bins for the histogram
 */
Histogram::Histogram(int bins) : Histogram() {
    if (bins <= 0) {
        throw invalid_argument("Number of bins must be greater than 0");
    }
    this->bins = bins;
}

/*!
 * @brief Constructor for the Histogram class
 * @details This constructor sets the number of bins and the range for the Histogram class.
 * The scale is linear.
 * @param bins Number of bins for the histogram
 * @param min_range Minimum value of the range
 * @param max_range Maximum value of the range
 */
Histogram::Histogram(int bins, double min_range, double max_range) : Histogram(bins) {
    if (min_range >= max_range) {
        throw invalid_argument("Min range must be smaller than max range");
    }
    if (min_range < 0 || max_range > 1) {
        throw invalid_argument("Min range and max range must be between 0 and 1");
    }
    this->min_range = min_range;
    this->max_range = max_range;
}

/*!
 * @brief Constructor for the Histogram class
 * @details This constructor sets the number of bins, the range and the scale for the Histogram class.
 * @param bins Number of bins for the histogram
 * @param min_range Minimum value of the range
 * @param max_range Maximum value of the range
 * @param log_scale Boolean to choose the scale of the histogram
 */
Histogram::Histogram(int bins, double min_range, double max_range, bool log_scale) : Histogram(bins, min_range, max_range) {
    this->log = log_scale;
}

/*!
 * @brief Getter for the number of bins
 * @return Number of bins
 */
int Histogram::getBins() const {
    return this->bins;
}

/*!
 * @brief Getter for the minimum value of the range
 * @return Minimum value of the range
 */
double Histogram::getMinRange() const {
    return this->min_range;
}

/*!
 * @brief Getter for the maximum value of the range
 * @return Maximum value of the range
 */
double Histogram::getMaxRange() const {
    return this->max_range;
}

/*!
 * @brief Getter for the scale of the histogram
 * @return Boolean to choose the scale of the histogram
 */
bool Histogram::getLog() const {
    return this->log;
}

/*!
 * @brief Setter for the number of bins
 * @param bins Number of bins
 */
void Histogram::setBins(int bins) {
    if (bins <= 0) {
        throw invalid_argument("Number of bins must be greater than 0");
    }
    this->bins = bins;
}

/*!
 * @brief Setter for the minimum value of the range
 * @param min_range Minimum value of the range
 */
void Histogram::setMinRange(double min_range) {
    if (min_range >= this->max_range) {
        throw invalid_argument("Min range must be smaller than max range");
    }
    if (min_range < 0) {
        throw invalid_argument("Min range must be between 0 and max range");
    }
    this->min_range = min_range;
}

/*!
 * @brief Setter for the maximum value of the range
 * @param max_range Maximum value of the range
 */
void Histogram::setMaxRange(double max_range) {
    if (max_range <= this->min_range) {
        throw invalid_argument("Max range must be greater than min range");
    }
    if (max_range > 1) {
        throw invalid_argument("Max range must be between min range and 1");
    }
    this->max_range = max_range;
}

/*!
 * @brief Setter for the scale of the histogram
 * @param log_scale Boolean to choose the scale of the histogram
 */
void Histogram::setLogScale(bool log_scale) {
    this->log = log_scale;
}

/*!
 * @brief Function to compute the histogram of an image
 * @details This function computes the histogram of an image and returns it as a vector.
 * Each element of the vector contains another vector with the bin value and the number
 * of pixels in that bin. If the image provided is RGB, it is first converted to grayscale.
 * @param image Image to compute the histogram
 * @return Histogram of the image
 */
vector<vector<double>> Histogram::computeHistogram(Image image) const {
    vector<vector<double>> output;
    if (image.getChannels() != 1) {
        cout << "CAUTION: Image is not grayscale. Converting to grayscale..." << endl;
        image = image.reduceChannels();
    }
    for (int i = 0; i < this->bins; i++) {
        output.push_back({this->min_range + i * (this->max_range - this->min_range) / this->bins, 0});
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

/*!
 * @brief Function to show and save the histogram of an image
 * @details This function takes an image and plots the histogram of it.
 * It also saves the histogram in a file.
 * @param image Image to compute the histogram
 * @param output Path to save the histogram
 * @param show Boolean to show the histogram
 */
void Histogram::getHistogram(const Image &image, bool show, const string &output) const {
    if (!show and output.empty()) {
        throw invalid_argument("This method needs to either show the histogram or save it to a file");
    }
    vector<vector<double>> hist = this->computeHistogram(image);
    // plot histogram using gnuplot
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (!output.empty()) {
        fprintf(gnuplotPipe, "set terminal png size 800,600\n");
        fprintf(gnuplotPipe, "set output '%s'\n", output.c_str());
    }
    else {
        fprintf(gnuplotPipe, "set terminal qt size 800,600\n");
    }
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

    if (!output.empty() and show) {
        fprintf(gnuplotPipe, "set terminal qt size 800,600\n");
        fprintf(gnuplotPipe, "set output\n");
        fprintf(gnuplotPipe, "replot\n");
    }

    fflush(gnuplotPipe);
    pclose(gnuplotPipe);
}