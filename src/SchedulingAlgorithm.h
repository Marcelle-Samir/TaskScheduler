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
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::atomic<bool>& stop) override;

    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::atomic<bool>& stop);

};

class RoundRobin : public SchedulingAlgorithm
{
public:
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::atomic<bool>& stop) override;

    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::atomic<bool>& stop) override;
};

class SJN : public SchedulingAlgorithm
{
public:
    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::atomic<bool>& stop) override;
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::atomic<bool>& stop) override;
};

class PriorityScheduling : public SchedulingAlgorithm
{
public:
    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::atomic<bool>& stop) override;
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::atomic<bool>& stop) override;
};

#endif //SCHEDULINGALGORITHM_H