//
// Created by André Charneca on 07.12.22.
//

#include <gtest/gtest.h>
#include "operations.hpp"

class GradientTests : public ::testing::Test {
protected:
    void SetUp() override {
        // create simple image
        Eigen::ArrayXXd image_data = Eigen::ArrayXXd::Zero(9,9);
        image_data(4,4) = 0.5;
        image_data(4,5) = 1.;
        image_data(4,6) = 0.5;
        image_RGB = Image(3, image_data);
        image_GS = Image(1, image_data);
    }

    Image image_RGB;
    Image image_GS;
};

TEST_F(GradientTests, xAxisGradientAcceptsRGBImages) {
    EXPECT_NO_THROW(computeGradientX(image_RGB));
}

TEST_F(GradientTests, xAxisGradientThrowsExceptionIfImageSmallerThanSobelKernel) {
    Eigen::ArrayXXd image_data = Eigen::ArrayXXd::Zero(2,2);
    Image image(1, image_data);
    ASSERT_THROW(computeGradientX(image), invalid_argument);
}

TEST_F(GradientTests, xAxisGradientReturnsExpectedResult) {
    Eigen::ArrayXXd gradient = computeGradientX(image_GS);
    Eigen::ArrayXXd expected_gradient = Eigen::ArrayXXd::Zero(9,9);
    expected_gradient(3,3) = 0.5;
    expected_gradient(3,4) = 1.0;
    expected_gradient(3,5) = 0.0;
    expected_gradient(3,6) = -1.0;
    expected_gradient(3,7) = -0.5;
    expected_gradient(4,3) = 1.0;
    expected_gradient(4,4) = 2.0;
    expected_gradient(4,5) = 0.0;
    expected_gradient(4,6) = -2.0;
    expected_gradient(4,7) = -1.0;
    expected_gradient(5,3) = 0.5;
    expected_gradient(5,4) = 1.0;
    expected_gradient(5,5) = 0.0;
    expected_gradient(5,6) = -1.0;
    expected_gradient(5,7) = -0.5;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            ASSERT_NEAR(gradient(i,j), expected_gradient(i,j), 1e-6);
        }
    }
}

TEST_F(GradientTests, yAxisGradientAcceptsRGBImages) {
    EXPECT_NO_THROW(computeGradientY(image_RGB));
}

TEST_F(GradientTests, yAxisGradientThrowsExceptionIfImageSmallerThanSobelKernel) {
    Eigen::ArrayXXd image_data = Eigen::ArrayXXd::Zero(2,2);
    Image image(1, image_data);
    ASSERT_THROW(computeGradientY(image), invalid_argument);
}

TEST_F(GradientTests, yAxisGradientReturnsExpectedResult) {
    Eigen::ArrayXXd gradient = computeGradientY(image_GS);
    Eigen::ArrayXXd expected_gradient = Eigen::ArrayXXd::Zero(9,9);
    expected_gradient(3,3) = 0.5;
    expected_gradient(3,4) = 2.0;
    expected_gradient(3,5) = 3.0;
    expected_gradient(3,6) = 2.0;
    expected_gradient(3,7) = 0.5;
    expected_gradient(5,3) = -0.5;
    expected_gradient(5,4) = -2.0;
    expected_gradient(5,5) = -3.0;
    expected_gradient(5,6) = -2.0;
    expected_gradient(5,7) = -0.5;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            ASSERT_NEAR(gradient(i, j), expected_gradient(i,j), 1e-6);
        }
    }
}

TEST_F(GradientTests, gradientMagnitudeThrowsExceptionIfImageSmallerThanSobelKernel) {
    Eigen::ArrayXXd image_data = Eigen::ArrayXXd::Zero(2,2);
    Image image(1, image_data);
    ASSERT_THROW(computeGradientMagnitude(image), invalid_argument);
}