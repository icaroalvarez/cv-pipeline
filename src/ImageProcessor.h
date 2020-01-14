#pragma once

#include <unordered_map>
#include <any>
#include <opencv2/core/mat.hpp>
#include "ProcessorConfiguration.h"

class ImageProcessor
{
public:
    ImageProcessor(std::string name);

    ProcessorConfiguration& getConfiguration();

    virtual void processImage(const cv::Mat &image) = 0;

    cv::Mat getPostProcessedImage() const;
    void setPostProcessedImage(cv::Mat image);

    cv::Mat getDebugImage() const;
    void setDebugImage(cv::Mat image);

    std::string getName() const;

private:
    std::string name;
    ProcessorConfiguration configuration;
    cv::Mat postProcessedImage, debugImage;
};
