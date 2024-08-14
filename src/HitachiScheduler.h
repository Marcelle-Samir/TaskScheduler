#ifndef HITACHISCHEDULER_H
#define HITACHISCHEDULER_H

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

struct TaskComparator
{
    bool operator()(const std::unique_ptr<Task>& lhs, const std::unique_ptr<Task>& rhs) const
    {
        return lhs->priority < rhs->priority;
    }
};

class SchedulingAlgorithm
{
public:
    virtual ~SchedulingAlgorithm() = default;
    virtual void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop) = 0;
    virtual void schedule(std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, TaskComparator>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop) = 0;
    virtual void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop) = 0;
};

class FCFS : public SchedulingAlgorithm
{
public:
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop) override
    {
        std::cout << __FUNCTION__ << " is Called in FCFS Algorithm." << std::endl;
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [&taskQueue, &stop]() { return stop || !taskQueue.empty(); });
        if (stop && taskQueue.empty())
        {
            return;
        }
        if (!taskQueue.empty())
        {
            std::cout << "taskQueue is not empty." << std::endl;
            std::unique_ptr<Task> task = std::move(const_cast<std::unique_ptr<Task>&>(taskQueue.front()));
            taskQueue.pop();
            lock.unlock();
            task->start();
            cv.notify_one();
        }
    }
    void schedule(std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, TaskComparator>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop)  override
    {
        // not required
    }
    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop)
    {
        // not required
    }

};

class RoundRobin : public SchedulingAlgorithm
{
public:
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop) override
    {
        std::cout << __FUNCTION__ << " is Called in RoundRobin Algorithm." << std::endl;
        static const int timeSlice = 100;
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [&taskQueue](){ return !taskQueue.empty(); });

        if (!taskQueue.empty())
        {
            std::cout << "taskQueue is not empty." << std::endl;
            std::unique_ptr<Task> task = std::move(taskQueue.front());
            taskQueue.pop();
            taskQueue.push(std::move(task));
            lock.unlock();
            task->runFor(timeSlice);
            if (!task->isComplete())
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                taskQueue.push(std::move(task));
            }
            else
            {
                task->end();
            }
            taskQueue.back()->start();
            cv.notify_one();
        }
    }
    void schedule(std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, TaskComparator>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop)  override
    {
        // not required
    }
    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop)    override
    {
        // not required
    }
};

class SJN : public SchedulingAlgorithm
{
public:
    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop) override
    {
        std::cout << __FUNCTION__ << " is Called in SJN Algorithm." << std::endl;
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [&taskList]() { return !taskList.empty(); });
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
    }
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop) override
    {
        // not required
    }
    void schedule(std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, TaskComparator>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop)  override
    {
        // not required
    }
};

class PriorityScheduling : public SchedulingAlgorithm
{
public:
    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop) override
    {
        std::cout << __FUNCTION__ << " is Called in PriorityScheduling Algorithm." << std::endl;
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [&taskList]() { return !taskList.empty(); });
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
    }
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop) override
    {
        // not required
    }
    void schedule(std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, TaskComparator>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv, std::atomic<bool>& stop)    override
    {
        // not required
    }
};

class HitachiScheduler {
public:
    HitachiScheduler(std::unique_ptr<SchedulingAlgorithm> algo)
        : schedulingAlgorithm(std::move(algo)), stop(false), stopFlag(false)
    {
        std::cout << __FUNCTION__ << " is Called." << std::endl;
        workerThread = std::thread(&HitachiScheduler::executeTasks, this);
    }

    ~HitachiScheduler()
    {
        stopScheduler();
        if (workerThread.joinable())
        {
            workerThread.join();
        }
    }

    void setAlgorithm(std::unique_ptr<SchedulingAlgorithm> algo);
    void addTask(std::unique_ptr<Task> task);
    void removeTask(int taskId);
    void stopScheduler();
    void startScheduler();
    void executeTasks();

private:
    std::queue<std::unique_ptr<Task>> taskQueue;
    std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, TaskComparator> priorityQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::atomic<bool> stop{false};
    std::thread workerThread;
    std::unique_ptr<SchedulingAlgorithm> schedulingAlgorithm;
    std::vector<std::shared_ptr<Task>> tasks;
    std::vector<std::thread> threads;
    std::atomic<bool> stopFlag;
};

#endif // HITACHISCHEDULER_H
