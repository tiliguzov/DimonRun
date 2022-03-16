cmake_minimum_required(VERSION 3.21)
project(DimonRun)

set(CMAKE_CXX_STANDARD 17)

add_executable(DimonRun
        main.cpp
        entity_manager.cpp
        component_manager.cpp)
