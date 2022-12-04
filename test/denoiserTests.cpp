//
// Created by tsfeith on 04/12/22.
//

#include <gtest/gtest.h>
#include "Denoiser.hpp"


class denoiserTests : public ::testing::Test
{
protected:
    void SetUp() override {
        unit_kernel = Eigen::ArrayXXd::Zero(9, 9);
        unit_kernel(4, 4) = 1;
        unit_image = Image(3, unit_kernel);
    }
    Image unit_image;
    Eigen::ArrayXXd unit_kernel;
};

TEST_F(denoiserTests, defaultConstructorThrowsNoException) {
    ASSERT_NO_THROW(Denoiser());
}

TEST_F(denoiserTests, defaultConstructorGivesNormalizedKernel) {
    Denoiser denoiser;
    Eigen::ArrayXXd kernel = denoiser.getKernel();
    ASSERT_NEAR(kernel.sum(), 1, 1e-6);
}

TEST_F(denoiserTests, eigenConstructorThrowsExceptionOnEmptyKernel) {
    Eigen::ArrayXXd empty_kernel = Eigen::ArrayXXd::Zero(0, 0);
    ASSERT_THROW(Denoiser denoiser(empty_kernel), invalid_argument);
}

TEST_F(denoiserTests, eigenConstructorThrowsExceptionOnNonSquareKernel) {
    Eigen::ArrayXXd non_square_kernel = Eigen::ArrayXXd::Zero(3, 5);
    ASSERT_THROW(Denoiser denoiser(non_square_kernel), invalid_argument);
}

TEST_F(denoiserTests, eigenConstructorThrowsExceptionOnEvenKernel) {
    Eigen::ArrayXXd even_kernel = Eigen::ArrayXXd::Zero(4, 4);
    ASSERT_THROW(Denoiser denoiser(even_kernel), invalid_argument);
}

TEST_F(denoiserTests, eigenConstructorThrowsExceptionOnNonNormalizedKernel) {
    Eigen::ArrayXXd non_normalized_kernel = Eigen::ArrayXXd::Zero(3, 3);
    non_normalized_kernel(1, 1) = 2;
    ASSERT_THROW(Denoiser denoiser(non_normalized_kernel), invalid_argument);
}

TEST_F(denoiserTests, eigenConstructorWorksWithValidKernel) {
    Denoiser denoiser(unit_kernel);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_EQ(unit_kernel(i, j), denoiser.getKernel()(i, j));
        }
    }
}

TEST_F(denoiserTests, eigenConstructorGivesNormalizedKernel) {
    Denoiser denoiser(unit_kernel);
    ASSERT_NEAR(denoiser.getKernel().sum(), 1, 1e-6);
}

TEST_F(denoiserTests, parametricConstructorThrowsExceptionOnEvenSize) {
    ASSERT_THROW(Denoiser denoiser(4, 1), invalid_argument);
}

TEST_F(denoiserTests, parametricConstructorThrowsExceptionOnNegativeSize) {
    ASSERT_THROW(Denoiser denoiser(-1, 1), invalid_argument);
}

TEST_F(denoiserTests, parametricConstructorThrowsExceptionOnNegativeSigma) {
    ASSERT_THROW(Denoiser denoiser(3, -1), invalid_argument);
}

TEST_F(denoiserTests, parametricConstructorWithZeroSigmaReturnsMeanFilter) {
    Denoiser denoiser(3, 0);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_EQ(1.0 / 9.0, denoiser.getKernel()(i, j));
        }
    }
}

TEST_F(denoiserTests, parametricConstructorWithZeroSigmaReturnsNormalizedKernel) {
    Denoiser denoiser(3, 0);
    ASSERT_NEAR(denoiser.getKernel().sum(), 1, 1e-6);
}

TEST_F(denoiserTests, parametricConstructorWithNonZeroSigmaReturnsGaussianFilter) {
    Denoiser denoiser(3, 1);
    Eigen::ArrayXXd gaussian_kernel = Eigen::ArrayXXd::Zero(3, 3);
    gaussian_kernel(0, 0) = 0.075114;
    gaussian_kernel(0, 1) = 0.123841;
    gaussian_kernel(0, 2) = 0.075114;
    gaussian_kernel(1, 0) = 0.123841;
    gaussian_kernel(1, 1) = 0.204180;
    gaussian_kernel(1, 2) = 0.123841;
    gaussian_kernel(2, 0) = 0.075114;
    gaussian_kernel(2, 1) = 0.123841;
    gaussian_kernel(2, 2) = 0.075114;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_NEAR(gaussian_kernel(i, j), denoiser.getKernel()(i, j), 0.000001);
        }
    }
}

TEST_F(denoiserTests, parametricConstructorWithNonZeroSigmaReturnsNormalizedKernel) {
    Denoiser denoiser(3, 1);
    ASSERT_NEAR(denoiser.getKernel().sum(), 1, 1e-6);
}

TEST_F(denoiserTests, denoiseReturnsKernelForUnitImage) {
    Denoiser denoiser(3, 1);
    Image denoised_image = denoiser.denoise(unit_image);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_EQ(unit_kernel(i, j), denoised_image.getData(0)(i, j));
        }
    }
}

TEST_F(denoiserTests, denoiseReturnsImageForUnitKernel) {
    Denoiser denoiser(unit_kernel);
    Image denoised_image = denoiser.denoise(unit_image);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_EQ(unit_image.getData(0)(i, j), denoised_image.getData(0)(i, j));
        }
    }
}

TEST_F(denoiserTests, denoiseAndSaveOutputIsSavingImage) {
    Denoiser denoiser(3, 1);
    denoiser.denoise(unit_image, "test.png");
}

TEST_F(denoiserTests, denoiseAndSaveCanHnadleLargerImages) {
    Denoiser denoiser(5, 1);
    Image image = Image("teapot.png");
    denoiser.denoise(image, "test.png");
}

TEST_F(denoiserTests, getKernelWorks) {
    Denoiser denoiser(3, 1);
    Eigen::ArrayXXd kernel = denoiser.getKernel();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_EQ(denoiser.getKernel()(i, j), kernel(i, j));
        }
    }
}