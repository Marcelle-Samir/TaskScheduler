#include "HitachiScheduler.h"
#include <iostream>

void taskAction(int id)
{
    std::cout << "Task " << id << " is being executed." << std::endl;
}

int main()
{
    std::unique_ptr<SchedulingAlgorithm> fcfsScheduler = std::make_unique<FCFS>();
    HitachiScheduler scheduler(std::move(fcfsScheduler));

    for (int i = 0; i < 5; ++i)
    {
        scheduler.addTask(std::make_unique<Task>([i]() { taskAction(i); }, i));
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::unique_ptr<SchedulingAlgorithm> roundRobinScheduler = std::make_unique<RoundRobin>();
    scheduler.setAlgorithm(std::move(roundRobinScheduler));

    for (int i = 5; i < 10; ++i)
    {
        scheduler.addTask(std::make_unique<Task>([i]() { taskAction(i); }, i));
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::unique_ptr<SchedulingAlgorithm> sjnScheduler = std::make_unique<SJN>();
    scheduler.setAlgorithm(std::move(sjnScheduler));

    for (int i = 10; i < 15; ++i)
    {
        scheduler.addTask(std::make_unique<Task>([i]() { taskAction(i); }, i * 10, i * 2));
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::unique_ptr<SchedulingAlgorithm> prioritySchedulingScheduler = std::make_unique<PriorityScheduling>();
    scheduler.setAlgorithm(std::move(prioritySchedulingScheduler));

    for (int i = 15; i < 20; ++i)
    {
        scheduler.addTask(std::make_unique<Task>([i]() { taskAction(i); }, 20 - i));
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    scheduler.stopScheduler();

    return 0;
}
