#include "catch2/catch.hpp"
#include "JsonStorage.h"
#include <fstream>

namespace
{
    nlohmann::json createProcessorConfiguration()
    {
        return {
                {
                        {"parameter_name", "parameter_boolean"},
                        {"parameter_type", "boolean"},
                        {"value", true}
                },
                {
                        {"parameter_name", "parameter_options"},
                        {"parameter_type", "options"},
                        {"value", 1}
                },
                {
                        {"parameter_name", "parameter_decimal"},
                        {"parameter_type", "decimal"},
                        {"value", 1.0}
                },
                {
                        {"parameter_name", "parameter_integer"},
                        {"parameter_type", "integer"},
                        {"value", 1}
                }
        };
    }

    nlohmann::json createProcessor2Configuration()
    {
        return {
                {
                        {"parameter_name", "parameter_integer"},
                        {"parameter_type", "integer"},
                        {"value", 2}
                }
        };
    }

    nlohmann::json createImageProcessorsConfiguration()
    {
        return {
                {
                        {"name", "processor1"},
                        {"configuration", createProcessorConfiguration()}
                },
                {
                        {"name", "processor2"},
                        {"configuration", createProcessor2Configuration()}
                }
        };
    }

    nlohmann::json createPipelineConfigurationJson()
    {
        return {
                {"frame_source_path", "frame_source_path"},
                {"image_processors",  createImageProcessorsConfiguration()}
        };
    }

    Configuration createConfigurationProcessor1()
    {
        return {
                {"parameter_integer", 1},
                {"parameter_decimal", 1.0},
                {"parameter_boolean", true},
                {"parameter_options", std::size_t{1}}
        };
    }

    Configuration createConfigurationProcessor2()
    {
        return {
                {"parameter_integer", 2}
        };
    }

    PipelineConfiguration createExpectedConfiguration()
    {
        return {"frame_source_path",
                {
                        {"processor1", createConfigurationProcessor1()},
                        {"processor2", createConfigurationProcessor2()}
                }
        };
    }

    TEST_CASE("Load pipeline configuration from json")
    {
        PipelineConfiguration configuration;
        REQUIRE_NOTHROW(
                configuration = JsonStorage::createPipelineConfigurationFromJson(createPipelineConfigurationJson()));
        REQUIRE(configuration == createExpectedConfiguration());
    }

    TEST_CASE("Load pipeline configuration from json file")
    {
        constexpr auto path{fixtures_path"json/configuration.json"};
        PipelineConfiguration configuration;
        REQUIRE_NOTHROW(configuration = JsonStorage::createPipelineConfigurationFromJsonFile(path));
        REQUIRE(configuration == createExpectedConfiguration());
    }

    nlohmann::json loadJsonFromDisk(const std::string &path)
    {
        nlohmann::json jsonFile;
        std::ifstream file(path);
        file >> jsonFile;
        return jsonFile;
    }

    SCENARIO("Saving pipeline configuration to json file")
    {
        GIVEN("A pipeline configuration")
        {
            const auto configuration{createExpectedConfiguration()};
            AND_WHEN("saving configuration is requested")
            {
                const auto outputPath{"/tmp/configuration.json"};
                JsonStorage::saveProcessorConfigurationTo(configuration, outputPath);

                THEN("configuration is saved")
                {
                    constexpr auto expectedConfigurationPath{fixtures_path"json/configuration.json"};
                    const auto expectedConfiguration{loadJsonFromDisk(expectedConfigurationPath)};
                    const auto configurationSaved{loadJsonFromDisk(outputPath)};
                    REQUIRE(expectedConfiguration == configurationSaved);
                }
            }
        }
    }
}