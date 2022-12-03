cmake_minimum_required(VERSION 3.0)
project(ImageProcessing CXX)

set(CMAKE_CXX_STANDARD 17)
include_directories(src)

# add eigen
include_directories(libraries/eigen)
# add opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main main.cpp src/Operations.hpp)
target_link_libraries(main ${OpenCV_LIBS})


# build test suite
add_subdirectory(googletest)
add_executable(test_suite test/test.cc src/Operations.hpp)
target_link_libraries(test_suite gtest_main gtest)

add_custom_target(test ./test_suite DEPENDS test_suite)
