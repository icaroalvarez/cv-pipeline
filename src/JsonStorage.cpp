#include <fstream>
#include <iomanip>
#include "JsonStorage.h"

namespace JsonStorage
{
    constexpr auto frameSourcePathKey{"frame_source_path"};
    constexpr auto imageProcessorsKey{"image_processors"};
    constexpr auto configurationKey{"configuration"};
    constexpr auto parameterNameKey{"parameter_name"};
    constexpr auto parameterTypeKey{"parameter_type"};
    constexpr auto parameterValueKey{"value"};
    constexpr auto processorNameKey{"name"};
    constexpr auto integerTypeKey{"integer"};
    constexpr auto decimalTypeKey{"decimal"};
    constexpr auto booleanTypeKey{"boolean"};
    constexpr auto optionsTypeKey{"options"};

    PipelineConfiguration createPipelineConfigurationFromJsonFile(const std::string& path)
    {
        std::ifstream file(path);
        if(!file.is_open())
        {
            throw std::invalid_argument("Error opening file: "+path);
        }
        nlohmann::json configurationJson;
        file >> configurationJson;

        return createPipelineConfigurationFromJson(configurationJson);
    }

    PipelineConfiguration createPipelineConfigurationFromJson(const nlohmann::json& json)
    {
        PipelineConfiguration configuration;
        configuration.frameSourcePath = json.at(frameSourcePathKey);

        for(const auto& processor : json.at(imageProcessorsKey))
        {
            Configuration processorConfiguration;
            for(const auto& parameter: processor.at(configurationKey))
            {
                const auto parameterKey{static_cast<std::string>(parameter.at(parameterTypeKey))};
                const auto parameterName{static_cast<std::string>(parameter.at(parameterNameKey))};
                if(parameterKey == integerTypeKey)
                {
                    processorConfiguration[parameterName] =
                            static_cast<int>(parameter.at(parameterValueKey));
                }else if(parameterKey == decimalTypeKey)
                {
                    processorConfiguration[parameterName] =
                            static_cast<double>(parameter.at(parameterValueKey));
                }
                else if(parameterKey == booleanTypeKey)
                {
                    processorConfiguration[parameterName] =
                            static_cast<bool>(parameter.at(parameterValueKey));
                }else if(parameterKey == optionsTypeKey)
                {
                    processorConfiguration[parameterName] =
                            static_cast<std::size_t>(parameter.at(parameterValueKey));
                }else{
                    throw std::runtime_error("Error parsing json file. Unknown parameter type:" + parameterName);
                }
            }
            configuration.imageProcessors.emplace_back(processor.at(processorNameKey), std::move(processorConfiguration));
        }
        return configuration;
    }

    using json = nlohmann::json;

// overloaded helper from std::variant documentation
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>; // not needed as of C++20
    void saveProcessorConfigurationTo(const PipelineConfiguration& configuration, const std::string& path)
    {
        json outputJson;
        outputJson[frameSourcePathKey] = configuration.frameSourcePath;
        json imageProcessorsJson;
        for(const auto& processor : configuration.imageProcessors)
        {
            json processorJson;
            processorJson[processorNameKey] = std::get<0>(processor);

            json configurationJson;
            for(const auto& parameter : std::get<1>(processor))
            {
                json parameterJson;
                parameterJson[parameterNameKey] = parameter.first;

                std::visit(overloaded{
                        [&](int value)
                        {
                            parameterJson[parameterValueKey] = value;
                            parameterJson[parameterTypeKey] = integerTypeKey;
                        },
                        [&](double value)
                        {
                            parameterJson[parameterValueKey] = value;
                            parameterJson[parameterTypeKey] = decimalTypeKey;
                        },
                        [&](bool value)
                        {
                            parameterJson[parameterValueKey] = value;
                            parameterJson[parameterTypeKey] = booleanTypeKey;
                        },
                        [&](SelectedOptionIndex value)
                        {
                            parameterJson[parameterValueKey] = value;
                            parameterJson[parameterTypeKey] = optionsTypeKey;
                        }
                }, parameter.second);
                configurationJson.emplace_back(std::move(parameterJson));
            }
            processorJson[configurationKey] = std::move(configurationJson);
            imageProcessorsJson.emplace_back(std::move(processorJson));
        }
        outputJson[imageProcessorsKey] = std::move(imageProcessorsJson);

        std::ofstream output{path};
        output << std::setw(4) << outputJson << std::endl;
    }

}