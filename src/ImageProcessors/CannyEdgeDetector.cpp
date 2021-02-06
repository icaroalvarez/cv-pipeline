#include "CannyEdgeDetector.h"
#include "opencv4/opencv2/imgproc.hpp"

constexpr auto thresholdParamName{"threshold"};
constexpr auto ratioParamName{"ratio"};
constexpr auto apertureSizeParamName{"aperture_size"};
constexpr auto l2GradientParamName{"l2_gradient"};

CannyEdgeDetector::CannyEdgeDetector()
:ImageProcessor{"canny_edge_detector"}
{
    getParameters().registerParameter(thresholdParamName,
                                      IntegerParameter{100, 0,255});
    getParameters().registerParameter(ratioParamName,
                                      DecimalParameter{3.0, 0.0,
                                                       5.0, 0.1, 1});
    getParameters().registerParameter(apertureSizeParamName,
                                      IntegerParameter{3, 1, 7});
    getParameters().registerParameter(l2GradientParamName, false);
}

int createNextOddValue(int value)
{
    return (value%2) ? value : value+1;
}

cv::Mat CannyEdgeDetector::processImage(const cv::Mat &image)
{
    const auto isEightBits{image.type() == CV_8U};
    if(isEightBits)
    {
        throw std::invalid_argument("Image received in canny detector has not 8 bits type");
    }

    const auto threshold{getParameters().getParameterValue<int>(thresholdParamName)};
    const auto ratio{getParameters().getParameterValue<double>(ratioParamName)};
    const auto apertureSize{getParameters().getParameterValue<int>(apertureSizeParamName)};
    const auto apertureSizeOdd{createNextOddValue(apertureSize)};
    const auto l2Gradient{getParameters().getParameterValue<bool>(l2GradientParamName)};

    cv::Mat edges{image.size(), CV_8UC1};
    cv::Canny(image, edges, threshold, threshold*ratio, apertureSizeOdd, l2Gradient);

    setDebugImage(edges);
    return edges;
}


