#include "HoughLines.h"
#include "opencv2/imgproc.hpp"
#include "Utils.h"

constexpr auto modeParamName{"mode"};
constexpr auto standardModeName{"standard"};
constexpr auto probabilisticModeName{"probabilistic"};
constexpr auto thresholdParamName{"threshold"};
constexpr auto minThetaParamName{"min_theta_degrees"};
constexpr auto maxThetaParamName{"max_theta_degrees"};
constexpr auto minLineLengthParamName{"min_line_length"};
constexpr auto maxLineGapParamName{"max_line_gap"};
constexpr auto rho{1.0};
constexpr auto theta{CV_PI/180};

HoughLines::HoughLines()
:ImageProcessor("hough_lines")
{
    getParameters().registerParameter(modeParamName,
            OptionsParameter{0, {standardModeName, probabilisticModeName}});
    getParameters().registerParameter(thresholdParamName,
                                      IntegerParameter{30, 0,255});
    getParameters().registerParameter(minThetaParamName,
                                      IntegerParameter{0, 0,360});
    getParameters().registerParameter(maxThetaParamName,
                                      IntegerParameter{360, 0,360});
    getParameters().registerParameter(minLineLengthParamName,
                                      IntegerParameter{0, 0, std::numeric_limits<int>::max()});
    getParameters().registerParameter(maxLineGapParamName,
                                      IntegerParameter{0, 0,std::numeric_limits<int>::max()});
}

std::vector<cv::Vec4i> filterLinesByAngle(std::vector<cv::Vec4i> lines, double minAngleDegrees, double maxAngleDegrees)
{
    std::vector<cv::Vec4i> outputLines;
    for (auto &line : lines) {
        double angle = fabs(atan2(line[1] - line[3], line[0] - line[2]));
        if(angle >= Utils::degreesToRadians(minAngleDegrees) &&
           angle <= Utils::degreesToRadians(maxAngleDegrees))
        {
            outputLines.push_back(line);
        }
    }
    return outputLines;
}

cv::Mat HoughLines::processStandardMode(const cv::Mat &image)
{
    const auto threshold{getParameters().getParameterValue<int>(thresholdParamName)};
    const auto minThetaDegrees{getParameters().getParameterValue<int>(minThetaParamName)};
    const auto minThetaRadians{Utils::degreesToRadians(minThetaDegrees)};
    const auto maxThetaDegrees{getParameters().getParameterValue<int>(maxThetaParamName)};
    const auto maxThetaRadians{std::max(Utils::degreesToRadians(maxThetaDegrees), minThetaRadians+0.01)};

    std::vector<cv::Vec2f> lines;
    cv::HoughLines(image, lines, rho, theta, threshold, 0, 0, minThetaRadians, maxThetaRadians);

    cv::Mat debugImage{image.size(), CV_8UC3};
    cv::cvtColor(image, debugImage, cv::COLOR_GRAY2BGR);
    for(const auto& line : lines)
    {
        const auto lineRho{line[0]};
        const auto lineTheta{line[1]};
        const auto a{cos(lineTheta)};
        const auto b{sin(lineTheta)};
        const auto x0{a*lineRho};
        const auto y0{b*lineRho};
        const auto xMax{image.cols*10};
        cv::Point leftPoint, rightPoint;
        leftPoint.x = cvRound(x0 + xMax * -b);
        leftPoint.y = cvRound(y0 + xMax * a);
        rightPoint.x = cvRound(x0 - xMax * -b);
        rightPoint.y = cvRound(y0 - xMax * a);
        cv::line(debugImage, leftPoint, rightPoint, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
    }
    cv::putText(debugImage, std::to_string(lines.size()),
                cv::Point(0, image.rows), 1, 2.0,
                cv::Scalar::all(255), 1,cv::LINE_AA);
    return debugImage;
}

cv::Mat HoughLines::processProbabilisticMode(const cv::Mat &image)
{
    const auto threshold{getParameters().getParameterValue<int>(thresholdParamName)};
    const auto minLineLength{getParameters().getParameterValue<int>(minLineLengthParamName)};
    const auto maxLineGap{getParameters().getParameterValue<int>(maxLineGapParamName)};
    const auto minThetaDegrees{getParameters().getParameterValue<int>(minThetaParamName)};
    const auto maxThetaDegrees{getParameters().getParameterValue<int>(maxThetaParamName)};

    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(image, lines, rho, theta, threshold, minLineLength, maxLineGap);
    auto filteredLines = filterLinesByAngle(lines, minThetaDegrees, maxThetaDegrees);

    cv::Mat debugImage{image.size(), CV_8UC3};
    cv::cvtColor(image, debugImage, cv::COLOR_GRAY2BGR);
    for(auto& line : filteredLines)
    {
        cv::line(debugImage, cv::Point(line[0], line[1]),
                 cv::Point(line[2], line[3]),
                 cv::Scalar(0,0,255), 1, cv::LINE_AA);
    }

    cv::putText(debugImage, std::to_string(lines.size()),
                cv::Point(0, image.rows), 1, 2.0,
                cv::Scalar::all(255), 1,cv::LINE_AA);
    return debugImage;
}

cv::Mat HoughLines::processImage(const cv::Mat &image)
{
    const auto imageIsEightBitsSingleChannel{image.type() == CV_8UC1};
    if(not imageIsEightBitsSingleChannel)
    {
        throw std::runtime_error("Error in hough lines processor: input image is not 8 bit single channel.");
    }

    cv::Mat debugImage;
    const auto mode{getParameters().getParameterValue<std::size_t>(modeParamName)};
    const auto isStandardMode{mode == 0};
    if(isStandardMode)
    {
        debugImage = processStandardMode(image);
    }else
    {
        debugImage = processProbabilisticMode(image);
    }

    setDebugImage(debugImage);
    return image;
}