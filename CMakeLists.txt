cmake_minimum_required(VERSION 3.0)
project(ImageProcessing CXX)

set(CMAKE_CXX_STANDARD 17)
include_directories(src)

file(WRITE ${CMAKE_SOURCE_DIR}/src/location.hpp
        "#include<string>\nstd::string loc=\"${CMAKE_SOURCE_DIR}\";")

# add eigen
include_directories(libraries/eigen)

# add opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main main.cpp src/operations.cpp src/Image.cpp src/Denoiser.cpp parameters.hpp src/ContourExtractor.cpp src/Histogram.cpp)
target_link_libraries(main ${OpenCV_LIBS})

# REMOVE BEFORE SUBMISSION
add_executable(andre andre.cpp src/operations.cpp src/Image.cpp src/Denoiser.cpp parameters.hpp src/ContourExtractor.cpp)
target_link_libraries(andre ${OpenCV_LIBS})

# build test suite
add_subdirectory(googletest)
add_executable(test_suite test/convolutionsTests.cpp test/imageTests.cpp test/denoiserTests.cpp src/operations.hpp src/Image.hpp src/Denoiser.cpp test/contourExtractorTests.cpp src/ContourExtractor.cpp test/gradientTests.cpp)
target_link_libraries(test_suite gtest_main gtest ${OpenCV_LIBS})

add_custom_target(test ./test_suite DEPENDS test_suite)

# build documentation using doxygen
find_package(Doxygen
             REQUIRED dot)
doxygen_add_docs(Doxyfile.in
                 ${CMAKE_CURRENT_SOURCE_DIR}/src
                 COMMENT "Generate documentation")