#include <any>
#include <Observer.h>
#include <thread>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "trompeloeil.hpp"
#include "catch2/catch.hpp"
#include "PipelineController.h"
#include <PipelineConfiguration.h>

namespace
{
    constexpr auto parameterIntegerName{"parameter_integer"};
    constexpr auto parameterIntegerValue{1};
    constexpr auto processor2ParameterIntegerValue{2};

    Configuration createConfigurationProcessor1()
    {
        Configuration configuration;
        configuration[parameterIntegerName] = parameterIntegerValue;
        return configuration;
    }

    Configuration createConfigurationProcessor2()
    {
        Configuration configuration;
        configuration[parameterIntegerName] = processor2ParameterIntegerValue;
        return configuration;
    }

    constexpr auto frameSourcePathValue{fixtures_path"/images/Lenna.png"};
    constexpr auto processor1Name{"processor1"};
    constexpr auto processor2Name{"processor2"};

    PipelineConfiguration createPipelineConfiguration()
    {
        PipelineConfiguration configuration;
        configuration.frameSourcePath = frameSourcePathValue;
        configuration.imageProcessors.emplace_back(processor1Name, std::move(createConfigurationProcessor1()));
        configuration.imageProcessors.emplace_back(processor2Name, std::move(createConfigurationProcessor2()));
        return configuration;
    }

    constexpr auto integerParameter{IntegerParameter{1, 0, 100}};

    class Processor1 : public ImageProcessor
    {
    public:
        Processor1() : ImageProcessor(processor1Name)
        {
            getParameters().registerParameter(parameterIntegerName, integerParameter);
        };

        cv::Mat processImage(const cv::Mat &image) override
        {
            // just draw a line in the image
            auto cloned{image.clone()};
            cv::Scalar greenColor{0, 255, 0};
            cv::line(cloned, {0, 0}, {cloned.cols, cloned.rows}, greenColor);
            setDebugImage(cloned.clone());
            return cloned;
        }
    };

    class Processor2 : public ImageProcessor
    {
    public:
        Processor2() : ImageProcessor(processor2Name)
        {
            getParameters().registerParameter(parameterIntegerName, integerParameter);
        }

        cv::Mat processImage(const cv::Mat &image) override
        {
            // just draw a line in the image
            auto cloned{image.clone()};
            cv::Scalar redColor{0, 0, 255};
            cv::line(cloned, {cloned.cols, 0}, {0, cloned.rows}, redColor);
            setDebugImage(cloned.clone());
            return cloned;
        }
    };

    class MockObserver : public Observer
    {
    public:
        MAKE_MOCK0 (update, void(), override);
    };

    SCENARIO("Pipeline usage")
    {
        GIVEN("A pipeline with two image processors and a frame source loaded")
        {
            auto dummyImagesValueIndex{0};
            cv::Mat lennaImage{cv::imread(fixtures_path"/images/Lenna.png")};
            REQUIRE_FALSE(lennaImage.empty());

            PipelineController controller;
            controller.registerImageProcessor<Processor1>(processor1Name);
            controller.registerImageProcessor<Processor2>(processor2Name);
            controller.loadPipeline(createPipelineConfiguration());

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
                            auto path{fixtures_path"/images/acceptance_debug_processor_1.png"};
                            //cv::imwrite(path, debugImageProcessor1);
                            auto expectedImage{cv::imread(path)};
                            REQUIRE(std::equal(debugImageProcessor1.begin<uchar>(), debugImageProcessor1.end<uchar>(),
                                               expectedImage.begin<uchar>()));

                            constexpr auto processor2Index{1};
                            const auto debugImageProcessor2{controller.getDebugImage(processor2Index)};
                            path = fixtures_path"/images/acceptance_debug_processor_2.png";
                            //cv::imwrite(path, debugImageProcessor2);
                            expectedImage = cv::imread(path);
                            REQUIRE(std::equal(debugImageProcessor2.begin<uchar>(), debugImageProcessor2.end<uchar>(),
                                               expectedImage.begin<uchar>()));
                        }
                    }
                }
            }
        }
    }
}