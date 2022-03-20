cmake_minimum_required(VERSION 3.21)
project(DimonRun)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/home/marmotikon/Proga_second_sem/Qt/6.2.3/gcc_64/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(DimonRun
        main.cpp
        engine/entity_manager.cpp
        engine/component_manager.cpp
        engine/system.cpp
        engine/system_manager.cpp
        engine/coordinator.cpp core/connector.cpp core/connector.h core/scene.cpp core/scene.h core/components.h systems/movement_system.cpp systems/movement_system.h)

target_link_libraries(DimonRun
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )