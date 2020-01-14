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

    std::string path("/home/ialvarez/Code/panoramix/test/fixtures/Lenna.png");
    controller.loadImage(path);

    MainWindow window(&controller);
    window.show();
    window.setWindowState(Qt::WindowMaximized);

    return a.exec();
}