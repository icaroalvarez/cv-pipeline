#include <opencv2/imgproc.hpp>
#include "AdaptiveThreshold.h"

AdaptiveThreshold::AdaptiveThreshold()
:ImageProcessor("adaptive_threshold")
{
    getParameters().registerParameter("block_size", IntegerParameter{10, 1, 999});
    getParameters().registerParameter("c_mean", IntegerParameter{-10, -999, 999});
    getParameters().registerParameter("adaptive_method",OptionsParameter{0, {"mean", "gaussian"}});
}

int makeOdd(int value)
{
    if(value % 2 == 0) {
        return value+1;
    }
    return value;
}

cv::Mat AdaptiveThreshold::processImage(const cv::Mat& image)
{
    cv::Mat imageGray{image};
    if(image.channels() > 1)
    {
        cv::cvtColor(image, imageGray, cv::COLOR_RGB2GRAY);
    }

    int blockSize = makeOdd(getParameters().getParameterValue<int>("block_size"));
    auto cMean = static_cast<double>(getParameters().getParameterValue<int>("c_mean"));

    int option = cv::ADAPTIVE_THRESH_GAUSSIAN_C;
    if(getParameters().getParameterValue<SelectedOptionIndex>("adaptive_method") == 0)
    {
        option = cv::ADAPTIVE_THRESH_MEAN_C;
    }
    cv::Mat outputImage;
    cv::adaptiveThreshold(imageGray, outputImage, 255, option,
            cv::THRESH_BINARY, blockSize, cMean);

    setDebugImage(outputImage);
    return outputImage;
}
