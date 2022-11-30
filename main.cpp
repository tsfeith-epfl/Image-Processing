#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <filesystem>

int main(int argc, char **argv) {
    // test script for opencv
    std::cout << std::filesystem::current_path() << std::endl;
    std::ifstream f("./images/teapot.png");
    std::cout << f.good() << std::endl;
    cv::Mat img = cv::imread("./images/teapot.png");
    cv::imshow("image", img);
    cv::waitKey(0);
    return 0;
}
