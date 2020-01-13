//
// Created by ialvarez on 20/02/18.
//

#include "ProcessorConfiguration.h"

using json = nlohmann::json;

void ProcessorConfiguration::addIntParameter(const std::string &name, int value, int minValue, int maxValue)
{
    configuration.emplace_back(json({
                    {"name", name},
                    {"type", "integer"},
                    {"value", value},
                    {"max_value", maxValue},
                    {"min_value", minValue}
            }));
}

void ProcessorConfiguration::addFloatParameter(const std::string &name, float value, float minValue,
                                               float maxValue, float step, int decimals) {

    configuration.emplace_back(json({
                                       {"name", name},
                                       {"type", "float"},
                                       {"value", value},
                                       {"max_value", maxValue},
                                       {"min_value", minValue},
                                       {"step", step},
                                       {"decimals", decimals}
                               }));

}

void ProcessorConfiguration::addBooleanParameter(const std::string &name, bool value) {
    configuration.emplace_back(json({
                                       {"name", name},
                                       {"type", "boolean"},
                                       {"value", value}
                               }));
}

void ProcessorConfiguration::addOptionsParameter(const std::string& name,
        std::vector<std::string> options, int selected)
{
    configuration.emplace_back(json({
            {"name", name},
            {"type", "options"},
            {"value", selected},
            {"options", json(options)}
    }));
}

nlohmann::json ProcessorConfiguration::getConfiguration() const {
    return configuration;
}

void ProcessorConfiguration::configure(const nlohmann::json &parameters)
{
    if(!parameters.is_array())
    {
        throw std::invalid_argument("Configuration parameters is not an array of parameter objects");
    }

    for(auto& parameter: parameters)
    {
        std::string paramName = parameter["name"];

        // search for the parameter to update
        for(auto& configurationParameter: configuration)
        {
            if(configurationParameter["name"] == paramName)
            {
                configurationParameter["value"] = parameter["value"];
            }
        }
    }
}

int ProcessorConfiguration::getIntParameter(const std::string &name) {
    for(auto& parameter: configuration)
    {
        if(parameter["name"] == name)
        {
            return parameter["value"];
        }
    }
}

float ProcessorConfiguration::getFloatParameter(const std::string &name) {
    for(auto& parameter: configuration)
    {
        if(parameter["name"] == name)
        {
            return parameter["value"];
        }
    }
}

bool ProcessorConfiguration::getBooleanParameter(const std::string &name) {
    for(auto& parameter: configuration)
    {
        if(parameter["name"] == name)
        {
            return parameter["value"];
        }
    }
}

std::string ProcessorConfiguration::getOptionsParameter(const std::string &name) {
    for(auto& parameter: configuration)
    {
        if(parameter["name"] == name)
        {
            int index = parameter["value"];
            json values = parameter["options"];
            return values.at(index);
        }
    }
}
int ProcessorConfiguration::getOptionsParameterIndex(const std::string& name)
{
    for(auto& parameter: configuration)
    {
        if(parameter["name"] == name)
        {
            return parameter["value"];
        }
    }
}
