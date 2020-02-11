#include <iostream>
#include "Runnable.h"
using namespace std::chrono_literals;

Runnable::Runnable()
:stopThread{false}, iterationFired{false}
{
    start();
}

Runnable::~Runnable()
{
    stop();
}

void Runnable::fireIteration()
{
    std::unique_lock<std::mutex> lock(threadMutex, std::try_to_lock);
    if(lock.owns_lock())
    {
        iterationFired = true;
        conditionRunIteration.notify_one();
    }
}

void Runnable::run()
{
    while(not stopThread)
    {
        std::unique_lock<std::mutex> lock(threadMutex);
        conditionRunIteration.wait(lock, [&]{ return iterationFired; });
        iterationFired = false;

        if(not stopThread)
        {
            runIteration();
        }
    }
}

void Runnable::start()
{
    thread = std::thread([&]{run();});
}

void Runnable::stop()
{
    stopThread = true;
    fireIteration();
    if(thread.joinable())
    {
        thread.join();
    }
}