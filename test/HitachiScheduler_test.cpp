#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <vector>
#include <memory>
#include "../src/HitachiScheduler.h"

class HitachiSchedulerTest : public ::testing::Test {
protected:
    void SetUp() override {
        scheduler = std::make_unique<HitachiScheduler>();
        scheduler->startScheduler();
    }

    void TearDown() override {
        scheduler->stopScheduler();
    }

    std::unique_ptr<HitachiScheduler> scheduler;
};

// Test adding and executing a single task
// TEST_F(HitachiSchedulerTest, ExecuteSingleTask) {
//     bool executed = false;
//     scheduler->addTask({1, 1, std::chrono::system_clock::now() + std::chrono::seconds(1), [&]() {
//         executed = true;
//     }});

//     std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for the task to execute
//     ASSERT_TRUE(executed);
// }

// // Test adding and removing a task
// TEST_F(HitachiSchedulerTest, AddRemoveTask) {
//     bool executed = false;
//     scheduler->addTask({1, 1, std::chrono::system_clock::now() + std::chrono::seconds(2), [&]() {
//         executed = true;
//     }});

//     scheduler->removeTask(1);
//     std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait longer than task execution time
//     ASSERT_FALSE(executed); // Task should not be executed
// }

// // Test executing multiple tasks
// TEST_F(HitachiSchedulerTest, ExecuteMultipleTasks) {
//     int counter = 0;
//     scheduler->addTask({1, 1, std::chrono::system_clock::now() + std::chrono::seconds(1), [&]() {
//         counter++;
//     }});
//     scheduler->addTask({2, 2, std::chrono::system_clock::now() + std::chrono::seconds(1), [&]() {
//         counter++;
//     }});

//     std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for tasks to execute
//     ASSERT_EQ(counter, 2);
// }

// // Test task execution order
// TEST_F(HitachiSchedulerTest, TaskExecutionOrder) {
//     std::vector<int> executionOrder;
//     scheduler->addTask({1, 1, std::chrono::system_clock::now() + std::chrono::seconds(2), [&]() {
//         executionOrder.push_back(1);
//     }});
//     scheduler->addTask({2, 2, std::chrono::system_clock::now() + std::chrono::seconds(1), [&]() {
//         executionOrder.push_back(2);
//     }});

//     std::this_thread::sleep_for(std::chrono::seconds(3)); // Wait for tasks to execute
//     ASSERT_EQ(executionOrder.size(), 2);
//     ASSERT_EQ(executionOrder[0], 2);
//     ASSERT_EQ(executionOrder[1], 1);
// }
