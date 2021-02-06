#include "Utils.h"
#include <opencv4/opencv2/core/cvdef.h>

double Utils::degreesToRadians(double degrees)
{
    return degrees*CV_PI/180.0;
}
