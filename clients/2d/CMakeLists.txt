cmake_minimum_required(VERSION 3.20)

add_executable(2d
    "src/draw.hpp"
    "src/draw.cpp"
    "src/main_scene.hpp"
    "src/main_scene.cpp"
    "src/main.cpp"
)

target_link_libraries(2d PRIVATE bb-engine)

set_property(TARGET 2d PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

set_warnings_and_standard(2d)
