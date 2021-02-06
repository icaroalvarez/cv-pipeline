#include "Registerer.h"
#include "ResizeImage.h"
#include "Roi.h"
#include "ColorSpace.h"
#include "Convolution.h"
#include "threshold/AdaptiveThreshold.h"
#include "threshold/Percentile.h"
#include "threshold/Threshold.h"
#include "GaborFilter.h"
#include "CannyEdgeDetector.h"
#include "HoughLines.h"

void Registerer::registerProcessors(const std::shared_ptr<PipelineController>& controller)
{
    controller->registerImageProcessor<ResizeImage>("resize");
    controller->registerImageProcessor<RoiProcessor>("roi");
    controller->registerImageProcessor<ColorSpace>("color_space");
    controller->registerImageProcessor<Convolution>("convolution");
    controller->registerImageProcessor<AdaptiveThreshold>("adaptive_threshold");
    controller->registerImageProcessor<Threshold>("threshold");
    controller->registerImageProcessor<Percentile>("percentile_threshold");
    controller->registerImageProcessor<GaborFilter>("gabor_filter");
    controller->registerImageProcessor<CannyEdgeDetector>("canny_edge_detector");
    controller->registerImageProcessor<HoughLines>("hough_lines");
}
