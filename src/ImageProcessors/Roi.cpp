#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc.hpp>
#include "Roi.h"

RoiProcessor::RoiProcessor()
:ImageProcessor("roi")
{
    getParameters().registerParameter("top_left_x", IntegerParameter{5, 0, std::numeric_limits<int>::max()});
    getParameters().registerParameter("top_left_y", IntegerParameter{5, 0, std::numeric_limits<int>::max()});
    getParameters().registerParameter("top_right_x", IntegerParameter{50, 0, std::numeric_limits<int>::max()});
    getParameters().registerParameter("top_right_y", IntegerParameter{5, 0, std::numeric_limits<int>::max()});
    getParameters().registerParameter("bottom_left_x", IntegerParameter{5, 0, std::numeric_limits<int>::max()});
    getParameters().registerParameter("bottom_left_y", IntegerParameter{50, 0, std::numeric_limits<int>::max()});
    getParameters().registerParameter("bottom_right_x", IntegerParameter{50, 0, std::numeric_limits<int>::max()});
    getParameters().registerParameter("bottom_right_y", IntegerParameter{50, 0, std::numeric_limits<int>::max()});
}

int getXValueOrMax(int value, const cv::Size& imageSize)
{
    auto xValue = std::max(0, value);
    xValue = std::min(xValue, imageSize.width-1);
    return xValue;
}

int getYValueOrMax(int value, const cv::Size& imageSize)
{
    auto yValue = std::max(0, value);
    yValue = std::min(yValue, imageSize.height-1);
    return yValue;
}

cv::Mat RoiProcessor::processImage(const cv::Mat &image)
{
    mosaic.reset();

    const auto topLeftX{getXValueOrMax(getParameters().getParameterValue<int>("top_left_x"), image.size())};
    const auto topLeftY{getYValueOrMax(getParameters().getParameterValue<int>("top_left_y"), image.size())};
    const auto topRightX{getXValueOrMax(getParameters().getParameterValue<int>("top_right_x"), image.size())};
    const auto topRightY{getYValueOrMax(getParameters().getParameterValue<int>("top_right_y"), image.size())};
    const auto bottomLeftX{getXValueOrMax(getParameters().getParameterValue<int>("bottom_left_x"), image.size())};
    const auto bottomLeftY{getYValueOrMax(getParameters().getParameterValue<int>("bottom_left_y"), image.size())};
    const auto bottomRightX{getXValueOrMax(getParameters().getParameterValue<int>("bottom_right_x"), image.size())};
    const auto bottomRightY{getYValueOrMax(getParameters().getParameterValue<int>("bottom_right_y"), image.size())};
    std::vector<cv::Point> points{{topRightX, topRightY}, {bottomRightX, bottomRightY}, {bottomLeftX, bottomLeftY}, {topLeftX, topLeftY}};
    cv::Mat mask{cv::Mat::zeros(image.size(), CV_8U)};
    const cv::Scalar whiteColor = cv::Scalar::all(255);
    constexpr auto contourIndex{-1};
    const std::vector<std::vector<cv::Point>> contours{{points}};
    cv::drawContours(mask, contours, contourIndex, whiteColor, cv::FILLED);
    mosaic.addImage(mask, 0, 0, "mask");
    cv::Mat output{cv::Mat::zeros(image.size(), image.type())};
    image.copyTo(output, mask);
    mosaic.addImage(output, 1, 0, "output");
    setDebugImage(mosaic.createMosaic());
    return output;
}
