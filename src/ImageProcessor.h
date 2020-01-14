#pragma once

#include <unordered_map>
#include <any>
#include <opencv2/core/mat.hpp>
#include "ProcessorConfiguration.h"

class ImageProcessor
{
public:
    ImageProcessor(std::string name);

    /**
     * Get the current processor configuration
     * @return the configuration
     */
    ProcessorConfiguration& getConfiguration();

    /**
     * Implement this method when creating new image processors
     * @param image the input image to be processed
     */
    virtual void processImage(const cv::Mat &image) = 0;

    /**
     * Get the image generated after processing the image
     * @return the image processed
     */
    cv::Mat getPostProcessedImage() const;

    /**
     * Set the image generated after processing the image
     * @param image the image processed
     */
    void setPostProcessedImage(cv::Mat image);

    /**
     * Get the debug image generated while processing the image
     * @return the image processed
     */
    cv::Mat getDebugImage() const;

    /**
     * Set the debug image generated while processing the image
     * @param image the image processed
     */
    void setDebugImage(cv::Mat image);

    /**
     * Get the name of the processor
     * @return the name of the processor
     */
    std::string getName() const;

private:
    std::string name;
    ProcessorConfiguration configuration;
    cv::Mat postProcessedImage, debugImage;
};
