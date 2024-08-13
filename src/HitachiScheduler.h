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
    virtual void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv) = 0;
    virtual void schedule(std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, TaskComparator>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv) = 0;
    virtual void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::condition_variable& cv) = 0;
};

class FCFS : public SchedulingAlgorithm
{
public:
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv) override
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [&taskQueue]() { return !taskQueue.empty(); });
        if (!taskQueue.empty())
        {
            std::unique_ptr<Task> task = std::move(const_cast<std::unique_ptr<Task>&>(taskQueue.front()));
            taskQueue.pop();
            lock.unlock();
            task->start();
            cv.notify_one();
        }
    }
    void schedule(std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, TaskComparator>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv)  override
    {
        // not required
    }
    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::condition_variable& cv)
    {
        // not required
    }

};

class RoundRobin : public SchedulingAlgorithm
{
public:
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv) override
    {
        static const int timeSlice = 100;
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [&taskQueue](){ return !taskQueue.empty(); });

        if (!taskQueue.empty())
        {
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
    void schedule(std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, TaskComparator>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv)  override
    {
        // not required
    }
    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::condition_variable& cv)    override
    {
        // not required
    }
};

class SJN : public SchedulingAlgorithm
{
public:
    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::condition_variable& cv) override
    {
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
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv) override
    {
        // not required
    }
    void schedule(std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, TaskComparator>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv)  override
    {
        // not required
    }
};

class PriorityScheduling : public SchedulingAlgorithm
{
public:
    void schedule(std::vector<std::unique_ptr<Task>>& taskList, std::mutex& queueMutex, std::condition_variable& cv) override
    {
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
    void schedule(std::queue<std::unique_ptr<Task>>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv) override
    {
        // not required
    }
    void schedule(std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, TaskComparator>& taskQueue, std::mutex& queueMutex, std::condition_variable& cv)    override
    {
        // not required
    }
};

class HitachiScheduler {
public:
    HitachiScheduler(std::unique_ptr<SchedulingAlgorithm> algo)
        : schedulingAlgorithm(std::move(algo)), stop(false)
    {
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
    bool stop;
    std::thread workerThread;
    std::unique_ptr<SchedulingAlgorithm> schedulingAlgorithm;
};

#endif // HITACHISCHEDULER_H
