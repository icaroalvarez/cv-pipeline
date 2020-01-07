#include <any>
#include "trompeloeil.hpp"
#include "catch2/catch.hpp"

SCENARIO("An image processor pipeline can be loaded")
{
    GIVEN("A pipeline controller with two image processors registered")
    {
        PipelineController controller;
        controller.registerImageProcessor<MockProcessor1>("image_processor_1");
        controller.registerImageProcessor<MockProcessor2>("image_processor_2");

        THEN("Fails loading a pipeline with non existing image processors.")
        {
            constexpr auto pipeline = {"non_existing_processor1", "non_existing_processor2"};
            constexpr auto exceptionMessage{"Couldn't load pipeline, processors not found: non_existing_processor1, non_existing_processor2"};
            CHECK_THROWS_WITH(controller.loadPipeline(pipeline), exceptionMessage);
        }

        THEN("Load successfully a pipeline with existing image processors")
        {
            constexpr auto pipeline = {"image_processor_1", "image_processor_2"};
            CHECK_NOTHROW(controller.loadPipeline(pipeline));
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
            constexpr auto path{"non_existing_path"};
            constexpr auto exceptionMessage{std::string{"Couldn't load frame source. Non path found: "}+path};
            CHECK_THROWS_WITH(controller.loadFrameSource(path), exceptionMessage);
        }

        THEN("Load successfully a existing frame source")
        {
            constexpr auto path{"fixtures_path/Lenna.png"};
            CHECK_NOTHROW(controller.loadFrameSource(path));
        }
    }
}

std::unique_ptr<PipelineController> createPipelineController()
{
    const auto controller{std::make_unique<PipelineController>()};
    controller->registerImageProcessor<MockProcessor1>("image_processor_1");
    controller->registerImageProcessor<MockProcessor2>("image_processor_2");
    constexpr auto pipeline = {"image_processor_1", "image_processor_2"};
    controller->loadPipeline(pipeline);

    constexpr auto frameSourcePath{"fixtures_path/Lenna.png"};
    controller->loadFrameSource(frameSourcePath);
    return controller;
}

SCENARIO("An image from frame source can be processed through the pipeline")
{
    GIVEN("A pipeline and a frame source are already loaded")
    {
        const auto controller{createPipelineController()};

        WHEN("An observer is registered and an image is fired to be processed")
        {
            MockObserver mockObserver;
            controller->addObserver(mockObserver);
            controller->fireNextImage();

            THEN("The observer is notified when the image is processed by all image processors")
            {

                AND_THEN("The original, pre-processed, post-processed and debug images can be retrieved from an image processor")
                {
                    const auto originalImage{controller->getOriginalImage()};
                    constexpr auto processorIndex{1};
                    const auto preProcessedImage{controller->getPreprocessedImage(processorIndex)};
                    const auto postProcessedImage{controller->getPostProcessedImage(processorIndex)};
                    const auto debugImage{controller->getDebugImage(processorIndex)};
                }
            }
        }
    }
}

SCENARIO("A pipeline's image processor can be configured")
{
    GIVEN("A pipeline successfully loaded")
    {
        auto controller{createPipelineController()};
        THEN("It fails if a non existing image processor is configured")
        {
            constexpr auto imageProcessorName{"non_existing_image_processor"};
            std::unordered_map<std::string, std::any> configuration;
            constexpr auto exceptionMessage{std::string{"Couldn't configure image processor. Image processor not found: "}+imageProcessorName};
            CHECK_THROWS_WITH(controller->configureProcessor(imageProcessorName, configuration), exceptionMessage);
        }

        THEN("It fails if the Image processor is configured with incorrect parameters")
        {
            constexpr auto imageProcessorName{"image_processor_1"};
            constexpr auto parameterName{"non_existing_parameter"};
            std::unordered_map<std::string, std::any> configuration{{parameterName, 1}};
            constexpr auto exceptionMessage{std::string{"Couldn't configure image processor. Parameter not found: "}+parameterName};
            CHECK_THROWS_WITH(controller->configureProcessor(imageProcessorName, configuration), exceptionMessage);
        }

        THEN("Doesn't fail if the Image processor is configured with correct parameters")
        {
            constexpr auto imageProcessorName{"image_processor_1"};
            constexpr auto parameterName{"parameter_1"};
            std::unordered_map<std::string, std::any> configuration{{parameterName, 1}};
            CHECK_NOTHROW(controller->configureProcessor(imageProcessorName, configuration));
        }
    }
}