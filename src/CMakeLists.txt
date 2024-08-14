add_executable(${PROJECT_NAME}
    HitachiScheduler
    HitachiScheduler.h
    HitachiScheduler.cpp
    SchedulingAlgorithm.h
    SchedulingAlgorithm.cpp
    Task.h
    Task.cpp
    main.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(${CMAKE_PROJECT_NAME} pthread)