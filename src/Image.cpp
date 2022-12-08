//
// Created by tsfeith on 03/12/22.
//

#include <fstream>
#include <utility>
#include "Image.hpp"
#include "location.hpp"

/*!
 * @brief Default constructor.
 * @details This constructor creates an empty image with dimensions (50 x 50 x 3).
 * The dimensions choice is arbitrary.
 */
Image::Image() {
    this->width = 50;
    this->height = 50;
    this->channels = 3;
    this->data = vector<Eigen::ArrayXXd>(this->channels, Eigen::ArrayXXd::Zero(this->height, this->width));
}

/*!
 * @brief Constructor from filename.
 * @details This constructor creates an image from a given filename. It
 * first tries to open the image assuming 'filename' is the absolute path.
 * If it fails, then it looks in the 'images' folder for it.
 * @param filename The filename of the image to be loaded.
 */
Image::Image(string filename) {
    // first try opening the file assuming the user gave the full path
    cout << "Trying to open " << filename << endl;
    ifstream file(filename);
    bool need_retry;
    if (!file.good()) {
        filename = loc + "/images/" + filename;
        cout << "File not found.\nTrying to open " << filename << endl;
        need_retry = true;
    }
    else {
        cout << "File " << filename << " found." << endl;
        need_retry = false;
        this->absolute_path = true;
    }
    if (need_retry) {
        ifstream file_retry(filename);
        if (!file_retry.good()) {
            throw invalid_argument(
                    "File not found. Make sure to either give the absolute path or place the file in the images folder.");
        }
        else {
            cout << "File " << filename << " found.\n" << endl;
        }
        file_retry.close();
    }
    file.close();
    this->path = filename;
    cv::Mat image = cv::imread(filename);
    if (image.empty()) {
        throw invalid_argument("Could not open the image.");
    }
    this->width = image.cols;
    this->height = image.rows;
    this->channels = image.channels();
    this->data = vector<Eigen::ArrayXXd>(this->channels, Eigen::ArrayXXd::Zero(this->height, this->width));
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            for (int k = 0; k < this->channels; k++) {
                this->data[k](i, j) = image.at<cv::Vec3b>(i, j)[k] / 255.0;
            }
        }
    }
}

/*!
 * @brief Constructor for empty image with given dimensions.
 * @details This constructor creates an empty image with given dimensions.
 * @param width The width of the image. Must be positive.
 * @param height The height of the image. Must be positive.
 * @param channels The number of channels of the image. Must be positive.
 */
Image::Image(int width, int height, int channels) {
    if (width <= 0 || height <= 0 || channels <= 0) {
        throw std::invalid_argument("Width, height and number of channels must be positive");
    }
    this->width = width;
    this->height = height;
    this->channels = channels;
    this->data = vector<Eigen::ArrayXXd>(this->channels, Eigen::ArrayXXd::Zero(this->height, this->width));
    this->absolute_path = false;
}

/*!
 * @brief Constructor to replicate an array over multiple channels.
 * @details This constructor creates an image from the given array and replicates it over the given number of channels.
 * @param channels The number of channels of the image. Must be positive.
 * @param data The array to be replicated over all channels. Value must be between in [0, 1].
 */
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

/*!
 * @brief Constructor to construct a 1-channel image from an array.
 * @details This constructor creates an image from the given array as a 1-channel image. It is a shortcut to calling
 * Image(channels, data) with channels = 1.
 * @param channels The number of channels of the image. Must be positive.
 * @param data The array to be replicated over all channels. Value must be between in [0, 1].
 */
Image::Image(Eigen::ArrayXXd data) : Image(1, std::move(data)) {}

/*!
 * @brief Constructor to construct an image from a vector of arrays.
 * @details This constructor creates an image from the given vector of arrays. The number of channels is determined by the
 * size of the vector. The dimensions of the image are determined by the dimensions of the arrays. They must be consistent
 * across all channels.
 * @param data The vector of arrays to be used as the image data. Value must be between in [0, 1].
 */
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

/*!
 * @brief Simple copy constructor.
 * @details This constructor creates a copy of the given image.
 * @param other The image to be copied.
 */
Image::Image(const Image& image) {
    this->width = image.width;
    this->height = image.height;
    this->channels = image.channels;
    this->data = image.data;
}

/*!
 * @brief Simple width getter.
 * @details This getter returns the width of the image.
 * @return The width of the image.
 */
int Image::getWidth() const {
    return this->width;
}

