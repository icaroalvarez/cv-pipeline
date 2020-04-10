#pragma once
#include "FrameSource.h"
#include <opencv2/videoio.hpp>

class VideoFrameSource: public FrameSource
{
public:
    void loadFrom(const std::string& path) override;

    unsigned int framesCount() const override;

    cv::Mat getFrameFromIndex(unsigned int index) override;

private:
    cv::VideoCapture videoCapture;
    unsigned int totalFrames{0};
    unsigned int prevFrameIndex{0};
    std::string path;
    cv::Mat lastLoadedFrame;
};



