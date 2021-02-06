#include <opencv2/imgproc.hpp>
#include "Convolution.h"
#include "Utils.h"

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
    getParameters().registerParameter("horizontal_size",
            IntegerParameter{5, 1, 99});
    getParameters().registerParameter("vertical_size",
            IntegerParameter{20, 1, 99});
    getParameters().registerParameter("theta_degrees",
            IntegerParameter{0, 0, 360});
    getParameters().registerParameter("sigma_stddev",
            DecimalParameter{1.0, 0.0, 99.9, 0.1, 1});
    getParameters().registerParameter("lambda_wavelength",
            DecimalParameter{ 4.8, 0.0, 99.9, 0.1, 1});
    getParameters().registerParameter("gamma_aspect_ratio",
            DecimalParameter{0.f, 0.0, 99.9, 0.1, 1});
    getParameters().registerParameter("psi_phase_offset",
            DecimalParameter{0.f, 0.0, 99.9, 0.1, 1});
}

cv::Mat createImageDebugKernel(const cv::Mat& kernel, const cv::Size& canvasSize)
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

cv::Mat Convolution::processImage(const cv::Mat& image)
{
    mosaic.reset();
    cv::Size size(getParameters().getParameterValue<int>("horizontal_size"),
                  getParameters().getParameterValue<int>("vertical_size"));
    const auto radians{Utils::degreesToRadians(getParameters().getParameterValue<int>("theta_degrees"))};
    const auto sigma_stddev{getParameters().getParameterValue<double>("sigma_stddev")};
    const auto lambda_wavelength{getParameters().getParameterValue<double>("lambda_wavelength")};
    const auto gamma_aspect_ratio{getParameters().getParameterValue<double>("gamma_aspect_ratio")};
    const auto psi_phase_offset{getParameters().getParameterValue<double>("psi_phase_offset")};
    cv::Mat kernel = cv::getGaborKernel(size, sigma_stddev, radians, lambda_wavelength,
            gamma_aspect_ratio, psi_phase_offset);

    cv::Mat outputImage;
    cv::filter2D(image, outputImage, CV_32F, kernel);
    mosaic.addImage(createImageDebugKernel(kernel, image.size()), 0, 0, "");

    cv::normalize(outputImage, outputImage, 0, 255, cv::NORM_MINMAX);
    outputImage.convertTo(outputImage, CV_8U);
    mosaic.addImage(jetColorFrom(outputImage), 1, 0, "");
    mosaic.addImage(jetColorFrom(image), 1, 1, "original");

    setDebugImage(mosaic.createMosaic());
    return outputImage;
}

