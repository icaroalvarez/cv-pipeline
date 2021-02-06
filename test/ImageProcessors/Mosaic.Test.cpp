#include "ImageProcessors/Mosaic.h"
#include <catch2/catch.hpp>
#include <opencv2/highgui.hpp>

bool checkImageSimilarity(const cv::Mat &image1, const cv::Mat &image2)
{
    if((image1.size() != image2.size()) || (image1.type() != image2.type()))
    {
        return false;
    }
    cv::Mat S;
    cv::subtract(image1, image2, S, cv::noArray(), CV_32F);
    cv::multiply(S, S, S);
    double diff = sum(S).val[0]/image1.total();
    return diff<1e-10;
}

TEST_CASE("Create mosaic image specifying row and col positions")
{
    cv::Mat image = cv::imread(fixtures_path"images/Lenna.png");
    cv::Mat roi_bottom_right = cv::imread(fixtures_path"images/Lenna_bottom_right.png");
    cv::Mat roi_bottom_left = cv::imread(fixtures_path"images/Lenna_bottom_left.png");
    cv::Mat roi_top_right = cv::imread(fixtures_path"images/Lenna_top_right.png");
    cv::Mat roi_top_left = cv::imread(fixtures_path"images/Lenna_top_left.png");

    SECTION("create mosaic")
    {
        Mosaic mosaic;
        mosaic.addImage(roi_top_left, 0, 0);
        mosaic.addImage(roi_top_right, 1, 0);
        mosaic.addImage(roi_bottom_left, 0, 1);
        mosaic.addImage(roi_bottom_right, 1, 1);
        REQUIRE(checkImageSimilarity(mosaic.createMosaic(), image));
    }

    SECTION("Reuse mosaic by resetting it")
    {
        Mosaic mosaic;
        mosaic.addImage(roi_top_left, 0, 0);
        mosaic.addImage(roi_top_right, 1, 0);
        mosaic.addImage(roi_bottom_left, 0, 1);
        mosaic.addImage(roi_bottom_right, 1, 1);
        mosaic.reset();
        mosaic.addImage(roi_top_left, 0, 0);
        mosaic.addImage(roi_top_right, 1, 0);
        cv::Mat lenna_top = cv::imread(fixtures_path"images/Lenna_top.png");
        REQUIRE(checkImageSimilarity(mosaic.createMosaic(), lenna_top));
    }
}
