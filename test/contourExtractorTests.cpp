//
// Created by André Charneca on 06.12.22.
//

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

TEST_F(contourExtractorTests, defaultConstructorGivesRightThreshold) {
    ContourExtractor contourExtractor;
    ASSERT_EQ(contourExtractor.getThreshold(), 0.5);
}

TEST_F(contourExtractorTests, constructorWithParamsThrowsNoException) {
    ASSERT_NO_THROW(ContourExtractor(0.5, 3, 1));
}


