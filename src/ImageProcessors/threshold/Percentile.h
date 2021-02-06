#pragma once
#include <ImageProcessor.h>

class Percentile: public ImageProcessor {
public:
    Percentile();
    cv::Mat processImage(const cv::Mat& image) override;
};