#include <opencv2/imgproc.hpp>
#include "Convolution.h"

double degreesToRadians(double degrees)
{
    return degrees*CV_PI/180.0;
}

cv::Mat jetColorFrom(const cv::Mat& image)
{
    cv::Mat output;
    cv::normalize(image, output, 0, 255, cv::NORM_MINMAX);
    if(output.type() != CV_8U)
    {
        output.convertTo(output, CV_8U);
    }
    cv::applyColorMap(output, output, cv::COLORMAP_JET);
    return output;
}

Convolution::Convolution()
        :ImageProcessor("convolution")
{
    getConfiguration().addIntParameter("horizontal_size", 5, 1, 99);
    getConfiguration().addIntParameter("vertical_size", 20, 1, 99);
    getConfiguration().addIntParameter("theta_degrees", 0, 0, 360);
    getConfiguration().addFloatParameter("sigma_stddev", 1.0, 0.0, 99.9, 0.1, 1);
    getConfiguration().addFloatParameter("lambda_wavelength", 4.8, 0.0, 99.9, 0.1, 1);
    getConfiguration().addFloatParameter("gamma_aspect_ratio", 0.f, 0.0, 99.9, 0.1, 1);
    getConfiguration().addFloatParameter("psi_phase_offset", 0.f, 0.0, 99.9, 0.1, 1);
}

cv::Mat createImageDebugKernel(const cv::Mat& kernel, cv::Size canvasSize)
{
    // calculate min max value
    double minValue, maxValue;
    cv::minMaxIdx(kernel, &minValue, &maxValue);

    cv::Mat normalizedKernel;
    cv::normalize(kernel, normalizedKernel, 0, 255, cv::NORM_MINMAX);
    normalizedKernel.convertTo(normalizedKernel, CV_8U);
    normalizedKernel = jetColorFrom(normalizedKernel);
    cv::Mat output = cv::Mat::zeros(canvasSize, CV_8UC3);
    // first resize height to fit canvas
    float aspectRatio = static_cast<float>(normalizedKernel.cols)/static_cast<float>(normalizedKernel.rows);
    cv::Mat kernelResized;
    cv::resize(normalizedKernel, kernelResized, cv::Size(static_cast<int>(canvasSize.height*aspectRatio), canvasSize.height));
    // calculate x position to copy into canvas
    auto xPos = static_cast<int>((canvasSize.width - kernelResized.cols) / 2.0);
    // copy into canvas
    kernelResized.copyTo(output({xPos, 0, kernelResized.cols, kernelResized.rows}));

    cv::putText(output, "min: "+std::to_string(minValue), {0,20}, 1, 1.0, cv::Scalar::all(255));
    cv::putText(output, "max: "+std::to_string(maxValue), {0,40}, 1, 1.0, cv::Scalar::all(255));
    return output;
}

void Convolution::processImage(const cv::Mat& image)
{
    mosaic.reset();

    cv::Size size(getConfiguration().getIntParameter("horizontal_size"),
            getConfiguration().getIntParameter("vertical_size"));
    float radians = degreesToRadians(getConfiguration().getIntParameter("theta_degrees"));
    float sigma_stddev = getConfiguration().getFloatParameter("sigma_stddev");
    float lambda_wavelength = getConfiguration().getFloatParameter("lambda_wavelength");
    float gamma_aspect_ratio = getConfiguration().getFloatParameter("gamma_aspect_ratio");
    float psi_phase_offset = getConfiguration().getFloatParameter("psi_phase_offset");
    cv::Mat kernel = cv::getGaborKernel(size, sigma_stddev, radians, lambda_wavelength,
            gamma_aspect_ratio, psi_phase_offset);

    cv::Mat outputImage;
    cv::filter2D(image, outputImage, CV_32F, kernel);

    // show kernel
    mosaic.addImage(createImageDebugKernel(kernel, image.size()), 0, 0, "");

    // show output
    cv::normalize(outputImage, outputImage, 0, 255, cv::NORM_MINMAX);
    outputImage.convertTo(outputImage, CV_8U);
    mosaic.addImage(jetColorFrom(outputImage), 1, 0, "");
    mosaic.addImage(jetColorFrom(image), 1, 1, "original");

    setDebugImage(mosaic.createMosaic());
    setPostProcessedImage(outputImage);
}

