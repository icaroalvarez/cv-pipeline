#include "Notifier.h"

void Notifier::notifyObservers()
{
    for(const auto& observer : observerList)
    {
        observer->update();
    }
}

void Notifier::registerObserver(std::shared_ptr<Observer> observer)
{
    observerList.push_back(std::move(observer));
}