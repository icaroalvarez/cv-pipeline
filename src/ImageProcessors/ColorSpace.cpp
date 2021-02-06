#include <opencv2/imgproc.hpp>
#include "ColorSpace.h"

ColorSpace::ColorSpace()
:ImageProcessor("color_space")
{
    getParameters().registerParameter("output",
            OptionsParameter{0, {"hls_h_channel", "hls_l_channel", "hls_s_channel"}});
}

cv::Mat ColorSpace::processImage(const cv::Mat& image)
{
    if(image.channels() < 3)
    {
        throw std::invalid_argument("Not 3 channels image received in color space processor");
    }
    mosaic.reset();

    std::vector<cv::Mat> bgr_channels;
    cv::split(image, bgr_channels);
    mosaic.addImage(bgr_channels.at(2), 0, 0, "R");
    mosaic.addImage(bgr_channels.at(1), 1, 0, "G");
    mosaic.addImage(bgr_channels.at(0), 2, 0, "B");

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> hsv_channels;
    cv::split(hsv, hsv_channels);
    mosaic.addImage(hsv_channels.at(0), 0, 1, "H");
    mosaic.addImage(hsv_channels.at(1), 1, 1, "S");
    mosaic.addImage(hsv_channels.at(2), 2, 1, "V");

    cv::Mat hls;
    cv::cvtColor(image, hls, cv::COLOR_BGR2HLS);
    std::vector<cv::Mat> hls_channels;
    cv::split(hls, hls_channels);
    mosaic.addImage(hls_channels.at(0), 0, 2, "H");
    mosaic.addImage(hls_channels.at(1), 1, 2, "L");
    mosaic.addImage(hls_channels.at(2), 2, 2, "S");

    cv::Mat lab;
    cv::cvtColor(image, lab, cv::COLOR_BGR2Lab);
    std::vector<cv::Mat> lab_channels;
    cv::split(lab, lab_channels);
    mosaic.addImage(lab_channels.at(0), 0, 3, "L");
    mosaic.addImage(lab_channels.at(1), 1, 3, "A");
    mosaic.addImage(lab_channels.at(2), 2, 3, "B");

    setDebugImage(mosaic.createMosaic());

    cv::Mat outputImage{image};
    const auto selectedOption{getParameters().getParameterValue<SelectedOptionIndex>("output")};
    if(selectedOption == 0)
    {
        outputImage = hls_channels.at(0);
    } else if (selectedOption == 1)
    {
        outputImage = hls_channels.at(1);
    }else if(selectedOption == 2)
    {
        outputImage = hls_channels.at(2);
    }
    return outputImage;
}
