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

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-march=native" _march_native_works)
check_cxx_compiler_flag("-xHost" _xhost_works)

set(_CXX_FLAGS)
if(_march_native_works)
  message(STATUS "Using processor's vector instructions (-march=native compiler flag set)")
  set(_CXX_FLAGS "-march=native")
elseif(_xhost_works)
  message(STATUS "Using processor's vector instructions (-xHost compiler flag set)")
  set(_CXX_FLAGS "-xHost")
else()
  message(STATUS "No suitable compiler flag found for vectorization")
endif()

set(_CXX_FLAGS "-O3")

add_executable(main main.cpp src/operations.cpp src/Image.cpp src/Denoiser.cpp parameters.hpp src/ContourExtractor.cpp src/Histogram.cpp src/FourierImage.cpp src/FourierImage.hpp)
target_link_libraries(main ${OpenCV_LIBS})
target_compile_options(main PRIVATE ${_CXX_FLAGS})

# REMOVE BEFORE SUBMISSION
add_executable(andre andre.cpp src/operations.cpp src/Image.cpp src/Denoiser.cpp parameters.hpp src/ContourExtractor.cpp src/Histogram.cpp src/FourierImage.cpp src/FourierImage.hpp)
target_link_libraries(andre ${OpenCV_LIBS})

# build documentation using doxygen
find_package(Doxygen
        REQUIRED dot)
doxygen_add_docs(Doxyfile.in
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        COMMENT "Generate documentation")

# build test suite
add_subdirectory(googletest)
add_executable(test_suite test/histogramTests.cpp test/gradientTests.cpp test/convolutionsTests.cpp test/imageTests.cpp test/denoiserTests.cpp src/operations.hpp src/Image.hpp src/Denoiser.cpp test/contourExtractorTests.cpp src/ContourExtractor.cpp test/gradientTests.cpp src/Histogram.cpp src/FourierImage.cpp src/FourierImage.hpp test/fourierImageTests.cpp)
target_link_libraries(test_suite gtest_main gtest ${OpenCV_LIBS})

add_custom_target(test ./test_suite DEPENDS test_suite)