/*!
 * @brief Simple height getter.
 * @details This getter returns the height of the image.
 * @return The height of the image.
 */
int Image::getHeight() const {
    return this->height;
}

/*!
 * @brief Simple channels getter.
 * @details This getter returns the number of channels of the image.
 * @return The number of channels of the image.
 */
int Image::getChannels() const {
    return this->channels;
}

/*!
 * @brief Simple data getter.
 * @details This getter returns the data of the image.
 * @return The data of the image.
 */
vector<Eigen::ArrayXXd> Image::getData() const {
    return this->data;
}

/*!
 * @brief Simple data getter for a single channel.
 * @details This getter returns the data of the image for a single channel.
 * @param channel The channel to be returned. Must be in [0, channels).
 * @return The data of the image for the given channel.
 */
Eigen::ArrayXXd Image::getData(int channel) const {
    if (channel < 0 || channel >= this->channels) {
        throw std::invalid_argument("Channel selected is not valid, must be between 0 and " + to_string(this->channels - 1));
    }
    return this->data[channel];
}

/*!
 * @brief Simple pixel getter.
 * @details This getter returns the N-dimensional pixel at the given coordinates.
 * @param x 'x' coordinate of the pixel. Must be in [0, width).
 * @param y 'y' coordinate of the pixel. Must be in [0, height).
 * @return The N-dimensional pixel at the given coordinates.
 */
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

/*!
 * @brief Simple pixel getter for a single channel.
 * @details This getter returns the pixel at the given coordinates for a single channel.
 * @param x 'x' coordinate of the pixel. Must be in [0, width).
 * @param y 'y' coordinate of the pixel. Must be in [0, height).
 * @param channel The channel to be returned. Must be in [0, channels).
 * @return The pixel at the given coordinates for the given channel.
 */
double Image::getPixel(int x, int y, int channel) const {
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
        throw std::invalid_argument("Pixel selected is not valid, must be between (0, 0) and (" + to_string(this->width - 1) + ", " + to_string(this->height - 1) + ")");
    }
    if (channel < 0 || channel >= this->channels) {
        throw std::invalid_argument("Channel selected is not valid, must be between 0 and " + to_string(this->channels - 1));
    }
    return this->data[channel](y, x);
}

/*!
 * @brief Get image path
 * @details If the image was created from an image file, this method returns the absolute path to the image file.
 * Otherwise it returns an empty string.
 * @return The absolute path to the image file.
 */
string Image::getPath() const {
    return this->path;
}

/*!
 * @brief Was an absolute path provided?
 * @details If the image was created from an image file using an absolute path, this method returns true. If if was
 * created using a relative path or from a vector of arrays, it returns false.
 */
 bool Image::usedAbsolutePath() const {
    return this->absolute_path;
}

/*!
 * @brief N-dimensional data setter
 * @details This setter sets the data of the image to the given vector of arrays. The new data needs to be consistent
 * with the old data (same dimensions, same number of channels), and it still needs to be in [0, 1].
 * @param data The new data to be set.
 * @return
 */
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

/*!
 * @brief Single channel data setter
 * @details This setter sets the data of the image to the given array for a single channel. The new data needs to be
 * consistent with the old data (same dimensions, valid channel), and it still needs to be in [0, 1].
 * @param data The new data to be set.
 * @param channel The channel to be set. Must be in [0, channels).
 * @return
 */
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

/*!
 * @brief N-dimensional pixel setter
 * @details This setter sets the pixel at the given coordinates to the given N-dimensional pixel. The new pixel needs
 * to be consistent with the old data (same number of channels), and it still needs to be in [0, 1].
 * @param x 'x' coordinate of the pixel. Must be in [0, width).
 * @param y 'y' coordinate of the pixel. Must be in [0, height).
 * @param pixel The new pixel to be set.
 * @return
 */
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

/*!
 * @brief 1-dimensional pixel setter
 * @details This setter sets the pixel at the given coordinates to the given value for a single channel. The new pixel
 * needs to be consistent with the old data (valid channel), and it still needs to be in [0, 1].
 * @param x 'x' coordinate of the pixel. Must be in [0, width).
 * @param y 'y' coordinate of the pixel. Must be in [0, height).
 * @param pixel The new pixel to be set.
 * @param channel The channel to be set. Must be in [0, channels).
 * @return
 */
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

/*!
 * @brief Overload () operator for channel access.
 * @details This overload allows the user to access the data of a single channel of the image using the () operator.
 * @param channel The channel to be accessed. Must be in [0, channels).
 * @return The data of the selected channel.
 */
