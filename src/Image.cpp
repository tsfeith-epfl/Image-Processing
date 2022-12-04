//
// Created by tsfeith on 03/12/22.
//

#include <fstream>
#include "Image.hpp"

// default image is (50 x 50 x 3) full of zeros
// this is an arbitrary choice
Image::Image() {
    this->width = 50;
    this->height = 50;
    this->channels = 3;
    this->data = vector<Eigen::ArrayXXd>(this->channels, Eigen::ArrayXXd::Zero(this->height, this->width));
}

Image::Image(string filename) {
    // fix filepath to take into account being in build directory
    string cpp_path = __FILE__;
    string dir_path = cpp_path.substr(0, cpp_path.find_last_of("/\\"));
    dir_path = dir_path.substr(0, dir_path.find_last_of("/\\"));
    filename = dir_path + "/images/" + filename;

    ifstream file(filename);
    if (!file.good()) {
        throw invalid_argument("File " + filename + " does not exist.");
    }
    cv::Mat image = cv::imread(filename);
    if (image.empty()) {
        throw invalid_argument("Could not open or find the image");
    }
    this->width = image.cols;
    this->height = image.rows;
    this->channels = image.channels();
    this->data = vector<Eigen::ArrayXXd>(this->channels, Eigen::ArrayXXd::Zero(this->height, this->width));
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            for (int k = 0; k < this->channels; k++) {
                this->data[k](i, j) = image.at<cv::Vec3b>(i, j)[k];
            }
        }
    }
}

// when no data is given, the image is full of zeros
Image::Image(int width, int height, int channels) {
    if (width <= 0 || height <= 0 || channels <= 0) {
        throw std::invalid_argument("Width, height and number of channels must be positive");
    }
    this->width = width;
    this->height = height;
    this->channels = channels;
    this->data = vector<Eigen::ArrayXXd>(this->channels, Eigen::ArrayXXd::Zero(this->height, this->width));
}

Image::Image(int channels, Eigen::ArrayXXd data) {
    if (channels <= 0) {
        throw std::invalid_argument("Number of channels must be positive");
    }
    if (data.cols() == 0 || data.rows() == 0) {
        throw std::invalid_argument("Data must not be empty");
    }
    for (int j = 0; j < data.rows(); j++) {
        for (int k = 0; k < data.cols(); k++) {
            if (data(j, k) < 0 || data(j, k) > 1) {
                throw std::invalid_argument("Pixel values must be between 0 and 1");
            }
        }
    }
    this->width = data.cols();
    this->height = data.rows();
    this->channels = channels;
    this->data = vector<Eigen::ArrayXXd>(this->channels, data);
}

Image::Image(Eigen::ArrayXXd data) {
    if (data.cols() == 0 || data.rows() == 0) {
        throw std::invalid_argument("Data must not be empty");
    }
    for (int j = 0; j < data.rows(); j++) {
        for (int k = 0; k < data.cols(); k++) {
            if (data(j, k) < 0 || data(j, k) > 1) {
                throw std::invalid_argument("Pixel values must be between 0 and 1");
            }
        }
    }
    this->width = data.cols();
    this->height = data.rows();
    this->channels = 1;
    this->data = vector<Eigen::ArrayXXd>(this->channels, data);
}

Image::Image(vector<Eigen::ArrayXXd> data) {
    if (data.empty()) {
        throw std::invalid_argument("Data must not be empty");
    }

    int first_width = data[0].cols();
    int first_height = data[0].rows();
    for (auto & i : data) {
        if (i.cols() == 0 || i.rows() == 0) {
            throw std::invalid_argument("Data must not be empty");
        }
        if (i.cols() != first_width || i.rows() != first_height) {
            throw std::invalid_argument("Data must have the same dimensions along all channels");
        }
        for (int j = 0; j < i.rows(); j++) {
            for (int k = 0; k < i.cols(); k++) {
                if (i(j, k) < 0 || i(j, k) > 1) {
                    throw std::invalid_argument("Pixel values must be between 0 and 1");
                }
            }
        }
    }
    this->width = data[0].cols();
    this->height = data[0].rows();
    this->channels = data.size();
    this->data = data;
}

