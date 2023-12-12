cmake_minimum_required(VERSION 3.20)

add_executable(teapot
    "src/main.cpp"
    "src/my_camera_controller.cpp"
    "src/my_camera_controller.hpp"
)

target_link_libraries(teapot PRIVATE bb-engine)
