#pragma one

#include <opencv2/imgproc.hpp>
#include "ResizeImage.h"

ResizeImage::ResizeImage()
:ImageProcessor("resize")
{
    getConfiguration().addFloatParameter("factor_x", 0.3, 0.1, 1.0, 0.1, 1);
    getConfiguration().addFloatParameter("factor_y", 0.3, 0.1, 1.0, 0.1, 1);
    getConfiguration().addOptionsParameter("interpol_mode", {"linear", "cubic", "area", "landzos4"}, 1);
}

void ResizeImage::processImage(cv::Mat image)
{
    float factorX = getConfiguration().getFloatParameter("factor_x");
    float factorY = getConfiguration().getFloatParameter("factor_y");
    cv::Mat resized;

    int interpolation_mode = getConfiguration().getOptionsParameterIndex("interpol_mode");

    cv::resize(image, resized, cv::Size(), factorX, factorY, interpolation_mode);
    setPostProcessedImage(resized);
    setDebugImage(resized);
}
