#ifndef QLABELMAT_H
#define QLABELMAT_H

#include <QLabel>
#include <opencv2/core.hpp>

class QLabelMat: public QLabel
{
public:
    explicit QLabelMat(QWidget* parent = nullptr);
    void setImage(const cv::Mat& image);

public slots:
    void resizeEvent(QResizeEvent *) override;

private:
    QImage qImage;
    void showFrame();
    QImage createQImageFrom(cv::Mat image);
};

#endif // QLABELMAT_H
