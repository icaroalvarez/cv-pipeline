#include "Observer.h"
#include <list>
#include <memory>

class Notifier
{
public:

    void notifyObservers();

    void registerObserver(Observer* observer);

    void unregisterObserver(Observer* observer);

    std::size_t numberOfObservers() const;

private:
    std::list<Observer*> observerList;
};