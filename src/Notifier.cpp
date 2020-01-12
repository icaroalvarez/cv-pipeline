#include "Notifier.h"

void Notifier::notifyObservers()
{
    for(auto& observer : observerList)
    {
        observer->update();
    }
}

void Notifier::registerObserver(std::shared_ptr<Observer> observer)
{
    observerList.push_back(observer);
}

void Notifier::unregisterObserver(std::shared_ptr<Observer> observer)
{
    observerList.remove(observer);
}

std::size_t Notifier::numberOfObservers() const
{
    return observerList.size();
}