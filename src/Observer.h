#pragma once
#include <memory>

/**
 * Responsability: to be updated by the notifier after being registered as observer.
 * A class inheriting from this class can be notified by a notifier class.
 */
class Observer
{
public:
    /**
     * Method to be called by the notifier
     */
    virtual void update() = 0;
};