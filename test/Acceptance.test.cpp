#include <any>
#include <Observer.h>
#include <thread>
#include <opencv2/imgcodecs.hpp>
#include "trompeloeil.hpp"
#include "catch2/catch.hpp"
#include "PipelineController.h"
#include <nlohmann/json.hpp>
#include <PipelineConfiguration.h>

constexpr auto parameterIntegerName{"parameter_integer"};
constexpr auto parameterIntegerValue{1};
constexpr auto processor2ParameterIntegerValue{2};

Configuration createExpectedConfigurationProcessor1()
{
    Configuration configuration;
    configuration[parameterIntegerName] = parameterIntegerValue;
    return configuration;
}

Configuration createExpectedConfigurationProcessor2()
{
    Configuration configuration;
    configuration[parameterIntegerName] = processor2ParameterIntegerValue;
    return configuration;
}

constexpr auto frameSourcePathValue{"frame_source_path"};
constexpr auto processor1Name{"processor1"};
constexpr auto processor2Name{"processor2"};

PipelineConfiguration createPipelineConfiguration()
{
    PipelineConfiguration configuration;
    configuration.frameSourcePath = frameSourcePathValue;
    configuration.imageProcessors.emplace_back(processor1Name, std::move(createExpectedConfigurationProcessor1()));
    configuration.imageProcessors.emplace_back(processor2Name, std::move(createExpectedConfigurationProcessor2()));
    return configuration;
}

SCENARIO("A frame source can be loaded", "[acceptance]")
{
    GIVEN("A pipeline controller")
    {
        PipelineController controller;

        THEN("Throw if frame source index is set before been loaded")
        {
            CHECK_THROWS_WITH(controller.setFrameSourceIndex(1),
                              "Frame source is not loaded yet, index cannot be set");
        }

        THEN("Fails loading a non existing frame source")
        {
            const auto path{"non_existing_path"};
            const auto exceptionMessage{std::string("Not able to create frame source from path: ")+path
                                        +". Files supported: .jpg, .png, .bmp, .mp4."};
            CHECK_THROWS_WITH(controller.loadFrameSourceFrom(path), exceptionMessage);
        }

        THEN("Load successfully a existing frame source")
        {
            constexpr auto path{fixtures_path"/images/Lenna.png"};
            REQUIRE_NOTHROW(controller.loadFrameSourceFrom(path));

            AND_THEN("Fails setting incorrect frame source index")
            {
                CHECK_THROWS_WITH(controller.setFrameSourceIndex(1),
                                  "Frame source index out of bound (requested: 1, max: 0)");
            }

            AND_THEN("Set frame index if is not out of bound")
            {
                CHECK_NOTHROW(controller.setFrameSourceIndex(0));
            }
        }
    }
}

constexpr auto integerParameter{IntegerParameter{1, 0, 100}};

class MockProcessor1: public ImageProcessor
{
public:
    MockProcessor1()
            : ImageProcessor(processor1Name)
    {
        getParameters().registerParameter(parameterIntegerName, integerParameter);
    };

    MAKE_MOCK1(processImage, cv::Mat(const cv::Mat&), override);
};

class MockProcessor2: public ImageProcessor
{
public:
    MockProcessor2()
            :ImageProcessor(std::string(processor2Name))
    {
        getParameters().registerParameter(parameterIntegerName, integerParameter);
    }
    MAKE_MOCK1(processImage, cv::Mat(const cv::Mat&), override);
};

class FrameSourceMock : public FrameSource
{
    MAKE_MOCK1(loadFrom, void(const std::string &), override);
    MAKE_CONST_MOCK0(framesCount, unsigned int(), override);
    MAKE_MOCK1(getFrameFromIndex, cv::Mat(unsigned int), override);
};

class FrameSourceFactoryMock : public FrameSourceFactory
{
    MAKE_MOCK1(createAndLoadFromPath, std::unique_ptr<FrameSource>(const std::string&), override);
};

