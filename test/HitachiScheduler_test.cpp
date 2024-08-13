#include <gtest/gtest.h>
#include "../src/HitachiScheduler.h"
#include <chrono>
#include <thread>

class SchedulerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // to be added
    }

    void TearDown() override
    {
        // to be added
    }
};

TEST_F(SchedulerTest, FCFS_ScheduleTest)
{
    auto fcfsScheduler = std::make_unique<FCFS>();
    HitachiScheduler scheduler(std::move(fcfsScheduler));

    std::promise<void> task1Promise;
    std::promise<void> task2Promise;
    bool task1Executed = false;
    bool task2Executed = false;

    scheduler.addTask(std::make_unique<Task>([&]()
    {
        task1Executed = true;
        task1Promise.set_value();
    }, 1));

    scheduler.addTask(std::make_unique<Task>([&]()
    {
        task2Executed = true;
        task2Promise.set_value();
    }, 2));

    task1Promise.get_future().wait();
    task2Promise.get_future().wait();

    EXPECT_TRUE(task1Executed);
    EXPECT_TRUE(task2Executed);
}

TEST_F(SchedulerTest, RoundRobin_ScheduleTest)
{
    auto roundRobinScheduler = std::make_unique<RoundRobin>();
    HitachiScheduler scheduler(std::move(roundRobinScheduler));

    std::promise<void> task1Promise;
    std::promise<void> task2Promise;
    bool task1Executed = false;
    bool task2Executed = false;

    scheduler.addTask(std::make_unique<Task>([&]()
    {
        task1Executed = true;
        task1Promise.set_value();
    }, 1));

    scheduler.addTask(std::make_unique<Task>([&]()
    {
        task2Executed = true;
        task2Promise.set_value();
    }, 2));

    task1Promise.get_future().wait();
    task2Promise.get_future().wait();

    EXPECT_TRUE(task1Executed);
    EXPECT_TRUE(task2Executed);
}

TEST_F(SchedulerTest, SJN_ScheduleTest)
{
    auto sjnScheduler = std::make_unique<SJN>();
    HitachiScheduler scheduler(std::move(sjnScheduler));

    std::promise<void> task1Promise;
    std::promise<void> task2Promise;
    bool task1Executed = false;
    bool task2Executed = false;

    scheduler.addTask(std::make_unique<Task>([&]()
    {
        task1Executed = true;
        task1Promise.set_value();
    }, 1, 5));

    scheduler.addTask(std::make_unique<Task>([&]()
    {
        task2Executed = true;
        task2Promise.set_value();
    }, 2, 3));

    task1Promise.get_future().wait();
    task2Promise.get_future().wait();

    EXPECT_TRUE(task1Executed);
    EXPECT_TRUE(task2Executed);
}

TEST_F(SchedulerTest, PriorityScheduling_ScheduleTest)
{
    auto priorityScheduler = std::make_unique<PriorityScheduling>();
    HitachiScheduler scheduler(std::move(priorityScheduler));

    std::promise<void> task1Promise;
    std::promise<void> task2Promise;
    bool task1Executed = false;
    bool task2Executed = false;

    scheduler.addTask(std::make_unique<Task>([&]()
    {
        task1Executed = true;
        task1Promise.set_value();
    }, 1));

    scheduler.addTask(std::make_unique<Task>([&]()
    {
        task2Executed = true;
        task2Promise.set_value();
    }, 2));

    task1Promise.get_future().wait();
    task2Promise.get_future().wait();

    EXPECT_TRUE(task1Executed);
    EXPECT_TRUE(task2Executed);
}

TEST_F(SchedulerTest, HitachiScheduler_Test)
{
    auto fcfsScheduler = std::make_unique<FCFS>();
    HitachiScheduler scheduler(std::move(fcfsScheduler));

    std::promise<void> task1Promise;
    bool task1Executed = false;

    scheduler.addTask(std::make_unique<Task>([&]()
    {
        task1Executed = true;
        task1Promise.set_value();
    }, 1));

    task1Promise.get_future().wait();
    EXPECT_TRUE(task1Executed);

    auto roundRobinScheduler = std::make_unique<RoundRobin>();
    scheduler.setAlgorithm(std::move(roundRobinScheduler));

    std::promise<void> task2Promise;
    bool task2Executed = false;

    scheduler.addTask(std::make_unique<Task>([&]()
    {
        task2Executed = true;
        task2Promise.set_value();
    }, 2));

    task2Promise.get_future().wait();
    EXPECT_TRUE(task2Executed);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
