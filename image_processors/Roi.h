#pragma once

#include <ImageProcessor.h>
#include "Mosaic.h"

class RoiProcessor: public ImageProcessor {
public:
    RoiProcessor();
    void processImage(const cv::Mat &image) override;
private:
    Mosaic mosaic;
};