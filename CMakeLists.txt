cmake_minimum_required(VERSION 3.0)
project(ImageProcessing CXX)

set(CMAKE_CXX_STANDARD 17)
include_directories(src)

# add eigen
include_directories(libraries/eigen)

# add opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

## Note: remove andre.cpp before submission
add_executable(main main.cpp andre.cpp src/operations.cpp src/Image.cpp src/Denoiser.cpp parameters.hpp src/ContourExtractor.cpp)
target_link_libraries(main andre ${OpenCV_LIBS})

# build test suite
add_subdirectory(googletest)
add_executable(test_suite test/convolutionsTests.cpp test/imageTests.cpp test/denoiserTests.cpp src/operations.hpp src/Image.hpp src/Denoiser.cpp test/contourExtractorTests.cpp src/ContourExtractor.cpp)
target_link_libraries(test_suite gtest_main gtest ${OpenCV_LIBS})

add_custom_target(test ./test_suite DEPENDS test_suite)

# build documentation using doxygen
find_package(Doxygen
             REQUIRED dot)
doxygen_add_docs(Doxyfile.in
                 ${CMAKE_CURRENT_SOURCE_DIR}/src
                 COMMENT "Generate documentation")