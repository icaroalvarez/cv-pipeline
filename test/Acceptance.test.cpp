#include <any>
#include <Observer.h>
#include <thread>
#include <opencv2/imgcodecs.hpp>
#include "trompeloeil.hpp"
#include "catch2/catch.hpp"
#include "PipelineController.h"

class MockProcessor1: public ImageProcessor
{
public:
    MockProcessor1()
            : ImageProcessor("mock_processor_1"){};

    MAKE_MOCK1(processImage, cv::Mat(const cv::Mat&), override);
};

class MockProcessor2: public ImageProcessor
{
public:
    MockProcessor2()
            :ImageProcessor("mock_processor_2")
    {
        getConfiguration().registerParameter("parameter_1", IntegerParameter{1, 0, 100});
    }
    MAKE_MOCK1(processImage, cv::Mat(const cv::Mat&), override);
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
            const auto exceptionMessage{std::string("Unable to load image from: ")+path};
            CHECK_THROWS_WITH(controller.loadFrameSourceFrom(path), exceptionMessage);
        }

        THEN("Load successfully a existing frame source")
        {
            constexpr auto path{fixtures_path"Lenna.png"};
            CHECK_NOTHROW(controller.loadFrameSourceFrom(path));
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
    controller->loadFrameSourceFrom(frameSourcePath);
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
        auto dummyImagesValueIndex{0};
        cv::Mat lennaImage{cv::imread(fixtures_path"Lenna.png")};
        auto controller{std::make_unique<PipelineController>()};
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

        controller->addImageProcessor(std::move(imageProcessor1));
        controller->addImageProcessor(std::move(imageProcessor2));

        constexpr auto frameSourcePath{fixtures_path"Lenna.png"};
        REQUIRE_NOTHROW(controller->loadFrameSourceFrom(frameSourcePath));

        WHEN("An observer is registered")
        {
            auto mockObserver{std::make_shared<MockObserver>()};
            std::atomic_bool observerUpdated{false};
            REQUIRE_CALL(*mockObserver, update())
            .LR_SIDE_EFFECT(observerUpdated = true);

            controller->registerObserver(mockObserver.get());

            AND_WHEN("Pipeline processing is fired")
            {
                controller->firePipelineProcessing();

                THEN("The observer is notified after the image is processed by all image processors")
                {
                    while (!observerUpdated)
                    {
                        std::this_thread::sleep_for(std::chrono::microseconds(100));
                    }

                    AND_THEN("The debug image can be retrieved")
                    {
                        constexpr auto processor1Index{0};
                        const auto debugImageProcessor1{controller->getDebugImage(processor1Index)};
                        REQUIRE(std::equal(debugImageProcessor1.begin<uchar>(), debugImageProcessor1.end<uchar>(),
                                dummyDebugImageProcessor1.begin<uchar>()));

                        constexpr auto processor2Index{1};
                        const auto debugImageProcessor2{controller->getDebugImage(processor2Index)};
                        REQUIRE(std::equal(debugImageProcessor2.begin<uchar>(), debugImageProcessor2.end<uchar>(),
                                   dummyDebugImageProcessor2.begin<uchar>()));
                    }
                }
            }
        }
    }
}