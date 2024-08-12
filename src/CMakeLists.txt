add_executable(${PROJECT_NAME}
	main.cpp
    HitachiPlugin.cpp
    HitachiPlugin.h
)

target_include_directories(${CMAKE_PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE project_options)
