#pragma once

#include <unordered_map>
#include <any>
#include <opencv2/core/mat.hpp>
#include "ProcessorParameters.h"

/**
 * Responsibility: process an image.
 * Base class for creating custom image processing implementations.
 * Processor configuration is stored in this class.
 */
class ImageProcessor
{
public:
    explicit ImageProcessor(std::string name);

    /**
     * Get the current processor configuration
     * @return the configuration
     */
    ProcessorParameters& getConfiguration();

    /**
     * Implement this method when creating new image processors
     * @param image the input image to be processed
     */
    virtual cv::Mat processImage(const cv::Mat &image) = 0;

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
    ProcessorParameters configuration;
    cv::Mat debugImage;
};
