#pragma once

#include <QGridLayout>
#include <QObject>
#include <QScrollArea>
#include <ProcessorConfiguration.h>
#include <QtCore/QVariant>
#include <nlohmann/json.hpp>

class QWidgetHandler: public QObject
{
Q_OBJECT

public:
    QWidget* createQWidgetFromJson(nlohmann::json configuration);
    QWidget* createQWidget(nlohmann::json configuration);

private:
    void addFloatControlTo(QLayout *layout, std::string name, float value, float minValue,
                           float maxValue, float step, int decimals);

    void addIntControlTo(QLayout *layout, std::string name, int value, int minValue, int maxValue);

    void addBooleanControlTo(QLayout *layout, std::string name, bool value);

    void addOptionsControlTo(QLayout *layout, std::string name, std::vector<std::string> options, int selected);

public slots:
    void receiveInt(int value);
    void receiveDouble(double value);
    void receiveBool(bool value);
    void receiveOption(int option_selected);

signals:
    void configureProcessor(QString processorName, QVariant value);
};