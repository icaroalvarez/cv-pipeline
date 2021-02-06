#include <opencv2/imgproc.hpp>
#include "Percentile.h"

Percentile::Percentile():ImageProcessor("percentile")
{
    getParameters().registerParameter("percentile",
            DecimalParameter{0.04, 0.01, 1.0, 0.01, 2});
}

int calculatePercentile(const cv::Mat& image, float percentile)
{
    if(image.channels()>1)
    {
        throw std::invalid_argument("calculate percentile do not work with multichannel images");
    }
    if (image.empty())
    {
        throw std::invalid_argument("empty image calculating percentile");
    }

    cv::Mat hist;
    std::vector<cv::Mat> mats = { image };
    cv::calcHist(mats, {0}, cv::Mat(), hist, {256}, {0, 256}, false);

    float total=image.total()*percentile;
    float c=0;
    for(int i=255;i>0;i--){
        c+=hist.at<float>(i);
        if(c>total)	return i;
    }
    return 0;
}

cv::Mat Percentile::processImage(const cv::Mat& image)
{
    const auto percentile{getParameters().getParameterValue<double>("percentile")};
    auto thresholdValue{calculatePercentile(image, percentile)};

    cv::Mat outputImage;
    cv::threshold(image, outputImage, thresholdValue, 255, cv::THRESH_BINARY);

    setDebugImage(outputImage);
    return outputImage;
}
