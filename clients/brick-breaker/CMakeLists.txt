cmake_minimum_required(VERSION 3.20)

add_subdirectory(extern/json)

add_executable(brick-breaker
    "src/ball.hpp"
    "src/brick.hpp"
    "src/collision.cpp"
    "src/collision.hpp"
    "src/constants.hpp"
    "src/events.hpp"
    "src/id_generator.hpp"
    "src/level.cpp"
    "src/level.hpp"
    "src/main.cpp"
    "src/menu.cpp"
    "src/menu.hpp"
    "src/my_camera_controller.cpp"
    "src/my_camera_controller.hpp"
    "src/orb.hpp"
    "src/paddle.hpp"
)

target_link_libraries(brick-breaker PRIVATE bb-engine nlohmann_json)

set_property(TARGET brick-breaker PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
