#include <catch2/catch.hpp>
#include <ImageFrameSource.h>
#include <opencv2/imgcodecs.hpp>

SCENARIO("Loads an image")
{
    GIVEN("An image frame source")
    {
        ImageFrameSource frameSource;

        THEN("Frame index count is zero before loading the source")
        {
            CHECK(frameSource.framesCount() == 0);
        }

        THEN("When wrong source path is loaded")
        {
            const std::string wrongSourcePath{"wrong_source_path"};
            CHECK_THROWS_WITH(frameSource.loadFrom(wrongSourcePath),
                              "Unable to load image from: " + wrongSourcePath);

        }

        WHEN("An existing image path is loaded")
        {
            const std::string imageSourcePath{fixtures_path"Lenna.png"};
            REQUIRE_NOTHROW(frameSource.loadFrom(imageSourcePath));

            THEN("Frame index count is correct")
            {
                CHECK(frameSource.framesCount() == 1);
            }

            THEN("Throws getting an image with an incorrect index number")
            {
                CHECK_THROWS_WITH(frameSource.getFrameFromIndex(1),
                                  "Unable to get frame from index 1. Index is out of bound (max index is 0)");
            }

            THEN("The image is retrieve with a correct index number")
            {
                cv::Mat lennaImage{cv::imread(fixtures_path"Lenna.png")};
                auto frame{frameSource.getFrameFromIndex(0)};
                CHECK(std::equal(lennaImage.begin<uchar>(), lennaImage.end<uchar>(), frame.begin<uchar>()));
            }
        }
    }
}
