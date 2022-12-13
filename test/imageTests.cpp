//
// Created by tsfeith on 03/12/22.
//

#include "Image.cpp"
#include "gtest/gtest.h"
#include <opencv2/opencv.hpp>

class imageTests : public ::testing::Test {
protected:
    void SetUp() override
    {
        input_vector = vector<Eigen::ArrayXXd>(3, Eigen::ArrayXXd::Zero(5, 5));
        invalid_vector = vector<Eigen::ArrayXXd>(3, Eigen::ArrayXXd::Zero(5, 5));
        for (int i = 0; i < 5; i++) {
            input_vector[0](i, i) = 1;
            input_vector[1](i, i) = 1;
            input_vector[2](i, i) = 1;
        }

        invalid_vector[0](0, 0) = 5;
    }
    vector<Eigen::ArrayXXd> input_vector;
    vector<Eigen::ArrayXXd> invalid_vector;
};

TEST_F(imageTests, defaultConstructor) {
    Image image;
    EXPECT_EQ(image.getChannels(), 3);
    EXPECT_EQ(image.getHeight(), 50);
    EXPECT_EQ(image.getWidth(), 50);
}

TEST_F(imageTests, constructorFromFilenameFailsWhenFileDoesNotExist) {
    EXPECT_THROW(Image("nonexistent_file"), std::invalid_argument);
}

TEST_F(imageTests, constructorFromFilenameFailsWhenImageCannotBeRead) {
    EXPECT_THROW(Image("invalid_image.png"), std::invalid_argument);
}

TEST_F(imageTests, constructorFromFilenameWorksForRealImage) {
    EXPECT_NO_THROW(Image("teapot.png"));
}

TEST_F(imageTests, constructorWithNoDataThrowsExceptionOnInvalidDimensions) {
    EXPECT_THROW(Image(0, 0, 0), std::invalid_argument);
    EXPECT_THROW(Image(0, 1, 1), std::invalid_argument);
    EXPECT_THROW(Image(1, 0, 1), std::invalid_argument);
    EXPECT_THROW(Image(1, 1, 0), std::invalid_argument);
}

TEST_F(imageTests, constructorWithNoDataAndValidParametersCreatesCorrectImage) {
    Image image(1, 2, 3);
    EXPECT_EQ(image.getChannels(), 3);
    EXPECT_EQ(image.getHeight(), 2);
    EXPECT_EQ(image.getWidth(), 1);
}

TEST_F(imageTests, constructorFromEigenArrayWithChannelsThrowsExceptionOnInvalidDimensions) {
    EXPECT_THROW(Image(0, input_vector[0]), std::invalid_argument);
}

TEST_F(imageTests, constructorFromEigenArrayWithChannelsThrowsExceptionOnEmptyArray) {
    Eigen::ArrayXXd empty_array;
    EXPECT_THROW(Image(1, empty_array), std::invalid_argument);
}

TEST_F(imageTests, constructorFromEigenArrayWithChannelsThrowsExceptionOnInvalidData) {
    EXPECT_THROW(Image(3, invalid_vector[0]), std::invalid_argument);
}

TEST_F(imageTests, constructorFromEigenArrayWithChannelsWorksWhenValid) {
    Image image(3, input_vector[0]);
    EXPECT_EQ(image.getChannels(), 3);
    EXPECT_EQ(image.getHeight(), 5);
    EXPECT_EQ(image.getWidth(), 5);
    EXPECT_EQ(image(0, 0, 0), 1);
    EXPECT_EQ(image(1, 1, 1), 1);
    EXPECT_EQ(image(2, 2, 2), 1);
}

TEST_F(imageTests, constructorFromEigenArrayWithoutParametersThrowsExceptionOnEmptyArray) {
    Eigen::ArrayXXd empty_array = Eigen::ArrayXXd();
    EXPECT_THROW(Image image(empty_array), std::invalid_argument);
}

