#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(std::string name)
:name{std::move(name)}
{
}

ProcessorConfiguration& ImageProcessor::getConfiguration()
{
    return configuration;
}

cv::Mat ImageProcessor::getPreProcessedImage() const {
    return preProcessedImage;
}

void ImageProcessor::setPreProcessedImage(cv::Mat image)
{
    preProcessedImage = std::move(image);
}

cv::Mat ImageProcessor::getPostProcessedImage() const
{
    return postProcessedImage;
}

void ImageProcessor::setPostProcessedImage(cv::Mat image)
{
    postProcessedImage = std::move(image);
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
