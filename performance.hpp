#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <chrono>
#include <vector>

using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;
using std::chrono::time_point;

struct PerformanceMeasure
{
    nanoseconds accumulated = nanoseconds::zero();
    time_point<high_resolution_clock> startTime;
    bool running = false;

    void start()
    {
        if (running)
            throw std::logic_error("Clock is already running.");
        running = true;
        startTime = std::chrono::high_resolution_clock::now();
    }

    void stop()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        if (!running)
            throw std::logic_error("Clock is not running");
        running = false;
        accumulated += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    }
};

PerformanceMeasure mathmeasure;
PerformanceMeasure clipMeasure;
PerformanceMeasure passMeasure;
PerformanceMeasure optMeasure;

#endif