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

    scheduler.stopScheduler();

    return 0;
}
