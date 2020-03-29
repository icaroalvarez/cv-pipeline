#pragma once
#include <string>
#include <vector>
#include "ProcessorParameters.h"

struct PipelineConfiguration
{
    std::string frameSourcePath;
    std::vector<std::tuple<std::string, Configuration>> imageProcessors;

    bool operator==(const PipelineConfiguration& rightHandSide) const
    {
        return frameSourcePath == rightHandSide.frameSourcePath &&
               imageProcessors == rightHandSide.imageProcessors;
    }
};