#include "catch2/catch.hpp"
#include "ImageFrameSource.h"
#include "VideoFrameSource.h"
#include "FrameSourceFactory.h"

SCENARIO("Create frame source")
{
    GIVEN("A frame source factory")
    {
        FrameSourceFactory factory;
        THEN("Throws if a frame source is created from a wrong path")
        {
            const std::string sourcePath{"non_existing_path"};
            CHECK_THROWS_WITH(factory.createAndLoadFromPath(sourcePath),
                    "Not able to create frame source from path: "+sourcePath
                    +". Files supported: .jpg, .png, .bmp, .mp4.");
        }

        WHEN("A frame source is created from a valid image path")
        {
            const auto sourcePath{fixtures_path"/images/Lenna.png"};
            const auto frameSource{factory.createAndLoadFromPath(sourcePath)};

            THEN("The frame source type is ImageFrameSource type")
            {
                const auto derivedIsImageFrameSource{dynamic_cast<ImageFrameSource*>(frameSource.get())};
                CHECK(derivedIsImageFrameSource);
            }
        }

        WHEN("A frame source is created from a valid video path")
        {
            const auto sourcePath{fixtures_path"/video/chaplin.mp4"};
            const auto frameSource{factory.createAndLoadFromPath(sourcePath)};

            THEN("The frame source type is VideoFrameSource type")
            {
                const auto derivedIsVideoFrameSource{dynamic_cast<VideoFrameSource*>(frameSource.get())};
                CHECK(derivedIsVideoFrameSource);
            }
        }
    }
}
