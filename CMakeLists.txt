cmake_minimum_required(VERSION 3.21)
project(DimonRun)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "~/Proga_second_sem/Qt/6.2.3/gcc_64/")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(DimonRun
        core/connector.cpp
        core/dungeon_name.h
        core/scene.cpp
        core/serializer.cpp
        core/keyboard.cpp

        engine/component_manager.cpp
        engine/coordinator.cpp
        engine/entity_manager.cpp
        engine/system.cpp
        engine/system_manager.cpp

        main.cpp
        resources.qrc

        systems/movement_system.cpp
        systems/joystick_system.cpp
        systems/painting_system.cpp)


target_link_libraries(DimonRun
        Qt::Core
        Qt::Gui
        Qt::Widgets)
