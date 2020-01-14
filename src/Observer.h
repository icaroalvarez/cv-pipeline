#pragma once
#include <memory>

class Observer
{
public:
    /**
     * Method to be called by the notifier
     */
    virtual void update() = 0;
};