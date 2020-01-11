#include <opencv2/imgcodecs.hpp>
#include "PipelineController.h"

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

void PipelineController::loadImage(const std::string &path)
{
    currentImage = cv::imread(path);
    if(!currentImage.data)
    {
        throw std::invalid_argument("Couldn't load frame source. Non path found: "+path);
    }
}

void PipelineController::configureProcessor(unsigned int index,
                                            const Configuration& configuration)
{
    if(index >= imageProcessors.size())
    {
        throw std::invalid_argument("Couldn't configure the image processor. The index "
                                    +std::to_string(index)+" is out of range ("
                                    +std::to_string(imageProcessors.size())+")");
    }

    imageProcessors[index]->configure(configuration);
}
