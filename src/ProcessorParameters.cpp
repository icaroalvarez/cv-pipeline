#include "ProcessorParameters.h"

Parameters ProcessorParameters::getParameters() const
{
    return parameters;
}

void throwIfNotFoundParameters(const Configuration &configuration, const Parameters& parameters)
{
    std::string notFoundParameters;
    for(const auto& parameter : configuration)
    {
        const auto& parameterName{parameter.first};
        if(parameters.find(parameterName) == parameters.end())
        {
            notFoundParameters.append(parameterName+", ");
        }
    }
    if(not notFoundParameters.empty())
    {
        notFoundParameters.pop_back();
        notFoundParameters.pop_back();
        throw std::invalid_argument("Couldn't configure image processor. Parameters not found: "+notFoundParameters);
    }
}

// overloaded helper from std::variant documentation
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>; // not needed as of C++20
void ProcessorParameters::configure(const Configuration &configuration)
{
    throwIfNotFoundParameters(configuration, parameters);
    std::string wrongTypeParameters;
    for(const auto& parameter : configuration)
    {
        const auto& parameterName{parameter.first};
        auto &parameterToBeConfigured{parameters[parameterName]};

        std::visit(overloaded{
                [&](int value)
                {
                    auto integerParameter{std::get_if<IntegerParameter>(&parameterToBeConfigured)};
                    if(integerParameter)
                    {
                        integerParameter->value = value;
                    }else{
                        wrongTypeParameters.append(parameterName+", ");
                    }
                },
                [&](double value)
                {
                        auto decimalParameter{std::get_if<DecimalParameter>(&parameterToBeConfigured)};
                        if(decimalParameter)
                        {
                            decimalParameter->value = value;
                        }else{
                            wrongTypeParameters.append(parameterName+", ");
                        }
                },
                [&](bool value)
                {
                    auto booleanParameter{std::get_if<bool>(&parameterToBeConfigured)};
                    if(booleanParameter)
                    {
                        *booleanParameter = value;
                    }else{
                        wrongTypeParameters.append(parameterName+", ");
                    }
                },
                [&](std::size_t value)
                {
                    auto optionParameter{std::get_if<OptionsParameter>(&parameterToBeConfigured)};
                    if(optionParameter)
                    {
                        optionParameter->selectedOptionIndex = value;
                    }else{
                        wrongTypeParameters.append(parameterName+", ");
                    }
                }
        }, parameter.second);
    }

    if(not wrongTypeParameters.empty())
    {
        wrongTypeParameters.pop_back();
        wrongTypeParameters.pop_back();
        throw std::invalid_argument("Couldn't configure image processor. Wrong type in parameters: "+wrongTypeParameters);
    }
}

template<>
int ProcessorParameters::getParameterValue<int>(const std::string &parameterName)
{
    return std::get<IntegerParameter>(parameters[parameterName]).value;
}

template<>
double ProcessorParameters::getParameterValue<double>(const std::string &parameterName)
{
    return std::get<DecimalParameter>(parameters[parameterName]).value;
}

template<>
std::size_t ProcessorParameters::getParameterValue<std::size_t>(const std::string &parameterName)
{
    return std::get<OptionsParameter>(parameters[parameterName]).selectedOptionIndex;
}