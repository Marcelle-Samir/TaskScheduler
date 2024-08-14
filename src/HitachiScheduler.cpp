#include "HitachiScheduler.h"
#include <iostream>

void HitachiScheduler::setAlgorithm(std::unique_ptr<SchedulingAlgorithm> algo)
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    std::lock_guard<std::mutex> lock(queueMutex);
    schedulingAlgorithm = std::move(algo);
}

void HitachiScheduler::addTask(std::unique_ptr<Task> task)
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    std::lock_guard<std::mutex> lock(queueMutex);
    if (dynamic_cast<FCFS*>(schedulingAlgorithm.get()) ||
        dynamic_cast<RoundRobin*>(schedulingAlgorithm.get()))
    {
        taskQueue.push(std::move(task));
    }
    else if (dynamic_cast<SJN*>(schedulingAlgorithm.get()) ||
               dynamic_cast<PriorityScheduling*>(schedulingAlgorithm.get()))
    {
        priorityQueue.push(std::move(task));
    }
    cv.notify_one();
}

void HitachiScheduler::removeTask(int taskId)
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    std::lock_guard<std::mutex> lock(queueMutex);
    // to be implemented
}

void HitachiScheduler::stopScheduler()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    stop = true;
    cv.notify_one();
}

void HitachiScheduler::startScheduler()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    if (stop)
    {
        stop = false;
        workerThread = std::thread(&HitachiScheduler::executeTasks, this);
    }
}

void HitachiScheduler::executeTasks()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    while (true) {
        if (stop) {
            std::cout << "Scheduler stopping..." << std::endl;
            break;
        }
        if (auto* fcfsAlgo = dynamic_cast<FCFS*>(schedulingAlgorithm.get()))
        {
            fcfsAlgo->schedule(taskQueue, queueMutex, cv, stop);
        }
        else if (auto* rrAlgo = dynamic_cast<RoundRobin*>(schedulingAlgorithm.get()))
        {
            rrAlgo->schedule(taskQueue, queueMutex, cv, stop);
        }
        else if (auto* sjnAlgo = dynamic_cast<SJN*>(schedulingAlgorithm.get()))
        {
            sjnAlgo->schedule(priorityQueue, queueMutex, cv, stop);
        }
        else if (auto* psAlgo = dynamic_cast<PriorityScheduling*>(schedulingAlgorithm.get()))
        {
            psAlgo->schedule(priorityQueue, queueMutex, cv, stop);
        }
    }
}
