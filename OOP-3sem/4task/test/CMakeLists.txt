cmake_minimum_required(VERSION 3.11)
project(train_simulation CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Find or download raylib
#include(FetchContent)
#FetchContent_Declare(raylib
#    URL https://github.com/raysan5/raylib/releases/download/4.5.0/raylib-4.5.0_linux_amd64.tar.gz
#)
#FetchContent_MakeAvailable(raylib)

# Add executable
add_executable(train_simulation
    main.cpp
    train_simulation.cpp
    train_simulation.hpp
    config.hpp
    statistics.hpp
    resources.h
)

# Link libraries
target_link_libraries(train_simulation raylib)

# Include directories
target_include_directories(train_simulation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Platform-specific settings
if(MSVC)
    target_compile_options(train_simulation PRIVATE /W4)
else()
    target_compile_options(train_simulation PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set output directory
set_target_properties(train_simulation PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)