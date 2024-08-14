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
    std::promise<void> startPromise;
    std::unique_ptr<std::thread> threadPtr;
    std::chrono::system_clock::time_point execution_time;
    std::function<void()> action;
    int duration;

    Task(std::function<void()> action, int pri, int dur = 0)
        : action(action), priority(pri), duration(dur)
        {
        static int taskCounter = 0;
        id = taskCounter++;
        threadPtr = std::make_unique<std::thread>([this, action]() mutable
        {
            while (true) {
                startPromise.get_future().wait();
                action();
                startPromise = std::promise<void>();
            }
        });
    }

    Task(Task&& other) noexcept
        : id(other.id), priority(other.priority),
          action(other.action),
          execution_time(other.execution_time),
          duration(other.duration),
          startPromise(std::move(other.startPromise)),
          threadPtr(std::move(other.threadPtr)) {}

    Task& operator=(Task&& other) noexcept
    {
        if (this != &other)
        {
            id = other.id;
            priority = other.priority;
            action = other.action;
            execution_time = other.execution_time;
            duration = other.duration;
            startPromise = std::move(other.startPromise);
            threadPtr = std::move(other.threadPtr);
        }
        return *this;
    }

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    ~Task()
    {
        if (threadPtr && threadPtr->joinable())
        {
            threadPtr->join();
        }
    }

    void end()
    {
        if (!isTaskComplete)
        {
            std::cout << "Task completed successfully.\n";
        }
    }

    void start()
    {
        startPromise.set_value();
        std::this_thread::sleep_for(std::chrono::milliseconds(totalDuration));
        isTaskComplete = true;
        end();
    }

    int getDuration() const
    {
        return totalDuration;
    }

    int getPriority() const
    {
        return Taskpriority;
    }

    void runFor(int timeSlice)
    {
        if (remainingTime > 0)
        {
            int timeToRun = std::min(timeSlice, remainingTime);
            std::this_thread::sleep_for(std::chrono::milliseconds(timeToRun));
            remainingTime -= timeToRun;

            if (remainingTime <= 0)
            {
                isTaskComplete = true;
                end();
            }
        }
    }

    bool isComplete() const
    {
        return isTaskComplete;
    }

private:
    int totalDuration;
    int remainingTime;
    int Taskpriority;
    std::atomic<bool> isTaskComplete;
};

#endif //TASK_H