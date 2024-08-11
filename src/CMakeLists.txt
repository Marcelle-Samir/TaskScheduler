project(HitachiPlugin)

set(sources
    HitachiPlugin.cpp
)

set(headers
    HitachiPlugin.h
)

add_library(${PROJECT_NAME} SHARED ${headers} ${sources})

target_link_libraries(${PROJECT_NAME} PRIVATE HitachiPlugin::HitachiPlugin)
target_include_directories(${PROJECT_NAME}
        PRIVATE HitachiPlugin::HitachiPlugin
        PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
        )
Hitachi_PLUGIN(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} ${HitachiPlugin_TARGET_TYPE} DESTINATION plug-ins/${HitachiPlugin_VERSION})