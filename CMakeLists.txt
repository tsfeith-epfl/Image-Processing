cmake_minimum_required(VERSION 3.0)
project(ImageProcessing CXX)

set(CMAKE_CXX_STANDARD 17)
include_directories(src)

# add eigen
include_directories(libraries/eigen)
# add jsoncpp
find_package(jsoncpp REQUIRED)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSON_INC_PATH})
# add opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main main.cpp src/Operations.cpp src/Image.cpp src/Denoiser.cpp)
target_link_libraries(main ${OpenCV_LIBS} jsoncpp_lib)

# build test suite
add_subdirectory(googletest)
add_executable(test_suite test/convolutionsTests.cpp test/imageTests.cpp test/denoiserTests.cpp src/Operations.hpp src/Image.hpp src/Denoiser.cpp)
target_link_libraries(test_suite gtest_main gtest ${OpenCV_LIBS})

add_custom_target(test ./test_suite DEPENDS test_suite)
