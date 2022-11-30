cmake_minimum_required(VERSION 3.0)
project(ImageProcessing CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV PATHS libraries/opencv/build NO_DEFAULT_PATH REQUIRED)
find_package(Eigen3 PATHS libraries/eigen/build NO_DEFAULT_PATH REQUIRED)

add_subdirectory(googletest)

add_executable(main main.cpp)
target_link_libraries(main OpenCV Eigen3::Eigen)

add_executable(test_pcsc test/test.cc)
target_link_libraries(test_pcsc gtest_main gtest)

add_custom_target(test ./test_pcsc DEPENDS test_pcsc)
