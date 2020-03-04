#include "Observer.h"
#include <list>
#include <memory>

/**
 * Responsibility: notify all the observers registered
 */
class Notifier
{
public:
    void notifyObservers();

    void registerObserver(std::shared_ptr<Observer> observer);

private:
    std::list<std::shared_ptr<Observer>> observerList;
};