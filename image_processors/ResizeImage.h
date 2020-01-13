//
// Created by ialvarez on 7/03/18.
//

#ifndef PLAYGROUND_RESIZEIMAGE_H
#define PLAYGROUND_RESIZEIMAGE_H

#include "ImageProcessor.h"

class ResizeImage: public ImageProcessor {
public:
    ResizeImage();
    void processImage(cv::Mat image) override;
};

#endif //PLAYGROUND_RESIZEIMAGE_H
