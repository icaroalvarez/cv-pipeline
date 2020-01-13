#include <iostream>
#include <QHBoxLayout>
#include "ProcessorWidget.h"
#include "easylogging++.h"

using json = nlohmann::json;

ProcessorWidget::ProcessorWidget(PipelineController *controller, int indexTab)
        :controller(controller), indexTab(indexTab)
{
    // set vertical layout
    this->setLayout(new QVBoxLayout());

    // add image label
    this->layout()->addWidget(&imageLabel);

    // get processor configuration
    auto configuration = controller->getProcessorConfigurationFrom(static_cast<unsigned int>(indexTab));

    // add configuration widget
    this->layout()->addWidget(widgetHandler.createQWidgetFromJson(configuration));

    // connect configuration widget
    QObject::connect(&widgetHandler, SIGNAL(configureProcessor(QString, QVariant)),
                     this, SLOT(configureProcessor(QString, QVariant)));

    // process current image
    controller->processCurrentImage();
}

void ProcessorWidget::configureProcessor(QString paramName, QVariant value)
{
    json parameter{{"name", paramName.toStdString()}};

    if(!std::string(value.typeName()).compare("int"))
    {
        parameter["value"] = value.toInt();
    }else if(!std::string(value.typeName()).compare("double"))
    {
        parameter["value"] = value.toFloat();
    }else if(!std::string(value.typeName()).compare("bool"))
    {
        parameter["value"] = value.toBool();
    }else if(!std::string(value.typeName()).compare("QString"))
    {
        parameter["value"] = value.toInt();
    }else{
        throw std::invalid_argument("Processor widget receive unknown parameter: "+std::string(value.typeName()));
    }

    json configuration;
    configuration.push_back(parameter);

    // configure processor
    controller->configureProcessor(static_cast<unsigned int>(indexTab), configuration);

    // fire new process
    controller->processCurrentImage();
}

void ProcessorWidget::setDebugImage(const cv::Mat& image) {
    imageLabel.setImage(image);
}