#include "qwidgethandler.h"
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include "easylogging++.h"
using json = nlohmann::json;

void QWidgetHandler::receiveInt(int value)
{
    QObject* pObject = sender();
    configureProcessor(pObject->objectName(), value);
}

void QWidgetHandler::receiveDouble(double value)
{
    QObject* pObject = sender();
    configureProcessor(pObject->objectName(), value);
}

void QWidgetHandler::receiveBool(bool value)
{
    QObject* pObject = sender();
    configureProcessor(pObject->objectName(), value);
}

void QWidgetHandler::receiveOption(int option_selected)
{
    QObject* pObject = sender();
    configureProcessor(pObject->objectName(), option_selected);
}

void QWidgetHandler::addIntControlTo(QLayout *layout, std::string name, int value, int minValue, int maxValue) {
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    QLabel *label = new QLabel(QString::fromStdString(name));
    QSpinBox *spinBox = new QSpinBox();
    spinBox->setMaximum(maxValue);
    spinBox->setMinimum(minValue);
    spinBox->setValue(value);
    spinBox->QObject::setObjectName(QString::fromStdString(name));

    QObject::connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(receiveInt(int)));
    hBoxLayout->addWidget(label);
    hBoxLayout->addWidget(spinBox);
    QWidget *widgetInt = new QWidget();
    widgetInt->setLayout(hBoxLayout);
    layout->addWidget(widgetInt);
}

void QWidgetHandler::addFloatControlTo(QLayout *layout, std::string name, float value, float minValue,
                                       float maxValue, float step, int decimals) {
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    QLabel *label = new QLabel(QString::fromStdString(name));
    QDoubleSpinBox *spinBox = new QDoubleSpinBox();
    spinBox->setDecimals(decimals);
    spinBox->setMaximum(maxValue);
    spinBox->setMinimum(minValue);
    spinBox->setSingleStep(step);
    spinBox->setValue(value);
    spinBox->QObject::setObjectName(QString::fromStdString(name));

    QObject::connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(receiveDouble(double)));
    hBoxLayout->addWidget(label);
    hBoxLayout->addWidget(spinBox);
    QWidget *widget = new QWidget();
    widget->setLayout(hBoxLayout);
    layout->addWidget(widget);
}

void QWidgetHandler::addBooleanControlTo(QLayout *layout, std::string name, bool value) {
    QCheckBox *checkbox = new QCheckBox(QString::fromStdString(name));
    checkbox->setChecked(value);
    checkbox->QObject::setObjectName(QString::fromStdString(name));
    QObject::connect(checkbox, SIGNAL(toggled(bool)), this, SLOT(receiveBool(bool)));
    layout->addWidget(checkbox);
}

void QWidgetHandler::addOptionsControlTo(QLayout *layout, std::string name,
        std::vector<std::string> options, int selected)
{
    QComboBox *comboBox = new QComboBox();
    for (auto& option : options) {
        comboBox->addItem(QString::fromStdString(option));
        comboBox->QObject::setObjectName(QString::fromStdString(name));
        comboBox->setCurrentIndex(selected);
        QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(receiveOption(int)));
        layout->addWidget(comboBox);
    }
}

QWidget* QWidgetHandler::createQWidgetFromJson(json configuration)
{
    LOG(INFO) << "Creating QWidget from configuration: " << configuration;
    QWidget *widget = new QWidget();
    widget->setLayout(new QVBoxLayout());

    for(auto &param : configuration)
    {
        if(param["type"] == "integer")
        {
            addIntControlTo(widget->layout(), param["name"], param["value"],
                             param["min_value"], param["max_value"]);
        }else if(param["type"] == "float")
        {
            addFloatControlTo(widget->layout(), param["name"], param["value"],
                              param["min_value"], param["max_value"], param["step"], param["decimals"]);
        }else if(param["type"] == "boolean")
        {
            addBooleanControlTo(widget->layout(), param["name"], param["value"]);
        }else if(param["type"] == "options")
        {
            addOptionsControlTo(widget->layout(), param["name"], param["options"], param["value"]);
        }else{
            std::string paramType = param["type"];
            throw std::invalid_argument("Unknown parameter type: " + paramType);
        }
    }
    return widget;
}
