#include <catch2/catch.hpp>
#include <ImageFrameSource.h>
#include <opencv2/imgcodecs.hpp>

TEST_CASE("Loads the frame source")
{
    ImageFrameSource frameSource;

    const std::string wrongSourcePath{"wrong_source_path"};
    CHECK_THROWS_WITH(frameSource.loadFrom(wrongSourcePath),
            "Unable to load image from: "+wrongSourcePath);

    const std::string imageSourcePath{fixtures_path"Lenna.png"};
    REQUIRE_NOTHROW(frameSource.loadFrom(imageSourcePath));

    CHECK(frameSource.framesCount() == 1);
    cv::Mat lennaImage{cv::imread(fixtures_path"Lenna.png")};

    auto frame{frameSource.getFrameFromIndex(0)};
    CHECK(std::equal(lennaImage.begin<uchar>(), lennaImage.end<uchar>(), frame.begin<uchar>()));
}
