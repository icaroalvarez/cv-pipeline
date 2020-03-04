#pragma once
#include <thread>
#include <condition_variable>
#include <atomic>

/**
 * Responsibility: manages to execute a piece of code in a single thread which is in sleep mode between iterations.
 * It starts a thread when created and waits until a single iteration is requested to be executed.
 * When the iteration is finished the thread waits in sleep mode until the next iteration is required.
 * It discards any request received while executing an iteration.
 */
class Runnable {
public:
    Runnable();
    virtual ~Runnable();
    void fireIteration();

private:
    void run();
    void start();
    void stop();
    virtual void runIteration() = 0;

    std::thread thread;
    std::mutex threadMutex;
    std::condition_variable conditionRunIteration;
    std::atomic_bool stopThread;
    bool iterationFired;
};



