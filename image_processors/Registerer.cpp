#include "Registerer.h"
#include "ResizeImage.h"
#include "Roi.h"
#include "ColorSpace.h"
#include "Convolution.h"
#include "AdaptiveThreshold.h"

void Registerer::registerProcessors(PipelineController &controller)
{
    controller.registerImageProcessor<ResizeImage>("resize");
    controller.registerImageProcessor<RoiProcessor>("roi");
    controller.registerImageProcessor<ColorSpace>("color_space");
    controller.registerImageProcessor<Convolution>("convolution");
    controller.registerImageProcessor<AdaptiveThreshold>("adaptive_threshold");
}
