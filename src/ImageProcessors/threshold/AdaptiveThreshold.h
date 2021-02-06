#pragma once
#include <ImageProcessor.h>

class AdaptiveThreshold: public ImageProcessor {
public:
    AdaptiveThreshold();
    cv::Mat processImage(const cv::Mat &image) override;
};