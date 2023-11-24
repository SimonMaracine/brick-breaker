cmake_minimum_required(VERSION 3.20)

project("brick-breaker" LANGUAGES CXX)

add_subdirectory(extern/SDL)

add_executable(brick_breaker "src/main.cpp")

target_link_libraries(brick_breaker PRIVATE SDL2::SDL2-static)

if(UNIX)
    target_compile_options(brick_breaker PRIVATE "-Wall" "-Wextra" "-Wpedantic")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(brick_breaker PRIVATE "/W3")
else()
    message(WARNING "Compiler is not MSVC or platform is not supported, warnings are not enabled")
endif()

target_compile_features(brick_breaker PRIVATE cxx_std_17)
set_target_properties(brick_breaker PROPERTIES CXX_EXTENSIONS OFF)