TEST_F(imageTests, constructorFromEigenArrayWithoutParametersThrowsExceptionOnInvalidData) {
    EXPECT_THROW(Image image(invalid_vector[0]), std::invalid_argument);
}

TEST_F(imageTests, constructorFromEigenArrayWithoutParametersWorksWhenValid) {
    Image image(input_vector[0]);
    EXPECT_EQ(image.getChannels(), 1);
    EXPECT_EQ(image.getHeight(), 5);
    EXPECT_EQ(image.getWidth(), 5);
    EXPECT_EQ(image(0, 0, 0), 1);
    EXPECT_EQ(image(1, 1, 0), 1);
    EXPECT_EQ(image(2, 2, 0), 1);
}

TEST_F(imageTests, constructorFromVectorWithoutParametersThrowsExceptionOnEmptyVector) {
    vector<Eigen::ArrayXXd> empty_vector;
    EXPECT_THROW(Image image(empty_vector), std::invalid_argument);
}

TEST_F(imageTests, constructorFromVectorWithoutParametersThrowsExceptionOnInvalidData) {
    EXPECT_THROW(Image image(invalid_vector), std::invalid_argument);
}

TEST_F(imageTests, constructorFromVectorWithoutParametersThrowsExceptionOnVectorWithEmptyArrays) {
    vector<Eigen::ArrayXXd> empty_array_vector;
    empty_array_vector.emplace_back(Eigen::ArrayXXd());
    EXPECT_THROW(Image image(empty_array_vector), std::invalid_argument);
}

TEST_F(imageTests, constructorFromVectorWithoutParametersThrowsExceptionWhenFilledWithInconsistentArrays) {
    vector<Eigen::ArrayXXd> inconsistent_array_vector;
    inconsistent_array_vector.emplace_back(Eigen::ArrayXXd::Zero(5, 5));
    inconsistent_array_vector.emplace_back(Eigen::ArrayXXd::Zero(5, 5));
    inconsistent_array_vector.emplace_back(Eigen::ArrayXXd::Zero(5, 6));
    EXPECT_THROW(Image image(inconsistent_array_vector), std::invalid_argument);
}

TEST_F(imageTests, constructorFromVectorWithoutParametersWorksWhenValid) {
    Image image(input_vector);
    EXPECT_EQ(image.getChannels(), 3);
    EXPECT_EQ(image.getHeight(), 5);
    EXPECT_EQ(image.getWidth(), 5);
    EXPECT_EQ(image(0, 0, 0), 1);
    EXPECT_EQ(image(1, 1, 1), 1);
    EXPECT_EQ(image(2, 2, 2), 1);
}

TEST_F(imageTests, constructorFromImageWorks) {
    Image image(input_vector);
    Image image_copy(image);
    EXPECT_EQ(image_copy.getChannels(), 3);
    EXPECT_EQ(image_copy.getHeight(), 5);
    EXPECT_EQ(image_copy.getWidth(), 5);
    EXPECT_EQ(image_copy(0, 0, 0), 1);
    EXPECT_EQ(image_copy(1, 1, 1), 1);
    EXPECT_EQ(image_copy(2, 2, 2), 1);
}

TEST_F(imageTests, assignmentOperatorWorks) {
    Image image(input_vector);
    Image image_copy = image;
    EXPECT_EQ(image_copy.getChannels(), 3);
    EXPECT_EQ(image_copy.getHeight(), 5);
    EXPECT_EQ(image_copy.getWidth(), 5);
    EXPECT_EQ(image_copy(0, 0, 0), 1);
    EXPECT_EQ(image_copy(1, 1, 1), 1);
    EXPECT_EQ(image_copy(2, 2, 2), 1);
}

TEST_F(imageTests, getHeightWorks) {
    Image image(input_vector);
    EXPECT_EQ(image.getHeight(), 5);
}

TEST_F(imageTests, getWidthWorks) {
    Image image(input_vector);
    EXPECT_EQ(image.getWidth(), 5);
}

