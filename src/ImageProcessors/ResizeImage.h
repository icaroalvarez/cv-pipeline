#pragma once
#include "ImageProcessor.h"

class ResizeImage: public ImageProcessor
{
public:
    ResizeImage();
    cv::Mat processImage(const cv::Mat &image) override;
};
