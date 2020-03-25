#pragma once
#include "FrameSource.h"

class FrameSourceFactory
{
public:
    static std::unique_ptr<FrameSource> createAndLoadFromPath(const std::string& path);
};



