#pragma once

#include <QMainWindow>
#include <QtCore/QVariant>
#include "PipelineController.h"
#include "qwidgethandler.h"
#include "FrameSourceNavigationWidget.h"
#include <QtWidgets/QSplitter>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow, public Observer
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow() override;
    void update() override;

private:
    Ui::MainWindow *ui;
    PipelineController controller;
    QTabWidget tabWidget;
    FrameSourceNavigationWidget frameSourceNavigation;
};