Image::Image(const Image& image) {
    this->width = image.width;
    this->height = image.height;
    this->channels = image.channels;
    this->data = image.data;
}

int Image::getWidth() const {
    return this->width;
}

int Image::getHeight() const {
    return this->height;
}

int Image::getChannels() const {
    return this->channels;
}

vector<Eigen::ArrayXXd> Image::getData() const {
    return this->data;
}

Eigen::ArrayXXd Image::getData(int channel) const {
    if (channel < 0 || channel >= this->channels) {
        throw std::invalid_argument("Channel selected is not valid, must be between 0 and " + to_string(this->channels - 1));
    }
    return this->data[channel];
}

Eigen::ArrayXd Image::getPixel(int x, int y) const {
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
        throw std::invalid_argument("Pixel selected is not valid, must be between (0, 0) and (" + to_string(this->width - 1) + ", " + to_string(this->height - 1) + ")");
    }
    Eigen::ArrayXd pixel(this->channels);
    for (int i = 0; i < this->channels; i++) {
        pixel[i] = this->data[i](y, x);
    }
    return pixel;
}

double Image::getPixel(int x, int y, int channel) const {
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
        throw std::invalid_argument("Pixel selected is not valid, must be between (0, 0) and (" + to_string(this->width - 1) + ", " + to_string(this->height - 1) + ")");
    }
    if (channel < 0 || channel >= this->channels) {
        throw std::invalid_argument("Channel selected is not valid, must be between 0 and " + to_string(this->channels - 1));
    }
    return this->data[channel](y, x);
}

void Image::setData(vector<Eigen::ArrayXXd> new_data) {
    if (new_data.size() != this->channels) {
        throw std::invalid_argument("Number of channels in new data must match number of channels in image");
    }
    for (auto & i : new_data) {
        if (i.rows() != this->height || i.cols() != this->width) {
            throw std::invalid_argument("Image dimensions do not match data dimensions");
        }
        for (int j = 0; j < this->height; j++) {
            for (int k = 0; k < this->width; k++) {
                if (i(j, k) < 0 || i(j, k) > 1) {
                    throw std::invalid_argument("Pixel values must be between 0 and 1");
                }
            }
        }
    }
    this->data = new_data;
}

void Image::setData(int channel, Eigen::ArrayXXd new_data) {
    if (channel < 0 || channel >= this->channels) {
        throw std::invalid_argument("Channel selected is not valid, must be between 0 and " + to_string(this->channels - 1));
    }
    if (new_data.rows() != this->height || new_data.cols() != this->width) {
        throw std::invalid_argument("Image dimensions do not match data dimensions");
    }
    for (int j = 0; j < this->height; j++) {
        for (int k = 0; k < this->width; k++) {
            if (new_data(j, k) < 0 || new_data(j, k) > 1) {
                throw std::invalid_argument("Pixel values must be between 0 and 1");
            }
        }
    }
    this->data[channel] = new_data;
}

void Image::setPixel(int x, int y, Eigen::ArrayXd pixel) {
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
        throw std::invalid_argument("Pixel selected is not valid, must be between (0, 0) and (" + to_string(this->width - 1) + ", " + to_string(this->height - 1) + ")");
    }
    if (pixel.size() != this->channels) {
        throw std::invalid_argument("Pixel must have the same number of channels as the image");
    }
    for (int i = 0; i < this->channels; i++) {
        if (pixel[i] < 0 || pixel[i] > 1) {
            throw std::invalid_argument("Pixel values must be between 0 and 1");
        }
    }
    for (int i = 0; i < this->channels; i++) {
        this->data[i](y, x) = pixel[i];
    }
}

