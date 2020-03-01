#include <catch2/catch.hpp>
#include "ProcessorParameters.h"

constexpr auto integerParameterName{"integer_parameter"};
constexpr auto decimalParameterName{"decimal_parameter"};
constexpr auto booleanParameterName{"boolean_parameter"};
constexpr auto optionsParameterName{"options_parameter"};

constexpr auto integerParameterInitialValue{1};
constexpr auto decimalParameterInitialValue{1.2};
constexpr auto booleanParameterInitialValue{true};
constexpr std::size_t optionIndexParameterInitialValue{0};

constexpr auto integerParameterConfigureValue{2};
constexpr auto decimalParameterConfigureValue{2.0};
constexpr auto booleanParameterConfigureValue{false};
constexpr std::size_t optionIndexParameterConfigureValue{1};

TEST_CASE("Parameters equal operators")
{
    IntegerParameter integerParameter{1, 0, 10};
    CHECK(integerParameter == integerParameter);
    DecimalParameter decimalParameter{1.0, 0.0, 1.9, 2.0, 2};
    CHECK(decimalParameter == decimalParameter);
    OptionsParameter optionsParameter{0, {"option1", "option2"}};
    CHECK(optionsParameter == optionsParameter);
}

Configuration createDummyConfiguration()
{
    Configuration configuration;
    configuration[integerParameterName] = integerParameterConfigureValue;
    configuration[decimalParameterName] = decimalParameterConfigureValue;
    configuration[booleanParameterName] = booleanParameterConfigureValue;
    configuration[optionsParameterName] = optionIndexParameterConfigureValue;
    return configuration;
}

SCENARIO("Parameters can be registered and configured")
{
    GIVEN("A parameters element")
    {
        ProcessorParameters parameters;

        WHEN("Parameters are registered")
        {

            parameters.registerParameter(integerParameterName,
                                         IntegerParameter{integerParameterInitialValue, 0, 10});
            parameters.registerParameter(decimalParameterName,
                                         DecimalParameter{decimalParameterInitialValue, 0.1,10.9, 0.2, 2});
            parameters.registerParameter(booleanParameterName, booleanParameterInitialValue);
            parameters.registerParameter(optionsParameterName,
                                         OptionsParameter{optionIndexParameterInitialValue, {"option1", "option2"}});

            THEN("Parameter values can be obtained")
            {
                CHECK(parameters.getParameterValue<int>(integerParameterName) == integerParameterInitialValue);
                CHECK(parameters.getParameterValue<double>(decimalParameterName) == decimalParameterInitialValue);
                CHECK(parameters.getParameterValue<bool>(booleanParameterName) == booleanParameterInitialValue);
                CHECK(parameters.getParameterValue<std::size_t>(optionsParameterName) == optionIndexParameterInitialValue);
            }

            AND_WHEN("Parameters are configured")
            {
                parameters.configure(createDummyConfiguration());

                THEN("New parameter values can be obtained")
                {
                    CHECK(parameters.getParameterValue<int>(integerParameterName) == integerParameterConfigureValue);
                    CHECK(parameters.getParameterValue<double>(decimalParameterName) == decimalParameterConfigureValue);
                    CHECK(parameters.getParameterValue<bool>(booleanParameterName) == booleanParameterConfigureValue);
                    CHECK(parameters.getParameterValue<size_t>(optionsParameterName) == optionIndexParameterConfigureValue);
                }
            }

            AND_WHEN("Non existing parameters are configured")
            {
                Configuration wrongConfiguration{{"non_existing_parameter_1", 1},
                                                 {"non_existing_parameter_2", 2}};
                THEN("Throws")
                {
                    CHECK_THROWS_WITH(parameters.configure(wrongConfiguration),
                                      Catch::Contains("Parameters not found: ") &&
                                      Catch::Contains("non_existing_parameter_1") &&
                                      Catch::Contains("non_existing_parameter_2"));
                }
            }

            AND_WHEN("Non wrong parameter types are configured")
            {
                Configuration wrongConfiguration = {{integerParameterName, 3.7},
                                                    {booleanParameterName, 1},
                                                    {optionsParameterName, true},
                                                    {decimalParameterName, std::size_t(1)}};

                THEN("Throws")
                {
                    CHECK_THROWS_WITH(parameters.configure(wrongConfiguration),
                                      Catch::Contains("Couldn't configure image processor. Wrong type in parameters: ") &&
                                      Catch::Contains(integerParameterName) &&
                                      Catch::Contains(booleanParameterName) &&
                                      Catch::Contains(optionsParameterName) &&
                                      Catch::Contains(decimalParameterName));
                }
            }
        }
    }
}
