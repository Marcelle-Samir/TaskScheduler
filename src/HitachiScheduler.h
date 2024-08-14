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
#include "SchedulingAlgorithm.h"

class HitachiScheduler {
public:
    HitachiScheduler(std::unique_ptr<SchedulingAlgorithm> algo);
    ~HitachiScheduler();

    void setAlgorithm(std::unique_ptr<SchedulingAlgorithm> algo);
    void addTask(std::unique_ptr<Task> task);
    void removeTask(int taskId);
    void stopScheduler();
    void startScheduler();
    void executeTasks();
    bool isStopped();

private:
    std::queue<std::unique_ptr<Task>> taskQueue;
    std::mutex queueMutex;
    std::atomic<bool> stop{false};
    std::thread workerThread;
    std::unique_ptr<SchedulingAlgorithm> schedulingAlgorithm;
    std::vector<std::shared_ptr<Task>> tasks;
    std::vector<std::unique_ptr<Task>> taskList;
};

#endif // HITACHISCHEDULER_H
