//
// Created by André Charneca on 12.12.22.
//
// Tests for fourier image class and dft functions in operations.hpp

#include <gtest/gtest.h>
#include "FourierImage.hpp"
#include "operations.hpp"

class fourierImageTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up the test image
        Eigen::ArrayXXd image_data = Eigen::ArrayXXd::Zero(4,4);
        image_data << 1, 2, 3, 4,
                      5, 6, 7, 8,
                      9, 10, 11, 12,
                      13, 14, 15, 16;
        image_data = normalize(image_data);

        image_3ch = FourierImage(3, image_data);
        image_3ch.applyTransform();

        // setup test image with 1 channel
        image_1ch = FourierImage(1, image_data);
        image_1ch.applyTransform();

        // expected fourier transform
        expected_transf = Eigen::ArrayXXcd::Zero(4,4);
        expected_transf <<  0.i,0.i,(-2.1333333333+0.i), 0.i,
                (-0.+0.i),-0.i,(-2.1333333333-2.1333333333i),(-0.+0.i),
                (-0.5333333333+0.i),(-0.5333333333-0.5333333333i),(8.+0.i),(-0.5333333333+0.5333333333i),
                (-0.+0.i),(-0.+0.i),(-2.1333333333+2.1333333333i),0.i;

        // create a Fourier Image without the transformation applied
        image_1ch_no_transf = FourierImage(1, image_data);

    }

    FourierImage image_3ch;
    FourierImage image_1ch;
    FourierImage image_1ch_no_transf;
    Eigen::ArrayXXcd expected_transf;
};

TEST_F(fourierImageTests, assignmentOperatorWorks) {
    FourierImage image_1ch_copy = image_1ch;
    ASSERT_EQ(image_1ch_copy.getChannels(), image_1ch.getChannels());

    for (int j = 0; j < image_1ch.getWidth(); j++) {
        for (int k = 0; k < image_1ch.getHeight(); k++) {
            ASSERT_EQ(image_1ch_copy.getTransform()(j, k), image_1ch.getTransform()(j, k));
        }
    }
}

TEST_F(fourierImageTests, copyConstructorWorks) {
    FourierImage image_1ch_copy(image_1ch);
    ASSERT_EQ(image_1ch_copy.getChannels(), image_1ch.getChannels());

    for (int j = 0; j < image_1ch.getWidth(); j++) {
        for (int k = 0; k < image_1ch.getHeight(); k++) {
            ASSERT_EQ(image_1ch_copy.getTransform()(j, k), image_1ch.getTransform()(j, k));
        }
    }
}

TEST_F(fourierImageTests, applyTransformComputesFTCorrectly) {
 // test 3 channel image
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            //check equality up to 1e-10
            ASSERT_NEAR(image_3ch.getTransform()(i,j).real(), expected_transf(i,j).real(), 1e-10);
            ASSERT_NEAR(image_3ch.getTransform()(i,j).imag(), expected_transf(i,j).imag(), 1e-10);
        }
    }

    // test 1 channel image
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            //check equality up to 1e-10
            ASSERT_NEAR(image_1ch.getTransform()(i,j).real(), expected_transf(i,j).real(), 1e-10);
            ASSERT_NEAR(image_1ch.getTransform()(i,j).imag(), expected_transf(i,j).imag(), 1e-10);
        }
    }
}

TEST_F(fourierImageTests, applyInverseTransformComputesIFTCorrectly) {
    FourierImage inverse_transform = image_1ch.applyInverseTransform();

    // test 1 channel image
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            //check equality up to 1e-10
            ASSERT_NEAR(image_1ch(0)(i,j), inverse_transform(0)(i,j), 1e-10);
        }
    }
}

TEST_F(fourierImageTests, applyInverseTransformBeforeDirectTransformThrowsException) {
    ASSERT_THROW(image_1ch_no_transf.applyInverseTransform(), std::runtime_error);
}

// Getters

TEST_F(fourierImageTests, getTransformReturnsCorrectTransform){
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //check equality up to 1e-10
            ASSERT_NEAR(image_1ch.getTransform()(i, j).real(), expected_transf(i, j).real(), 1e-10);
            ASSERT_NEAR(image_1ch.getTransform()(i, j).imag(), expected_transf(i, j).imag(), 1e-10);
        }
    }
}

TEST_F(fourierImageTests, getMagnitudeBeforeTransformThrowsException){
    FourierImage image = FourierImage(1, Eigen::ArrayXXd::Zero(4,4));
    ASSERT_THROW(image.getMagnitude(), std::runtime_error);
}

TEST_F(fourierImageTests, getMagnitudeReturnsCorrectMagnitude){
    Eigen::ArrayXXd expected_magnitude = expected_transf.abs();

    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //check equality up to 1e-10
            ASSERT_NEAR(image_1ch.getMagnitude()(i, j), expected_magnitude(i, j), 1e-10);
        }
    }
}

TEST_F(fourierImageTests, getMagnitudeLogReturnsCorrectMagnitudeLog){
    Eigen::ArrayXXd expected_magnitude = expected_transf.abs()+1e-8;
    Eigen::ArrayXXd expected_magnitude_log = expected_magnitude.log();
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //check equality up to 1e-6
            ASSERT_NEAR(image_1ch.getMagnitude(true)(i, j), expected_magnitude_log(i, j), 1e-6);
        }
    }
}

TEST_F(fourierImageTests, getPhaseBeforeTransformThrowsException){
    FourierImage image = FourierImage(1, Eigen::ArrayXXd::Zero(4,4));
    ASSERT_THROW(image.getPhase(), std::runtime_error);
}

