#include <any>
#include <Observer.h>
#include <thread>
#include "trompeloeil.hpp"
#include "catch2/catch.hpp"
#include "PipelineController.h"

class MockProcessor1: public ImageProcessor
{
public:
    MockProcessor1()
            : ImageProcessor("mock_processor_1"){};

    MAKE_MOCK1(processImage, void(const cv::Mat&), override);
};

class MockProcessor2: public ImageProcessor
{
public:
    MockProcessor2()
            :ImageProcessor("mock_processor_2")
    {
        getConfiguration().registerParameter("parameter_1", IntegerParameter{1, 0, 100});
    }
    MAKE_MOCK1(processImage, void(const cv::Mat&), override);
};

SCENARIO("An image processor pipeline can be loaded")
{
    GIVEN("A pipeline controller")
    {
        PipelineController controller;

        WHEN("Two image processors are registered")
        {
            controller.registerImageProcessor<MockProcessor1>("mock_processor_1");
            controller.registerImageProcessor<MockProcessor2>("mock_processor_2");

            THEN("Fails loading a pipeline with not previously registered image processors.")
            {
                const std::vector<std::string> pipeline{"non_existing_processor1", "non_existing_processor2"};
                constexpr auto exceptionMessage{
                        "Couldn't load pipeline, processors not found: non_existing_processor1, non_existing_processor2"};
                CHECK_THROWS_WITH(controller.loadPipeline(pipeline), exceptionMessage);
            }

            THEN("Successfully loads a pipeline with previously registered image processors")
            {
                const std::vector<std::string> pipelineDescription{"mock_processor_1", "mock_processor_2"};
                CHECK_NOTHROW(controller.loadPipeline(pipelineDescription));
                CHECK(controller.getPipelineDescription() == pipelineDescription);
            }
        }
    }
}

SCENARIO("A frame source can be loaded")
{
    GIVEN("A pipeline controller")
    {
        PipelineController controller;

        THEN("Fails loading a non existing frame source")
        {
            const auto path{"non_existing_path"};
            const auto exceptionMessage{std::string("Couldn't load frame source. Non path found: ")+path};
            CHECK_THROWS_WITH(controller.loadImage(path), exceptionMessage);
        }

        THEN("Load successfully a existing frame source")
        {
            constexpr auto path{fixtures_path"Lenna.png"};
            CHECK_NOTHROW(controller.loadImage(path));
        }
    }
}

std::unique_ptr<PipelineController> createPipelineController()
{
    auto controller{std::make_unique<PipelineController>()};
    controller->registerImageProcessor<MockProcessor1>("image_processor_1");
    controller->registerImageProcessor<MockProcessor2>("image_processor_2");
    const std::vector<std::string> pipeline = {"image_processor_1", "image_processor_2"};
    controller->loadPipeline(pipeline);

    constexpr auto frameSourcePath{fixtures_path"Lenna.png"};
    controller->loadImage(frameSourcePath);
    return std::move(controller);
}

SCENARIO("A pipeline's image processor can be configured")
{
    GIVEN("A pipeline successfully loaded")
    {
        auto controller{createPipelineController()};
        THEN("It fails if incorrect image processor index is configured")
        {
            constexpr auto imageProcessorIndex{3};
            const auto exceptionMessage{std::string{"Couldn't configure the image processor. The index 3 is out of range (2)"}};
            CHECK_THROWS_WITH(controller->configureProcessor(imageProcessorIndex, {}), exceptionMessage);
        }

        THEN("It fails if the Image processor is configured with incorrect parameters")
        {
            constexpr auto imageProcessorIndex{1};
            constexpr auto parameterName{"non_existing_parameter"};
            constexpr auto parameterName2{"non_existing_parameter_2"};
            Configuration configuration{{parameterName, 1}, {parameterName2, 2}};
            const auto exceptionMessage{std::string{"Couldn't configure image processor. Parameters not found: "}
                                        +parameterName+", "+parameterName2};
            CHECK_THROWS_WITH(controller->configureProcessor(imageProcessorIndex, configuration),
                              Catch::Contains("Couldn't configure image processor. Parameters not found: ")
                              && Catch::Contains(parameterName) && Catch::Contains(parameterName2));
        }

        THEN("Doesn't fail if the Image processor is configured with correct parameters")
        {
            constexpr auto imageProcessorIndex{1};
            constexpr auto parameterName{"parameter_1"};
            Configuration configuration{{parameterName, 1}};
            CHECK_NOTHROW(controller->configureProcessor(imageProcessorIndex, configuration));
        }
    }
}

class MockObserver: public Observer
{
public:
    MAKE_MOCK0(update, void(), override);
};

SCENARIO("An image from frame source can be processed through the pipeline")
{
    GIVEN("A pipeline and a frame source are already loaded")
    {
        auto controller{std::make_unique<PipelineController>()};
        auto imageProcessor1{std::make_unique<MockProcessor1>()};
        REQUIRE_CALL(*imageProcessor1, processImage(trompeloeil::_));
        auto imageProcessor2{std::make_unique<MockProcessor1>()};
        REQUIRE_CALL(*imageProcessor2, processImage(trompeloeil::_));
        controller->addImageProcessor(std::move(imageProcessor1));
        controller->addImageProcessor(std::move(imageProcessor2));

        constexpr auto frameSourcePath{fixtures_path"Lenna.png"};
        controller->loadImage(frameSourcePath);

        WHEN("An observer is registered and an image is fired to be processed")
        {
            auto mockObserver{std::make_shared<MockObserver>()};
            std::atomic_bool observerUpdated{false};
            REQUIRE_CALL(*mockObserver, update())
            .LR_SIDE_EFFECT(observerUpdated = true);

            controller->registerObserver(mockObserver.get());
            controller->processCurrentImage();

            THEN("The observer is notified when the image is processed by all image processors")
            {
                while(!observerUpdated)
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(100));
                }

                AND_THEN("The original, pre-processed, post-processed and debug images can be retrieved from an image processor")
                {
                    const auto originalImage{controller->getCurrentLoadedImage()};
                    constexpr auto processorIndex{1};
                    const auto postProcessedImage{controller->getPostProcessedImage(processorIndex)};
                    const auto debugImage{controller->getDebugImage(processorIndex)};
                }
            }
        }
    }
}