#include "ImageProcessor.h"
#include <utility>

void ImageProcessor::configure(const Configuration& configuration)
{
    std::string parameterNotFoundNames;
    for (const auto& parameter : configuration)
    {
        const auto iterator{this->configuration.find(parameter.first)};
        if(iterator == this->configuration.end())
        {
            parameterNotFoundNames.append(parameter.first+", ");
            continue;
        }
        this->configuration[parameter.first] = parameter.second;
    }
    if(!parameterNotFoundNames.empty())
    {
        parameterNotFoundNames.pop_back();
        parameterNotFoundNames.pop_back();
        throw std::invalid_argument("Couldn't configure image processor. Parameters not found: "+parameterNotFoundNames);
    }
}

Configuration& ImageProcessor::getConfiguration()
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
