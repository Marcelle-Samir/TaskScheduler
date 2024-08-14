#ifndef SCHEDULINGALGORITHM_H
#define SCHEDULINGALGORITHM_H

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
#include "Task.h"

class SchedulingAlgorithm
{
public:
    virtual ~SchedulingAlgorithm() = default;
    virtual void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::atomic<bool>& stop) = 0;
    virtual void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::atomic<bool>& stop) = 0;
};

class FCFS : public SchedulingAlgorithm
{
public:
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::atomic<bool>& stop) override
    {
        std::cout << __FUNCTION__ << " is Called in FCFS Algorithm." << std::endl;
        std::unique_lock<std::mutex> lock(queueMutex);
        if (taskQueue.empty())
        {
            std::cout << "taskQueue is empty." << std::endl;
            stop = true;
        }
        if (!taskQueue.empty())
        {
            std::cout << "taskQueue is not empty." << std::endl;
            std::unique_ptr<Task> task = std::move(const_cast<std::unique_ptr<Task>&>(taskQueue.front()));
            taskQueue.pop();
            lock.unlock();
            task->start();
        }
    }

    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::atomic<bool>& stop)
    {
        // not required
    }

};

class RoundRobin : public SchedulingAlgorithm
{
public:
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::atomic<bool>& stop) override
    {
        std::cout << __FUNCTION__ << " is Called in RoundRobin Algorithm." << std::endl;
        static const int timeSlice = 1;
        std::unique_lock<std::mutex> lock(queueMutex);
        if (!taskQueue.empty())
        {
            std::cout << "taskQueue is not empty." << std::endl;
            std::unique_ptr<Task> task = std::move(taskQueue.front());
            taskQueue.pop();
            lock.unlock();
            task->runFor(timeSlice);
            if (!task->isComplete())
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                std::cout << "Task is not completed!" << std::endl;
                taskQueue.push(std::move(task));
            }
            else
            {
                std::cout << "Task is completed!" << std::endl;
                task->end();
            }
        }
        else
        {
            std::cout << "taskQueue is empty." << std::endl;
            stop = true;
        }
    }

    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::atomic<bool>& stop)    override
    {
        // not required
    }
};

class SJN : public SchedulingAlgorithm
{
public:
    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::atomic<bool>& stop) override
    {
        std::cout << __FUNCTION__ << " is Called in SJN Algorithm." << std::endl;
        std::unique_lock<std::mutex> lock(queueMutex);
        auto shortestTaskIt = std::min_element(taskList.begin(), taskList.end(),
                                           [](const std::unique_ptr<Task>& a, const std::unique_ptr<Task>& b)
                                           {
                                               return a->getDuration() < b->getDuration();
                                           });

        if (shortestTaskIt != taskList.end())
        {
            auto task = std::move(*shortestTaskIt);
            taskList.erase(shortestTaskIt);
            lock.unlock();
            task->start();
        }
        else
        {
            std::cout << "TaskList is empty." << std::endl;
            stop = true;
        }
    }
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::atomic<bool>& stop) override
    {
        std::cout << "is empty." << std::endl;
        // not required
    }
};

class PriorityScheduling : public SchedulingAlgorithm
{
public:
    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::atomic<bool>& stop) override
    {
        std::cout << __FUNCTION__ << " is Called in PriorityScheduling Algorithm." << std::endl;
        std::unique_lock<std::mutex> lock(queueMutex);
        auto highestPriorityTaskIt = std::min_element(taskList.begin(), taskList.end(),
                                                  [](const std::unique_ptr<Task>& a, const std::unique_ptr<Task>& b)
                                                  {
                                                      return a->getPriority() < b->getPriority();
                                                  });

        if (highestPriorityTaskIt != taskList.end())
        {
            auto task = std::move(*highestPriorityTaskIt);
            taskList.erase(highestPriorityTaskIt);
            lock.unlock();
            task->start();
        }
        else
        {
            std::cout << "taskList is empty." << std::endl;
            stop = true;
        }
    }
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::atomic<bool>& stop) override
    {
        // not required
    }
};

#endif //SCHEDULINGALGORITHM_H