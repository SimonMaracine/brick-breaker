cmake_minimum_required(VERSION 3.20)

add_executable(teapot
    "src/main.cpp"
    "src/my_camera_controller.cpp"
    "src/my_camera_controller.hpp"
)

target_link_libraries(teapot PRIVATE bb-engine)

set_property(TARGET teapot PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
