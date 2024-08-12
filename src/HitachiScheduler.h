#ifndef HITACHISCHEDULER_H
#define HITACHISCHEDULER_H

#include <iostream>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <vector>
#include <future>

class Task {
public:
    int id;
    int priority;
    std::promise<void> startPromise;
    std::unique_ptr<std::thread> threadPtr;
    std::chrono::system_clock::time_point execution_time;
    std::function<void()> action;
    Task(std::function<void()> action, int pri) : action(action), priority(pri){
        static int taskCounter = 0;
        id = taskCounter++;
        threadPtr = std::make_unique<std::thread>([this]() mutable {
            while (true) {
                // Wait until startPromise is set
                startPromise.get_future().wait();

                // Run the action
                //action();

                // Reset the promise to allow reuse
                startPromise = std::promise<void>();
            }
        });
    }
    ~Task(){
        if (threadPtr && threadPtr->joinable()) {
            threadPtr->join();  // Ensure the thread is joined before destruction
        }
    }

    bool operator<(const Task& other) const {
        return execution_time > other.execution_time;
    }
};

//                         Task.startPromise.set_value();                        //

class HitachiScheduler {
public:
    HitachiScheduler() : stop(false) {
        workerThread = std::thread(&HitachiScheduler::executeTasks, this);
    }

    ~HitachiScheduler() {
        stopScheduler();
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }

    void addTask(const Task& task);
    void removeTask(int taskId);
    void stopScheduler();
    void startScheduler();
    void executeTasks();

private:
    std::priority_queue<Task> taskQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    bool stop;
    std::thread workerThread;

};

#endif // HITACHISCHEDULER_H
