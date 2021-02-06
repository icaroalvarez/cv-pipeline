#pragma once

#include <PipelineController.h>

class Registerer
{
public:
    static void registerProcessors(const std::shared_ptr<PipelineController>& pipelineController);
};
