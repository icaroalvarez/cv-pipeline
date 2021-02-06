#include <opencv4/opencv2/imgproc.hpp>
#include "GaborFilter.h"

GaborFilter::GaborFilter()
        :ImageProcessor("gabor_filter")
{
    getParameters().registerParameter("sigma",
                                      DecimalParameter{0.04, 0.01,
                                                       1.0, 0.01, 2});
    getParameters().registerParameter("theta",
                                      DecimalParameter{0.04, 0.01,
                                                       1.0, 0.01, 2});
    getParameters().registerParameter("lambda",
                                      DecimalParameter{0.04, 0.01,
                                                       1.0, 0.01, 2});
    getParameters().registerParameter("gamma",
                                      DecimalParameter{0.04, 0.01,
                                                       1.0, 0.01, 2});
    getParameters().registerParameter("psi",
                                      DecimalParameter{0.04, 0.01,
                                                       1.0, 0.01, 2});
}

cv::Mat GaborFilter::processImage(const cv::Mat &image)
{
    const auto sigma{getParameters().getParameterValue<double>("sigma")};
    const auto theta{getParameters().getParameterValue<double>("theta")};
    const auto lambda{getParameters().getParameterValue<double>("lambda")};
    const auto gamma{getParameters().getParameterValue<double>("gamma")};
    const auto psi{getParameters().getParameterValue<double>("psi")};

    auto kernel{cv::getGaborKernel(cv::Size(image.rows, image.cols), sigma, theta, lambda, gamma, psi)};
    cv::Mat output;
    cv::filter2D(image, output, -1, kernel);
    setDebugImage(output);
    return output;
}
