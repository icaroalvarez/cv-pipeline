#include "FrameSourceFactory.h"
#include "ImageFrameSource.h"
#include "VideoFrameSource.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

std::string createAllFilesSupportedString(const std::vector<std::string>& imageExtensions,
                                          const std::vector<std::string>& videoExtensions)
{
    std::string allFilesSupported;
    std::for_each(imageExtensions.begin(), imageExtensions.end(),
                  [&](const auto& extension){allFilesSupported.append(extension+", ");});
    std::for_each(videoExtensions.begin(), videoExtensions.end(),
                  [&](const auto& extension){allFilesSupported.append(extension+", ");});
    allFilesSupported.pop_back();
    allFilesSupported.back() = '.';
    return allFilesSupported;
}

std::unique_ptr<FrameSource> FrameSourceFactory::createAndLoadFromPath(const std::string &path)
{
    const fs::path frameSourcePath{path};
    const auto extension{frameSourcePath.extension()};
    const std::vector<std::string> imageExtensions{".jpg", ".png", ".bmp"};
    const std::vector<std::string> videoExtensions{".mp4"};
    const auto isImage{std::find(imageExtensions.begin(), imageExtensions.end(), extension) not_eq imageExtensions.end()};
    const auto isVideo{std::find(videoExtensions.begin(), videoExtensions.end(), extension) not_eq videoExtensions.end()};

    std::unique_ptr<FrameSource> frameSource;
    if(isImage)
    {
        frameSource = std::make_unique<ImageFrameSource>();
    }else if(isVideo)
    {
        frameSource = std::make_unique<VideoFrameSource>();
    }else{
        const auto allFilesSupported{createAllFilesSupportedString(imageExtensions, videoExtensions)};
        throw std::invalid_argument("Not able to create frame source from path: "+path
                                    +". Files supported: "+allFilesSupported);
    }

    frameSource->loadFrom(path);
    return std::move(frameSource);
}