TEST_F(fourierImageTests, getPhaseReturnsCorrectPhase){
    // phase is a bit tricky because for very small values that should be 0, we dont get 0 phase
    FourierImage image = FourierImage(1, Eigen::ArrayXXd::Zero(4,4));
    image.setTransform(expected_transf);
    Eigen::ArrayXXd expected_phase = expected_transf.arg();
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //check equality up to 1e-10
            ASSERT_NEAR(image.getPhase()(i, j), expected_phase(i, j), 1e-10);
        }
    }
}

TEST_F(fourierImageTests, getRealBeforeTransformThrowsException){
    FourierImage image = FourierImage(1, Eigen::ArrayXXd::Zero(4,4));
    ASSERT_THROW(image.getReal(), std::runtime_error);
}

TEST_F(fourierImageTests, getRealReturnsCorrectReal){
    Eigen::ArrayXXd expected_real = expected_transf.real();
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //check equality up to 1e-10
            ASSERT_NEAR(image_1ch.getReal()(i, j), expected_real(i, j), 1e-10);
        }
    }
}

TEST_F(fourierImageTests, getImaginaryBeforeTransformThrowsException){
    FourierImage image = FourierImage(1, Eigen::ArrayXXd::Zero(4,4));
    ASSERT_THROW(image.getImaginary(), std::runtime_error);
}

TEST_F(fourierImageTests, getImaginaryReturnsCorrectImaginary){
    Eigen::ArrayXXd expected_imaginary = expected_transf.imag();
    for(int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //check equality up to 1e-10
            ASSERT_NEAR(image_1ch.getImaginary()(i, j), expected_imaginary(i, j), 1e-10);
        }
    }
}

// Setters

TEST_F(fourierImageTests, setTransformSetsTransformCorrectly) {
    FourierImage image = FourierImage(1, Eigen::ArrayXXd::Zero(4, 4));
    image.setTransform(expected_transf);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //check equality up to 1e-10
            ASSERT_NEAR(image.getTransform()(i, j).real(), expected_transf(i, j).real(), 1e-10);
            ASSERT_NEAR(image.getTransform()(i, j).imag(), expected_transf(i, j).imag(), 1e-10);
        }
    }
}

TEST_F(fourierImageTests, setTransformInvalidSizeThrowsException) {
    FourierImage image = FourierImage(1, Eigen::ArrayXXd::Zero(4, 4));
    ASSERT_THROW(image.setTransform(Eigen::ArrayXXcd::Zero(3, 3)), std::runtime_error);
}

// Filters
TEST_F(fourierImageTests, applyLowPassBeforeTransformThrowsException) {
    ASSERT_THROW(image_1ch_no_transf.applyLowPassFilter(1), std::runtime_error);
}

TEST_F(fourierImageTests, applyHighPassBeforeTransformThrowsException) {
    ASSERT_THROW(image_1ch_no_transf.applyHighPassFilter(1), std::runtime_error);
}

TEST_F(fourierImageTests, applyBandPassBeforeTransformThrowsException) {
    ASSERT_THROW(image_1ch_no_transf.applyBandPassFilter(1, 2), std::runtime_error);
}

TEST_F(fourierImageTests, applyLowPassInvalidCutOffThrowsException) {
    ASSERT_THROW(image_1ch.applyLowPassFilter(-1), std::invalid_argument);
}

TEST_F(fourierImageTests, applyHighPassInvalidCutOffThrowsException) {
    ASSERT_THROW(image_1ch.applyHighPassFilter(-1), std::invalid_argument);
}

TEST_F(fourierImageTests, applyBandPassNegativeLowerCutOffThrowsException) {
    ASSERT_THROW(image_1ch.applyBandPassFilter(-1, 1), std::invalid_argument);
}

TEST_F(fourierImageTests, applyBandPassNegativeUpperCutOffThrowsException) {
    ASSERT_THROW(image_1ch.applyBandPassFilter(1, -1), std::invalid_argument);
}

TEST_F(fourierImageTests, applyBandPassLowerCutOffGreaterThanUpperThrowsException) {
    ASSERT_THROW(image_1ch.applyBandPassFilter(2, 1), std::invalid_argument);
}

TEST_F(fourierImageTests, lowPassFilterWithZeroCutoffReturnsZeroImage) {
    image_1ch.applyLowPassFilter(0);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ASSERT_NEAR(image_1ch.getTransform()(i, j).real(), 0, 1e-10);
            ASSERT_NEAR(image_1ch.getTransform()(i, j).imag(), 0, 1e-10);
        }
    }
}

TEST_F(fourierImageTests, highPassFilterWithZeroCutoffReturnsOriginalImage) {
    image_1ch.applyHighPassFilter(0);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ASSERT_NEAR(image_1ch.getTransform()(i, j).real(), expected_transf(i, j).real(), 1e-10);
            ASSERT_NEAR(image_1ch.getTransform()(i, j).imag(), expected_transf(i, j).imag(), 1e-10);
        }
    }
}

TEST_F(fourierImageTests, bandPassFilterWithSameCutoffsReturnsZeroImage) {
    image_1ch.applyBandPassFilter(1, 1);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ASSERT_NEAR(image_1ch.getTransform()(i, j).real(), 0, 1e-10);
            ASSERT_NEAR(image_1ch.getTransform()(i, j).imag(), 0, 1e-10);
        }
    }
}
