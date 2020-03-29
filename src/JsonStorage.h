#pragma once
#include "PipelineConfiguration.h"
#include "nlohmann/json.hpp"

namespace JsonStorage
{
    PipelineConfiguration createPipelineConfigurationFromJson(const nlohmann::json& json);
    PipelineConfiguration createPipelineConfigurationFromJsonFile(const std::string& path);
    void saveProcessorConfigurationTo(const PipelineConfiguration& configuration, const std::string& path);
}



