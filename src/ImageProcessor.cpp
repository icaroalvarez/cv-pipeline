#include "ImageProcessor.h"

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
