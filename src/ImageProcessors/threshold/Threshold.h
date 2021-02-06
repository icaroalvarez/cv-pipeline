#pragma once
#include <ImageProcessor.h>

class Threshold: public ImageProcessor {
public:
    Threshold();
    cv::Mat processImage(const cv::Mat& image) override;
};

