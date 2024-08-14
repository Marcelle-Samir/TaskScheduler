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
        : action(action), priority(pri), duration(dur),
        totalDuration(dur), remainingTime(dur), isTaskComplete(std::make_shared<std::atomic<bool>>(false))
        {
        std::cout << __FUNCTION__ << " is Called." << std::endl;
        static int taskCounter = 0;
        id = taskCounter++;
        threadPtr = std::make_unique<std::thread>([this, action]() mutable
        {
            while (true) {
                startPromise.get_future().wait();
                action();
                *isTaskComplete = true;
                resetPromise();
            }
        });
    }

    Task(Task&& other) noexcept
        : id(other.id),
          priority(other.priority),
          action(std::move(other.action)),
          execution_time(std::move(other.execution_time)),
          duration(other.duration),
          startPromise(std::move(other.startPromise)),
          threadPtr(std::move(other.threadPtr)),
          isTaskComplete(std::move(other.isTaskComplete))
          {
          }

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
            isTaskComplete = std::move(other.isTaskComplete);
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
        std::cout << __FUNCTION__ << " is Called." << std::endl;
        if (!isTaskComplete)
        {
            std::cout << "Task completed successfully.\n";
        }
    }

    void start()
    {
        std::cout << __FUNCTION__ << " is Called." << std::endl;
        startPromise.set_value();
        std::this_thread::sleep_for(std::chrono::milliseconds(totalDuration));
        *isTaskComplete = true;
        end();
    }

    int getDuration() const
    {
        std::cout << __FUNCTION__ << " is Called." << std::endl;
        return totalDuration;
    }

    int getPriority() const
    {
        std::cout << __FUNCTION__ << " is Called." << std::endl;
        return Taskpriority;
    }

    void runFor(int timeSlice)
    {
        std::cout << __FUNCTION__ << " is Called." << std::endl;
        if (remainingTime > 0)
        {
            int timeToRun = std::min(timeSlice, remainingTime);
            std::this_thread::sleep_for(std::chrono::milliseconds(timeToRun));
            remainingTime -= timeToRun;

            if (remainingTime <= 0)
            {
                *isTaskComplete = true;
                end();
            }
        }
    }

    bool isComplete() const
    {
        std::cout << __FUNCTION__ << " is Called." << std::endl;
        return *isTaskComplete;
    }

private:
    int totalDuration;
    int remainingTime;
    int Taskpriority;
    std::shared_ptr<std::atomic<bool>> isTaskComplete;
    void resetPromise()
    {
        startPromise = std::promise<void>();  // Reset the promise for the next execution
    }
};

#endif //TASK_H