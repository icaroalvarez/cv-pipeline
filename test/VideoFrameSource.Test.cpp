#include <catch2/catch.hpp>
#include <VideoFrameSource.h>
#include <opencv2/imgcodecs.hpp>

SCENARIO("Loads a video frame source")
{
    GIVEN("An video frame source")
    {
        VideoFrameSource frameSource;

        THEN("Frame index count is zero before loading the source")
        {
            CHECK(frameSource.framesCount() == 0);
        }

        THEN("When wrong source path is loaded")
        {
            const std::string wrongSourcePath{"wrong_source_path"};
            CHECK_THROWS_WITH(frameSource.loadFrom(wrongSourcePath),
                              "Video frame source not able to open video at: " + wrongSourcePath);

        }

        WHEN("An existing video path is loaded")
        {
            const std::string videoSourcePath{fixtures_path"video/chaplin.mp4"};
            REQUIRE_NOTHROW(frameSource.loadFrom(videoSourcePath));

            THEN("Frame index count is correct")
            {
                constexpr auto totalFrames{172};
                CHECK(frameSource.framesCount() == totalFrames);
            }

            THEN("Throws getting an image with an incorrect index number")
            {
                constexpr auto wrongIndex{172};
                CHECK_THROWS_WITH(frameSource.getFrameFromIndex(wrongIndex),
                                  "Unable to get frame from index 172. Index is out of bound (max index is 171)");
            }

            THEN("The image is retrieve with a correct index number")
            {
                constexpr auto totalFrames{10};
                for(auto iFrame = 0; iFrame < totalFrames; iFrame += 2)
                {
                    const auto frame{frameSource.getFrameFromIndex(iFrame)};

                    const auto framePath{fixtures_path"images/chaplin/chaplin_frame_"
                                         +std::to_string(iFrame)+".bmp"};
                    const auto chaplinFrame{cv::imread(framePath)};
                    CHECK(std::equal(frame.begin<uchar>(), frame.end<uchar>(), chaplinFrame.begin<uchar>()));
                }
            }
        }
    }
}
