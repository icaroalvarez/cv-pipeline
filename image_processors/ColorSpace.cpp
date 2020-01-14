#include <opencv2/imgproc.hpp>
#include "ColorSpace.h"

ColorSpace::ColorSpace()
:ImageProcessor("color_space")
{
    getConfiguration().addOptionsParameter("output", {"hls_l_channel"}, 0);
}

void ColorSpace::processImage(const cv::Mat& image)
{
    if(image.channels() < 3)
    {
        throw std::invalid_argument("Not 3 channels image received in color space processor");
    }
    mosaic.reset();

    // rgb
    std::vector<cv::Mat> bgr_channels;
    cv::split(image, bgr_channels);
    mosaic.addImage(bgr_channels.at(2), 0, 0, "R");
    mosaic.addImage(bgr_channels.at(1), 1, 0, "G");
    mosaic.addImage(bgr_channels.at(0), 2, 0, "B");

    // hsv
    cv::Mat hsv;
    cv::cvtColor(image, hsv, CV_BGR2HSV);
    std::vector<cv::Mat> hsv_channels;
    cv::split(hsv, hsv_channels);
    mosaic.addImage(hsv_channels.at(0), 0, 1, "H");
    mosaic.addImage(hsv_channels.at(1), 1, 1, "S");
    mosaic.addImage(hsv_channels.at(2), 2, 1, "V");

    // hsl
    cv::Mat hls;
    cv::cvtColor(image, hls, CV_BGR2HLS);
    std::vector<cv::Mat> hls_channels;
    cv::split(hls, hls_channels);
    mosaic.addImage(hls_channels.at(0), 0, 2, "H");
    mosaic.addImage(hls_channels.at(1), 1, 2, "L");
    mosaic.addImage(hls_channels.at(2), 2, 2, "S");

    // lab
    cv::Mat lab;
    cv::cvtColor(image, lab, CV_BGR2Lab);
    std::vector<cv::Mat> lab_channels;
    cv::split(lab, lab_channels);
    mosaic.addImage(lab_channels.at(0), 0, 3, "L");
    mosaic.addImage(lab_channels.at(1), 1, 3, "A");
    mosaic.addImage(lab_channels.at(2), 2, 3, "B");


    setDebugImage(mosaic.createMosaic());
    setPostProcessedImage(mosaic.createMosaic());

    if(getConfiguration().getOptionsParameter("output") == "hls_l_channel")
    {
        setPostProcessedImage(hls_channels.at(1));
    }
}
