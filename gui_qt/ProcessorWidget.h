#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <PipelineController.h>
#include "qlabelmat.h"
#include "qwidgethandler.h"

class ProcessorWidget: public QWidget {
    Q_OBJECT
public:
    ProcessorWidget(PipelineController *controller, int indexTab);

    void setDebugImage(const cv::Mat& image);

private:
    PipelineController *controller;
    QComboBox comboBoxProcessors;
    QString processorName;
    QWidget configurationWidget;
    QLabelMat imageLabel;
    QWidgetHandler widgetHandler;
    int indexTab;

public slots:
    void configureProcessor(QString paramName, QVariant value);
};
