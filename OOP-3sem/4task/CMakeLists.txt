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

# Статические ресурсы (ИСПРАВЛЕННОЕ)
set(STATIC_RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/static")

if(WIN32)
    # Debug и Release - копируем рядом с .exe
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/static"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${STATIC_RESOURCES_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/static"
        COMMENT "Copying static resources to executable directory"
    )
else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/static"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${STATIC_RESOURCES_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/static"
    )
endif()

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
