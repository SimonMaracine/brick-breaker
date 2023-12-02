cmake_minimum_required(VERSION 3.20)

project("brick-breaker" LANGUAGES CXX C)

add_library(glad STATIC "extern/glad/src/glad.c")

target_include_directories(glad PUBLIC "extern/glad/include")

set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)

add_subdirectory(extern/SDL EXCLUDE_FROM_ALL)
add_subdirectory(extern/entt)
add_subdirectory(extern/glm)

add_executable(brick-breaker
    "src/engine/application_properties.hpp"
    "src/engine/application.cpp"
    "src/engine/application.hpp"
    "src/engine/buffer.cpp"
    "src/engine/buffer.hpp"
    "src/engine/engine.hpp"
    "src/engine/events.hpp"
    "src/engine/framebuffer.cpp"
    "src/engine/framebuffer.hpp"
    "src/engine/input.cpp"
    "src/engine/input.hpp"
    "src/engine/logging.cpp"
    "src/engine/logging.hpp"
    "src/engine/panic.hpp"
    "src/engine/renderer.cpp"
    "src/engine/renderer.hpp"
    "src/engine/scene.cpp"
    "src/engine/scene.hpp"
    "src/engine/shader.cpp"
    "src/engine/shader.hpp"
    "src/engine/texture.cpp"
    "src/engine/texture.hpp"
    "src/engine/vertex_array.cpp"
    "src/engine/vertex_array.hpp"
    "src/engine/vertex_buffer_layout.cpp"
    "src/engine/vertex_buffer_layout.hpp"
    "src/engine/window.cpp"
    "src/engine/window.hpp"
    "src/main.cpp"
)

target_link_libraries(brick-breaker PRIVATE SDL2::SDL2-static glad EnTT::EnTT glm::glm)

target_include_directories(brick-breaker PRIVATE "src")

if(UNIX)
    target_compile_options(brick-breaker PRIVATE "-Wall" "-Wextra" "-Wpedantic")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(brick-breaker PRIVATE "/W3")
else()
    message(WARNING "Compiler is not MSVC or platform is not supported, warnings are not enabled")
endif()

target_compile_features(brick-breaker PRIVATE cxx_std_17)
set_target_properties(brick-breaker PROPERTIES CXX_EXTENSIONS OFF)
