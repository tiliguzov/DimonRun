cmake_minimum_required(VERSION 3.20.2)
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
        core/serializer.cpp
        core/keyboard.cpp
        core/animation_pack.cpp
        core/serializer.cpp
        core/location_manager/location_manager.cpp
        core/location_manager/settings_manager.cpp

        engine/component_manager.cpp
        engine/coordinator.cpp
        engine/entity_manager.cpp
        engine/system.cpp
        engine/system_manager.cpp

        main.cpp

        resources/hero.qrc
        resources/background.qrc
        resources/hand_created_dungeons.qrc
        resources/view.qrc

        systems/movement_system.cpp
        systems/joystick_system.cpp
        systems/painting_system.cpp
        systems/animation_system.cpp
        systems/collision_system.cpp
        systems/illness_system.cpp

        view/buttons/menu_button.cpp
        view/widgets/fast_menu.cpp
        view/widgets/vault.cpp
        view/widgets/scroll.cpp)

target_link_libraries(DimonRun
        Qt::Core
        Qt::Gui
        Qt::Widgets)

add_executable(MapCreator
        core/connector.cpp
        core/scene.cpp
        core/serializer.cpp
        core/keyboard.cpp
        core/animation_pack.cpp
        core/serializer.cpp

        engine/component_manager.cpp
        engine/coordinator.cpp
        engine/entity_manager.cpp
        engine/system.cpp
        engine/system_manager.cpp

        resources/hero.qrc
        resources/background.qrc
        resources/hand_created_dungeons.qrc
        resources/view.qrc

        systems/movement_system.cpp
        systems/joystick_system.cpp
        systems/painting_system.cpp
        systems/animation_system.cpp
        systems/collision_system.cpp
        systems/illness_system.cpp

        view/buttons/menu_button.cpp
        view/widgets/fast_menu.cpp
        view/widgets/vault.cpp

        map_creator/map_creator.cpp
        map_creator/map_creator_main.cpp
        )

target_link_libraries(MapCreator
        Qt::Core
        Qt::Gui
        Qt::Widgets)