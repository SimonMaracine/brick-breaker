cmake_minimum_required(VERSION 3.20)

add_subdirectory(extern/json)

add_executable(brick-breaker
    "src/about.cpp"
    "src/about.hpp"
    "src/ball.hpp"
    "src/brick.hpp"
    "src/collision.cpp"
    "src/collision.hpp"
    "src/constants.hpp"
    "src/events.hpp"
    "src/id_generator.hpp"
    "src/level.cpp"
    "src/level.hpp"
    "src/levels.cpp"
    "src/levels.hpp"
    "src/main.cpp"
    "src/menu_selection.hpp"
    "src/menu.cpp"
    "src/menu.hpp"
    "src/my_camera_controller.cpp"
    "src/my_camera_controller.hpp"
    "src/orb.hpp"
    "src/paddle.hpp"
)

target_link_libraries(brick-breaker PRIVATE bb-engine nlohmann_json)

set_property(TARGET brick-breaker PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

set_warnings_and_standard(brick-breaker)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(brick-breaker PRIVATE "/utf-8")
endif()
