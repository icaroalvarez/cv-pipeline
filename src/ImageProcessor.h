#pragma once
#include <unordered_map>
#include <any>
#include <opencv2/core/mat.hpp>

using Configuration = std::unordered_map<std::string, int>;

class ImageProcessor
{
public:
    void configure(const Configuration& configuration);
    Configuration& getConfiguration();
    virtual void processImage(cv::Mat image) = 0;
    cv::Mat getPreProcessedImage() const;
    void setPreProcessedImage(cv::Mat image);
    cv::Mat getPostProcessedImage() const;
    void setPostProcessedImage(cv::Mat image);
    cv::Mat getDebugImage() const;
    void setDebugImage(cv::Mat image);

private:
    Configuration configuration;
    cv::Mat preProcessedImage, postProcessedImage, debugImage;
};
