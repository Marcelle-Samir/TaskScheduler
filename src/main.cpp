#include "HitachiScheduler.h"
#include <iostream>
#include <sched.h>
#include <unistd.h>

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
        scheduler.addTask(std::make_unique<Task>([i]() { taskAction(i); }, i, 2));
    }

    scheduler.startScheduler();
    while(!scheduler.isStopped())
    {
        sleep(1);
        sched_yield();
    }

    std::unique_ptr<SchedulingAlgorithm> roundRobinScheduler = std::make_unique<RoundRobin>();
    scheduler.setAlgorithm(std::move(roundRobinScheduler));

    for (int i = 5; i < 10; ++i)
    {
        scheduler.addTask(std::make_unique<Task>([i]() { taskAction(i); }, i, 5));
    }

    scheduler.startScheduler();
    while(!scheduler.isStopped())
    {
        sleep(1);
        sched_yield();
    }

    std::unique_ptr<SchedulingAlgorithm> sjnScheduler = std::make_unique<SJN>();
    scheduler.setAlgorithm(std::move(sjnScheduler));

    for (int i = 10; i < 15; ++i)
    {
        scheduler.addTask(std::make_unique<Task>([i]() { taskAction(i); }, i * 10, i * 2));
    }

    scheduler.startScheduler();
    while(!scheduler.isStopped())
    {
        sleep(1);
        sched_yield();
    }

    std::unique_ptr<SchedulingAlgorithm> prioritySchedulingScheduler = std::make_unique<PriorityScheduling>();
    scheduler.setAlgorithm(std::move(prioritySchedulingScheduler));

    for (int i = 15; i < 20; ++i)
    {
        scheduler.addTask(std::make_unique<Task>([i]() { taskAction(i); }, 20 - i, 3));
    }

    scheduler.startScheduler();
    while(!scheduler.isStopped())
    {
        sleep(1);
        sched_yield();
    }

    return 0;
}
