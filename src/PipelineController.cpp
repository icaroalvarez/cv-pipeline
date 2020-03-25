#include <opencv2/imgcodecs.hpp>
#include <thread>
#include "PipelineController.h"
#include "FrameSourceFactory.h"

void PipelineController::loadPipeline(const std::vector<std::string>& imageProcessorNames)
{
    std::string notRegisteredNames;
    for(const auto& name : imageProcessorNames)
    {
        try
        {
            imageProcessors.emplace_back(imageProcessorFactory.make(name));
        }catch(const std::invalid_argument&)
        {
            notRegisteredNames.append(name+", ");
        }
    }
    if(notRegisteredNames.size() > 2)
    {
        notRegisteredNames.pop_back();
        notRegisteredNames.pop_back();
        throw(std::invalid_argument("Couldn't load pipeline, processors not found: "+notRegisteredNames));
    }
}

std::vector<std::string> PipelineController::getPipelineDescription() const
{
    std::vector<std::string> processorNames;
    for (const auto &processor : imageProcessors)
    {
        processorNames.push_back(processor->getName());
    }
    return processorNames;
}

void PipelineController::loadFrameSourceFrom(const std::string &path)
{
    frameSource = std::move(FrameSourceFactory::createAndLoadFromPath(path));
    frameSourceIndex = 0;
}

void checkImageProcessorRange(const std::vector<std::unique_ptr<ImageProcessor>>& imageProcessors,
                              unsigned int index,
                              const std::string& exceptionMessage)
{
    if(index >= imageProcessors.size())
    {
        throw std::invalid_argument(exceptionMessage+"The index "
                                    +std::to_string(index)+" is out of range ("
                                    +std::to_string(imageProcessors.size())+")");
    }
}

void PipelineController::configureProcessor(unsigned int index,
                                            const Configuration& configuration)
{
    checkImageProcessorRange(imageProcessors, index, "Couldn't configure the image processor. ");
    imageProcessors[index]->getParameters().configure(configuration);
}

void PipelineController::firePipelineProcessing()
{
    fireIteration();
}

void PipelineController::addImageProcessor(std::unique_ptr<ImageProcessor> imageProcessor)
{
    imageProcessors.emplace_back(std::move(imageProcessor));
}

cv::Mat PipelineController::getCurrentLoadedImage()
{
    return currentImage;
}

cv::Mat PipelineController::getDebugImage(unsigned int processorIndex)
{
    checkImageProcessorRange(imageProcessors, processorIndex, "Couldn't debug image. ");
    return imageProcessors[processorIndex]->getDebugImage();
}

Parameters PipelineController::getProcessorParameters(unsigned int processorIndex) const {
    checkImageProcessorRange(imageProcessors, processorIndex, "Couldn't get image processor parameters. ");
    return imageProcessors[processorIndex]->getParameters().getParameters();
}

void PipelineController::runIteration()
{
    currentImage = frameSource->getFrameFromIndex(frameSourceIndex);
    cv::Mat imageFromPreviousProcessor{currentImage};
    for(const auto& processor: imageProcessors)
    {
        imageFromPreviousProcessor = processor->processImage(imageFromPreviousProcessor);
    }
    notifyObservers();
}

void PipelineController::setFrameSourceIndex(unsigned index)
{
    if(not frameSource)
    {
        throw std::runtime_error("Frame source is not loaded yet, index cannot be set");
    }
    const auto framesCount{frameSource->framesCount()};
    if(index >= framesCount)
    {
        const auto maxIndex{std::max(static_cast<int>(framesCount-1), 0)};
        throw std::invalid_argument("Frame source index out of bound (requested: "+
                                    std::to_string(index)+", max: "+std::to_string(maxIndex)+")");
    }
    frameSourceIndex = index;
}

void PipelineController::loadPipelineFromJson(const nlohmann::json &configurationFile)
{
    const std::string imageSourcePath{configurationFile.at("input_image_path")};
    loadFrameSourceFrom(imageSourcePath);
    const std::vector<std::string> imageProcessorNames = configurationFile.at("image_processors_to_be_loaded");
    loadPipeline(imageProcessorNames);
}