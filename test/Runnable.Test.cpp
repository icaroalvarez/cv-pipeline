#include <catch2/catch.hpp>
#include <trompeloeil.hpp>
#include <iostream>
#include "Runnable.h"
using namespace std::chrono_literals;

class MockRunnable: public Runnable
{
    MAKE_MOCK0(runIteration, void(), override);
};

TEST_CASE("Can be created and destroyed and no iteration is executed")
{
    MockRunnable runnable;
    FORBID_CALL(runnable, runIteration());
}

TEST_CASE("Runs a single iteration")
{
    std::atomic_bool iterationExecuted(false);
    MockRunnable runnable;
    REQUIRE_CALL(runnable, runIteration())
            .LR_SIDE_EFFECT(iterationExecuted = true)
            .TIMES(1);

    runnable.fireIteration();

    while(not iterationExecuted)
    {
        std::this_thread::sleep_for(1ns);
    }
}

TEST_CASE("Discards any other requests while executing one iteration")
{
    std::atomic_bool iterationExecuting{false};
    std::atomic_bool iterationExecuted{false};
    std::atomic_bool secondIterationFired{false};
    MockRunnable runnable;
    REQUIRE_CALL(runnable, runIteration())
            .LR_SIDE_EFFECT(iterationExecuting = true)
            .LR_SIDE_EFFECT(while(not secondIterationFired){std::this_thread::sleep_for(1ns);})
            .LR_SIDE_EFFECT(iterationExecuted = true)
            .TIMES(1);

    runnable.fireIteration();
    while(not iterationExecuting)
    {
        std::this_thread::sleep_for(1ns);
    }

    for(int i=0; i<100; i++)
    {
        runnable.fireIteration();
    }

    secondIterationFired = true;
    while(not iterationExecuted)
    {
        std::this_thread::sleep_for(1ns);
    }
}

TEST_CASE("Fire more than one iterations")
{
    constexpr auto nIterations{10};
    std::atomic_bool iterationExecuted{false};
    MockRunnable runnable;
    REQUIRE_CALL(runnable, runIteration())
            .LR_SIDE_EFFECT(iterationExecuted = true)
            .TIMES(nIterations);

    for(int i=0; i < nIterations; i++)
    {
        runnable.fireIteration();
        while (not iterationExecuted)
        {
            std::this_thread::sleep_for(1ns);
        }
        iterationExecuted = false;
    }
}