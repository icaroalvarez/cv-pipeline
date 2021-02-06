#include <opencv2/imgproc.hpp>
#include <opencv4/opencv2/imgproc/types_c.h>
#include "Threshold.h"

Threshold::Threshold()
:ImageProcessor("threshold")
{
    getParameters().registerParameter("threshold_value", IntegerParameter{100, 0, 255});
}

cv::Mat Threshold::processImage(const cv::Mat& image)
{
    cv::Mat output;
    const auto thresholdValue{static_cast<double>(getParameters().getParameterValue<int>("threshold_value"))};
    cv::threshold(image, output,thresholdValue,
            255, CV_THRESH_BINARY);
    setDebugImage(output);
    return output;
}
