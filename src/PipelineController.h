#pragma once
#include "ImageProcessor.h"
#include "Factory.h"
#include "Notifier.h"
#include "Runnable.h"
#include <any>
#include <opencv2/core/mat.hpp>

/**
 * Responsibility: allows to control the pipeline from the outside (a GUI for example).
 * Image processors can be registered and loaded into the pipeline
 * An image can be loaded and processed through the image processors loaded into the pipeline.
 * The image processors loaded into the pipeline can be configured.
 * Post-processed and debug images of each image processor can be retrieved. *
 */
class PipelineController: public Notifier, Runnable
{
public:

    /**
     * Load a set of image processors into the pipeline
     * @param imageProcessorNames the names of the image processors to be loaded
     */
    void loadPipeline(const std::vector<std::string>& imageProcessorNames);

    /**
     * Get a list of image processors loaded in the pipeline
     * @return names of the image processors loaded
     */
    std::vector<std::string> getPipelineDescription() const;

    /**
     * Load the image to be processed through the pipeline
     * @param path of the image
     */
    void loadImage(const std::string& path);

    /**
     * Configure the parameters of an image processor
     * @param index the index of the processor in the pipeline
     * @param configuration the parameters to be configured in json format
     */
    void configureProcessor(unsigned int index, const Configuration& configuration);

    /**
     * Get the parameters of an image processor
     * @param processorIndex index of the processor to get the parameters
     * @return the configuration parameters of the image processor
     */
    Parameters getProcessorConfigurationFrom(unsigned int processorIndex) const;

    /**
     * Start the loaded image processing through the pipeline
     */
    void processCurrentImage();

    /**
     * Add an image processor to the back of the pipeline
     * @param imageProcessor the processor to be added
     */
    void addImageProcessor(std::unique_ptr<ImageProcessor> imageProcessor);

    /**
     * Get loaded image
     * @return loaded image
     */
    cv::Mat getCurrentLoadedImage();

    /**
     * Get processed image of a specific image processor
     * @param processorIndex the index of the processor in the pipeline
     * @return the image processed
     */
    cv::Mat getPostProcessedImage(unsigned int processorIndex);

    /**
     * Get a debug image generated by a specific image processor
     * @param processorIndex the index of the processor in the pipeline
     * @return
     */
    cv::Mat getDebugImage(unsigned int processorIndex);

    /**
     * Register an image processor in order to used when creating pipelines by name
     * @tparam T Processor type
     * @param processorName name to register the processor
     */
    template <typename T>
    void registerImageProcessor(const std::string& processorName)
    {
        imageProcessorFactory.registerMaker<T>(processorName);
    }

private:
    std::vector<std::unique_ptr<ImageProcessor>> imageProcessors;
    Factory<ImageProcessor> imageProcessorFactory;
    cv::Mat currentImage;

    void runIteration() override;
};