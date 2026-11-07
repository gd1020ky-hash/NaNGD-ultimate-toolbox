cmake_minimum_required(VERSION 3.21)

project(ultimate_toolbox VERSION 1.3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED)

find_package(Geode REQUIRED)
setup_geode_mod(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_sources(${PROJECT_NAME} PRIVATE
    src/main.cpp
    src/Toolbox.cpp
    src/FrameAnalyzer.cpp
    src/HUDOverlay.cpp
    src/ToolboxPopup.cpp
    src/Hacks.cpp
)
