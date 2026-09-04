cmake_minimum_required(VERSION 3.28)

option(PRODUCTION_BUILD "Make this a production build!" OFF)

project(TheGame)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL
    "Build SFML as static libraries"
    FORCE
)

set(IMGUI_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui-docking/imgui"
    CACHE PATH
    "Path to the bundled ImGui source"
    FORCE
)

set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL
    "Use the bundled SFML target instead of searching for an installed package"
    FORCE
)

add_subdirectory(external/SFML-3.1.0)
add_subdirectory(external/imgui-docking)
add_subdirectory(external/imgui-sfml)

file(
    GLOB_RECURSE PROJECT_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
        -Wno-unused-variable
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Release" OR PRODUCTION_BUILD)
        target_link_options(${PROJECT_NAME} PRIVATE
            -static-libgcc
            -static-libstdc++
        )
    endif()

elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        /wd4100
        /wd4189
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Release" OR PRODUCTION_BUILD)
        set(CMAKE_MSVC_RUNTIME_LIBRARY
            "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        )

        set(CMAKE_MSVC_RUNTIME_LIBRARY
            "MultiThreaded$<$<CONFIG:Release>:Release>"
        )
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR PRODUCTION_BUILD)
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        RESOURCES_PATH="./resources/"
        PRODUCTION_BUILD=1
    )
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/"
        PRODUCTION_BUILD=0
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    ImGui-SFML
)