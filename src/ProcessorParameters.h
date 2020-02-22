#pragma once
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>

struct IntegerParameter{
    int value;
    int minValue;
    int maxValue;
};

struct DecimalParameter{
    double value;
    double minValue;
    double maxValue;
    double incrementalStep;
    unsigned int decimalsPlaces;
};

struct OptionsParameter
{
    std::size_t selectedOptionIndex;
    std::vector<std::string> options;
};

using Configuration = std::unordered_map<std::string, std::variant<int, double, bool, std::size_t>>;
using Parameters = std::unordered_map<std::string, std::variant<IntegerParameter,
        DecimalParameter, bool, OptionsParameter>>;

/**
 * Responsibility: to manage the configuration of an image processor.
 * Parameters can be added by type: int, float, boolean and options.
 * Parameters can be configured using json format.
 * Current configuration in json format can be retrieve.
 */
class ProcessorParameters {
public:

    template <typename  T>
    void registerParameter(const std::string& name, T parameter){parameters[name]=parameter;};

    Parameters getParameters() const;

    void configure(const Configuration& configuration);

    template<typename T>
    T getParameterValue(const std::string& parameterName){return std::get<T>(parameters[parameterName]);};

private:
    Parameters parameters;
};

template<>
int ProcessorParameters::getParameterValue<int>(const std::string &parameterName);

template<>
double ProcessorParameters::getParameterValue<double>(const std::string &parameterName);

template<>
std::size_t ProcessorParameters::getParameterValue<std::size_t>(const std::string &parameterName);
