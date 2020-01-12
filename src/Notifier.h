#include "Observer.h"
#include <list>
#include <memory>

class Notifier
{
public:

    void notifyObservers();

    void registerObserver(std::shared_ptr<Observer> observer);

    void unregisterObserver(std::shared_ptr<Observer> observer);

    std::size_t numberOfObservers() const;

private:
    std::list<std::shared_ptr<Observer>> observerList;
};