TEST_F(imageTests, getChannelsWorks) {
    Image image(input_vector);
    EXPECT_EQ(image.getChannels(), 3);
}

TEST_F(imageTests, getFullDataWorks) {
    Image image(input_vector);
    vector<Eigen::ArrayXXd> full_data = image.getData();
    EXPECT_EQ(full_data.size(), 3);
    EXPECT_EQ(full_data[0](0, 0), 1);
    EXPECT_EQ(full_data[1](1, 1), 1);
    EXPECT_EQ(full_data[2](2, 2), 1);
}

TEST_F(imageTests, getSingleChannelDataWorks) {
    Image image(input_vector);
    Eigen::ArrayXXd single_channel_data = image.getData(1);
    EXPECT_EQ(single_channel_data(0, 0), 1);
    EXPECT_EQ(single_channel_data(1, 1), 1);
    EXPECT_EQ(single_channel_data(2, 2), 1);
}

TEST_F(imageTests, getSingleChannelDataThrowsExceptionOnInvalidChannel) {
    Image image(input_vector);
    EXPECT_THROW(image.getData(3), std::invalid_argument);
}

TEST_F(imageTests, getFullPixelWorksThrowsExceptionOnInvalidCoordinates) {
    Image image(input_vector);
    EXPECT_THROW(image.getPixel(5, 5), std::invalid_argument);
}

TEST_F(imageTests, getFullPixelWorks) {
    Image image(input_vector);
    Eigen::ArrayXd pixel = image.getPixel(2, 2);
    EXPECT_EQ(pixel.size(), 3);
    EXPECT_EQ(pixel[0], 1);
    EXPECT_EQ(pixel[1], 1);
    EXPECT_EQ(pixel[2], 1);
}

TEST_F(imageTests, getSingleChannelPixelThrowsExceptionOnInvalidCoordinates) {
    Image image(input_vector);
    EXPECT_THROW(image.getPixel(5, 5, 1), std::invalid_argument);
}

TEST_F(imageTests, getSingleChannelPixelThrowsExceptionOnInvalidChannel) {
    Image image(input_vector);
    EXPECT_THROW(image.getPixel(2, 2, 3), std::invalid_argument);
}

TEST_F(imageTests, getSingleChannelPixelWorks) {
    Image image(input_vector);
    double pixel = image.getPixel(2, 2, 1);
    EXPECT_EQ(pixel, 1);
}

TEST_F(imageTests, setFullDataThrowsExceptionOnNewDataWithWrongChannels) {
    Image image(input_vector);
    vector<Eigen::ArrayXXd> new_data;
    new_data.emplace_back(Eigen::ArrayXXd::Zero(5, 5));
    new_data.emplace_back(Eigen::ArrayXXd::Zero(5, 5));
    EXPECT_THROW(image.setData(new_data), std::invalid_argument);
}

TEST_F(imageTests, setFullDataThrowsExceptionOnNewDataWithWrongArrayDimensions) {
    Image image(input_vector);
    vector<Eigen::ArrayXXd> new_data;
    new_data.emplace_back(Eigen::ArrayXXd::Zero(5, 5));
    new_data.emplace_back(Eigen::ArrayXXd::Zero(5, 5));
    new_data.emplace_back(Eigen::ArrayXXd::Zero(5, 6));
    EXPECT_THROW(image.setData(new_data), std::invalid_argument);
}

TEST_F(imageTests, setFullDataThrowsExceptionOnNewDataWithInvalidPixelValue) {
    Image image(input_vector);
    EXPECT_THROW(image.setData(invalid_vector), std::invalid_argument);
}

TEST_F(imageTests, setFullDataWorks) {
    Image image(input_vector);
    vector<Eigen::ArrayXXd> new_data;
    new_data.emplace_back(Eigen::ArrayXXd::Zero(5, 5));
    new_data.emplace_back(Eigen::ArrayXXd::Zero(5, 5));
    new_data.emplace_back(Eigen::ArrayXXd::Zero(5, 5));
    image.setData(new_data);
    EXPECT_EQ(image(0, 0, 0), 0);
    EXPECT_EQ(image(1, 1, 1), 0);
    EXPECT_EQ(image(2, 2, 2), 0);
}

