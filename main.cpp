#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <filesystem>

using namespace std;
int main(int argc, char **argv) {
    /*
     * test script for opencv
     */

    // get directory of the file
    string cpp_path = __FILE__;

    string dir_path = cpp_path.substr(0, cpp_path.find_last_of("/\\"));
    cout << "dir_path: " << dir_path << endl;

    std::ifstream f(dir_path + "/images/teapot.png");
    std::cout << f.good() << std::endl;

    cv::Mat img = cv::imread(dir_path + "/images/teapot.png");
    cv::imshow("image", img);
    cv::waitKey(0);
    return 0;
}