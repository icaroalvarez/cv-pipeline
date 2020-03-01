#include "Notifier.h"

void Notifier::notifyObservers()
{
    for(auto& observer : observerList)
    {
        observer->update();
    }
}

void Notifier::registerObserver(Observer* observer)
{
    observerList.push_back(observer);
}