TEST_F(imageTests, setSingleChannelDataThrowsExceptionOnNewDataWithWrongArrayDimensions) {
    Image image(input_vector);
    EXPECT_THROW(image.setData(1, Eigen::ArrayXXd::Zero(5, 6)), std::invalid_argument);
}

TEST_F(imageTests, setSingleChannelDataThrowsExceptionOnNewDataWithInvalidPixelValue) {
    Image image(input_vector);
    EXPECT_THROW(image.setData(1, invalid_vector[0]), std::invalid_argument);
}

TEST_F(imageTests, setSingleChannelDataThrowsExceptionOnInvalidChannel) {
    Image image(input_vector);
    EXPECT_THROW(image.setData(3, Eigen::ArrayXXd::Zero(5, 5)), std::invalid_argument);
}

TEST_F(imageTests, setSingleChannelDataWorks) {
    Image image(input_vector);
    image.setData(1, Eigen::ArrayXXd::Zero(5, 5));
    EXPECT_EQ(image(0, 0, 0), 1);
    EXPECT_EQ(image(1, 1, 1), 0);
    EXPECT_EQ(image(2, 2, 2), 1);
}

TEST_F(imageTests, setFullPixelThrowsExceptionOnNewPixelWithWrongSize) {
    Image image(input_vector);
    EXPECT_THROW(image.setPixel(0, 0, Eigen::ArrayXd::Zero(2)), std::invalid_argument);
}

TEST_F(imageTests, setFullPixelThrowsExceptionOnNewPixelWithInvalidValue) {
    Image image(input_vector);
    Eigen::ArrayXd invalid_pixel(3);
    invalid_pixel << 1, 1, 2;
    EXPECT_THROW(image.setPixel(0, 0, invalid_pixel), std::invalid_argument);
}

TEST_F(imageTests, setFullPixelThrowsExceptionOnInvalidCoordinates) {
    Image image(input_vector);
    EXPECT_THROW(image.setPixel(5, 5, Eigen::ArrayXd::Zero(3)), std::invalid_argument);
}

TEST_F(imageTests, setFullPixelWorks) {
    Image image(input_vector);
    image.setPixel(0, 0, Eigen::ArrayXd::Zero(3));
    EXPECT_EQ(image(0, 0, 0), 0);
    EXPECT_EQ(image(0, 0, 1), 0);
    EXPECT_EQ(image(0, 0, 2), 0);
}

TEST_F(imageTests, setSingleChannelPixelThrowsExceptionOnNewPixelWithInvalidValue) {
    Image image(input_vector);
    EXPECT_THROW(image.setPixel(0, 0, 0, -1), std::invalid_argument);
}

TEST_F(imageTests, setSingleChannelPixelThrowsExceptionOnInvalidCoordinates) {
    Image image(input_vector);
    EXPECT_THROW(image.setPixel(5, 5, 0, 0), std::invalid_argument);
}

TEST_F(imageTests, setSingleChannelPixelThrowsExceptionOnInvalidChannel) {
    Image image(input_vector);
    EXPECT_THROW(image.setPixel(0, 0, 3, 0), std::invalid_argument);
}

TEST_F(imageTests, setSingleChannelPixelWorks) {
    Image image(input_vector);
    image.setPixel(0, 0, 0, 0);
    EXPECT_EQ(image(0, 0, 0), 0);
    EXPECT_EQ(image(0, 0, 1), 1);
    EXPECT_EQ(image(0, 0, 2), 1);
}

TEST_F(imageTests, fullChannelParenthesisOperatorThrowsExceptionOnInvalidChannel) {
    Image image(input_vector);
    EXPECT_THROW(image(3), std::invalid_argument);
}

