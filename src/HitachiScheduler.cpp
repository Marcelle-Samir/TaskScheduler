#include "HitachiScheduler.h"
#include <iostream>

void HitachiScheduler::addTask(const Task& task) {
    taskQueue.push(task);
    cv.notify_one();
}

void HitachiScheduler::removeTask(int taskId) {
    std::lock_guard<std::mutex> lock(queueMutex);
    std::priority_queue<Task> newQueue;
    while (!taskQueue.empty()) {
        auto task = taskQueue.top();
        taskQueue.pop();
        if (task.id != taskId) {
            newQueue.push(task);
        }
    }
    std::swap(taskQueue, newQueue);
}

void HitachiScheduler::executeTasks() {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (stop) break;

        if (!taskQueue.empty()) {
            auto nextTask = taskQueue.top();
            if (cv.wait_until(lock, nextTask.execution_time, [this, &nextTask] {
                    return stop || std::chrono::system_clock::now() >= nextTask.execution_time;
                })) {
                if (!stop) {
                    taskQueue.pop();
                    lock.unlock();
                    try {
                        nextTask.action();
                    } catch (const std::exception& e) {
                        std::cerr << "Exception caught during task execution: " << e.what() << std::endl;
                    } catch (...) {
                        std::cerr << "Unknown exception caught during task execution." << std::endl;
                    }
                }
            }
        } else {
            cv.wait(lock);
        }
    }
}

void HitachiScheduler::startScheduler() {
    workerThread = std::thread(&HitachiScheduler::executeTasks, this);
}

void HitachiScheduler::stopScheduler() {
            {
            std::lock_guard<std::mutex> lock(queueMutex);
            stop = true;
        }
        cv.notify_all();
        if (workerThread.joinable()) {
            workerThread.join();
        }
}
