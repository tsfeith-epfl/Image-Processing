//
// Created by André Charneca on 06.12.22.
//

//NOTE: tests missing for the following methods:
// - ContourExtractor::extractContours()
// - ContourExtractor::getThreshold()
// - ContourExtractor::getDenoiser()
// - ContourExtractor::setThreshold()
// - ContourExtractor::setDenoiser()


#include "ContourExtractor.hpp"
#include <gtest/gtest.h>

class contourExtractorTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up the test image
        Eigen::ArrayXXd image_data = Eigen::ArrayXXd::Zero(9, 9);
        image_data(4, 4) = 1;
        image_data(4, 5) = 1;
        image_data(5, 4) = 1;
        image_data(5, 5) = 1;
        image = Image(3, image_data);
    }

    Image image;

};

TEST_F(contourExtractorTests, defaultConstructorThrowsNoException) {
    ASSERT_NO_THROW(ContourExtractor());
}

TEST_F(contourExtractorTests, constructorFromParamsThrowsExceptionForInvalidThreshold) {
    ASSERT_THROW(ContourExtractor(-1, 3, 0), std::invalid_argument);
    ASSERT_THROW(ContourExtractor(2, 3, 0), std::invalid_argument);
}

TEST_F(contourExtractorTests, constructorFromParamsCreatesCorrectObject) {
    ContourExtractor contourExtractor(0.5, 3, 0);
    ASSERT_EQ(contourExtractor.getThreshold(), 0.5);
    Eigen::ArrayXXd kernel = contourExtractor.getDenoiser().getKernel();
    ASSERT_EQ(kernel.rows(), 3);
    ASSERT_EQ(kernel.cols(), 3);
}

TEST_F(contourExtractorTests, constructorFromKernelThrowsExceptionForInvalidThreshold) {
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(3, 3);
    ASSERT_THROW(ContourExtractor(-1, kernel), std::invalid_argument);
    ASSERT_THROW(ContourExtractor(2, kernel), std::invalid_argument);
}

TEST_F(contourExtractorTests, constructorFromKernelCreatesCorrectObject) {
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(3, 3);
    kernel(1, 1) = 1;
    ContourExtractor contourExtractor(0.5, kernel);
    ASSERT_EQ(contourExtractor.getThreshold(), 0.5);
    Eigen::ArrayXXd kernel2 = contourExtractor.getDenoiser().getKernel();
    ASSERT_EQ(kernel2.rows(), 3);
    ASSERT_EQ(kernel2.cols(), 3);
}

TEST_F(contourExtractorTests, getThresholdReturnsCorrectValue) {
    ContourExtractor contourExtractor(0.5, 3, 0);
    ASSERT_EQ(contourExtractor.getThreshold(), 0.5);
}

TEST_F(contourExtractorTests, getDenoiserReturnsCorrectObject) {
    ContourExtractor contourExtractor(0.5, 3, 0);
    Eigen::ArrayXXd kernel = contourExtractor.getDenoiser().getKernel();
    ASSERT_EQ(kernel.rows(), 3);
    ASSERT_EQ(kernel.cols(), 3);
}

TEST_F(contourExtractorTests, setThresholdThrowsExceptionForInvalidThreshold) {
    ContourExtractor contourExtractor(0.5, 3, 0);
    ASSERT_THROW(contourExtractor.setThreshold(-1), std::invalid_argument);
    ASSERT_THROW(contourExtractor.setThreshold(2), std::invalid_argument);
}

TEST_F(contourExtractorTests, setThresholdSetsCorrectValue) {
    ContourExtractor contourExtractor(0.5, 3, 0);
    contourExtractor.setThreshold(0.7);
    ASSERT_EQ(contourExtractor.getThreshold(), 0.7);
}

TEST_F(contourExtractorTests, setDenoiserSetsCorrectObject) {
    ContourExtractor contourExtractor(0.5, 3, 0);
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(3, 3);
    kernel(1, 1) = 1;
    Denoiser denoiser(kernel);
    contourExtractor.setDenoiser(denoiser);
    Eigen::ArrayXXd kernel2 = contourExtractor.getDenoiser().getKernel();
    ASSERT_EQ(kernel2.rows(), 3);
    ASSERT_EQ(kernel2.cols(), 3);
}

TEST_F(contourExtractorTests, setDenoiserParametricThrowsExceptionForOddKernelSize) {
    ContourExtractor contourExtractor(0.5, 3, 0);
    ASSERT_THROW(contourExtractor.setDenoiser(2, 1.), std::invalid_argument);
}

TEST_F(contourExtractorTests, setDenoiserParametricThrowsExceptionForNegativeKernelSize) {
    ContourExtractor contourExtractor(0.5, 3, 0);
    ASSERT_THROW(contourExtractor.setDenoiser(-2, 1.), std::invalid_argument);
}

TEST_F(contourExtractorTests, setDenoiserParametricThrowsExceptionForNegativeSigma) {
ContourExtractor contourExtractor(0.5, 3, 0);
    ASSERT_THROW(contourExtractor.setDenoiser(2, -1.), std::invalid_argument);
}

TEST_F(contourExtractorTests, setDenoiserParametricSetsCorrectObject) {
    ContourExtractor contourExtractor(0.5, 3, 0);
    contourExtractor.setDenoiser(3, 1.);
    Eigen::ArrayXXd kernel = contourExtractor.getDenoiser().getKernel();
    ASSERT_EQ(kernel.rows(), 3);
    ASSERT_EQ(kernel.cols(), 3);
}

TEST_F(contourExtractorTests, extractContoursTurnsRGBIntoGrayscale) {
    ContourExtractor contourExtractor(0.5, 3, 1);
    Image contour = contourExtractor.extractContours(image);
    ASSERT_EQ(contour.getChannels(), 1);
}

TEST_F(contourExtractorTests, extractContoursReturnsBinaryImage) {
    ContourExtractor contourExtractor(0.5, 3, 1);
    Image contour = contourExtractor.extractContours(image);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            ASSERT_TRUE(contour.getPixel(i, j, 0) == 0 || contour.getPixel(i, j, 0) == 1);
        }
    }
}

//do missing tests

