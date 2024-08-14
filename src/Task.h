#ifndef TASK_H
#define TASK_H

#include <memory>
#include <queue>
#include <vector>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <functional>
#include <mutex>
#include <future>
#include <chrono>
#include <algorithm>

class Task
{
public:
    int id;
    int priority;
    std::unique_ptr<std::thread> threadPtr;
    std::chrono::system_clock::time_point execution_time;
    std::function<void()> action;
    int duration;

    Task(std::function<void()> action, int pri, int dur);

    ~Task();

    void end();

    void start();

    int getDuration();

    int getPriority();

    void runFor(int timeSlice);

    bool isComplete() const;

private:
    int remainingTime;
    std::shared_ptr<std::atomic<bool>> isTaskComplete;
};

#endif //TASK_H