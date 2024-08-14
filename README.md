# Hitachi_Task

## Real-Time Task Scheduler
a real-time task scheduler in C++ that manages and executes tasks based on their
priority and timing constraints. The scheduler supports adding, removing, and executing tasks
according to the following scheduling algorithms.
- First-Come, First-Served (FCFS)
- Shortest Job Next (SJN)
- Priority Scheduling
- Round-Robin (with time quantum)
The algorithm can be selected in the run time by creating a pointer to an object of the algorithm type you want to select, \
then pass this pointer as an argument to the scheduler, as shown in the following lines:

    > std::unique_ptr<SchedulingAlgorithm> fcfsScheduler = std::make_unique<FCFS>(); \
    > HitachiScheduler scheduler(std::move(fcfsScheduler));

### Building and running
This scheduler was implemented using CMake as the build system in a dev-container.
the devcontainer configs are already part of this repo, so you can use it to build without worring about any dependencies.

- After cloning the repo reopen it in the devcontainer.
- Create a directory (ex. build).
- Change your current directory to it:
  > cd ./build
- run the following command for the CMake scripts:
  > cmake ..
- run the following command to build the code:
  > make

Now you have every thing and time to start the scheduler:
  > ./HitachiScheduler

Then to run the unit tests:
  > ./HitachiScheduler_test

**Attached to the repo in the testsAndRunLogs Directory the unit tests output and the code output.