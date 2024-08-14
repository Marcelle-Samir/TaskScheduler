#include <gtest/gtest.h>
#include "../src/HitachiScheduler.h"
#include <chrono>
#include <thread>

void TaskAction() {
    std::cout << "Task Executed" << std::endl;
}

class HitachiSchedulerTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(HitachiSchedulerTest, SetAlgorithm) {
    std::unique_ptr<SchedulingAlgorithm> roundRobinScheduler = std::make_unique<RoundRobin>();
    HitachiScheduler scheduler(std::make_unique<FCFS>());

    scheduler.setAlgorithm(std::move(roundRobinScheduler));

    EXPECT_NO_THROW(scheduler.startScheduler());
}

TEST_F(HitachiSchedulerTest, AddTaskFCFS) {
    std::unique_ptr<SchedulingAlgorithm> fcfsScheduler = std::make_unique<FCFS>();
    HitachiScheduler scheduler(std::move(fcfsScheduler));

    auto task = std::make_unique<Task>(TaskAction, 1, 5);
    scheduler.addTask(std::move(task));

    EXPECT_NO_THROW(scheduler.startScheduler());
}

TEST_F(HitachiSchedulerTest, AddTaskRoundRobin) {
    std::unique_ptr<SchedulingAlgorithm> roundRobinScheduler = std::make_unique<RoundRobin>();
    HitachiScheduler scheduler(std::move(roundRobinScheduler));

    auto task = std::make_unique<Task>(TaskAction, 1, 5);
    scheduler.addTask(std::move(task));

    EXPECT_NO_THROW(scheduler.startScheduler());
}

TEST_F(HitachiSchedulerTest, AddTaskSJN) {
    std::unique_ptr<SchedulingAlgorithm> sjnScheduler = std::make_unique<SJN>();
    HitachiScheduler scheduler(std::move(sjnScheduler));

    auto task = std::make_unique<Task>(TaskAction, 1, 5);
    scheduler.addTask(std::move(task));

    EXPECT_NO_THROW(scheduler.startScheduler());
}

TEST_F(HitachiSchedulerTest, AddTaskPriorityScheduling) {
    std::unique_ptr<SchedulingAlgorithm> priorityScheduler = std::make_unique<PriorityScheduling>();
    HitachiScheduler scheduler(std::move(priorityScheduler));

    auto task = std::make_unique<Task>(TaskAction, 1, 5);
    scheduler.addTask(std::move(task));

    EXPECT_NO_THROW(scheduler.startScheduler());
}

TEST_F(HitachiSchedulerTest, ExecuteTasksFCFS) {
    std::unique_ptr<SchedulingAlgorithm> fcfsScheduler = std::make_unique<FCFS>();
    HitachiScheduler scheduler(std::move(fcfsScheduler));

    for (int i = 0; i < 3; ++i) {
        scheduler.addTask(std::make_unique<Task>([i]() { std::cout << "Task " << i << " Executed" << std::endl; }, i, 5));
    }

    EXPECT_NO_THROW(scheduler.startScheduler());
}

TEST_F(HitachiSchedulerTest, ExecuteTasksRoundRobin) {
    std::unique_ptr<SchedulingAlgorithm> roundRobinScheduler = std::make_unique<RoundRobin>();
    HitachiScheduler scheduler(std::move(roundRobinScheduler));

    for (int i = 0; i < 3; ++i) {
        scheduler.addTask(std::make_unique<Task>([i]() { std::cout << "Task " << i << " Executed" << std::endl; }, i, 5));
    }

    EXPECT_NO_THROW(scheduler.startScheduler());
}

TEST_F(HitachiSchedulerTest, ExecuteTasksSJN) {
    std::unique_ptr<SchedulingAlgorithm> sjnScheduler = std::make_unique<SJN>();
    HitachiScheduler scheduler(std::move(sjnScheduler));

    for (int i = 0; i < 3; ++i) {
        scheduler.addTask(std::make_unique<Task>([i]() { std::cout << "Task " << i << " Executed" << std::endl; }, i, 5));
    }

    EXPECT_NO_THROW(scheduler.startScheduler());
}

TEST_F(HitachiSchedulerTest, ExecuteTasksPriorityScheduling) {
    std::unique_ptr<SchedulingAlgorithm> priorityScheduler = std::make_unique<PriorityScheduling>();
    HitachiScheduler scheduler(std::move(priorityScheduler));

    for (int i = 0; i < 3; ++i) {
        scheduler.addTask(std::make_unique<Task>([i]() { std::cout << "Task " << i << " Executed" << std::endl; }, i, 5));
    }

    EXPECT_NO_THROW(scheduler.startScheduler());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
