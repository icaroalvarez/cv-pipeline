#pragma one
#include <ImageProcessor.h>
#include "Mosaic.h"

class Convolution: public ImageProcessor
{
public:
    Convolution();
    cv::Mat processImage(const cv::Mat& image) override;
private:
    Mosaic mosaic;
};