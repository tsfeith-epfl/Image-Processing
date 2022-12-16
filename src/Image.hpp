//
// Created by tsfeith on 03/12/22.
//

#ifndef IMAGEPROCESSING_IMAGE_HPP
#define IMAGEPROCESSING_IMAGE_HPP

#include <Eigen/Eigen>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;

/**
 * @brief The Image class
 * @details This class provides basic functionalities for
 * image storage and manipulation.
 */
class Image {
private:
    /**
     * @var width
     * Width of the image.
     */
    int width;
    /**
     * @var height
     * Height of the image.
     */
    int height;
    /**
     * @var channels
     * Number of channels of the image.
     */
    int channels;
    /**
     * @var data
     * Vector of Eigen matrices containing the image data, where each matrix is a channel.
     */
    vector<Eigen::ArrayXXd> data;
    /**
     * @var absolute_path
     * If the image is loaded from a file, this variable states whether the path provided was absolute or relative.
     */
    bool absolute_path = false;
    /**
     * @var path
     * Path to the image file.
     */
    string path;

public:
    Image();
    explicit Image(string filename);
    Image(int width, int height, int channels);
    Image(int channels, Eigen::ArrayXXd data);
    explicit Image(Eigen::ArrayXXd data);
    explicit Image(vector<Eigen::ArrayXXd> data);
    Image(const Image& image);

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;
    [[nodiscard]] int getChannels() const;
    [[nodiscard]] vector<Eigen::ArrayXXd> getData() const;
    [[nodiscard]] Eigen::ArrayXXd getData(int channel) const;
    [[nodiscard]] Eigen::ArrayXd getPixel(int x, int y) const;
    [[nodiscard]] double getPixel(int x, int y, int channel) const;
    [[nodiscard]] string getPath() const;
    [[nodiscard]] bool usedAbsolutePath() const;

    void setData(vector<Eigen::ArrayXXd> new_data);
    void setData(int channel, Eigen::ArrayXXd data);
    void setPixel(int x, int y, Eigen::ArrayXd pixel);
    void setPixel(int x, int y, int channel, double value);

    Eigen::ArrayXXd operator()(int channel) const;
    Eigen::ArrayXd operator()(int x, int y) const;
    double operator()(int x, int y, int channel) const;
    bool operator==(const Image& image) const;
    bool operator!=(const Image& image) const;

    virtual void show(const string& window_name = "window");
    void save(string filename, bool absolute_path = false);
    cv::Mat toCvMat();
    Image reduceChannels();
};


#endif //IMAGEPROCESSING_IMAGE_HPP
