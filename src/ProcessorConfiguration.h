#ifndef PLAYGROUND_PROCESSORCONFIGURATION_H
#define PLAYGROUND_PROCESSORCONFIGURATION_H

#include <string>
#include "nlohmann/json.hpp"

class ProcessorConfiguration {
public:

    /**
     * @brief Add integer parameter to configuration
     * @param name parameter name
     * @param value parameter value
     * @param minValue min parameter value
     * @param maxValue max parameter value
     */
    void addIntParameter(const std::string &name, int value, int minValue, int maxValue);

    /**
     * @brief Add float parameter to configuration
     * @param name parameter name
     * @param value parameter value
     * @param minValue min parameter value
     * @param maxValue max parameter value
     * @param step step value
     * @param decimals number of decimals
     */
    void addFloatParameter(const std::string &name, float value, float minValue, float maxValue, float step, int decimals);

    /**
     * @brief Add boolean parameter to configuration
     * @param name parameter name
     * @param value parameter value
     */
    void addBooleanParameter(const std::string&name, bool value);

    /**
     * @brief Add options paremeter type to configuration
     * @param name parameter name
     * @param options string vector containing the options
     * @param selected selected option index
     */
    void addOptionsParameter(const std::string& name, std::vector<std::string> options, int selected);

    /**
     * @brief get configuration in json format
     * @return configuration in json format
     */
    nlohmann::json getConfiguration() const;

    /**
     * @brief configure parameter values from json file
     * @param configuration json file
     */
    void configure(const nlohmann::json& configuration);

    /**
     * @brief get integer parameter value
     * @param name parameter name
     * @return integer value
     */
    int getIntParameter(const std::string& name);

    /**
     * @brief get float parameter value
     * @param name parameter name
     * @return float value
     */
    float getFloatParameter(const std::string& name);

    /**
     * @brief get boolean parameter
     * @param name parameter name
     * @return boolean value
     */
    bool getBooleanParameter(const std::string& name);

    /**
     * @brief get options parameter
     * @param name parameter name
     * @return option value
     */
    std::string getOptionsParameter(const std::string &name);

    /**
     * @brief get option parameter index
     * @param name parameter name
     * @return option index
     */
    int getOptionsParameterIndex(const std::string &name);

private:
    nlohmann::json configuration;
};


#endif //PLAYGROUND_PROCESSORCONFIGURATION_H
