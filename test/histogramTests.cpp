//
// Created by tsfeith on 09/12/22.
//

#include "Histogram.hpp"
#include "gtest/gtest.h"

TEST(histogramTests, defaultConstructorExists) {
    EXPECT_NO_THROW(Histogram());
}

TEST(histogramTests, defaultConstructorSetsValidParameters) {
    Histogram histogram;
    EXPECT_GT(histogram.getBins(), 0);
    EXPECT_LT(histogram.getMinRange(), histogram.getMaxRange());
    EXPECT_GE(histogram.getMinRange(), 0);
    EXPECT_LE(histogram.getMaxRange(), 1);
}

TEST(histogramTests, binConstructorThrowsExceptionForNonPositiveBins) {
    EXPECT_THROW(Histogram(0), invalid_argument);
    EXPECT_THROW(Histogram(-1), invalid_argument);
}

TEST(histogramTests, binConstructorSetCorrectNumberOfBins) {
    Histogram histogram(10);
    EXPECT_EQ(histogram.getBins(), 10);
}

TEST(histogramTests, rangeConstructorThrowsExceptionForInvalidRange) {
    EXPECT_THROW(Histogram(10, 1, 0), invalid_argument);
    EXPECT_THROW(Histogram(10, -1, 1), invalid_argument);
    EXPECT_THROW(Histogram(10, 0, 2), invalid_argument);
}

TEST(histogramTests, rangeConstructorSetCorrectRange) {
    Histogram histogram(10, 0.1, 0.9);
    EXPECT_EQ(histogram.getMinRange(), 0.1);
    EXPECT_EQ(histogram.getMaxRange(), 0.9);
}

TEST(histogramTests, logConstructorSetCorrectScale) {
    Histogram histogram(10, 0.1, 0.9, true);
    EXPECT_TRUE(histogram.getLog());
}

TEST(histogramTests, binSetterThrowsExceptionForNonPositiveBins) {
    Histogram histogram;
    EXPECT_THROW(histogram.setBins(0), invalid_argument);
    EXPECT_THROW(histogram.setBins(-1), invalid_argument);
}

TEST(histogramTests, binSetterCorrectlyUpdatedBinValue) {
    Histogram histogram;
    histogram.setBins(10);
    EXPECT_EQ(histogram.getBins(), 10);
}

TEST(histogramTests, minRangeSetterThrowsExceptionForInvalidRange) {
    Histogram histogram;
    EXPECT_THROW(histogram.setMinRange(1), invalid_argument);
    EXPECT_THROW(histogram.setMinRange(-1), invalid_argument);
    EXPECT_THROW(histogram.setMinRange(2), invalid_argument);
}

TEST(histogramTests, minRageSetterThrowsExceptionForNewValidBiggerThanOrEqualToMaxRange) {
    Histogram histogram(10, 0.1, 0.9);
    EXPECT_THROW(histogram.setMinRange(0.95), invalid_argument);
}

TEST(histogramTests, minRangeSetterCorrectlyUpdatesMinRange) {
    Histogram histogram(10, 0.1, 0.9);
    histogram.setMinRange(0.2);
    EXPECT_EQ(histogram.getMinRange(), 0.2);
}

TEST(histogramTests, maxRangeSetterThrowsExceptionForInvalidRange) {
    Histogram histogram;
    EXPECT_THROW(histogram.setMaxRange(0), invalid_argument);
    EXPECT_THROW(histogram.setMaxRange(-1), invalid_argument);
    EXPECT_THROW(histogram.setMaxRange(2), invalid_argument);
}

TEST(histogramTests, maxRangeSetterThrowsExceptionForNewValidSmallerThanOrEqualToMinRange) {
    Histogram histogram(10, 0.1, 0.9);
    EXPECT_THROW(histogram.setMaxRange(0.05), invalid_argument);
}

TEST(histogramTests, maxRangeSetterCorrectlyUpdatesMaxRange) {
    Histogram histogram(10, 0.1, 0.9);
    histogram.setMaxRange(0.8);
    EXPECT_EQ(histogram.getMaxRange(), 0.8);
}

TEST(histogramTests, logSetterCorrectlyUpdatesLog) {
    Histogram histogram;
    histogram.setLog(true);
    EXPECT_TRUE(histogram.getLog());
}

TEST(histogramTests, computedHistogramMatchesExpectedBehavior) {
    Eigen::ArrayXXd data(6, 5);
    data << 0.0, 0.1, 0.2, 0.3, 0.4,
            0.5, 0.6, 0.7, 0.8, 0.9,
            0.0, 0.1, 0.2, 0.3, 0.4,
            0.5, 0.6, 0.7, 0.8, 0.9,
            0.0, 0.1, 0.2, 0.3, 0.4,
            0.5, 0.6, 0.7, 0.8, 0.9;
    Image image(data);

    Histogram histogram(10, 0, 1);
    vector<vector<double>> computed_histogram = histogram.computeHistogram(image);
    for (int i = 0; i < 10; i++) {
        EXPECT_NEAR(computed_histogram[i][0], i * 0.1, 0.0001);
        EXPECT_EQ(computed_histogram[i][1], 3);
    }
}

TEST(histogramTests, histogramGeneratorThrowsNoException) {
    Image image("teapot.png");
    Histogram histogram(10, 0, 1);
    EXPECT_NO_THROW(histogram.getHistogram(image, "test.png", false));
}

TEST(histogramTest, histogramGeneratorThrowsExceptionIfNoShowAndNoSave) {
    Image image("teapot.png");
    Histogram histogram(10, 0, 1);
    EXPECT_THROW(histogram.getHistogram(image, "", false), invalid_argument);
}