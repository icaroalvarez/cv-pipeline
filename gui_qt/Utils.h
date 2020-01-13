#pragma once
#include <vector>

class Utils {
public:
    static QStringList fromStlVector(std::vector<std::string> stlVector);
};