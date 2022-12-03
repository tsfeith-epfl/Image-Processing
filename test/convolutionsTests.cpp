#include <cmath>
#include "Operations.cpp"
#include "gtest/gtest.h"

/* Testing Operations.cpp */

// fixture for testing Operations.cpp
class convolutionsTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        input_eigen = Eigen::ArrayXXd::Zero(5, 5);
        input_eigen(1, 1) = 1;
    }
    Eigen::ArrayXXd input_eigen;
};

// test applyConvolution with a unit kernel
TEST_F(convolutionsTests, applyConvolution_unitKernel)
{
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(3, 3);
    kernel(1, 1) = 1;
    Eigen::ArrayXXd output = applyConvolution(input_eigen, kernel);
    Eigen::ArrayXXd expected = Eigen::ArrayXXd::Zero(5, 5);
    expected(1, 1) = 1;

    for (int i = 0; i < output.rows(); i++) {
        for (int j = 0; j < output.cols(); j++) {
            EXPECT_EQ(output(i, j), expected(i, j));
        }
    }
}

// test applyConvolution with a rectangular input
TEST_F(convolutionsTests, applyConvolution_rectangularInput)
{
    Eigen::ArrayXXd input = Eigen::ArrayXXd::Zero(5, 7);
    input(1, 1) = 1;
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(3, 3);
    kernel(1, 1) = 1;
    Eigen::ArrayXXd output = applyConvolution(input, kernel);
    Eigen::ArrayXXd expected = Eigen::ArrayXXd::Zero(5, 7);
    expected(1, 1) = 1;

    for (int i = 0; i < output.rows(); i++) {
        for (int j = 0; j < output.cols(); j++) {
            EXPECT_EQ(output(i, j), expected(i, j));
        }
    }
}

// test errors
TEST_F(convolutionsTests, ErrorEmptyInput)
{
    Eigen::ArrayXXd input = Eigen::ArrayXXd::Zero(0, 0);
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(3, 3);
    EXPECT_THROW(applyConvolution(input, kernel), std::invalid_argument);
}

TEST_F(convolutionsTests, ErrorEmptyKernel)
{
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(0, 0);
    EXPECT_THROW(applyConvolution(input_eigen, kernel), std::invalid_argument);
}

TEST_F(convolutionsTests, ErrorKernelSize)
{
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(2, 2);
    EXPECT_THROW(applyConvolution(input_eigen, kernel), std::invalid_argument);
}

TEST_F(convolutionsTests, ErrorKernelSquare)
{
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(3, 2);
    EXPECT_THROW(applyConvolution(input_eigen, kernel), std::invalid_argument);
}

TEST_F(convolutionsTests, ErrorKernelBiggerThanInput)
{
    Eigen::ArrayXXd kernel = Eigen::ArrayXXd::Zero(6, 6);
    EXPECT_THROW(applyConvolution(input_eigen, kernel), std::invalid_argument);
}