Eigen::ArrayXXd Image::operator()(int channel) const {
    if (channel < 0 || channel >= this->channels) {
        throw std::invalid_argument("Channel selected is not valid, must be between 0 and " + to_string(this->channels - 1));
    }
    return this->getData(channel);
}

/*!
 * @brief Overload () operator for N-dimensional pixel access.
 * @param x 'x' coordinate of the pixel. Must be in [0, width).
 * @param y 'y' coordinate of the pixel. Must be in [0, height).
 * @return The N-dimensional pixel at the given coordinates.
 */
Eigen::ArrayXd Image::operator()(int x, int y) const {
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
        throw std::invalid_argument("Pixel selected is not valid, must be between (0, 0) and (" + to_string(this->width - 1) + ", " + to_string(this->height - 1) + ")");
    }
    return this->getPixel(x, y);
}

/*!
 * @brief Overload () operator for 1-dimensional pixel access.
 * @param x 'x' coordinate of the pixel. Must be in [0, width).
 * @param y 'y' coordinate of the pixel. Must be in [0, height).
 * @param channel The channel to be accessed. Must be in [0, channels).
 * @return The value of the selected channel at the given coordinates.
 */
double Image::operator()(int x, int y, int channel) const {
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
        throw std::invalid_argument("Pixel selected is not valid, must be between (0, 0) and (" + to_string(this->width - 1) + ", " + to_string(this->height - 1) + ")");
    }
    if (channel < 0 || channel >= this->channels) {
        throw std::invalid_argument("Channel selected is not valid, must be between 0 and " + to_string(this->channels - 1));
    }
    return this->getPixel(x, y, channel);
}

/*!
 * @brief Method to convert image to OpenCV Mat.
 * @details This method converts the image to an OpenCV Mat. The image is converted to 8-bit unsigned integer, with
 * values scaled to [0, 255].
 * @return The image as an OpenCV Mat.
 */
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

/*!
 * @brief Method to show image in a window.
 * @details This method shows the image in a window. If the image is grayscale, it is converted to 3 identical channels
 * to be displayed as RGB. If the image has channels other than 1 or 3, an exception is thrown.an
 * @param window_name The name of the window.
 * @return
 */
void Image::show(const string& window_name) {
    cv::Mat image;
    if (this->channels == 1) {
        Image expanded_image(3, this->data[0]);
        image = expanded_image.toCvMat();
    }
    else if (this->channels == 3) {
        image = this->toCvMat();
    }
    else {
        throw std::invalid_argument("Image must have 1 or 3 channels for show to work");
    }
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::imshow(window_name, image);
    cv::waitKey(0);
}

/*!
 * @brief Method to save an image specifying the filename.
 * @details This method saves the image either using the absolute path or putting in the './output' directory.
 * @param filename The name of the file.
 * @return
 */
void Image::save(string filename) {
    cv::Mat image;
    if (this->channels == 1) {
        Image expanded_image(3, this->data[0]);
        image = expanded_image.toCvMat();
    }
    else if (this->channels == 3) {
        image = this->toCvMat();
    }
    else {
        throw std::invalid_argument("Image must have 1 or 3 channels for show to work");
    }
    if (this->absolute_path) {
        cv::imwrite(filename, image);
    } else {
        filename = loc + "/output/" + filename;
        cv::imwrite(filename, image);
    }
}

/*!
 * @brief Overload == operator.
 * @details This overload allows the user to compare two images using the == operator. It checks if the images
 * have the same dimensions and channels, and if the data is the same.
 * @param image The image to be compared.
 * @return True if the images are the same, false otherwise.
 */
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

/*!
 * @brief Overload != operator.
 * @details This overload allows the user to compare two images using the != operator. It just does the negative
 * of the == operator.
 * @param image The image to be compared.
 * @return True if the images are different, false otherwise.
 */
bool Image::operator!=(const Image &image) const {
    return !(*this == image);
}

/*!
 * @brief Method to get a reduced (i.e. single-channel) version of the image.
 * @details This method returns a reduced version of the image, with only one channel. If the image is already
 * single-channel, it returns a pointer to the same image. If the image has 3 channels, it performs a weighted mean
 * following a visual perception model (i.e. the human eye is more sensitive to green than red or blue). Otherwise,
 * it returns a simple mean of all the channels.
 * @return an Image object with only one channel.
 */
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
