cmake_minimum_required(VERSION 3.20)

project("brick-breaker" LANGUAGES CXX C)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

function(set_warnings_and_standard TARGET)
    if(UNIX)
        target_compile_options(${TARGET} PRIVATE "-Wall" "-Wextra" "-Wpedantic")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${TARGET} PRIVATE "/W3")
    else()
        message(WARNING "Compiler is not GCC or MSVC")
    endif()

    target_compile_features(${TARGET} PRIVATE cxx_std_17)
    set_target_properties(${TARGET} PROPERTIES CXX_EXTENSIONS OFF)
endfunction()

add_subdirectory(engine)
add_subdirectory(clients)
