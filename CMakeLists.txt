cmake_minimum_required(VERSION 3.21)
project(DimonRun)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(DimonRun
        main.cpp
        engine/entity_manager.cpp
        engine/component_manager.cpp
        engine/system.cpp
        engine/system_manager.cpp
        engine/coordinator.cpp)
