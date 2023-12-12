cmake_minimum_required(VERSION 3.20)

add_library(glad STATIC "extern/glad/src/glad.c")

target_include_directories(glad PUBLIC "extern/glad/include")

set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)

add_subdirectory(extern/SDL)
add_subdirectory(extern/entt)
add_subdirectory(extern/glm)
add_subdirectory(extern/SDL_image)
add_subdirectory(extern/resmanager)
add_subdirectory(extern/assimp)

add_library(bb-engine
    "src/engine/application_properties.hpp"
    "src/engine/application.cpp"
    "src/engine/application.hpp"
    "src/engine/buffer.cpp"
    "src/engine/buffer.hpp"
    "src/engine/camera_controller.hpp"
    "src/engine/camera.cpp"
    "src/engine/camera.hpp"
    "src/engine/engine.hpp"
    "src/engine/events.hpp"
    "src/engine/framebuffer.cpp"
    "src/engine/framebuffer.hpp"
    "src/engine/info_and_debug.cpp"
    "src/engine/info_and_debug.hpp"
    "src/engine/input.cpp"
    "src/engine/input.hpp"
    "src/engine/light.hpp"
    "src/engine/logging.cpp"
    "src/engine/logging.hpp"
    "src/engine/material.cpp"
    "src/engine/material.hpp"
    "src/engine/mesh.cpp"
    "src/engine/mesh.hpp"
    "src/engine/opengl.cpp"
    "src/engine/opengl.hpp"
    "src/engine/panic.hpp"
    "src/engine/post_processing.hpp"
    "src/engine/renderable.hpp"
    "src/engine/renderer.cpp"
    "src/engine/renderer.hpp"
    "src/engine/scene.cpp"
    "src/engine/scene.hpp"
    "src/engine/shader.cpp"
    "src/engine/shader.hpp"
    "src/engine/texture_data.cpp"
    "src/engine/texture_data.hpp"
    "src/engine/texture.cpp"
    "src/engine/texture.hpp"
    "src/engine/vertex_array.cpp"
    "src/engine/vertex_array.hpp"
    "src/engine/vertex_buffer_layout.cpp"
    "src/engine/vertex_buffer_layout.hpp"
    "src/engine/window.cpp"
    "src/engine/window.hpp"
)

target_link_libraries(bb-engine PRIVATE
    SDL2::SDL2-static
    glad
    SDL2_image::SDL2_image-static
    assimp::assimp
)

target_link_libraries(bb-engine PUBLIC
    EnTT::EnTT
    glm::glm
    resmanager
)

target_include_directories(bb-engine PUBLIC "src")

if(UNIX)
    target_compile_options(bb-engine PRIVATE "-Wall" "-Wextra" "-Wpedantic")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(bb-engine PRIVATE "/W3")
else()
    message(WARNING "Compiler is not MSVC or platform is not supported, warnings are not enabled")
endif()

target_compile_features(bb-engine PRIVATE cxx_std_17)
set_target_properties(bb-engine PROPERTIES CXX_EXTENSIONS OFF)
