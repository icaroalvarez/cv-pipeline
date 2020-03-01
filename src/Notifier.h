#include "Observer.h"
#include <list>
#include <memory>

/**
 * Responsibility: to notify all the observers registered
 */
class Notifier
{
public:
    void notifyObservers();

    void registerObserver(Observer* observer);

private:
    std::list<Observer*> observerList;
};