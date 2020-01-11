#pragma once
#include <unordered_map>
#include <any>

using Configuration = std::unordered_map<std::string, int>;

class ImageProcessor
{
public:
    void configure(const Configuration& configuration);
    Configuration& getConfiguration();

private:
    Configuration configuration;
};
