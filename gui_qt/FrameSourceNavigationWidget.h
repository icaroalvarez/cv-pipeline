#pragma once
#include <QtWidgets/QWidget>
#include <PipelineController.h>
#include <QtWidgets/QSlider>
#include <QObject>
#include "qlabelmat.h"

class FrameSourceNavigationWidget: public QWidget {
    Q_OBJECT
public:
    FrameSourceNavigationWidget(PipelineController *controller);
    void moveSliderRight();
    void moveSliderLeft();

private:
    QSlider slider;
    QLabel indexLabel;
    PipelineController *controller;
    QLabelMat originalImageLabel;

public slots:
    void sliderValueChanged(int value);
    void sliderReleased();

    void fireNewImage(int value);
};