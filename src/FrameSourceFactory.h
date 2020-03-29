#pragma once
#include "FrameSource.h"

class FrameSourceFactory
{
public:
    virtual std::unique_ptr<FrameSource> createAndLoadFromPath(const std::string& path);
};



