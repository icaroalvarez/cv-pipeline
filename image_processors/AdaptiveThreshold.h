#pragma once

#include <ImageProcessor.h>

class AdaptiveThreshold: public ImageProcessor {
public:
    AdaptiveThreshold();
    void processImage(const cv::Mat &image) override;
};