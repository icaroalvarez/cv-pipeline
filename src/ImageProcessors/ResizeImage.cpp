#include <opencv2/imgproc.hpp>
#include "ResizeImage.h"

ResizeImage::ResizeImage()
:ImageProcessor("resize")
{
    getParameters().registerParameter("factor_x",
            DecimalParameter{0.3, 0.1, 1.0, 0.1, 1});
    getParameters().registerParameter("factor_y",
            DecimalParameter{0.3, 0.1, 1.0, 0.1, 1});
    getParameters().registerParameter("interpol_mode",
            OptionsParameter{1, {"linear", "cubic", "area", "landzos4"}});
}

cv::Mat ResizeImage::processImage(const cv::Mat &image)
{
    const auto factorX{getParameters().getParameterValue<double>("factor_x")};
    const auto factorY{getParameters().getParameterValue<double>("factor_y")};
    const auto interpolation_mode{getParameters().getParameterValue<SelectedOptionIndex>("interpol_mode")};
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(), factorX, factorY, interpolation_mode);
    setDebugImage(resized);
    return resized;
}
