//
// Created by ialvarez on 9/03/18.
//

#include <opencv2/imgproc.hpp>
#include "AdaptiveThreshold.h"

AdaptiveThreshold::AdaptiveThreshold()
:ImageProcessor("adaptive_threshold")
{
    getConfiguration().addIntParameter("block_size", 10, 1, 999);
    getConfiguration().addIntParameter("c_mean", -15, -999, 999);
    getConfiguration().addOptionsParameter("adaptive_method", {"mean", "gaussian"}, 0);
}

int makeOdd(int value)
{
    if(value % 2 == 0) {
        return value+1;
    }
    return value;
}

void AdaptiveThreshold::processImage(const cv::Mat& image)
{
    if(image.channels() > 1)
    {
        throw std::invalid_argument("input image has more than 1 channel");
    }

    int blockSize = makeOdd(getConfiguration().getIntParameter("block_size"));
    float cMean = getConfiguration().getIntParameter("c_mean");

    int option = cv::ADAPTIVE_THRESH_GAUSSIAN_C;
    if(getConfiguration().getOptionsParameter("adaptive_method") == "mean")
    {
        option = cv::ADAPTIVE_THRESH_MEAN_C;
    }
    cv::Mat outputImage;
    cv::adaptiveThreshold(image, outputImage, 255, option,
            cv::THRESH_BINARY, blockSize, cMean);

    setDebugImage(outputImage);
    setPostProcessedImage(outputImage);
}
