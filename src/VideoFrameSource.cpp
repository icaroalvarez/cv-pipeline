#include "VideoFrameSource.h"

void VideoFrameSource::loadFrom(const std::string& path)
{
    this->path = path;

    if(!videoCapture.open(path))
    {
        throw std::invalid_argument("Video frame source not able to open video at: "+path);
    }

    totalFrames = static_cast<int>(videoCapture.get(cv::CAP_PROP_FRAME_COUNT));

    //Fix corrupted codec bug
    if(totalFrames <= 0)
    {
        totalFrames = 0;
        while (videoCapture.grab()){
            ++totalFrames;
        }
        videoCapture.release();
        videoCapture.open(path);
    }
}

unsigned int VideoFrameSource::framesCount() const
{
    return totalFrames;
}

cv::Mat VideoFrameSource::getFrameFromIndex(unsigned int index)
{
    if(index >= totalFrames)
    {
        throw std::invalid_argument("Unable to get frame from index "+std::to_string(index)
        +". Index is out of bound (max index is "+std::to_string(totalFrames-1)+")");
    }
    const auto setIndexNecessary{(index not_eq 0) and (prevFrameIndex not_eq (index-1))};
    prevFrameIndex = index;
    if(setIndexNecessary)
    {
        videoCapture.set(cv::CAP_PROP_POS_FRAMES, index);
    }

    cv::Mat image;
    if(!videoCapture.read(image))
    {
        throw std::runtime_error("Error while reading image from: "+path);
    }
    return image;
}
