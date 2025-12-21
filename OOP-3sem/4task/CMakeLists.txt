cmake_minimum_required(VERSION 3.10)
project(RailwaySimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Raylib
add_subdirectory(external/raylib)

# Исполняемый файл
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Линкуем raylib + платформо-зависимые libs
target_link_libraries(${PROJECT_NAME} raylib)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} gdi32 winmm opengl32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
elseif (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m pthread dl X11)
endif()

# Копируем ресурсы
file(GLOB_RECURSE RESOURCES 
    "static/*"
)
foreach(RESOURCE ${RESOURCES})
    if (IS_DIRECTORY ${RESOURCE})
        continue()
    endif()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${RESOURCE} $<TARGET_FILE_DIR:${PROJECT_NAME}>/
    )
endforeach()
