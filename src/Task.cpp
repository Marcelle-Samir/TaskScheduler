#include "Task.h"

Task::Task(std::function<void()> action, int pri, int dur)
    : action(action), priority(pri), duration(dur),
    remainingTime(dur), isTaskComplete(std::make_shared<std::atomic<bool>>(false))
    {
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    static int taskCounter = 0;
    id = taskCounter++;
}
Task::~Task()
{
    if (threadPtr && threadPtr->joinable())
    {
        threadPtr->join();
    }
}
void Task::end()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    if (isTaskComplete)
    {
        std::cout << "Task completed successfully.\n";
    }
}
void Task::start()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    action();
}

int Task::getDuration()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    return duration;
}

int Task::getPriority()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    return priority;
}

void Task::runFor(int timeSlice)
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    fflush(NULL);
    if (remainingTime > 0)
    {
        int timeToRun = std::min(timeSlice, remainingTime);
        std::this_thread::sleep_for(std::chrono::milliseconds(timeToRun));
        remainingTime -= timeToRun;
        if (remainingTime <= 0)
        {
            *isTaskComplete = true;
            end();
        }
    }
}
bool Task::isComplete() const
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    return *isTaskComplete;
}
