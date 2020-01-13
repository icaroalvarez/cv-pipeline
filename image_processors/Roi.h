#pragma once

#include <ImageProcessor.h>
#include "Mosaic.h"

class RoiProcessor: public ImageProcessor {
public:
    RoiProcessor();
    void processImage(cv::Mat image) override;
private:
    Mosaic mosaic;
};