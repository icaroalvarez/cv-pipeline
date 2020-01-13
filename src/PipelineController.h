#pragma once
#include "ImageProcessor.h"
#include "Factory.h"
#include "Notifier.h"
#include <any>
#include <opencv2/core/mat.hpp>

class PipelineController: public Notifier
{
public:

    void loadPipeline(const std::vector<std::string>& imageProcessorNames);

    void loadImage(const std::string& path);

    void configureProcessor(unsigned int index, const nlohmann::json& configuration);

    nlohmann::json getProcessorConfigurationFrom(unsigned int processorIndex) const;

    void processCurrentImage();

    void addImageProcessor(std::unique_ptr<ImageProcessor> imageProcessor);

    cv::Mat getCurrentLoadedImage();

    cv::Mat getPreProcessedImage(unsigned int processorIndex);

    cv::Mat getPostProcessedImage(unsigned int processorIndex);

    cv::Mat getDebugImage(unsigned int processorIndex);

    template <typename T>
    void registerImageProcessor(const std::string& processorName)
    {
        imageProcessorFactory.registerMaker<T>(processorName);
    }

private:
    std::vector<std::unique_ptr<ImageProcessor>> imageProcessors;
    Factory<ImageProcessor> imageProcessorFactory;
    cv::Mat currentImage;
};