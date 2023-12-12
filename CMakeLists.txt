cmake_minimum_required(VERSION 3.20)

project("brick-breaker" LANGUAGES CXX C)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

add_subdirectory(engine)
add_subdirectory(clients)
