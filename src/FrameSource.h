#pragma once
#include <string>
#include <opencv2/core/mat.hpp>

class FrameSource
{
public:
    virtual void loadFrom(const std::string &path) = 0;

    virtual unsigned int framesCount() const = 0;

    virtual cv::Mat getFrameFromIndex(unsigned int index) = 0;
};