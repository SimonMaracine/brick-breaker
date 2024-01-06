cmake_minimum_required(VERSION 3.20)

add_executable(minimal "src/main.cpp")

target_link_libraries(minimal PRIVATE bb-engine)

set_property(TARGET minimal PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

set_warnings_and_standard(minimal)
