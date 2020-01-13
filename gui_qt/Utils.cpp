#include <QtCore/QStringList>
#include "Utils.h"

QStringList Utils::fromStlVector(std::vector<std::string> stlVector)
{
    QStringList stringList;
    for(auto &text:stlVector)
    {
        stringList.push_back(QString::fromStdString(text));
    }
    return stringList;
}