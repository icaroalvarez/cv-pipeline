#include <ImageProcessor.h>
#include "Mosaic.h"

class Convolution: public ImageProcessor {
public:
    Convolution();
    void processImage(const cv::Mat& image) override;
private:
    Mosaic mosaic;
};