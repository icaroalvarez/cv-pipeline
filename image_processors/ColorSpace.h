#include <ImageProcessor.h>
#include "Mosaic.h"

class ColorSpace: public ImageProcessor {
public:
    ColorSpace();
    void processImage(const cv::Mat& image) override;
private:
    Mosaic mosaic;
};