void Image::setPixel(int x, int y, int channel, double pixel) {
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
        throw std::invalid_argument("Pixel selected is not valid, must be between (0, 0) and (" + to_string(this->width - 1) + ", " + to_string(this->height - 1) + ")");
    }
    if (channel < 0 || channel >= this->channels) {
        throw std::invalid_argument("Channel selected is not valid, must be between 0 and " + to_string(this->channels - 1));
    }
    if (pixel < 0 || pixel > 1) {
        throw std::invalid_argument("Pixel values must be between 0 and 1");
    }
    this->data[channel](y, x) = pixel;
}

Eigen::ArrayXXd Image::operator()(int channel) const {
    if (channel < 0 || channel >= this->channels) {
        throw std::invalid_argument("Channel selected is not valid, must be between 0 and " + to_string(this->channels - 1));
    }
    return this->getData(channel);
}

Eigen::ArrayXd Image::operator()(int x, int y) const {
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
        throw std::invalid_argument("Pixel selected is not valid, must be between (0, 0) and (" + to_string(this->width - 1) + ", " + to_string(this->height - 1) + ")");
    }
    return this->getPixel(x, y);
}

double Image::operator()(int x, int y, int channel) const {
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
        throw std::invalid_argument("Pixel selected is not valid, must be between (0, 0) and (" + to_string(this->width - 1) + ", " + to_string(this->height - 1) + ")");
    }
    if (channel < 0 || channel >= this->channels) {
        throw std::invalid_argument("Channel selected is not valid, must be between 0 and " + to_string(this->channels - 1));
    }
    return this->getPixel(x, y, channel);
}

cv::Mat Image::toCvMat() {
    cv::Mat cv_image(this->height, this->width, CV_8UC(this->channels));
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            for (int k = 0; k < this->channels; k++) {
                cv_image.at<cv::Vec3b>(i, j)[k] = this->data[k](i, j) * 255;
            }
        }
    }
    return cv_image;
}

void Image::show(const string& window_name) {
    cv::Mat image = this->toCvMat();
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::imshow(window_name, image);
    cv::waitKey(0);
}

void Image::save(string filename) {
    // fix filepath to take into account being in build directory
    string cpp_path = __FILE__;
    string dir_path = cpp_path.substr(0, cpp_path.find_last_of("/\\"));
    dir_path = dir_path.substr(0, dir_path.find_last_of("/\\"));
    filename = dir_path + "/images/" + filename;

    cv::Mat image = this->toCvMat();
    cv::imwrite(filename, image);
}

bool Image::operator==(const Image &image) const {
    if (this->width != image.width || this->height != image.height || this->channels != image.channels) {
        return false;
    }
    for (int i = 0; i < this->channels; i++) {
        if (!(this->data[i] == image.data[i]).all()) {
            return false;
        }
    }
    return true;
}

bool Image::operator!=(const Image &image) const {
    return !(*this == image);
}

Image Image::reduceChannels() {
    if (this->channels == 1) {
        return *this;
    }
    Eigen::ArrayXXd new_data = Eigen::ArrayXXd::Zero(this->height, this->width);
    if (this->channels != 3) {
        cerr << "Warning: For images with 3 channels, reduction to 1 channels is done by "
                "perceptual conversion to grayscale. For images with " << this->channels
                << " channels, reduction to 1 channel is done by averaging all channels." << endl;
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < this->width; j++) {
                for (int k = 0; k < this->channels; k++) {
                    new_data(i, j) += this->data[k](i, j);
                }
                new_data(i, j) /= this->channels;
            }
        }
    }
    else {
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < this->width; j++) {
                // Formula for converting to grayscale from https://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale
                new_data(i, j) = 0.2126 * this->data[0](i, j) + 0.7152 * this->data[1](i, j) + 0.0722 * this->data[2](i, j);
            }
        }
    }
    return Image(new_data);
}
