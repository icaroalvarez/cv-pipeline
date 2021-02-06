#pragma once
#include <ImageProcessor.h>

class CannyEdgeDetector: public ImageProcessor
{
public:
    CannyEdgeDetector();
    cv::Mat processImage(const cv::Mat& image) override;

};



