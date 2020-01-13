#include "Registerer.h"
#include "ResizeImage.h"
#include "Roi.h"

void Registerer::registerProcessors(PipelineController &controller)
{
    controller.registerImageProcessor<ResizeImage>("resize");
    controller.registerImageProcessor<RoiProcessor>("roi");
}
