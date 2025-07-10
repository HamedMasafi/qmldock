cmake_minimum_required(VERSION 3.18)
project(DockWidget)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)


# Qt 6 dependency and configurations
find_package(Qt6
        COMPONENTS
        Gui
        Qml
        Quick
        SerialBus
        Test
        Widgets
        QuickControls2
        REQUIRED
        )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add all sub-directories
add_subdirectory(src EXCLUDE_FROM_ALL)
add_subdirectory(examples EXCLUDE_FROM_ALL)
