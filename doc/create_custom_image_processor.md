# How to create your own custom image processor
The goal of panoramix tool is to visually test image processor implementations. For this purpose a common ImageProcessor is defined where you can put your algorithm code and the different parameters to be configured. Later you can create a pipeline including the custom processor and configure it to see the different output results.

In the following, the steps necessary to test your custom processor are explained.

## 1) Create your own image processor

Create an ImageProcessor derived class and implement processImage method.

### ResizeImage.h
```cpp
#pragma once
#include "ImageProcessor.h"

class ResizeImage: public ImageProcessor
{
public:
    ResizeImage();
    cv::Mat processImage(const cv::Mat &image) override;
};
```

### ResizeImage.cpp
```cpp
#include <opencv2/imgproc.hpp>
#include "ResizeImage.h"

ResizeImage::ResizeImage()
:ImageProcessor("resize")
{
    getParameters().registerParameter("factor_x",
            DecimalParameter{0.3, 0.1, 1.0, 0.1, 1});
    getParameters().registerParameter("factor_y",
            DecimalParameter{0.3, 0.1, 1.0, 0.1, 1});
    getParameters().registerParameter("interpol_mode",
            OptionsParameter{1, {"linear", "cubic", "area", "landzos4"}});
}

cv::Mat ResizeImage::processImage(const cv::Mat &image)
{
    const auto factorX{getParameters().getParameterValue<double>("factor_x")};
    const auto factorY{getParameters().getParameterValue<double>("factor_y")};
    const auto interpolation_mode{getParameters().getParameterValue<SelectedOptionIndex>("interpol_mode")};
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(), factorX, factorY, interpolation_mode);
    setDebugImage(resized);
    return resized;
}
```

## 2) Register your processor in the pipeline
```cpp
PipelineController controller;
controller.registerImageProcessor<ResizeImage>("resize");
```