SCENARIO("A pipeline's image processor can be configured")
{
    GIVEN("A pipeline successfully loaded")
    {
        auto frameSourceMock{std::make_unique<FrameSourceMock>()};
        auto frameSourceFactoryMock{std::make_unique<FrameSourceFactoryMock>()};
        REQUIRE_CALL(*frameSourceFactoryMock, createAndLoadFromPath(frameSourcePathValue))
        .LR_RETURN(std::move(frameSourceMock));

        PipelineController controller{std::move(frameSourceFactoryMock)};
        controller.registerImageProcessor<MockProcessor1>(processor1Name);
        controller.registerImageProcessor<MockProcessor2>(processor2Name);
        controller.loadPipeline(createPipelineConfiguration());

        THEN("It fails if incorrect image processor index is configured")
        {
            constexpr auto imageProcessorIndex{3};
            const auto exceptionMessage{std::string{"Couldn't configure the image processor. The index 3 is out of range (2)"}};
            CHECK_THROWS_WITH(controller.configureProcessor(imageProcessorIndex, {}), exceptionMessage);
        }

        THEN("It fails if the Image processor is configured with incorrect parameters")
        {
            constexpr auto imageProcessorIndex{1};
            constexpr auto parameterName{"non_existing_parameter"};
            constexpr auto parameterName2{"non_existing_parameter_2"};
            Configuration configuration{{parameterName, 1}, {parameterName2, 2}};
            const auto exceptionMessage{std::string{"Couldn't configure image processor. Parameters not found: "}
                                        +parameterName+", "+parameterName2};
            CHECK_THROWS_WITH(controller.configureProcessor(imageProcessorIndex, configuration),
                              Catch::Contains("Couldn't configure image processor. Parameters not found: ")
                              && Catch::Contains(parameterName) && Catch::Contains(parameterName2));
        }

        THEN("Configure Image processor if parameters are correct")
        {
            constexpr auto imageProcessor2Index{1};
            constexpr auto parameter1NewValue{2};
            Configuration configuration{{parameterIntegerName, parameter1NewValue}};
            CHECK_NOTHROW(controller.configureProcessor(imageProcessor2Index, configuration));

            auto newIntegerParameter{integerParameter};
            newIntegerParameter.value = parameter1NewValue;
            Parameters expectedParameters{{parameterIntegerName, newIntegerParameter}};
            const auto parameters{controller.getProcessorParameters(imageProcessor2Index)};
            REQUIRE(parameters == expectedParameters);
        }
    }
}

nlohmann::json createProcessorConfiguration(int parameterValue)
{
    return {
            {
                    {"parameter_name", parameterIntegerName},
                    {"parameter_type", "integer"},
                    {"value", parameterValue}
            }
    };
}

nlohmann::json createProcessorsConfiguration()
{
    return {
            {
                    {"name", processor1Name},
                    {"configuration", createProcessorConfiguration(parameterIntegerValue)}
            },
            {
                    {"name", processor2Name},
                    {"configuration", createProcessorConfiguration(processor2ParameterIntegerValue)}
            }
    };
}

nlohmann::json createPipelineConfigurationJson()
{
    return nlohmann::json{
            {"frame_source_path", frameSourcePathValue},
            {"image_processors",  createProcessorsConfiguration()}
    };
}

SCENARIO("Pipeline can be loaded using a pipeline configuration")
{
    GIVEN("A pipeline controller with two image processor registered")
    {
        auto frameSourceMock{std::make_unique<FrameSourceMock>()};
        auto frameSourceFactoryMock{std::make_unique<FrameSourceFactoryMock>()};
        REQUIRE_CALL(*frameSourceFactoryMock, createAndLoadFromPath(frameSourcePathValue))
        .LR_RETURN(std::move(frameSourceMock));

        PipelineController controller(std::move(frameSourceFactoryMock));
        controller.registerImageProcessor<MockProcessor1>(processor1Name);
        controller.registerImageProcessor<MockProcessor2>(processor2Name);
        const auto pipelineConfiguration{createPipelineConfiguration()};

        WHEN("Pipeline configuration is loaded")
        {
            REQUIRE_NOTHROW(controller.loadPipeline(pipelineConfiguration));

            THEN("Pipeline is correctly configured")
            {
                const auto currentConfiguration{controller.getPipelineConfiguration()};
                CHECK(currentConfiguration == pipelineConfiguration);
            }
        }

        WHEN("Pipeline configuration is loaded from json")
        {
            REQUIRE_NOTHROW(controller.loadPipelineFromJson(createPipelineConfigurationJson()));

            THEN("Pipeline is correctly configured")
            {
                const auto currentConfiguration{controller.getPipelineConfiguration()};
                CHECK(currentConfiguration == pipelineConfiguration);
            }
        }

        WHEN("Pipeline configuration is loaded from a json file")
        {
            const auto path{fixtures_path"/json/acceptance_configuration.json"};
            REQUIRE_NOTHROW(controller.loadPipelineFromJsonFile(path));

            THEN("Pipeline is correctly configured")
            {
                const auto currentConfiguration{controller.getPipelineConfiguration()};
                CHECK(currentConfiguration == pipelineConfiguration);
            }
        }
    }
}

