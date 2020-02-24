#pragma once
#include <opencv2/core/mat.hpp>

class ImageFrameSource
{
public:
    void loadFrom(const std::string& path);

    unsigned int framesCount();

    cv::Mat getFrameFromIndex(unsigned int index);

private:
    cv::Mat image;
};



