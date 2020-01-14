#include "mainwindow.h"
#include <QApplication>
#include <Registerer.h>
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    PipelineController controller;
    Registerer::registerProcessors(controller);
    const std::vector<std::string> pipeLineDescription{"roi",  "resize"};
    controller.loadPipeline(pipeLineDescription);

    MainWindow window(&controller);
    window.show();
    window.setWindowState(Qt::WindowMaximized);

    return a.exec();
}