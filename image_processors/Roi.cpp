#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc.hpp>
#include "Roi.h"

RoiProcessor::RoiProcessor()
:ImageProcessor("roi")
{
    getConfiguration().addIntParameter("top_left_x", 116, 0, 9999);
    getConfiguration().addIntParameter("top_left_y", 60, 0, 9999);
    getConfiguration().addIntParameter("width", 127, 0, 9999);
    getConfiguration().addIntParameter("height", 161, 0, 9999);
}

void RoiProcessor::processImage(const cv::Mat &image)
{
    mosaic.reset();

    // x value
    int xValue = MAX(0, getConfiguration().getIntParameter("top_left_x"));
    xValue = MIN(xValue, image.cols-1);
    // y value
    int yValue = MAX(0, getConfiguration().getIntParameter("top_left_y"));
    yValue = MIN(yValue, image.rows-1);
    // width value
    int width = MAX(0,  getConfiguration().getIntParameter("width"));
    width = MIN(width, image.cols-xValue);
    // height value
    int height = MAX(0,  getConfiguration().getIntParameter("height"));
    height = MIN(height, image.rows-yValue);

    cv::Mat debugImage;
    if(image.channels() < 3)
    {
        cv::cvtColor(image, debugImage, CV_GRAY2BGR);
    }else{
        debugImage = image.clone();
    }
    cv::rectangle(debugImage, {xValue, yValue, width, height}, {0,255,0});
    mosaic.addImage(debugImage, 0, 0);

    cv::Mat outputImage = image({xValue, yValue, width, height}).clone();
    cv::Mat outputResized;
    cv::resize(outputImage, outputResized, image.size());
    mosaic.addImage(outputResized, 1, 0);
    setPostProcessedImage(outputImage);
    setDebugImage(mosaic.createMosaic());
}
