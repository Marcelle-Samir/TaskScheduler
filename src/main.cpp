#include <iostream>
#include "HitachiScheduler.h"

int main() {
    HitachiScheduler scheduler;
    std::function<void()> action;
    Task task1 = new Task(1, action);
    Task task2 = new Task(2, action);
    Task task3 = new Task(3, action);
    task1.execution_time = std::chrono::system_clock::now() + std::chrono::seconds(5);
    task2.execution_time = std::chrono::system_clock::now() + std::chrono::seconds(3);
    task3.execution_time = std::chrono::system_clock::now() + std::chrono::seconds(1);

    scheduler.addTask(task1);
    scheduler.addTask(task2);
    scheduler.addTask(task3);

    // Start the execution thread
    std::thread execution_thread(&HitachiScheduler::executeTasks, &scheduler);

    // Simulate a running system for 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Stop the scheduler
    scheduler.stopScheduler();
    execution_thread.join();

    return 0;
}