TEST_F(imageTests, fullChannelParenthesisOperatorWorks) {
    Image image(input_vector);
    Eigen::ArrayXXd channel  = image(0);
    EXPECT_EQ(channel(0, 0), 1);
    EXPECT_EQ(channel(1, 1), 1);
    EXPECT_EQ(channel(2, 2), 1);
}

TEST_F(imageTests, fullPixelParenthesisOperatorThrowsExceptionOnInvalidCoordinates) {
    Image image(input_vector);
    EXPECT_THROW(image(5, 5), std::invalid_argument);
}

TEST_F(imageTests, fullPixelParenthesisOperatorWorks) {
    Image image(input_vector);
    Eigen::ArrayXd pixel = image(0, 0);
    EXPECT_EQ(pixel(0), 1);
    EXPECT_EQ(pixel(1), 1);
    EXPECT_EQ(pixel(2), 1);
}

TEST_F(imageTests, singleChannelPixelParenthesisOperatorThrowsExceptionOnInvalidCoordinates) {
    Image image(input_vector);
    EXPECT_THROW(image(5, 5, 0), std::invalid_argument);
}

TEST_F(imageTests, singleChannelPixelParenthesisOperatorThrowsExceptionOnInvalidChannel) {
    Image image(input_vector);
    EXPECT_THROW(image(0, 0, 3), std::invalid_argument);
}

TEST_F(imageTests, singleChannelPixelParenthesisOperatorWorks) {
    Image image(input_vector);
    double pixel = image(0, 0, 0);
    EXPECT_EQ(pixel, 1);
}

TEST_F(imageTests, conversionToCvMatWorks) {
    Image image(input_vector);
    cv::Mat cv_image = image.toCvMat();
    EXPECT_EQ(cv_image.rows, 5);
    EXPECT_EQ(cv_image.cols, 5);
    EXPECT_EQ(cv_image.channels(), 3);
}

TEST_F(imageTests, saveImageWorks) {
    Image image(input_vector);
    image.save("test_image.png", false);

}

TEST_F(imageTests, equalityOperatorReturnsTrueForEqualImages) {
    Image image1(input_vector);
    Image image2(input_vector);
    EXPECT_TRUE(image1 == image2);
}

TEST_F(imageTests, equalityOperatorReturnsFalseForImagesWithDifferentValues) {
    Image image1(input_vector);
    Image image2(input_vector);
    image2.setPixel(0, 0, 0, 0);
    EXPECT_FALSE(image1 == image2);
}

TEST_F(imageTests, equalityOperatorReturnsFalseForImagesWithDifferentDimensions) {
    Image image1(input_vector);
    Image image2(input_vector[0]);
    EXPECT_FALSE(image1 == image2);
}

TEST_F(imageTests, differenceOperatorReturnsTrueForDifferentImages) {
    Image image1(input_vector);
    Image image2(input_vector);
    image2.setPixel(0, 0, 0, 0);
    EXPECT_TRUE(image1 != image2);
}

TEST_F(imageTests, differenceOperatorReturnsFalseForEqualImages) {
    Image image1(input_vector);
    Image image2(input_vector);
    EXPECT_FALSE(image1 != image2);
}

TEST_F(imageTests, differenceOperatorReturnsTrueForImagesWithDifferentDimensions) {
    Image image1(input_vector);
    Image image2(input_vector[0]);
    EXPECT_TRUE(image1 != image2);
}

TEST_F(imageTests, reduceChannelslreturnsSingleChannelImageUnchanged) {
    Image image(input_vector[0]);
    Image reduced_image = image.reduceChannels();
    EXPECT_EQ(reduced_image, image);
}

TEST_F(imageTests, reduceChannelreturnsImageWithReducedChannels) {
    Image image(input_vector);
    Image reduced_image = image.reduceChannels();
    EXPECT_EQ(reduced_image.getWidth(), 5);
    EXPECT_EQ(reduced_image.getHeight(), 5);
    EXPECT_EQ(reduced_image.getChannels(), 1);
}