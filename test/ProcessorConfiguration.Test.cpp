#include <catch2/catch.hpp>
#include "ProcessorConfiguration.h"

nlohmann::json createReferenceJsonParameterConfiguration()
{
    return nlohmann::json{
            {
                    {
                            {"name", "int_parameter"},
                            {"type", "integer"},
                            {"value", 1},
                            {"min_value", 0},
                            {"max_value", 10}
                    },
                    {
                            {"name", "float_parameter"},
                            {"type", "float"},
                            {"value", 1.100000023841858},
                            {"min_value", 0.10000000149011612},
                            {"max_value", 10.899999618530273},
                            {"step", 0.20000000298023224},
                            {"decimals", 2}
                    },
                    {
                            {"name", "boolean_parameter"},
                            {"type", "boolean"},
                            {"value", true}
                    },
                    {
                            {"name", "options_parameter"},
                            {"type", "options"},
                            {"value", 0},
                            {"options", {"option1", "option2"}}
                    }
            }
    };
}

nlohmann::json createConfiguration2()
{
    return nlohmann::json{
            {
                    {"name", "int_parameter"},
                    {"value", 2}
            },
            {
                    {"name", "float_parameter"},
                    {"value", 1.5}
            },
            {
                    {"name", "boolean_parameter"},
                    {"value", false}
            },
            {
                    {"name", "options_parameter"},
                    {"value", 1},
            }
    };
}

nlohmann::json createReferenceJsonParameterConfiguration2()
{
    return nlohmann::json{

            {
                    {"name", "int_parameter"},
                    {"type", "integer"},
                    {"value", 2},
                    {"min_value", 0},
                    {"max_value", 10}
            },
            {
                    {"name", "float_parameter"},
                    {"type", "float"},
                    {"value", 1.5},
                    {"min_value", 0.10000000149011612},
                    {"max_value", 10.899999618530273},
                    {"step", 0.20000000298023224},
                    {"decimals", 2}
            },
            {
                    {"name", "boolean_parameter"},
                    {"type", "boolean"},
                    {"value", false}
            },
            {
                    {"name", "options_parameter"},
                    {"type", "options"},
                    {"value", 1},
                    {"options", {"option1", "option2"}}
            }

    };
}

TEST_CASE("Configuration parameters can be created and modified")
{
    ProcessorConfiguration configuration;

    configuration.addIntParameter("int_parameter", 1, 0, 10);
    configuration.addFloatParameter("float_parameter", 1.1, 0.1, 10.9, 0.2, 2);
    configuration.addBooleanParameter("boolean_parameter", true);
    configuration.addOptionsParameter("options_parameter", {"option1", "option2"}, 0);

    auto jsonConfiguration{configuration.getConfiguration()};

    REQUIRE(jsonConfiguration == createReferenceJsonParameterConfiguration());

    configuration.configure(createConfiguration2());
    jsonConfiguration = configuration.getConfiguration();
    REQUIRE(jsonConfiguration == createReferenceJsonParameterConfiguration2());
}
