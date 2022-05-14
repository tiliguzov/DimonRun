cmake_minimum_required(VERSION 3.21)
project(DimonRun)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(DimonRun
        core/connector.cpp
        core/scene.cpp
        core/keyboard.cpp
        core/animation_pack.cpp

        engine/component_manager.cpp
        engine/coordinator.cpp
        engine/entity_manager.cpp
        engine/system.cpp
        engine/system_manager.cpp

        main.cpp

        resources/hero.qrc
        resources/background.qrc

        systems/movement_system.cpp
        systems/joystick_system.cpp
        systems/painting_system.cpp
        systems/animation_system.cpp)

target_link_libraries(DimonRun
        Qt::Core
        Qt::Gui
        Qt::Widgets)

add_executable(MapCreator
        map_creator.cpp
        map_creator_main.cpp
        resources/hero.qrc
        resources/background.qrc)

target_link_libraries(MapCreator
        Qt::Core
        Qt::Gui
        Qt::Widgets)