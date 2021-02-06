#pragma once
#include "ImageProcessor.h"

class HoughLines: public ImageProcessor
{
public:
    HoughLines();
    cv::Mat processImage(const cv::Mat& image) override;

private:
    cv::Mat processStandardMode(const cv::Mat& image);
    cv::Mat processProbabilisticMode(const cv::Mat& image);
};



