# Hitachi_Task

C++ Senior Software Engineer Task
Task: Implement a Real-Time Task Scheduler
Objective: Develop a real-time task scheduler in C++ that manages and executes tasks based on their
priority and timing constraints. The scheduler should support adding, removing, and executing tasks
according to specific scheduling algorithms. The task is designed to assess your understanding of data
structures, algorithms, and concurrency in C++.
Requirements

1. Core Functionality:
o Task Representation: Implement a Task class that represents a unit of work. Each task
should have a unique ID, priority level, and execution time.
o Scheduler Implementation: Implement a TaskScheduler class that manages and
executes tasks based on their priority and timing constraints. The scheduler should
support at least the following scheduling algorithms:
 First-Come, First-Served (FCFS)
 Shortest Job Next (SJN)
 Priority Scheduling
 Round-Robin (with time quantum)
o Concurrency: The scheduler should be able to execute tasks in parallel using threads.
The number of threads should be configurable.
o Task Management:
 Add Task: Add a new task to the scheduler.
 Remove Task: Remove a task from the scheduler before it starts executing.
 Execute Tasks: Start the task execution based on the selected scheduling
algorithm.

2. Technical Requirements:
o Use C++17 or later.
o Implement the solution using CMake as the build system.
o Write unit tests for your code using GoogleTest.
o Use Conan for dependency management if any external libraries are needed.
o Use std::thread or std::async for concurrency.

o Use appropriate data structures (e.g., priority queues, queues) to implement the
scheduling algorithms.

3. Advanced Requirements (Bonus Points):
o Dynamic Task Scheduling: Implement the ability to dynamically change the scheduling
algorithm while tasks are being executed.
o Logging and Monitoring: Add logging functionality to track task scheduling and
execution. Provide basic performance metrics (e.g., average wait time, CPU utilization).
o Task Dependencies: Implement support for task dependencies, where a task can only
start after one or more other tasks have completed.

4. Optional (Extra Bonus):
o Task Persistence: Implement functionality to save the current state of the scheduler and
tasks to disk and load it back into memory, allowing the scheduler to resume from
where it left off.
o Custom Memory Management: Implement custom memory management for the tasks
to optimize memory usage and allocation speed.

Task Submission
 Submit your solution as a Git repository (you can use GitHub, GitLab, or any other platform). The
repository should include:
o Source code for the application.
o Unit tests.
o CMake scripts.
o Instructions for setting up the development environment, building the application, and
running the tests.
o (Optional) Any additional features you implemented as per the advanced and optional
requirements.

Time Frame
 The task should be completed within 4 days.

Evaluation Criteria
 Code Quality: Clean, maintainable, and well-documented code.
 Scheduling Algorithms: Correct and efficient implementation of scheduling algorithms.
 Concurrency Handling: Correct and efficient handling of concurrency with minimal performance
overhead.
 Testing: Comprehensive and effective unit tests.
 Use of Tools: Proper use of CMake, GoogleTest, and Conan.
 Advanced Features: Implementation of any advanced features will be considered a plus.
 Optimization: Efficient use of memory and processing resources.