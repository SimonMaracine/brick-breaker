cmake_minimum_required(VERSION 3.20)

add_executable(brick-breaker
    "src/ball.cpp"
    "src/ball.hpp"
    "src/collision.cpp"
    "src/collision.hpp"
    "src/events.hpp"
    "src/level.cpp"
    "src/level.hpp"
    "src/main.cpp"
    "src/my_camera_controller.cpp"
    "src/my_camera_controller.hpp"
    "src/paddle.cpp"
    "src/paddle.hpp"
)

target_link_libraries(brick-breaker PRIVATE bb-engine)
