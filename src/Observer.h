#pragma once
#include <memory>

class Observer
{
public:
    virtual void update() = 0;
};