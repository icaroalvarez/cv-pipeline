#pragma once
#include "ImageProcessor.h"

class GaborFilter: public ImageProcessor
{
public:
    GaborFilter();
    cv::Mat processImage(const cv::Mat& image) override;
};



