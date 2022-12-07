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
        image_data(4,4) = 1;
        image_data(4,5) = 0.5;
        image_data(4,6) = -0.5;
        image = Image(3, image_data);
    }

    Image image;
};

