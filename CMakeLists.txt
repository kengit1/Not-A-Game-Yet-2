cmake_minimum_required(VERSION 3.28)

project(MyGame)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML
set(SFML_DIR "C:/Libraries/SFML-3.1.0/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

# ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)

add_subdirectory(external/imgui-sfml)

# Main executable
add_executable(MyGame
    src/main.cpp
)

# Compiler warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(MyGame PRIVATE
        -Wall
        -Wextra
        -Wpedantic

        -Wno-unused-parameter
        -Wno-unused-variable
    )
elseif (MSVC)
    target_compile_options(MyGame PRIVATE
        /W4
        /wd4100
        /wd4189
    )
endif()

# Libraries
target_link_libraries(MyGame
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        ImGui-SFML
)