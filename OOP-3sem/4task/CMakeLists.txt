cmake_minimum_required(VERSION 3.10)
project(RailwaySimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Raylib
add_subdirectory(external/raylib)
add_subdirectory(external/raygui)

# Исполняемый файл
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Линкуем raylib и raygui
target_link_libraries(${PROJECT_NAME} raylib raygui)

# Платформо-зависимые библиотеки
if (WIN32)
    target_link_libraries(${PROJECT_NAME} gdi32 winmm opengl32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
elseif (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m pthread dl X11)
elseif (APPLE)
    target_link_libraries(${PROJECT_NAME}
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()
