#pragma once
#include <ImageProcessor.h>
#include "Mosaic.h"

class ColorSpace: public ImageProcessor {
public:
    ColorSpace();
    cv::Mat processImage(const cv::Mat& image) override;
private:
    Mosaic mosaic;
};