class MockObserver: public Observer
{
public:
    MAKE_MOCK0(update, void(), override);
};

inline auto image_matcher(const cv::Mat& image)
{
    return trompeloeil::make_matcher<cv::Mat>( // matcher of cv::Mat

            // predicate lambda that checks the condition
            [](const cv::Mat& image, const cv::Mat& referenceImage) {
                return std::equal(image.begin<uchar>(), image.end<uchar>(), referenceImage.begin<uchar>());
            },

            // print lambda for error message
            [](std::ostream& os, const cv::Mat& valid) {
                os << " matching cv::Mat image";
            },

            // stored value
            image
    );
}

SCENARIO("Process an image through the pipeline")
{
    GIVEN("A pipeline with two image processors and a frame source loaded")
    {
        auto frameSourceMock{std::make_unique<FrameSourceMock>()};
        cv::Mat lennaImage{cv::imread(fixtures_path"/images/Lenna.png")};
        REQUIRE_CALL(*frameSourceMock, getFrameFromIndex(0)).RETURN(lennaImage);

        auto frameSourceFactoryMock{std::make_unique<FrameSourceFactoryMock>()};
        REQUIRE_CALL(*frameSourceFactoryMock, createAndLoadFromPath(frameSourcePathValue))
        .LR_RETURN(std::move(frameSourceMock));

        auto dummyImagesValueIndex{0};
        REQUIRE_FALSE(lennaImage.empty());
        PipelineController controller(std::move(frameSourceFactoryMock));
        auto imageProcessor1{std::make_unique<MockProcessor1>()};
        cv::Mat dummyDebugImageProcessor1{(dummyImagesValueIndex++)*cv::Mat::ones(10, 10, CV_8U)};
        imageProcessor1->setDebugImage(dummyDebugImageProcessor1);
        cv::Mat dummyImageProcessor1Output{(dummyImagesValueIndex++)*cv::Mat::ones(10, 10, CV_8U)};
        REQUIRE_CALL(*imageProcessor1, processImage(image_matcher(lennaImage)))
        .LR_RETURN(dummyImageProcessor1Output);

        auto imageProcessor2{std::make_unique<MockProcessor1>()};
        cv::Mat dummyDebugImageProcessor2{(dummyImagesValueIndex++)*cv::Mat::ones(10, 10, CV_8U)};
        imageProcessor2->setDebugImage(dummyDebugImageProcessor2);
        cv::Mat dummyImageProcessor2Output{(dummyImagesValueIndex++)*cv::Mat::ones(10, 10, CV_8U)};
        REQUIRE_CALL(*imageProcessor2, processImage(image_matcher(dummyImageProcessor1Output)))
        .LR_RETURN(dummyImageProcessor2Output);

        controller.addImageProcessor(std::move(imageProcessor1));
        controller.addImageProcessor(std::move(imageProcessor2));

        REQUIRE_NOTHROW(controller.loadFrameSourceFrom(frameSourcePathValue));

        WHEN("An observer is registered")
        {
            auto mockObserver{std::make_shared<MockObserver>()};
            std::atomic_bool observerUpdated{false};
            REQUIRE_CALL(*mockObserver, update())
            .LR_SIDE_EFFECT(observerUpdated = true);

            controller.registerObserver(mockObserver);

            AND_WHEN("Pipeline processing is fired")
            {
                controller.firePipelineProcessing();

                THEN("The observer is notified after the image is processed by all image processors")
                {
                    while (!observerUpdated)
                    {
                        std::this_thread::sleep_for(std::chrono::microseconds(100));
                    }

                    AND_THEN("Pipeline input image can be retrieved")
                    {
                        const auto inputImage{controller.getCurrentLoadedImage()};
                        REQUIRE(std::equal(inputImage.begin<uchar>(), inputImage.end<uchar>(),
                                           lennaImage.begin<uchar>()));
                    }

                    AND_THEN("The image processors debug image can be retrieved")
                    {
                        constexpr auto processor1Index{0};
                        const auto debugImageProcessor1{controller.getDebugImage(processor1Index)};
                        REQUIRE(std::equal(debugImageProcessor1.begin<uchar>(), debugImageProcessor1.end<uchar>(),
                                           dummyDebugImageProcessor1.begin<uchar>()));

                        constexpr auto processor2Index{1};
                        const auto debugImageProcessor2{controller.getDebugImage(processor2Index)};
                        REQUIRE(std::equal(debugImageProcessor2.begin<uchar>(), debugImageProcessor2.end<uchar>(),
                                           dummyDebugImageProcessor2.begin<uchar>()));
                    }
                }
            }
        }
    }
}