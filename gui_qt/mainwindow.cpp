#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ProcessorWidget.h"
#include "easylogging++.h"

MainWindow::MainWindow(PipelineController* pipelineController, QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        controller{pipelineController},
        frameSourceNavigation(controller)
{
    ui->setupUi(this);

    // create vertical layout
    ui->centralWidget->setLayout(new QHBoxLayout());
    ui->centralWidget->layout()->setSizeConstraint(QLayout::SetMinimumSize);

    // create tabs with each processors processor
    for(auto &processor : controller->getPipelineDescription())
    {
        // add processor widget
        auto* processorWidget = new ProcessorWidget(controller, tabWidget.count());
        tabWidget.addTab(processorWidget, QString::fromStdString(processor));
    }

    // add widgets to splitter
    auto *splitter = new QSplitter();
    splitter->addWidget(&frameSourceNavigation);
    splitter->addWidget(&tabWidget);
    ui->centralWidget->layout()->addWidget(splitter);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 5);

    // register main window as an observer of controller to be notified when image processing is finished
    controller->registerObserver(this);

    frameSourceNavigation.sliderReleased();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update() {
    for(int iTab = 0; iTab < tabWidget.count(); iTab++)
    {
        try {
            dynamic_cast<ProcessorWidget *>(tabWidget.widget(iTab))->setDebugImage(controller->getDebugImage(
                    static_cast<unsigned int>(iTab)));
        }catch(const std::exception& e)
        {
            LOG(WARNING) << e.what();
        }
    }
}
