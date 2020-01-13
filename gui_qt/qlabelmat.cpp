#include "qlabelmat.h"
#include <opencv2/imgproc.hpp>
#include <QDebug>

QLabelMat::QLabelMat(QWidget *parent):
    QLabel(parent)
{
    this->setMinimumSize(1, 1);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

void QLabelMat::resizeEvent(QResizeEvent * e)
{
    if(!qImage.isNull())
    {
        showFrame();
    }
}


void QLabelMat::setImage(const cv::Mat& image)
{
    qImage = createQImageFrom(image);
    if(!qImage.isNull())
    {
        showFrame();
    }
}

QImage QLabelMat::createQImageFrom(cv::Mat image)
{
    QImage qImage;
    if(image.channels() > 1)
    {
        cv::cvtColor(image, image, CV_BGR2RGB);
        qImage = QImage((const uchar*) image.data, image.cols,
                        image.rows, static_cast<int>(image.step), QImage::Format_RGB888);
        qImage.bits();
    }else
    {
        qImage = QImage((const uchar*) image.data, image.cols,
                        image.rows, static_cast<int>(image.step), QImage::Format_Indexed8);
        qImage.bits();
    }
    return qImage;
}

void QLabelMat::showFrame()
{
    setPixmap(QPixmap::fromImage(qImage).scaled(QSize(this->width(), this->height()),
                                              Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
