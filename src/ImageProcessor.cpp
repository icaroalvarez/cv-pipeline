#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(std::string name)
:name{std::move(name)}
{
}

ProcessorParameters& ImageProcessor::getParameters()
{
    return parameters;
}

cv::Mat ImageProcessor::getDebugImage() const {
    return debugImage;
}

void ImageProcessor::setDebugImage(cv::Mat image)
{
    debugImage = std::move(image);
}

std::string ImageProcessor::getName() const {
    return name;
}
