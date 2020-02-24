#include "ImageFrameSource.h"
#include <opencv2/imgcodecs.hpp>

void ImageFrameSource::loadFrom(const std::string &path)
{
    image = cv::imread(path);
    if(image.empty())
    {
        throw std::invalid_argument("Unable to load image from: "+path);
    }
}

unsigned int ImageFrameSource::framesCount()
{
    if(image.empty())
    {
        return 0;
    }
    return 1;
}

cv::Mat ImageFrameSource::getFrameFromIndex(unsigned int index)
{
    if(index >= framesCount())
    {
        throw std::invalid_argument("Unable to get frame from index "+std::to_string(index)+
                                    ". Index is out of bound (max index is "+std::to_string(framesCount()-1)+")");
    }
    return image;
}
